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

#ifndef IMAGE_KNIFE_C_IMAGEKNIFE_LOADER_H
#define IMAGE_KNIFE_C_IMAGEKNIFE_LOADER_H

#include "interceptor.h"

namespace ImageKnifeC {
class ImageKnifeLoader {
public:
    enum class Position {
        FRONT,
        END
    };

    virtual void AddMemoryCacheInterceptor(std::shared_ptr<MemoryCacheInterceptor> interceptor,
                                           Position postion = Position::FRONT) = 0;

    virtual void AddFileCacheInterceptor(std::shared_ptr<FileCacheInterceptor> interceptor,
                                         Position postion = Position::FRONT) = 0;

    virtual void AddDownloadInterceptor(std::shared_ptr<DownloadInterceptor> interceptor,
                                        Position postion = Position::FRONT) = 0;

    virtual void AddDecodeInterceptor(std::shared_ptr<DecodeInterceptor> interceptor,
                                      Position postion = Position::FRONT) = 0;

    static std::shared_ptr<ImageKnifeLoader> CreateDefaultImageLoader();
    static std::shared_ptr<ImageKnifeLoader> CreateEmptyImageLoader();
    virtual ~ImageKnifeLoader() = default;
};

} // end of namespace

#endif // IMAGE_KNIFE_C_IMAGEKNIFE_LOADER_H
