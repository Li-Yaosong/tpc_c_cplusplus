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

std::shared_future<std::string> ImageKnifeInternal::InitFileCacheAsync(std::string cachePath, int size, int memory, std::string path) 
{
    // 创建线程对象并分离线程
    std::thread initThread(&ImageKnifeInternal::InitFileCache, this, cachePath, size, memory, path);
    initThread.detach();  // 分离线程，主线程不需要等待
    return ImageKnifeC::FileCache::GetInstance()->promise.get_future().share();
}

void ImageKnifeInternal::InitFileCache(std::string cachePath, int size, int memory, std::string path) 
{
    if (path.empty()) {
        path = "ImageKnife";
    }

    // 初始化文件缓存 
    ImageKnifeC::FileCache::GetInstance()->Init(size,memory);
    ImageKnifeC::FileCache::GetInstance()->filePath = cachePath + "/" + path;

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

std::shared_ptr<ImageKnifeRequest> ImageKnifeInternal::Preload(std::shared_ptr<ImageKnifeOption> imageKnifeOption)
{
    auto request = std::make_shared<ImageKnifeRequestInternal>(imageKnifeOption);
    ImageKnife::GetInstance().Execute(request);
    return request;
}

void ImageKnifeInternal::Cancel(std::shared_ptr<ImageKnifeRequest> request)
{
    if (request == nullptr) {
        return;
    }
    // 使用 static_pointer_cast 进行类型转换
    auto internalRequest = std::static_pointer_cast<ImageKnifeRequestInternal>(request);

    // 调用 Destroy 方法
    internalRequest->Destroy();
}
} // namespace ImageKnifeC