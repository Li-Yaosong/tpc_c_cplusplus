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
#ifndef IMAGE_KNIFE_C_FILE_CACHE_INTERCEPTOR_OH_H
#define IMAGE_KNIFE_C_FILE_CACHE_INTERCEPTOR_OH_H

#include <sstream>
#include "interceptor.h"
#include "imageKnife_request.h"
#include "file_cache.h"
#include "imageKnife_internal.h"

namespace ImageKnifeC {
class FileCacheInterceptorOH : public FileCacheInterceptor {
public:
    FileCacheInterceptorOH()
    {
        name = "Default FileCacheInterceptor";
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
        std::string fileKey;
        if(!task->imageSrc->GetString(fileKey)) {
            return false;
        }
        fileKey = ImageKnife::GetInstance().GetEngineKeyImpl()->GenerateFileKey(task->imageSrc,
                                                                                task->request->GetImageKnifeOption()->signature);
        FileCache::FileData fileData;
        if (FileCache::GetInstance()->GetImageFromDisk(fileKey, fileData)) {
            task->product.imageBuffer = fileData.buffer;
            task->product.imageLength = fileData.size;
            return true;
        }
        return false;
    }

    bool Write(ImageKnifeTask *task)
    {
        std::string fileKey;
        if (!task->imageSrc->GetString(fileKey)) {
            return false;
        }
        fileKey = ImageKnife::GetInstance().GetEngineKeyImpl()->GenerateFileKey(task->imageSrc,
                                                task->request->GetImageKnifeOption()->signature);
        FileCache::GetInstance()->SaveImageToDisk(task->product.response->body.buffer,
                                                  task->product.response->body.length,
                                                  fileKey);
        return true;
    }
};
};

#endif // IMAGE_KNIFE_C_FILE_CACHE_INTERCEPTOR_OH_H
