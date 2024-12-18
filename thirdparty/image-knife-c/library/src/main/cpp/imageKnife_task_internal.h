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

#ifndef IMAGE_KNIFE_C_KEKE_IMAGEKNIFE_TASK_INTERNAL_H
#define IMAGE_KNIFE_C_KEKE_IMAGEKNIFE_TASK_INTERNAL_H

#include "imageKnife_task.h"
#include "imageKnife_request.h"
#include <cstdint>

namespace ImageKnifeC {

class ImageKnifeTaskInternal : public ImageKnifeTask {
public:
    // 用于onLoadSuccess 回调的pixelmap拷贝
    ImageDataBack pixelmapCallbackData;
    std::string memoryKey;

    ImageKnifeTaskInternal(ImageKnifeRequestSource type, std::shared_ptr<ImageKnifeRequest> request,
                           ImageData *imageSrc) : ImageKnifeTask(type, request, imageSrc)
    {
    }

    Interceptor *GetCurrentInterceptor() const
    {
        return interceptor_;
    }

    void SetInterceptor(Interceptor *interceptor)
    {
        interceptor_ = interceptor;
    }

    void ClearInterceptorPtr()
    {
        interceptor_ = nullptr;
    }

    bool IsSuccess() const
    {
        return success_;
    }

    void MarkSuccess()
    {
        success_ = true;
    }

    void SetTaskId(void *id)
    {
        taskId_ = id;
    }

    void *GetTaskId() const
    {
        return taskId_;
    }

    void FatalError(std::string errorInfo)
    {
        fatalError_ = true;
        EchoError("[Fatal Error] " + errorInfo);
    }

    void EchoError(std::string errorInfo) override
    {
        std::string source;
        if (interceptor_ != nullptr) {
            source = " (" + interceptor_.load()->name + ") ";
        }
        lastError_ = GetTaskInfo(type,request.get()) + errorInfo + source;
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN, "TaskError", "%{public}s", lastError_.c_str());
    }

    std::string GetLastError() const
    {
        return lastError_;
    }

    bool IsFatalErrorHappened()
    {
        return fatalError_;
    }

    static std::string GetTaskInfo(ImageKnifeRequestSource type, ImageKnifeRequest *request);

    void QueryDisplayPixelmapInfo();

    std::string GetDisplayPixelmapInfo() const;

private:
    struct PixelmapInfo {
        uint32_t width = 0;
        uint32_t height = 0;
        size_t  size = 0;
        std::string pixelFormat = "Unknown";
    };

    PixelmapInfo pixelmapInfo;

    std::atomic<Interceptor*> interceptor_ = nullptr;
    bool success_ = false;
    bool fatalError_ = false;
    void *taskId_ = nullptr;
    std::string lastError_;
}; 
} // end of namespace
#endif // IMAGE_KNIFE_C_KEKE_IMAGEKNIFE_TASK_INTERNAL_H
