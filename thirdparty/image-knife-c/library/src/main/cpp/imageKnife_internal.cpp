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

#include <future>
#include "imageKnife_internal.h"
#include "file_cache.h"
#include "key/default_engine_key.h"
#include "imageKnife_internal.h"

namespace ImageKnifeC {

// static
ImageKnife &ImageKnife::GetInstance()
{
    static ImageKnifeInternal imageKnifeInternal;
    return imageKnifeInternal;
}

void ImageKnifeInternal::Execute(std::shared_ptr<ImageKnifeRequest> request)
{
    this->dispatcher->Enqueue(std::static_pointer_cast<ImageKnifeRequestInternal>(request));
}

void ImageKnifeInternal::CancelRequest(std::shared_ptr<ImageKnifeRequest> request)
{
    this->dispatcher->CancelRequest(std::static_pointer_cast<ImageKnifeRequestInternal>(request));
}

void ImageKnifeInternal::InitFileCache(std::string context, int size, int memory, std::string path)
{
    if (path.empty()) {
        path = "ImageKnife";
    }

    // 初始化文件缓存 
    ImageKnifeC::FileCache::GetInstance()->Init(size,memory);
    ImageKnifeC::FileCache::GetInstance()->filePath = context + "/" + path;

    ImageKnifeC::FileCache::GetInstance()->InitFileCache();
}

void ImageKnifeInternal::SetEngineKeyImpl(IEngineKey *keyPtr)
{
    keyPtr_ = keyPtr;
}

IEngineKey *ImageKnifeInternal::GetEngineKeyImpl() const
{
    return keyPtr_;
}

void ImageKnifeInternal::SetMaxRequests(int concurrency)
{
    dispatcher->maxRequests = concurrency;
}

std::shared_ptr<ImageKnifeLoader> ImageKnifeInternal::GetDefaultImageKnifeLoader() const
{
    return imageLoader_;
}

void ImageKnifeInternal::SetDefaultImageKnifeLoader(std::shared_ptr<ImageKnifeLoader> imageLoader)
{
    imageLoader_ = imageLoader;
}

} // namespace ImageKnifeC