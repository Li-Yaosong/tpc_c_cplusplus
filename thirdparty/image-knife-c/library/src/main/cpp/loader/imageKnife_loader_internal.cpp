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

#include "imageKnife_request.h"
#include "imageKnife_loader_internal.h"

namespace ImageKnifeC {
void ImageKnifeLoaderInternal::AddMemoryCacheInterceptor(std::shared_ptr<MemoryCacheInterceptor> interceptor,
                                                         ImageKnifeLoader::Position position)
{
    if (interceptor == nullptr) {
        return;
    }

    // 责任链为空
    if (memoryInterceptorHead_ == nullptr) {
        memoryInterceptorHead_ = memoryInterceptorTail_ = interceptor;
        return;
    }

    if (position == ImageKnifeLoader::Position::FRONT) {
        interceptor->SetNext(memoryInterceptorHead_);
        memoryInterceptorHead_ = interceptor;
    } else {
        memoryInterceptorTail_->SetNext(interceptor);
        memoryInterceptorTail_ = interceptor;
    }
}

void ImageKnifeLoaderInternal::AddFileCacheInterceptor(std::shared_ptr<FileCacheInterceptor> interceptor,
                                                       ImageKnifeLoader::Position position)
{
    if (interceptor == nullptr) {
        return;
    }

    // 责任链为空
    if (fileInterceptorHead_ == nullptr) {
        fileInterceptorHead_ = fileInterceptorTail_ = interceptor;
        return;
    }

    if (position == ImageKnifeLoader::Position::FRONT) {
        interceptor->SetNext(fileInterceptorHead_);
        fileInterceptorHead_ = interceptor;
    } else {
        fileInterceptorTail_->SetNext(interceptor);
        fileInterceptorTail_ = interceptor;
    }
}

void ImageKnifeLoaderInternal::AddDownloadInterceptor(std::shared_ptr<DownloadInterceptor> interceptor,
                                                      ImageKnifeLoader::Position position)
{
    if (interceptor == nullptr) {
        return;
    }

    // 责任链为空
    if (downloadInterceptorHead_ == nullptr) {
        downloadInterceptorHead_ = downloadInterceptorTail_ = interceptor;
        return;
    }

    if (position == ImageKnifeLoader::Position::FRONT) {
        interceptor->SetNext(downloadInterceptorHead_);
        downloadInterceptorHead_ = interceptor;
    } else {
        downloadInterceptorTail_->SetNext(interceptor);
        downloadInterceptorTail_ = interceptor;
    }
}


void ImageKnifeLoaderInternal::AddDecodeInterceptor(std::shared_ptr<DecodeInterceptor> interceptor,
                                                    ImageKnifeLoader::Position position)
{
    if (interceptor == nullptr) {
        return;
    }

    // 责任链为空
    if (decodeInterceptorHead_ == nullptr) {
        decodeInterceptorHead_ = decodeInterceptorTail_ = interceptor;
        return;
    }

    if (position == ImageKnifeLoader::Position::FRONT) {
        interceptor->SetNext(decodeInterceptorHead_);
        decodeInterceptorHead_ = interceptor;
    } else {
        decodeInterceptorTail_->SetNext(interceptor);
        decodeInterceptorTail_ = interceptor;
    }
}

bool ImageKnifeLoaderInternal::LoadFromMemory(ImageKnifeTaskInternal *task)
{
    if (memoryInterceptorHead_ == nullptr) {
        task->FatalError("Not Find Memory Interceptor!");
        return false;
    }

    try {
        task->product.writeCache = false;
        return memoryInterceptorHead_->Process(task);
    } catch (std::exception err) {
        task->FatalError("MemoryInterceptor: " + std::string(err.what()));
        return false;
    }
}

bool ImageKnifeLoaderInternal::LoadFromFile(ImageKnifeTaskInternal *task)
{
    if (fileInterceptorHead_ == nullptr) {
        task->FatalError("Not Find File Interceptor!");
        return false;
    }

    try {
        task->product.writeCache = false;
        return fileInterceptorHead_->Process(task);
    } catch (std::exception err) {
        task->FatalError("FileInterceptor: " + std::string(err.what()));
        return false;
    }
}


bool ImageKnifeLoaderInternal::WriteCacheToMemory(ImageKnifeTaskInternal *task)
{
    if (memoryInterceptorHead_ == nullptr) {
        task->FatalError("Not Find Memory Interceptor!");
        return false;
    }

    try {
        task->product.writeCache = true;
        return memoryInterceptorHead_->Process(task);
    } catch (std::exception err) {
        task->FatalError("MemoryInterceptor: " + std::string(err.what()));
        return false;
    }
}

bool ImageKnifeLoaderInternal::WriteCacheToFile(ImageKnifeTaskInternal *task)
{
    if (fileInterceptorHead_ == nullptr) {
        task->FatalError("Not Find File Interceptor!");
        return false;
    }

    try {
        task->product.writeCache = true;
        return fileInterceptorHead_->Process(task);
    } catch (std::exception err) {
        task->FatalError("FileInterceptor: " + std::string(err.what()));
        return false;
    }
}

bool ImageKnifeLoaderInternal::DownloadImage(ImageKnifeTaskInternal *task)
{
    if (downloadInterceptorHead_ == nullptr) {
        task->FatalError("Not Find Download Interceptor!");
        return false;
    }

    try {
        return downloadInterceptorHead_->Process(task);
    } catch (std::exception err) {
        task->FatalError("DownloadInterceptor: " + std::string(err.what()));
        return false;
    }
}

bool ImageKnifeLoaderInternal::DecodeImage(ImageKnifeTaskInternal *task)
{
    if (decodeInterceptorHead_ == nullptr) {
        task->FatalError("Not Find Decode Interceptor!");
        return false;
    }

    try {
        return decodeInterceptorHead_->Process(task);
    } catch (std::exception err) {
        task->FatalError("DecodeInterceptor: " + std::string(err.what()));
        return false;
    }
}

} // end of namespace
