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

#include "imageKnife_request_internal.h"
#include "imageKnife_task_internal.h"

namespace ImageKnifeC {

bool ImageKnifeRequestInternal::IsMainSrcComplete() const
{
    return status_ == Status::COMPLETE || status_ == Status::ERRORCOMPLETE;
}

void ImageKnifeRequestInternal::MarkStatusComplete()
{
    if (status_ == Status::PROGRESS) {
        status_ = Status::COMPLETE;
    } else if (status_ == Status::ERROR) {
        status_ = Status::ERRORCOMPLETE;
    }
}

void ImageKnifeRequestInternal::Destroy()
{
    status_ = Status::DESTROY;
}

bool ImageKnifeRequestInternal::IsDestroy() const
{
    return status_ == Status::DESTROY;
}

void ImageKnifeRequestInternal::MarkStatusError()
{
    if (status_ == Status::PROGRESS) {
        status_ = Status::ERROR;
    }
}

void ImageKnifeRequestInternal::InsertTask(ImageKnifeTask *taskId, ImageKnifeRequestSource type)
{
    taskIdMap_[type] = taskId;
}

ImageKnifeTask* ImageKnifeRequestInternal::GetTask(ImageKnifeRequestSource type)
{
    if (taskIdMap_.find(type) == taskIdMap_.end()) {
        return nullptr;
    }
    return taskIdMap_[type];
}

void ImageKnifeRequestInternal::RemoveTask(ImageKnifeRequestSource type)
{
    if (taskIdMap_.find(type) != taskIdMap_.end()) {
        taskIdMap_.erase(type);
    }
}

bool ImageKnifeRequestInternal::IsSyncLoad() const
{
    return syncLoad_;
}

void ImageKnifeRequestInternal::SyncLoad(bool syncLoad)
{
    syncLoad_ = syncLoad;
}

void ImageKnifeRequestInternal::MarkDisplayStartTime(ImageKnifeRequestSource type, bool fromMemory)
{
    displayType_ = type;
    fromMemory_ = fromMemory;
    displayStart_ = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now());
}

void ImageKnifeRequestInternal::MarkDisplayEndTime()
{
    auto end = std::chrono::high_resolution_clock::now();
    auto totalTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start_);
    auto displayTime = std::chrono::duration_cast<std::chrono::microseconds>(end - displayStart_);

    std::string info = ImageKnifeTaskInternal::GetTaskInfo(displayType_, this);
    if (fromMemory_) {
        info += " From Memory";
    } else {
        info += " From Loading";
    }

    OH_LOG_Print(LOG_APP, LOG_DEBUG, LOG_DOMAIN,
                 "Performance",
                 "[%{public}lld] %{public}s Display Finished",
                 displayTime.count(),
                 info.c_str());
    OH_LOG_Print(LOG_APP, LOG_DEBUG, LOG_DOMAIN,
                 "Performance",
                 "[%{public}lld] %{public}s All Finished (Total Time)",
                 totalTime.count(),
                 info.c_str());
}

std::string ImageKnifeRequestInternal::GetNodeId() const
{
    return componentId_;
}

int ImageKnifeRequestInternal::GetVersion() const
{
    return componentVersion_;
}

ImageKnifeRequest::Status ImageKnifeRequestInternal::GetStatus() const
{
    return status_;
}

std::shared_ptr<ImageKnifeOption> ImageKnifeRequestInternal::GetImageKnifeOption() const
{
    return imageKnifeOption_;
}
bool ImageKnifeRequestInternal::IsPreload()
{
    return isPreload_;
}

void ImageKnifeRequestInternal::SetEnv(napi_env env)
{
    env_ = env;
}

void ImageKnifeRequestInternal::SetJsRequestRef(napi_ref jsRequestRef)
{
    jsRequestRef_ = jsRequestRef;
}
} // end of namespace
