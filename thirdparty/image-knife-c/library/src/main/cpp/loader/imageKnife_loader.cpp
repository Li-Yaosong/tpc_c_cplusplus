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

#include "imageKnife_loader.h"
#include "imageKnife_loader_internal.h"
#include "decode_interceptor_OH.h"
#include "download_interceptor_OH.h"
#include "file_cache_interceptor_OH.h"
#include "memory_cache_interceptor_OH.h"
#include "resource_interceptor_OH.h"

namespace ImageKnifeC {

// static
std::shared_ptr<ImageKnifeLoader> ImageKnifeLoader::CreateDefaultImageLoader()
{
    auto loader = std::make_shared<ImageKnifeLoaderInternal>();
    // 设置默认拦截器
    loader->AddMemoryCacheInterceptor(std::make_shared<MemoryCacheInterceptorOH>());
    loader->AddFileCacheInterceptor(std::make_shared<FileCacheInterceptorOH>());
    loader->AddDownloadInterceptor(std::make_shared<DownloadInterceptorOH>());
    loader->AddDownloadInterceptor(std::make_shared<ResourceInterceptorOH>());
    loader->AddDecodeInterceptor(std::make_shared<DecodeInterceptorOH>());

    return loader;
}

// static
std::shared_ptr<ImageKnifeLoader> ImageKnifeLoader::CreateEmptyImageLoader()
{
    return std::make_shared<ImageKnifeLoaderInternal>();
}
} // end of namespace