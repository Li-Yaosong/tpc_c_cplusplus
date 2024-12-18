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

#ifndef IMAGE_KNIFE_C_IJOB_QUEUE_H
#define IMAGE_KNIFE_C_IJOB_QUEUE_H
#include <cstddef>
#include "request/imageKnife_request_internal.h"

namespace ImageKnifeC {
class IJobQueue {
public:
    struct QueueElement {
        QueueElement() = default;

        QueueElement(std::shared_ptr<ImageKnifeRequestInternal> request, ImageKnifeRequestSource type)
            : request(request), type(type) {}

        QueueElement(const QueueElement &queueElement)
            : request(queueElement.request), type(queueElement.type) {}

        std::shared_ptr<ImageKnifeRequestInternal> request = nullptr;
        ImageKnifeRequestSource type;
    };

    IJobQueue() = default;
    virtual ~IJobQueue() = default;

    // 获取队列长度
    virtual size_t GetQueueLength() = 0;

    // 添加队列元素
    virtual void Add(QueueElement queueElement) = 0;

    // 移除并返回队列头部元素
    virtual QueueElement Pop() = 0;
};
} // namespace ImageKnightC
#endif // IMAGE_KNIFE_C_IJOB_QUEUE_H
