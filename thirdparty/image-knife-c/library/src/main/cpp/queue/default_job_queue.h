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

#ifndef IMAGE_KNIFE_C_DEFAULT_JOB_QUEUE_H
#define IMAGE_KNIFE_C_DEFAULT_JOB_QUEUE_H
#include <queue>
#include "IJob_queue.h"

namespace ImageKnifeC {
enum Priority {
    HIGH = 0,
    MEDIUM,
    LOW
};

class DefaultJobQueue : public IJobQueue {
public:
    DefaultJobQueue() = default;
    ~DefaultJobQueue() override = default;

    size_t GetQueueLength() override;

    void Add(QueueElement queueElement) override;

    QueueElement Pop() override;
private:
    std::queue<QueueElement> highQueue_;
    std::queue<QueueElement> normalQueue_;
    std::queue<QueueElement> lowQueue_;
    QueueElement PopFromQueue(std::queue<QueueElement> &queue);
};
}
#endif // IMAGE_KNIFE_C_DEFAULT_JOB_QUEUE_H
