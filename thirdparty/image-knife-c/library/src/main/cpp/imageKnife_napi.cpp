/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the 'License');
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an 'AS IS' BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "imageKnife_napi.h"
#include "imageKnife_internal.h"
#include "imageKnife.h"
#include "imageKnife_request.h"
#include "file_cache.h"

namespace ImageKnifeC {

// static
napi_value ImageKnifeNapi::CreateNativeRoot(napi_env env, napi_callback_info info)
{
    size_t argc = 4;
    const size_t syncLoadIndex = 3;
    const size_t optionIndex = 2;
    napi_value args[4] = {nullptr};
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);

    // 获取NodeContent
    ArkUI_NodeContentHandle contentHandle;
    OH_ArkUI_GetNodeContentFromNapiValue(env, args[0], &contentHandle);

    // 获取componentId
    size_t length;
    napi_get_value_string_utf8(env, args[1], nullptr, 0, &length);
    char componentId[length + 1];
    napi_get_value_string_utf8(env, args[1], componentId, length + 1, nullptr);

    // 获取ImageKnifeOption及创建imageKnifeNode
    auto imageKnifeOption = std::make_shared<ImageKnifeOption>(env, args[optionIndex]);

    // 获取syncLoad
    bool syncLoad = false;
    napi_get_value_bool(env, args[syncLoadIndex], &syncLoad);

    auto imageKnifeNode = std::make_shared<ImageKnifeNode>(std::string(componentId), imageKnifeOption);
    imageKnifeNode->SyncLoad(syncLoad);
    // 设置contentHandle, 挂载imageNode, 保持Native侧对象到管理类中，维护生命周期。
    NativeEntry::GetInstance()->SetRootNode(componentId, contentHandle, imageKnifeNode);

    ImageKnife::GetInstance().Execute(imageKnifeNode->GetImageKnifeRequest());

    return nullptr;
}

napi_value ImageKnifeNapi::UpdateNativeRoot(napi_env env, napi_callback_info info)
{
    size_t argc = 4;
    const size_t componentVersionIndex = 2;
    const size_t syncLoadIndex = 3;
    napi_value args[4] = {nullptr};
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);

    // 获取componentId
    size_t length;
    napi_get_value_string_utf8(env, args[0], nullptr, 0, &length);
    char componentId[length + 1];
    napi_get_value_string_utf8(env, args[0], componentId, length + 1, nullptr);

    // 获取imageKnifeOption
    auto imageKnifeOption = std::make_shared<ImageKnifeOption>(env, args[1]);

    // 获取componentVersion
    int componentVersion;
    napi_get_value_int32(env, args[componentVersionIndex], &componentVersion);

    // 获取syncLoad
    bool syncLoad = false;
    napi_get_value_bool(env, args[syncLoadIndex], &syncLoad);
    // 找到ImageNode
    auto baseNode = NativeEntry::GetInstance()->GetArkUIBaseNode(componentId).get();
    if (baseNode != nullptr) {
        ImageKnifeNode* imageKnifeNode = static_cast<ImageKnifeNode*>(baseNode);
        imageKnifeNode->Reuse(componentVersion);

        ImageKnife::GetInstance().CancelRequest(imageKnifeNode->GetImageKnifeRequest());
        // 通过imageKnifeOption 更新请求
        imageKnifeNode->UpdateImageKnifeRequest(imageKnifeOption);
        // request 更新完成后，更新request的 syncLoad状态
        imageKnifeNode->SyncLoad(syncLoad);
        ImageKnife::GetInstance().Execute(imageKnifeNode->GetImageKnifeRequest());
    }

    return nullptr;
}

napi_value ImageKnifeNapi::ClearNativeRoot(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value args[1] = {nullptr};
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);

    // 获取componentId
    size_t length;
    napi_get_value_string_utf8(env, args[0], nullptr, 0, &length);
    char componentId[length + 1];
    napi_get_value_string_utf8(env, args[0], componentId, length + 1, &length);

    auto baseNode = NativeEntry::GetInstance()->GetArkUIBaseNode(componentId).get();
    if (baseNode != nullptr) {
        ImageKnifeNode *imageKnifeNode = static_cast<ImageKnifeNode *>(baseNode);
        imageKnifeNode->Clear();
        ImageKnife::GetInstance().CancelRequest(imageKnifeNode->GetImageKnifeRequest());
    }
    return nullptr;
}

napi_value ImageKnifeNapi::DestroyNativeRoot(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value arg;
    napi_get_cb_info(env, info, &argc, &arg, nullptr, nullptr);

    size_t length;
    napi_get_value_string_utf8(env, arg, nullptr, 0, &length);
    char componentId[length + 1];
    napi_get_value_string_utf8(env, arg, componentId, length + 1, &length);

    std::string id = componentId;

    auto baseNode =  NativeEntry::GetInstance()->GetArkUIBaseNode(componentId).get();
    if (baseNode != nullptr) {
        ImageKnifeNode *imageKnifeNode = static_cast<ImageKnifeNode*>(baseNode);
        ImageKnife::GetInstance().CancelRequest(imageKnifeNode->GetImageKnifeRequest());

        // 从管理类中释放Native侧对象。
        NativeEntry::GetInstance()->DisposeRootNode(id);
    }

    return nullptr;
}

napi_value ImageKnifeNapi::InitFileCache(napi_env env, napi_callback_info info)
{
    // 创建 Promise
    napi_status status;
    napi_value promise;
 
    status = napi_create_promise(env, &FileCache::asyncData->deferred, &promise);
    if (status != napi_ok) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                     "ImageKnifeNapi::InitFileCache",
                     "Failed to create promise");
        napi_value result;
        napi_create_string_utf8(env, "Failed to create promise", NAPI_AUTO_LENGTH, &result);
        napi_reject_deferred(env, FileCache::asyncData->deferred, result);
        delete FileCache::asyncData;
        FileCache::asyncData = nullptr;
        return promise;
    }

    // 定义错误处理 lambda
    auto handleError = [env](const char *message)->napi_value {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                     "ImageKnifeNapi::InitFileCache",
                     "%{public}s",
                     message);
        napi_value result;
        napi_create_string_utf8(env, message, NAPI_AUTO_LENGTH, &result);
        napi_reject_deferred(env, FileCache::asyncData->deferred, result);
        delete FileCache::asyncData;
        FileCache::asyncData = nullptr;
    };

    // 获取传递的参数
    size_t argc = 4;
    napi_value args[argc];
    status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    if (status != napi_ok) {
        handleError("Failed to get callback info");
        return promise;
    }

    // 获取 filesDir 参数
    napi_value jsFilesDir = args[0];
    size_t strLength;
    status = napi_get_value_string_utf8(env, jsFilesDir, nullptr, 0, &strLength);
    if (status != napi_ok) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                     "ImageKnifeNapi::InitFileCache",
                     "status is: %{public}d",
                     status);
        handleError("Failed to get 'filesDir' length");
        return promise;
    }

    FileCache::asyncData->fileDir.resize(strLength);
    status = napi_get_value_string_utf8(env, jsFilesDir,
                                        &FileCache::asyncData->fileDir[0],
                                        strLength + 1,
                                        &strLength);
    if (status != napi_ok) {
        handleError("Failed to get 'filesDir' parameter");
        return promise;
    }

    // 获取 size 参数
    napi_value jsSize = args[1];
    status = napi_get_value_int32(env, jsSize, &FileCache::asyncData->size);
    if (status != napi_ok) {
        handleError("Failed to get 'size' parameter");
        return promise;
    }

    // 获取 memory 参数
    napi_value jsMemory = args[2];
    status = napi_get_value_int32(env, jsMemory, &FileCache::asyncData->memory);
    if (status != napi_ok) {
        handleError("Failed to get 'memory' parameter");
        return promise;
    }

    // 获取 path 参数
    napi_value jsPath = args[3];
    size_t pathLength = 0;
    status = napi_get_value_string_utf8(env, jsPath, nullptr, 0, &pathLength);
    if (status == napi_ok) {
        FileCache::asyncData->path.resize(pathLength);
        status = napi_get_value_string_utf8(env, jsPath,
                                            &FileCache::asyncData->path[0],
                                            pathLength + 1,
                                            &pathLength);
        if (status != napi_ok) {
            OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                         "ImageKnifeNapi::InitFileCache",
                         "Failed to get 'path' parameter");
        }
    } else {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                     "ImageKnifeNapi::InitFileCache",
                     "Failed to get 'path' length");
    }

    // 创建异步工作的名称
    napi_value asyncWorkName;
    status = napi_create_string_utf8(env, "InitFileCache", NAPI_AUTO_LENGTH, &asyncWorkName);
    if (status != napi_ok) {
        handleError("Failed to create async work name");
        return promise;
    }

    // 创建异步工作项
    status = napi_create_async_work(env, nullptr, asyncWorkName,
                                    FileCache::StartFileCacheInitThread,
                                    FileCache::FileCacheInitComplete,
                                    FileCache::asyncData,
                                    &FileCache::asyncData->asyncWork);
    if (status != napi_ok) {
        handleError("Failed to create async work");
        return promise;
    }

    // 将异步工作项加入队列
    status = napi_queue_async_work_with_qos(env, FileCache::asyncData->asyncWork, napi_qos_default);
    if (status != napi_ok) {
        handleError("Failed to queue async work");
        napi_delete_async_work(env, FileCache::asyncData->asyncWork);
        return promise;
    }

    return promise;
}

napi_value ImageKnifeNapi::Preload(napi_env env, napi_callback_info info)
{
    napi_status status;
    size_t argc = 1;
    napi_value args[1] = {nullptr};
    status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);

    // 获取传入的 request 参数
    napi_value jsRequest = args[0];

    // 从 request 中获取 imageKnifeOption 属性
    napi_value jsImageKnifeOption;
    status = napi_get_named_property(env, jsRequest, "imageKnifeOption", &jsImageKnifeOption);
    if (status != napi_ok) {
        napi_throw_error(env, "ImageKnifeNapi::Preload", "Failed to get 'imageKnifeOption' property from request");
        return nullptr;
    }

    auto imageKnifeOption = std::make_shared<ImageKnifeOption>(env, jsImageKnifeOption);

    std::shared_ptr<ImageKnifeRequestInternal> request =
        std::dynamic_pointer_cast<ImageKnifeRequestInternal>(ImageKnife::GetInstance().Preload(imageKnifeOption));
    if (!request) {
        napi_throw_error(env, "ImageKnifeNapi::Preload", "Failed to cast request");
        return nullptr;
    }

    request->SetEnv(env);

    // 创建对 jsRequest 的引用并保存到 request 中
    napi_ref jsRequestRef = nullptr;

    // 第四个参数必填项，即使不使用
    status = napi_wrap(env, args[0], request.get(), [](napi_env env, void* data, void* hint) {},
                       nullptr, &jsRequestRef);
    if (status != napi_ok) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN, "ImageKnifeNapi::Preload", "Failed to bind the JavaScript object");
    }

    request->SetJsRequestRef(jsRequestRef);
    return jsRequest;
}

napi_value ImageKnifeNapi::Cancel(napi_env env, napi_callback_info info)
{
    napi_status status;
    size_t argc = 1;
    napi_value args[1] = {nullptr};
    status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    if (status != napi_ok) {
        return nullptr;
    }

    ImageKnifeRequestInternal* request = nullptr;
    status = napi_unwrap(env, args[0], (void**)&request);
    if (status != napi_ok) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN, "ImageKnifeNapi::Cancel", "Failed to unwrap");
        return nullptr;
    }

    if (request == nullptr) {
        OH_LOG_Print(LOG_APP, LOG_DEBUG, LOG_DOMAIN, "ImageKnifeNapi::Cancel", "request is null");
        return nullptr;
    }
    // 恢复成智能指针
    auto requestSharePtr = request->shared_from_this();
    ImageKnife::GetInstance().Cancel(requestSharePtr);
    return nullptr;
}

napi_value ImageKnifeNapi::SetMaxRequests(napi_env env, napi_callback_info info)
{
    // 获取 concurrency 参数
    napi_status status;
    // 获取传递的参数数量
    size_t argc = 1;
    napi_value args[1];
    status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    napi_value jsConrrency = args[0];
    int concurrency = 0;
    status = napi_get_value_int32(env, jsConrrency, &concurrency);
    if (status != napi_ok) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                     "ImageKnifeNapi::SetMaxRequests",
                     "Failed to get 'concurrency' parameter");
    }
    ImageKnife::GetInstance().SetMaxRequests(concurrency);
    return nullptr;
}

} // end of namespace
