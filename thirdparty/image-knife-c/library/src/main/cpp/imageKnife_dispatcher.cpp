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


#include "imageKnife_dispatcher.h"
#include "task_worker.h"
#include "native_entry.h"
#include "lru_cache.h"
#include "imageKnife_loader.h"
#include "task_worker.h"
#include "imageKnife_internal.h"
#include "imageKnife_task_internal.h"
#include "loader/imageKnife_loader_internal.h"

namespace ImageKnifeC {

ImageKnifeDispatcher::ImageKnifeDispatcher()
{
}

ImageKnifeDispatcher::~ImageKnifeDispatcher()
{
    if (jobQueuePtr_ != nullptr) {
        delete jobQueuePtr_;
    }
}

void ImageKnifeDispatcher::Enqueue(std::shared_ptr<ImageKnifeRequestInternal> request)
{
    // js层start回调
    if (request->GetImageKnifeOption()->onLoadListener.onLoadStart != nullptr) {
        request->GetImageKnifeOption()->onLoadListener.onLoadStart();
    }

    if (request->GetImageKnifeOption()->imageLoader == nullptr) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                     "Enqueue",
                     "Can't find ImageKnifeLoader");
        if (request->GetImageKnifeOption()->onLoadListener.onLoadFailed != nullptr) {
            request->GetImageKnifeOption()->onLoadListener.onLoadFailed("Can't find ImageKnifeLoader");
        }
        return;
    }

    // 1.内存有的话直接渲染
    if (LoadSrcFromMemory(request, ImageKnifeRequestSource::MAINSRC)) {
        return;
    }

    bool foundPlaceHolder = LoadSrcFromMemory(request, ImageKnifeRequestSource::PLACESRC);

    // 判断是否排队
    if (executingJobMap_.size() >= maxRequests) {
        DefaultJobQueue::QueueElement mainQueueElement(request, ImageKnifeRequestSource::MAINSRC);
        jobQueuePtr_->Add(mainQueueElement);

        if (!foundPlaceHolder && !request->IsSyncLoad()) {
            DefaultJobQueue::QueueElement placeQueueElement(request, ImageKnifeRequestSource::PLACESRC);
            jobQueuePtr_->Add(placeQueueElement);
        }
        return;
    }

    // 启动加载主图
    ExecuteJob(ImageKnifeRequestSource::MAINSRC, request);

    // 启动加载占位图
    if (!foundPlaceHolder && !request->IsSyncLoad()) {
        ExecuteJob(ImageKnifeRequestSource::PLACESRC, request);
    }
}

void ImageKnifeDispatcher::LoadImageSource(void *arg)
{
    ImageKnifeTaskInternal *task = static_cast<ImageKnifeTaskInternal*>(arg);
    auto request = static_cast<ImageKnifeRequestInternal*>(task->request.get());
    auto imageLoader = static_cast<ImageKnifeLoaderInternal*>(request->GetImageKnifeOption()->imageLoader.get());

    // 检查文件缓存
    bool loadEnd = imageLoader->LoadFromFile(task);
    bool writeFileCache = !loadEnd;

    if (request->IsDestroy() || task->IsFatalErrorHappened()) {
        return;
    }

    // 加载结束
    if (!loadEnd) {
        loadEnd = imageLoader->DownloadImage(task);
    }

    if (request->IsDestroy() || task->IsFatalErrorHappened()) {
        return;
    }

    if (!loadEnd) {
        if (task->GetLastError().empty()) {
            task->EchoError("load Image Failed! No useful error message (Loading)");
        }
        return;
    }

    // 图片解码
    if (!imageLoader->DecodeImage(task) || task->IsFatalErrorHappened()) {
        if (task->GetLastError().empty()) {
            task->EchoError("image Decode error! No useful error message (Loading)");
        }
        return;
    }

    if (request->IsDestroy() || task->IsFatalErrorHappened()) {
        return;
    }

    // 解码成功后写原始文件缓存，下载的文件不一定是有效图片
    if (writeFileCache) {
        imageLoader->WriteCacheToFile(task);
    }

    if (request->IsDestroy()) {
        return;
    }

    // 图片形变, 默认没有图片形变
    TransformImage(task);

    if (request->IsDestroy() || task->IsFatalErrorHappened()) {
        return;
    }

    // 写内存缓存
    imageLoader->WriteCacheToMemory(task);

    task->QueryDisplayPixelmapInfo();
    task->MarkSuccess();
}

void ImageKnifeDispatcher::OnComplete(void *arg)
{
    ImageKnifeTaskInternal *task = static_cast<ImageKnifeTaskInternal*>(arg);
    auto request = static_cast<ImageKnifeRequestInternal*>(task->request.get());

    try {
        for (DefaultJobQueue::QueueElement queueElement : executingJobMap_[task->memoryKey]) {
            auto option = queueElement.request->GetImageKnifeOption();
            if (request->IsDestroy()) {
                // 任务在执行中被取消
                if (option->onLoadListener.onLoadCancel != nullptr) {
                    option->onLoadListener.onLoadCancel(task->GetTaskInfo(task->type,
                    task->request.get()) + "Request Abort");
                }
            } else if (task->IsSuccess()) {
                if (queueElement.type != ImageKnifeRequestSource::PLACESRC) {
                    queueElement.request->MarkStatusComplete();
                }
                // 显示前打印图片信息info日志
                std::string info = ImageKnifeTaskInternal::GetTaskInfo(queueElement.type, queueElement.request.get());
                info += task->GetDisplayPixelmapInfo();
                OH_LOG_Print(LOG_APP, LOG_INFO, LOG_DOMAIN, "Display", "%{public}s ,Form Loading", info.c_str());
                
                queueElement.request->MarkDisplayStartTime(queueElement.type, false);

                bool trigerCallback = true;
                // preload只加载不显示
                if (!queueElement.request->IsPreload()) {
                    trigerCallback = DisplayImage(queueElement.request, queueElement.type, task->product.pixelmap);
                }
                if (queueElement.type == ImageKnifeRequestSource::MAINSRC &&
                    option->onLoadListener.onLoadSuccess != nullptr && trigerCallback) {
                    // 触发onLoadSuccess回调
                    ImageKnifeData data;
                    ImageDataBack pixelmapCallbackData;
                    if (task->product.pixelmap != nullptr) {
                        pixelmapCallbackData.CopyFromPixelmap(task->product.pixelmap->GetPixelMap());
                    }

                    option->onLoadListener.onLoadSuccess(pixelmapCallbackData, data);
                }
            } else if (queueElement.type == ImageKnifeRequestSource::MAINSRC) {
                // 任务执行失败
                if (option->onLoadListener.onLoadFailed != nullptr) {
                    option->onLoadListener.onLoadFailed(task->GetLastError());
                }
                // 主图失败启动错误占位图
                queueElement.request->MarkStatusError();
                // 有缓存不会启用task加载，finalize会在更新或析构时调用的CancelRequest时触发，与错误图未启用时一样
                if (!LoadSrcFromMemory(queueElement.request, ImageKnifeRequestSource::ERRORSRC)) {
                    ExecuteJob(ImageKnifeRequestSource::ERRORSRC, queueElement.request);
                }
            }
        }
    } catch (std::exception err) {
        task->FatalError("Dispatcher::OnComplete " + std::string(err.what()));
    }

    executingJobMap_.erase(task->memoryKey);
    DispatchNextJob();

    // 移除task记录
    RemoveTask(task);
}

bool ImageKnifeDispatcher::DisplayImage(std::shared_ptr<ImageKnifeRequestInternal> request,
                                        ImageKnifeRequestSource type,
                                        std::shared_ptr<PixelMapWrapper> pixelmap)
{
    auto arkUINode = NativeEntry::GetInstance()->GetArkUIBaseNode(request->GetNodeId());
    if (arkUINode == nullptr || arkUINode->GetNodeState() == ArkUIBaseNode::NodeState::Cleared ||
        arkUINode->GetNodeVersion() != request->GetVersion()) {
        // 状态不处于available 或 request版本号不是最新则不进行显示
        if (type == ImageKnifeRequestSource::MAINSRC &&
            request->GetImageKnifeOption()->onLoadListener.onLoadCancel != nullptr) {
            request->GetImageKnifeOption()->
            onLoadListener.onLoadCancel(ImageKnifeTaskInternal::
            GetTaskInfo(type, request.get()) + "Request Overdue");
        }
        return false;
    }

    if (type == ImageKnifeRequestSource::PLACESRC && request->IsMainSrcComplete()) {
        // 主图已显示的情况下,不显示占位图
        return false;
    }

    if (arkUINode != nullptr) {
        // 设置图片填充效果
        ArkUI_ObjectFit fitEnum = ARKUI_OBJECT_FIT_COVER;
        switch (type) {
            case ImageKnifeRequestSource::MAINSRC:
                fitEnum = request->GetImageKnifeOption()->objectFit;
                break;
            case ImageKnifeRequestSource::PLACESRC:
                fitEnum = request->GetImageKnifeOption()->placeholderObjectFit;
                break;
            case ImageKnifeRequestSource::ERRORSRC:
                fitEnum = request->GetImageKnifeOption()->errorholderObjectFit;
                break;
            default:
                break;
        }
        arkUINode->FitImage(static_cast<int>(fitEnum));

        // 显示图片
        if (pixelmap != nullptr && pixelmap->GetPixelMap() != nullptr) {
            arkUINode->Display(OH_ArkUI_DrawableDescriptor_CreateFromPixelMap(pixelmap->GetPixelMap()));
            return true;
        } else {
            std::string errorInfo = ImageKnifeTaskInternal::GetTaskInfo(type, request.get()) +
                                    "Empty Pixelmap (Display Image)";
            OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN, "DisplayImage", "%{public}s", errorInfo.c_str());
            if (type == ImageKnifeRequestSource::MAINSRC &&
                request->GetImageKnifeOption()->onLoadListener.onLoadFailed != nullptr) {
                request->GetImageKnifeOption()->onLoadListener.onLoadFailed(errorInfo);
            }
        }
    }

    return false;
}

bool ImageKnifeDispatcher::LoadSrcFromMemory(std::shared_ptr<ImageKnifeRequestInternal> request,
                                             ImageKnifeRequestSource type)
{
    try {
        ImageData *data = GetImageSrc(request, type);
        if (data == nullptr) {
            return false;
        }

        auto imageLoader = static_cast<ImageKnifeLoaderInternal*>(request->
                           GetImageKnifeOption()->imageLoader.get());
        ImageKnifeTaskInternal task(type, request, data);
        if (imageLoader->LoadFromMemory(&task)) {
            // 显示前打印图片信息info日志
            task.QueryDisplayPixelmapInfo();
            std::string info = ImageKnifeTaskInternal::GetTaskInfo(type, request.get());
            info += task.GetDisplayPixelmapInfo();
            OH_LOG_Print(LOG_APP, LOG_INFO, LOG_DOMAIN,
                         "Display", "%{public}s ,Form Memory",
                         info.c_str());

            // 用内存缓存直接显示
            request->MarkDisplayStartTime(type, true);

            bool trigerCallback = true;
            // preload 只加载不显示
            if (!request->IsPreload()) {
                trigerCallback = DisplayImage(request, type, task.product.pixelmap);
            }

            if (type == ImageKnifeRequestSource::MAINSRC &&
                request->GetImageKnifeOption()->onLoadListener.onLoadSuccess != nullptr && trigerCallback) {
                // 拷贝pixelmap用于回调
                ImageDataBack pixelmapCallbackData;
                pixelmapCallbackData.CopyFromPixelmap(task.product.pixelmap->GetPixelMap());
                ImageKnifeData data;
                request->GetImageKnifeOption()->onLoadListener.onLoadSuccess(pixelmapCallbackData, data);
            }
            if (type != ImageKnifeRequestSource::PLACESRC) {
                request->MarkStatusComplete();
            }
            return true;
        }
    } catch (std::exception err) {
        std::string info = ImageKnifeTaskInternal::GetTaskInfo(type, request.get());
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                     "Dispatcher",
                     "%{public}s [Fatal Error] %{public}s (LoadSrcFromMemory)",
                     info.c_str(),
                     err.what());
    }

    return false;
}

ImageData *ImageKnifeDispatcher::GetImageSrc(std::shared_ptr<ImageKnifeRequestInternal> request,
                                             ImageKnifeRequestSource type)
{
    ImageData *data = nullptr;
    switch (type) {
        case ImageKnifeRequestSource::MAINSRC:
            data = &request->GetImageKnifeOption()->loadSrc;
            break;
        case ImageKnifeRequestSource::PLACESRC:
            data = &request->GetImageKnifeOption()->placeholderSrc;
            break;
        case ImageKnifeRequestSource::ERRORSRC:
            data = &request->GetImageKnifeOption()->errorholderSrc;
            break;
        default:
            break;
    }

    if (data == nullptr || data->GetType() == DataType::UNDEFINED) {
        // 该图片类型未启用
        if (type == ImageKnifeRequestSource::MAINSRC) {
            std::string errorInfo = ImageKnifeTaskInternal::GetTaskInfo(type, request.get()) +
                                    "Empty MAINSRC (GetImageSrc)";
            OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN, "Dispatcher", "%{public}s", errorInfo.c_str());
            if (request->GetImageKnifeOption()->onLoadListener.onLoadFailed != nullptr) {
                request->GetImageKnifeOption()->onLoadListener.onLoadFailed(errorInfo);
            }
        }
        return nullptr;
    }
    return data;
}

bool ImageKnifeDispatcher::ExecuteJob(ImageKnifeRequestSource type,
                                      std::shared_ptr<ImageKnifeRequestInternal> request)
{
    ImageData *data = GetImageSrc(request, type);
    if (data == nullptr) {
        return false;
    }

    std::string memoryKey;
    std::shared_ptr<ImageKnifeOption> imageKnifeOption = request->GetImageKnifeOption();

    // 设置默认执行：图片url，resource 进行加载解码，图形变换
    AsyncFunc func = [this](void* data) {
        this->LoadImageSource(data);
    };
    memoryKey = ImageKnife::GetInstance().GetEngineKeyImpl()->GenerateMemoryKey(data, type,
    imageKnifeOption.get(), request->GetImageKnifeOption()->signature);

    DefaultJobQueue::QueueElement queueElement(request, type);

    if (!memoryKey.empty()) {
        if (executingJobMap_.find(memoryKey) == executingJobMap_.end()) {
            executingJobMap_.emplace(memoryKey, std::list<DefaultJobQueue::QueueElement>({std::move(queueElement)}));
        } else {
            executingJobMap_[memoryKey].emplace_back(std::move(queueElement));
            return false;
        }
    }

    ImageKnifeTaskInternal* task = new ImageKnifeTaskInternal(type, request, data);
    task->memoryKey = memoryKey;

    OH_PixelmapNative *pixelmap = nullptr;
    if (data->MovePixelMap(pixelmap)) {
        task->product.pixelmap = std::make_shared<PixelMapWrapper>(pixelmap);
        // 设置执行函数：pixelmap直接图片变换
        func = [this](void* data) {
            this->TransformImage(data);
        };
    }

    if (request->IsSyncLoad()) {
        func(task);
        OnComplete(task);
    } else {
        AsyncFunc completeCallback = [this](void *data) {
            this->OnComplete(data);
        };
        std::string errorInfo;
        void *taskId = TaskWorker::GetInstance()->PushTask(func, completeCallback, task, errorInfo);
        if (taskId != nullptr) {
            task->SetTaskId(taskId);
            request->InsertTask(task, type);
            // 线程任务push时info日志
            OH_LOG_Print(LOG_APP, LOG_INFO, LOG_DOMAIN,
                         "ExecuteJob",
                         "%{public}s Push Task Success",
                         ImageKnifeTaskInternal::GetTaskInfo(type, request.get()).c_str());
        } else {
            task->EchoError(errorInfo + " (Create Task)");
            if (request->GetImageKnifeOption()->onLoadListener.onLoadFailed != nullptr) {
                request->GetImageKnifeOption()->onLoadListener.onLoadFailed(task->GetLastError());
            }
            request->Destroy();
            delete task;
            return false;
        }
    }
    return true;
}

void ImageKnifeDispatcher::TransformImage(void *arg)
{
    ImageKnifeTask* task = static_cast<ImageKnifeTask*>(arg);
    auto option = task->request->GetImageKnifeOption();
    if (option->multiTransformation != nullptr) {
        option->multiTransformation->Transform(task);
    } else if (option->transformation != nullptr) {
        option->transformation->Transform(task);
    }
}

// 在主线程中调用
void ImageKnifeDispatcher::CancelRequest(std::shared_ptr<ImageKnifeRequestInternal> request)
{
    request->Destroy();

    // 调用对应interceptor的cancel函数
    CancelInterceptor(request, ImageKnifeRequestSource::MAINSRC);
    CancelInterceptor(request, ImageKnifeRequestSource::PLACESRC);
    CancelInterceptor(request, ImageKnifeRequestSource::ERRORSRC);
}

void ImageKnifeDispatcher::CancelInterceptor(std::shared_ptr<ImageKnifeRequestInternal> request,
                                             ImageKnifeRequestSource type)
{
    ImageKnifeTaskInternal *task = static_cast<ImageKnifeTaskInternal*>(request->GetTask(type));
    if (task != nullptr) {
        auto interceptor = task->GetCurrentInterceptor();
        if (interceptor != nullptr) {
            interceptor->Cancel(task);
        }
    }
}

void ImageKnifeDispatcher::RemoveTask(ImageKnifeTaskInternal *task)
{
    if (task != nullptr) {
        static_cast<ImageKnifeRequestInternal*>(task->request.get())->RemoveTask(task->type);
        delete task;
    }
}

void ImageKnifeDispatcher::DispatchNextJob()
{
    if (executingJobMap_.size() >= maxRequests) {
        return;
    }
    while (true) {
        DefaultJobQueue::QueueElement queueElement = jobQueuePtr_->Pop();
        ImageKnifeRequest::Status status = queueElement.request->GetStatus();
        if (queueElement.request == nullptr) {
            break;
        } else if (status == ImageKnifeRequest::Status::PROGRESS || status == ImageKnifeRequest::Status::ERROR) {
            // 任务开启成功
            if (ExecuteJob(queueElement.type, queueElement.request)) {
                break;
            }
        } else if (status == ImageKnifeRequest::Status::DESTROY &&
                    queueElement.request->GetImageKnifeOption()->onLoadListener.onLoadCancel != nullptr) {
            queueElement.request->GetImageKnifeOption()->onLoadListener.onLoadCancel(
                ImageKnifeTaskInternal::GetTaskInfo(queueElement.type, queueElement.request.get()) + "Request DESTROY");
        }
    }
}
} //  end of namespace