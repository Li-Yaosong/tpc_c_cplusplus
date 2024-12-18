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

#ifndef IMAGE_KNIFE_C_LRU_CACHE_H
#define IMAGE_KNIFE_C_LRU_CACHE_H
#include <mutex>
#include "thirdparty/LRUCache11.h"
#include <string>
#include "imageKnife_task.h"

namespace ImageKnifeC {
class LruCache {
public:
    static LruCache* GetInstance();
    static void DestroyInstance();
    // 禁止复制构造和赋值
    LruCache(const LruCache&) = delete;
    LruCache& operator=(const LruCache&) = delete;
    size_t Size();
    bool Empty();
    void Put(const std::string &key, std::shared_ptr<PixelMapWrapper> value);
    std::shared_ptr<PixelMapWrapper> Get(const std::string &key);
    bool Contains(const std::string &key) const;

private:
    explicit LruCache(size_t allowMaxsize, size_t allowMaxMemory);
    ~LruCache() = default;

    static const long defaultSize;
    static const long defaultMemory;
    static const long defaultMaxSize;
    static const long defaultMaxMemory;
    static LruCache *cachePtr;
    long allowMaxSize_;
    long allowMaxMemory_;
    long currentMemory_;
    std::mutex lock_;
    // 聚合的缓存实例
    lru11::Cache<std::string, std::shared_ptr<PixelMapWrapper>, std::mutex> cache_;
};

} // namespace ImageKnifeC
#endif // IMAGE_KNIFE_C_LRU_CACHE_H
