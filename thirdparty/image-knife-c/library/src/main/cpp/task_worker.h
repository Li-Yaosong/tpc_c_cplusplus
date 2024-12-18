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

#ifndef IMAGE_KNIFE_C_TASK_WORKER_H
#define IMAGE_KNIFE_C_TASK_WORKER_H

#include "napi/native_api.h"
#include <string>
#include "hilog/log.h"

namespace ImageKnifeC {
typedef std::function<void(void*)> AsyncFunc;
typedef std::function<std::string(void*)> CustomCancelFunc;

class TaskWorker {
public:
    static TaskWorker *GetInstance()
    {
        static TaskWorker taskWorker;
        return &taskWorker;
    }

    void SetEnv(napi_env env)
    {
        if (env_ == nullptr)
            env_ = env;
    }

    void *PushTask(AsyncFunc execute, AsyncFunc completeCallback, void *data, std::string &errorInfo)
    {
        if (env_ == nullptr) {
            errorInfo = "ImageKnifeC::TaskRunner::PushTask napi_env is null";
            return nullptr;
        }

        InnerWorker *worker = new InnerWorker;
        worker->data_ = data;
        worker->execute_ = execute;
        worker->completeCallback_ = completeCallback;

        napi_status status;
        napi_value resourceId;
        status = napi_create_string_utf8(env_, "ImageKnifeC TaskRunner", NAPI_AUTO_LENGTH, &resourceId);
        if (status != napi_ok) {
            delete worker;
            errorInfo = "ImageKnifeC::TaskRunner::PushTask create resourceId failed, napi status: " +
                        std::to_string(status);
            return nullptr;
        }

        status = napi_create_async_work(env_, nullptr, resourceId, InnerWorker::ExecuteCallback,
                                        InnerWorker::CompleteCallback, worker, &worker->worker_);
        if (status != napi_ok) {
            delete worker;
            errorInfo = "ImageKnifeC::TaskRunner::PushTask create async work failed, napi status: " +
                        std::to_string(status);
            return nullptr;
        }

        status = napi_queue_async_work_with_qos(env_, worker->worker_, napi_qos_default);
        if (status != napi_ok) {
            napi_delete_async_work(env_, worker->worker_);
            errorInfo = "ImageKnifeC::TaskRunner::PushTask queue async work failed, napi status: " +
                        std::to_string(status);
            delete worker;
            return nullptr;
        }

        return worker;
    }

    std::string CancelTask(void *task, CustomCancelFunc func, void *data)
    {
        if (task == nullptr) {
            return "Cancel Failed: Task is null";
        }

        InnerWorker *worker = static_cast<InnerWorker*>(task);
        napi_status status; 
        status = napi_cancel_async_work(env_, worker->worker_);

        if (status == napi_generic_failure) {
            // Task已经开始
            if (func) {
                // 尝试用自定义func取消耗时操作
                return func(data);
            } else {
                return "Cancel Failed: Task is executing, can not cancel";
            }
        } else if (status == napi_ok) {
            // 成功取消, CompleteCallback 会被调用，对应status为 napi_cancelled
            return "Cancel Success";
        }
        return "Cancel Failed: unexpected error! napi status:" + std::to_string(status);
    }

private:
    class InnerWorker {
    public:
        napi_async_work worker_;
        void *data_;
        AsyncFunc execute_;
        AsyncFunc completeCallback_;

        static void ExecuteCallback(napi_env env, void *data)
        {
            InnerWorker* worker = static_cast<InnerWorker*>(data);
            if (worker->execute_ != nullptr) {
                worker->execute_(worker->data_);    
            }
        }

        static void CompleteCallback(napi_env env, napi_status status, void *data)
        {
            InnerWorker* worker = static_cast<InnerWorker*>(data);
            if (worker->completeCallback_ != nullptr && status != napi_cancelled) {
                worker->completeCallback_(worker->data_);
            }

            napi_delete_async_work(env, worker->worker_);
            delete worker;
        }
    };

    TaskWorker(){};
    napi_env env_ = nullptr;
};

} // end of namespace

#endif // IMAGE_KNIFE_C_TASK_WORKER_H
