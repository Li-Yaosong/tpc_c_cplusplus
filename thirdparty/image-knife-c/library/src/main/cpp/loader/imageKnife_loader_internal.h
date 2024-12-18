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

#ifndef IMAGE_KNIFE_C_KEKE_IMAGEKNIFE_LOADER_INTERNAL_H
#define IMAGE_KNIFE_C_KEKE_IMAGEKNIFE_LOADER_INTERNAL_H

#include "imageKnife_loader.h"
#include "imageKnife_task_internal.h"

namespace ImageKnifeC {

class ImageKnifeLoaderInternal : public ImageKnifeLoader {
public:
    bool LoadFromMemory(ImageKnifeTaskInternal *task);

    bool LoadFromFile(ImageKnifeTaskInternal *task);

    bool WriteCacheToMemory(ImageKnifeTaskInternal *task);

    bool WriteCacheToFile(ImageKnifeTaskInternal *task);

    bool DownloadImage(ImageKnifeTaskInternal *task);

    bool DecodeImage(ImageKnifeTaskInternal *task);

    void AddMemoryCacheInterceptor(std::shared_ptr<MemoryCacheInterceptor> interceptor,
                                   Position postion = Position::FRONT) override;

    void AddFileCacheInterceptor(std::shared_ptr<FileCacheInterceptor> interceptor,
                                 Position postion = Position::FRONT) override;

    void AddDownloadInterceptor(std::shared_ptr<DownloadInterceptor> interceptor,
                                Position postion = Position::FRONT) override;

    void AddDecodeInterceptor(std::shared_ptr<DecodeInterceptor> interceptor,
                              Position postion = Position::FRONT) override;

private:
    std::shared_ptr<MemoryCacheInterceptor> memoryInterceptorHead_ = nullptr; 
    std::shared_ptr<MemoryCacheInterceptor> memoryInterceptorTail_ = nullptr;

    std::shared_ptr<FileCacheInterceptor> fileInterceptorHead_ = nullptr;
    std::shared_ptr<FileCacheInterceptor> fileInterceptorTail_ = nullptr;

    std::shared_ptr<DownloadInterceptor> downloadInterceptorHead_ = nullptr;
    std::shared_ptr<DownloadInterceptor> downloadInterceptorTail_ = nullptr;

    std::shared_ptr<DecodeInterceptor> decodeInterceptorHead_ = nullptr;
    std::shared_ptr<DecodeInterceptor> decodeInterceptorTail_ = nullptr;
};

} // end of namespace

#endif // IMAGE_KNIFE_C_KEKE_IMAGEKNIFE_LOADER_INTERNAL_H
