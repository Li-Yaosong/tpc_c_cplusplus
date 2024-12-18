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

#ifndef IMAGE_KNIFE_C_FILE_CACHE_H
#define IMAGE_KNIFE_C_FILE_CACHE_H
#include <mutex>
#include <string>
#include <future>

#include "lru_cache.h"
#include "thirdparty/LRUCache11.h"
#include "imageKnife_request.h"
#include "imageKnife_task.h"

namespace ImageKnifeC {
class FileCache {
public:
    struct FileData {
        std::shared_ptr<uint8_t[]> buffer = nullptr;
        uint32_t size = 0;
    };

    struct FileInfo {
        std::string fileName;
        time_t lastAccessTime;
        size_t fileSize; // 文件大小
    };

    struct AsyncData {
        std::string fileDir;
        int size;
        int memory;
        std::string path;
        napi_deferred deferred;
        napi_async_work asyncWork;
    };

    static FileCache *GetInstance();
    static void DestroyInstance();

    //初始化lru,内存数量和大小
    void Init(int size, int memory);

    // 禁止复制构造和赋值
    FileCache(const FileCache&) = delete;
    FileCache& operator=(const FileCache&) = delete;

    size_t Size();

    bool Empty();

    void Put(const std::string &key, size_t value);

    size_t Get(const std::string &key);

    bool Contains(const std::string &key) const;

    void InitFileCache();

    void SaveImageToDisk(const char *buffer, uint32_t length, std::string &fileKey);
    bool GetImageFromDisk(std::string &fileKey, FileData &fileData);

    static void StartFileCacheInitThread(napi_env env, void *arg);
    static void FileCacheInitComplete(napi_env env, napi_status status, void *data);

    std::string filePath;
    bool isFileInitComplete_;

    static AsyncData *asyncData;
    std::promise<std::string> promise;

private:
    explicit FileCache(size_t allowMaxsize, size_t allowMaxMemory);
    ~FileCache() = default;

    bool DeleteFile(const std::string &fileName);

    void AddMemorySize(size_t value);
    void RemoveMemorySize(size_t value);

    static const long defaultSize;
    static const long defaultMemory;
    static const long defaultMaxSize;
    static const long defaultMaxMemory;

    // 聚合的缓存实例
    static lru11::Cache<std::string, size_t> *cachePtr;
    static FileCache *fileCachePtr;

    long allowMaxSize_;
    long allowMaxMemory_;
    long currentMemory_;

    // 记录线程中图片下载/加载/更新中的错误信息
    std::string lastError_;

    // pixelMap native
    OH_PixelmapNative *pixelMap_ = nullptr;

    // pixelMap图片desc
    void *imgDesc_ = nullptr;

    std::mutex lock_;
};

} // namespace ImageKnifeC

#endif // IMAGE_KNIFE_C_FILE_CACHE_H
