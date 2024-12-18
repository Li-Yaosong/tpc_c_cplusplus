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

#include "interceptor.h"
#include "imageKnife_task_internal.h"
#include <chrono>
namespace ImageKnifeC {

bool Interceptor::Process(ImageKnifeTask *task)
{
    ImageKnifeTaskInternal *taskInternal = static_cast<ImageKnifeTaskInternal*>(task);
    if (taskInternal->IsFatalErrorHappened()) {
        return false;
    }
    taskInternal->SetInterceptor(this);

    auto start = std::chrono::high_resolution_clock::now();
    if (Resolve(task)) {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        OH_LOG_Print(LOG_APP, LOG_DEBUG, LOG_DOMAIN,
                     "Performance",
                     "[%{public}lld] %{public}s %{public}s Finished WirteMode: %{public}d",
                     duration.count(), ImageKnifeTaskInternal::GetTaskInfo(task->type,
                     task->request.get()).c_str(),
                     name.c_str(),
                     task->product.writeCache);

        taskInternal->ClearInterceptorPtr();
        return true;
    } else if (next_ != nullptr) {
        return next_->Process(task);
    } else {
        taskInternal->ClearInterceptorPtr();
        return false;
    }
}

} // end of namespace
