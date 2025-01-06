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

#ifndef IMAGE_KNIFE_C_MEMORY_CACHE_INTERCEPTOR_OH_H
#define IMAGE_KNIFE_C_MEMORY_CACHE_INTERCEPTOR_OH_H

#include "interceptor.h"
#include "imageKnife_request.h"
#include "lru_cache.h"
#include "native_entry.h"
#include "imageKnife_internal.h"
#include <sstream>

namespace ImageKnifeC {
class MemoryCacheInterceptorOH : public MemoryCacheInterceptor {
public:
    MemoryCacheInterceptorOH()
    {
        name = "Default MemoryCacheInterceptor";
    }

    bool Resolve(ImageKnifeTask *task) override
    {
        if (task->product.writeCache) {
            return Write(task);
        } else {
            return Read(task);
        }
    }

private:
    bool Read(ImageKnifeTask *task)
    {
        std::shared_ptr<ImageKnifeOption> imageKnifeOption = task->request->GetImageKnifeOption();
        std::string memoryKey = ImageKnife::GetInstance().GetEngineKeyImpl()->
                                GenerateMemoryKey(task->imageSrc,
                                                  task->type,
                                                  imageKnifeOption.get(),
                                                  task->request->GetImageKnifeOption()->signature);
        if (LruCache::GetInstance()->Contains(memoryKey)) {
            task->product.pixelmap = LruCache::GetInstance()->Get(memoryKey);

            if (task->product.pixelmap == nullptr) {
                task->EchoError("pixelmap is nullptr");
                return false;
            }
            return true;
        }
        return false;
    }

    bool Write(ImageKnifeTask *task)
    {
        std::shared_ptr<ImageKnifeOption> imageKnifeOption = task->request->GetImageKnifeOption();
        std::string memoryKey = ImageKnife::GetInstance().GetEngineKeyImpl()->
                                GenerateMemoryKey(task->imageSrc,
                                                  task->type,
                                                  imageKnifeOption.get(),
                                                  task->request->GetImageKnifeOption()->signature);

        if (task->product.pixelmap != nullptr && task->product.pixelmap->GetPixelMap() != nullptr) {
            LruCache::GetInstance()->Put(memoryKey, task->product.pixelmap);
            return true;
        } else {
            task->EchoError("Try to Cache nullptr pixelmap");
            return false;
        }
    }
};

};

#endif // IMAGE_KNIFE_C_MEMORY_CACHE_INTERCEPTOR_OH_H
