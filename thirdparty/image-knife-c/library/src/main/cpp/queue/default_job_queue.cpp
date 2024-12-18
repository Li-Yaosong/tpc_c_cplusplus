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

#include "default_job_queue.h"

namespace ImageKnifeC {
size_t DefaultJobQueue::GetQueueLength()
{
    return highQueue_.size() + normalQueue_.size() + lowQueue_.size();
}

void DefaultJobQueue::Add(QueueElement queueElement)
{
    if (queueElement.request->GetImageKnifeOption()->priority == Priority::HIGH) {
        highQueue_.push(queueElement);
    } else if (queueElement.request->GetImageKnifeOption()->priority == Priority::MEDIUM) {
        normalQueue_.push(queueElement);
    } else {
        lowQueue_.push(queueElement);
    }
}

DefaultJobQueue::QueueElement DefaultJobQueue::Pop()
{
    if (!highQueue_.empty()) {
        return PopFromQueue(highQueue_);
    }

    if (!normalQueue_.empty()) {
        return PopFromQueue(normalQueue_);
    }

    if (!lowQueue_.empty()) {
        return PopFromQueue(lowQueue_);
    }

    return QueueElement{};
}

DefaultJobQueue::QueueElement DefaultJobQueue::PopFromQueue(std::queue<QueueElement> &queue)
{
    QueueElement queueElement = queue.front();
    queue.pop();
    return queueElement;
}

}