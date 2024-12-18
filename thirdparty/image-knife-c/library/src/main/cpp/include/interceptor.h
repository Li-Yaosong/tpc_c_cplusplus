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

#ifndef IMAGE_KNIFE_C_INTERCEPTOROH_H
#define IMAGE_KNIFE_C_INTERCEPTOROH_H

#include "imageKnife_task.h"

namespace ImageKnifeC {
class Interceptor {
public:
    // 仅用于日志调试中标识身份
    std::string name;

    bool Process(ImageKnifeTask *task);
    virtual bool Resolve(ImageKnifeTask *task) = 0;
    virtual void Cancel(ImageKnifeTask *task) {}
    virtual ~Interceptor() {}
protected:
    std::shared_ptr<Interceptor> next_ = nullptr;
};

class MemoryCacheInterceptor : public Interceptor {
public:
    void SetNext(std::shared_ptr<MemoryCacheInterceptor> interceptor)
    {
        next_ = interceptor;
    }
};

class FileCacheInterceptor : public Interceptor {
public:
    void SetNext(std::shared_ptr<FileCacheInterceptor> interceptor)
    {
        next_ = interceptor;
    }
};

class DownloadInterceptor : public Interceptor {
public:
    void SetNext(std::shared_ptr<DownloadInterceptor> interceptor)
    {
        next_ = interceptor;
    }
};

class DecodeInterceptor : public Interceptor {
public:
    void SetNext(std::shared_ptr<DecodeInterceptor> interceptor)
    {
        next_ = interceptor;
    }
};
};
#endif // IMAGE_KNIFE_C_INTERCEPTOROH_H
