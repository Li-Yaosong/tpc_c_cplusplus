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

#include "lru_cache.h"
#include <cstdio>
#include <stdio.h>

namespace ImageKnifeC {
const long LruCache::defaultSize = 50;
const long LruCache::defaultMemory = 128 * 1024 * 1024;
const long LruCache::defaultMaxSize = 4096;
const long LruCache::defaultMaxMemory = 1024 * 1024 * 1024;

LruCache *LruCache::cachePtr = nullptr;

LruCache *LruCache::GetInstance()
{
#define DEFAULT_MAX_SIZE 10
#define DEFAULT_MAX_MEMORY 1024 * 1024
    if (!cachePtr) {
        // js层传入的参数
        size_t allowMaxsize = DEFAULT_MAX_SIZE;
        size_t allowMaxMemory = DEFAULT_MAX_MEMORY;
        cachePtr = new LruCache(allowMaxsize, allowMaxMemory);
    }
    return cachePtr;
}

void LruCache::DestroyInstance()
{
    if (cachePtr) {
        delete cachePtr;
        cachePtr = nullptr;
    }
}

LruCache::LruCache(size_t allowMaxsize, size_t allowMaxMemory)
    : allowMaxSize_(allowMaxsize > 0 && allowMaxsize <= defaultMaxSize ? allowMaxsize : defaultSize),
    allowMaxMemory_(allowMaxMemory > 0 && allowMaxMemory <= defaultMaxMemory ? allowMaxMemory : defaultMemory),
    currentMemory_(0),
    cache_(static_cast<size_t>(allowMaxSize_), 0)
{
}

void LruCache::Put(const std::string &key, std::shared_ptr<PixelMapWrapper> value)
{
    cache_.insert(key, value);
}

size_t LruCache::Size()
{
    return cache_.size();
}

bool LruCache::Empty()
{
    return cache_.empty();
}

std::shared_ptr<PixelMapWrapper> LruCache::Get(const std::string &key)
{
    std::shared_ptr<PixelMapWrapper> pixelmap;
    bool result = cache_.tryGet(key, pixelmap);
    if (result) {
        return pixelmap;
    }
    return nullptr;
}

bool LruCache::Contains(const std::string &key) const
{
    return cache_.contains(key);
}
}