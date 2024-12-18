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


#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <ctime>
#include <algorithm>
#include <cstdio>
#include <cerrno>
#include <cstring>

#include "file_cache.h"
#include "native_entry.h"
#include "imageKnife.h"

namespace ImageKnifeC {

const long FileCache::defaultSize = 512;
const long FileCache::defaultMemory = 128 * 1024 * 1024;
const long FileCache::defaultMaxSize = 4096;
const long FileCache::defaultMaxMemory = 512 * 1024 * 1024;
FileCache::AsyncData *FileCache::asyncData = new FileCache::AsyncData();

FileCache *FileCache::fileCachePtr = nullptr;
lru11::Cache<std::string, size_t> *FileCache::cachePtr = nullptr;

void FileCache::Init(int size, int memory) 
{
    if(!cachePtr) {
       cachePtr = new lru11::Cache<std::string, size_t>(size, 0);
    }

   allowMaxSize_ = (size > 0 && size <= defaultMaxSize) ? size : defaultSize;
   allowMaxMemory_ = (memory > 0 && memory <= defaultMaxMemory) ? memory : defaultMemory;
}

FileCache *FileCache::GetInstance()
{
#define DEFAULT_MAX_SIZE 50
#define DEFAULT_MAX_MEMORY 100 * 1024 * 1024
    if (!fileCachePtr) {
        // 未正确调用情况下使用的默认数据
        size_t allowMaxsize = DEFAULT_MAX_SIZE;
        size_t allowMaxMemory = DEFAULT_MAX_MEMORY;
        fileCachePtr = new FileCache(allowMaxsize, allowMaxMemory);
    }
    return fileCachePtr;
}

void FileCache::DestroyInstance()
{
    if (fileCachePtr) {
        delete fileCachePtr;
        fileCachePtr = nullptr;
    }
    if (asyncData) {
        delete asyncData;
        asyncData = nullptr;
    }

    if (cachePtr) {
        delete cachePtr;
        cachePtr = nullptr;
    }
}

FileCache::FileCache(size_t allowMaxsize, size_t allowMaxMemory)
    : allowMaxSize_(allowMaxsize > 0 && allowMaxsize <= defaultMaxSize ? allowMaxsize : defaultSize),
    allowMaxMemory_(allowMaxMemory > 0 && allowMaxMemory <= defaultMaxMemory ? allowMaxMemory : defaultMemory),
    currentMemory_(0),
    isFileInitComplete_(false)
{
}

void FileCache::Put(const std::string &key, size_t value)
{
    if (!isFileInitComplete_) {
        return;
    }

    if (key == "") {
        return ;
    }

    while (currentMemory_ >= allowMaxMemory_ || cachePtr->size() >= allowMaxSize_) {
        std::string keyOut;
        size_t valueOut;
        if (cachePtr->getTail(keyOut, valueOut)) {
            cachePtr->remove(keyOut);
            DeleteFile(keyOut);
            RemoveMemorySize(valueOut);  
        }
    }
    cachePtr->insert(key, value);
    AddMemorySize(value);
}

size_t FileCache::Size()
{
    return cachePtr->size();
}

bool FileCache::Empty()
{
    return cachePtr->empty();
}

size_t FileCache::Get(const std::string &key)
{
    if (!isFileInitComplete_) {
        return 0;
    }
    size_t fileSize = 0;

    bool result = cachePtr->tryGet(key, fileSize);
    if (!result) {
        return 0;
    }
    return fileSize;
}

bool FileCache::Contains(const std::string &key) const
{
    return cachePtr->contains(key);
}

bool FileCache::DeleteFile( const std::string &fileName)
{
    std::string path = filePath + "/" + fileName;
    if (remove(path.c_str()) != 0) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN, "FileCache::DeleteFile", "DeleteFile failed");
        return false;
    }
    return true;
}

void FileCache::InitFileCache()
{
    DIR* dir = opendir(filePath.c_str());
    if (dir == nullptr) {
        OH_LOG_Print(LOG_APP, LOG_DEBUG, LOG_DOMAIN, "FileCache::DeleteFile",
                     "Cannot open directory %{public}s", filePath.c_str());

        // 设置promise为错误信息
        promise.set_value("Error: Cannot open directory: " + filePath);
        return;
    }
    struct dirent *entry;
    std::vector<FileInfo> filesVector;
    while ((entry = readdir(dir)) != nullptr) {
        std::string fileName = entry->d_name;

        // 跳过 "." 和 ".." 目录
        if (fileName == "." || fileName == "..") {
            continue;
        }

        std::string fullPath = filePath + "/" + fileName;

        struct stat fileStat;
        if (stat(fullPath.c_str(), &fileStat) == 0) {
            // 如果是普通文件，将其添加到 filesVector 中
            if (S_ISREG(fileStat.st_mode)) {
                FileInfo fileInfo;
                fileInfo.fileName = fileName;
                // 访问时间
                fileInfo.lastAccessTime = fileStat.st_atime;
                // 文件大小
                fileInfo.fileSize = fileStat.st_size;
                filesVector.push_back(fileInfo);
            }
        }
    }

    closedir(dir);

    // 最近访问的排在后面
    std::sort(filesVector.begin(), filesVector.end(), [](const FileInfo &a, const FileInfo &b)
    {
        return a.lastAccessTime < b.lastAccessTime; 
    });

    for (const auto &fileInfo : filesVector) {
        while (currentMemory_ >= allowMaxMemory_ || cachePtr->size() >= allowMaxSize_) {
            std::string keyOut;
            size_t valueOut;
            if (cachePtr->getTail(keyOut, valueOut)) {
                cachePtr->remove(keyOut);
                DeleteFile(keyOut);
                RemoveMemorySize(valueOut);
            }
        }
        cachePtr->insert(fileInfo.fileName, fileInfo.fileSize);
        AddMemorySize(fileInfo.fileSize);
    }
    // 标识位置true
    isFileInitComplete_ = true;

    // 设置promise为成功信息
    promise.set_value("File cache initialized successfully");
}

void FileCache::AddMemorySize(size_t value)
{
    currentMemory_ += value;
}

void FileCache::RemoveMemorySize(size_t value)
{
    currentMemory_ -= value;
}

void FileCache::SaveImageToDisk(const char *buffer, uint32_t length, std::string &fileKey)
{
    if (filePath == "") {
        return;
    }

    if (mkdir(filePath.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) != 0) {
        if (errno == EEXIST) {
            OH_LOG_Print(LOG_APP, LOG_DEBUG, LOG_DOMAIN,
                         "FileCache::SaveImageToDisk",
                         "Directory already exists");
        } else {
            OH_LOG_Print(LOG_APP, LOG_ERROR,LOG_DOMAIN,
                         "FileCache::SaveImageToDisk",
                         "Failed to create directory");
            return;
        }
    }

    std::lock_guard<std::mutex> lock(lock_);
    std::string fullPath = filePath + "/" + fileKey;
    FILE *fp = fopen(fullPath.c_str(), "w");
    if (fp) {
        size_t itemsWritten = fwrite(buffer, sizeof(char), length, fp);
        fclose(fp);
        if (itemsWritten == length) {
            // insert into LRU
            Put(fileKey, length);
        } else {
            OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                         "FileCache::SaveImageToDisk",
                         "Failed to write all items to file");
        }
    } else {
            OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                         "FileCache::SaveImageToDisk",
                         "write to file Failed!!");
    }
}

bool FileCache::GetImageFromDisk(std::string &fileKey, FileData &fileData)
{
    std::lock_guard<std::mutex> lock(lock_);
    if (Get(fileKey) == 0) {
        return false;
    }

    std::string fullPath = filePath + "/" + fileKey;

    // 打开文件
    FILE *fp = fopen(fullPath.c_str(), "rb");
    if (!fp) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                     "ImageKnifeNode",
                     "open file failed");
        return false;
    }

    // 获取文件大小
    fseek(fp, 0, SEEK_END);
    fileData.size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // 使用 new 直接分配堆内存
    fileData.buffer = std::shared_ptr<uint8_t[]>(new (std::nothrow) uint8_t[fileData.size]);
    if (!fileData.buffer) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                     "ImageKnifeNode",
                     "Memory allocation failed");
        fclose(fp);
        return false;
    }

    // 读取文件内容到堆上的内存
    const size_t chunkSize = 1024 * 1024;
    fread(fileData.buffer.get(), chunkSize, fileData.size, fp);

    if (ferror(fp)) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                     "ImageKnifeNode",
                     "Error reading file: %{public}s",
                     strerror(errno));
        fclose(fp);
        return false;
    }
    fclose(fp);

    return true;
}
void FileCache::StartFileCacheInitThread(napi_env env, void *data)
{
    ImageKnifeC::ImageKnife::GetInstance().InitFileCache(FileCache::asyncData->fileDir,
                                                         FileCache::asyncData->size,
                                                         FileCache::asyncData->memory,
                                                         FileCache::asyncData->path);
}

void FileCache::FileCacheInitComplete(napi_env env, napi_status status, void *data)
{
    napi_value result;

    // 判断任务是否成功完成，给 result 赋值
    if (status == napi_ok) {
        // 如果成功，可以设置一个成功的结果
        const char *successMessage = "File cache initialization completed successfully.";
        napi_create_string_utf8(env, successMessage, NAPI_AUTO_LENGTH, &result);

        // 解决 deferred Promise，传递成功的 result
        napi_resolve_deferred(env, FileCache::asyncData->deferred, result);
    } else {
        // 如果发生错误，传递错误信息
        const char* errorMessage = "File cache initialization failed.";
        napi_create_string_utf8(env, errorMessage, NAPI_AUTO_LENGTH, &result);

        // 拒绝 deferred Promise，传递错误的 result
        napi_reject_deferred(env, FileCache::asyncData->deferred, result);
    }

    napi_delete_async_work(env, FileCache::asyncData->asyncWork);
    // 释放 AsyncData 结构体的内存
    FileCache::asyncData->asyncWork = nullptr;
    delete FileCache::asyncData;
    FileCache::asyncData = nullptr;
}
}
