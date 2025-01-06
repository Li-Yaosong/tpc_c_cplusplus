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

#ifndef IMAGE_KNIFE_C_IMAGEKNIFE_NAPI_H
#define IMAGE_KNIFE_C_IMAGEKNIFE_NAPI_H

#include "native_entry.h"
#include <arkui/native_node_napi.h>
#include "node/imageKnife_node.h"
#include "task_worker.h"
#include "imageKnife.h"
#include "file_cache.h"
#include "imageKnife_internal.h"

namespace ImageKnifeC {

class ImageKnifeNapi {
public:
    static void Init(napi_env env, napi_value &exports)
    {
        // 设置env
        TaskWorker::GetInstance()->SetEnv(env);
        NativeModuleInstance::GetInstance()->SetNapiEnv(env);

        napi_property_descriptor desc[] = {
            {"createNativeRoot", nullptr, CreateNativeRoot, nullptr, nullptr, nullptr, napi_default, nullptr},
            {"updateNativeRoot", nullptr, UpdateNativeRoot, nullptr, nullptr, nullptr, napi_default, nullptr},
            {"clearNativeRoot", nullptr, ClearNativeRoot, nullptr, nullptr, nullptr, napi_default, nullptr},
            {"destroyNativeRoot", nullptr, DestroyNativeRoot, nullptr, nullptr, nullptr, napi_default, nullptr},
            {"initFileCache", nullptr, InitFileCache, nullptr, nullptr, nullptr, napi_default, nullptr},
            {"preload", nullptr, Preload, nullptr, nullptr, nullptr, napi_default, nullptr},
            {"cancel", nullptr, Cancel, nullptr, nullptr, nullptr, napi_default, nullptr},
            {"setMaxRequests", nullptr, SetMaxRequests, nullptr, nullptr, nullptr, napi_default, nullptr}
        };
        napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc);
    }
private:
    static napi_value CreateNativeRoot(napi_env env, napi_callback_info info);
    static napi_value UpdateNativeRoot(napi_env env, napi_callback_info info);
    static napi_value ClearNativeRoot(napi_env env, napi_callback_info info);
    static napi_value DestroyNativeRoot(napi_env env, napi_callback_info info);
    static napi_value InitFileCache(napi_env env, napi_callback_info info);
    static napi_value Preload(napi_env env, napi_callback_info info);

    // 用于取消等待队列的任务
    static napi_value Cancel(napi_env env, napi_callback_info info);
    static napi_value SetMaxRequests(napi_env env, napi_callback_info info);
};
} // end of namespace

#endif // IMAGE_KNIFE_C_IMAGEKNIFE_NAPI_H
