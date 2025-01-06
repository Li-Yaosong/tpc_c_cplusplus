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

#ifndef IMAGE_KNIFE_C_IMAGEKNIFE_DISPATCHER_H
#define IMAGE_KNIFE_C_IMAGEKNIFE_DISPATCHER_H
#include <unordered_map>
#include <list>

#include "request/imageKnife_request_internal.h"
#include "interceptor.h"
#include "queue/IJob_queue.h"
#include "queue/default_job_queue.h"
#include "imageKnife_task_internal.h"

namespace ImageKnifeC {

class ImageKnifeDispatcher {
public:
    ImageKnifeDispatcher();
    ~ImageKnifeDispatcher();

    void Enqueue(std::shared_ptr<ImageKnifeRequestInternal> request);

    void CancelRequest(std::shared_ptr<ImageKnifeRequestInternal> request);

    int maxRequests = 8;

private:
    std::unordered_map<std::string, std::list<DefaultJobQueue::QueueElement>> executingJobMap_;
    IJobQueue *jobQueuePtr_ = new DefaultJobQueue();

    bool ExecuteJob(ImageKnifeRequestSource type, std::shared_ptr<ImageKnifeRequestInternal> request);

    bool LoadSrcFromMemory(std::shared_ptr<ImageKnifeRequestInternal> request, ImageKnifeRequestSource type);

    ImageData *GetImageSrc(std::shared_ptr<ImageKnifeRequestInternal> request, ImageKnifeRequestSource type);

    void LoadImageSource(void *arg);

    void OnComplete(void *arg);

    bool DisplayImage(std::shared_ptr<ImageKnifeRequestInternal> request, ImageKnifeRequestSource type,
                        std::shared_ptr<PixelMapWrapper> pixelmap);

    void TransformImage(void *arg);

    void DispatchNextJob();

    void CancelInterceptor(std::shared_ptr<ImageKnifeRequestInternal> request, ImageKnifeRequestSource type);

    void RemoveTask(ImageKnifeTaskInternal *task);
};

}

#endif //IMAGE_KNIFE_C_IMAGEKNIFE_DISPATCHER_H
