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

#ifndef IMAGE_KNIFE_C_IMAGEKNIFE_DATA_H
#define IMAGE_KNIFE_C_IMAGEKNIFE_DATA_H

#include "napi/native_api.h"
#include <cstdint>
#include <multimedia/image_framework/image/pixelmap_native.h>
#include <string>
#include <resourcemanager/ohresmgr.h>

namespace ImageKnifeC {

enum DataType: unsigned int {
    UNDEFINED = 1,
    STRING = 1 << 1,
    NUMBER = 1 << 2,
    PIXELMAP = 1 << 3,
    RESOURCE = 1 << 4,
};

struct ImageKnifeData {
    // source: PixelMap | string
    // OptionData source = OptionData(DataType::)
    int imageWidth = 0;
    int imageHeight = 0;
    // type?:string
    std::string type;
    // imageAnimator?: Array<ImageFrameInfo>
};

struct Resource {
    std::string bundleName;
    std::string moduleName;
    int id;
    int number = 0;
    // params?: any[];
};

// Js UIAbilityContext
struct Context {
    NativeResourceManager *resourceManager = nullptr;
    void SetContext(napi_env env, napi_value value);
};

struct BorderOption {
    // 0: all border or top, 1: right, 2: bottom, 3: left
    float width[4] = {0};
    float radius[4] = {0};
    uint32_t color[4] = {0};
    int32_t style[4] = {0};

    bool percentageWidth = false;
    bool percentageRadius = false;

    bool hasWidth = false;
    bool hasColor = false;
    bool hasRadius = false;
    bool hasStyle = false;

    void SetBorderOption(napi_env env, napi_value &value);
private:
    void ParseWidth(napi_env env, napi_value &value);
    void ParseRadius(napi_env env, napi_value &value);
    void ParseColor(napi_env env, napi_value &value);
    void ParseStyle(napi_env env, napi_value &value);
    
    float ParseLength(napi_env env, napi_value &value);
    uint32_t ParseResourceColor(napi_env env, napi_value &value);
    int32_t parseBorderStyle(napi_env env, napi_value &value);
};

// ArkTs EventImage
struct EventImage {
    int32_t loadingStatus;
    float width;
    float height;
    float componentWidth;
    float componentHeight;
    float contentOffsetX;
    float contentOffsetY;
    float contentWidth;
    float contentHeight;
};

struct OptionData {
public:
    // 类型正确返回true
    bool GetString(std::string &result) const;

    bool GetNumber(int &result) const;
    // 获取pixelMap指针， pixelmap生命周期由该类管理进行释放。
    bool GetPixelMap(OH_PixelmapNative *&result) const;
    // 移交pixelMap管理权，该类不进行pixelmap释放，操作成功返回true，并将类型设成undefined
    bool MovePixelMap(OH_PixelmapNative *&result);

    bool GetResource(Resource &result) const;

    void SetString(std::string str);

    void SetNumber(int number);
    // 设置pixelmap 指针，接管其生命周期
    void SetPixelMap(OH_PixelmapNative *pixelMap);

    void CopyFromPixelmap(OH_PixelmapNative *pixelMap);

    void SetResource(Resource resource);

    void SetValue(napi_env env, napi_value &value);

    bool IsUndefined();

    void SetUndefined();

    std::string ToString() const;

    DataType GetType() const
    {
        return type_;
    }

    OptionData() : type_(DataType::UNDEFINED) {};
    explicit OptionData(unsigned int type) : type_(DataType::UNDEFINED), availableType_(type) {};
    ~OptionData()
    {
        CleanData();
    };
private:
    union ImageSrc {
        std::string str;
        int number;
        // pixelMap has attr: createAlphaPixelmap
        OH_PixelmapNative *pixelMap;
        // Resource has attr: id
        Resource resource;
        // 由OptionData管理
        ImageSrc() : pixelMap(nullptr) {};
        ~ImageSrc() {};
    };

    unsigned int availableType_;
    DataType type_;
    ImageSrc data_;

    void CleanData();
    std::string ParseNapiString(napi_env env, napi_value vaue);
};

struct ImageData : public OptionData {
    ImageData() : OptionData(DataType::STRING | DataType::PIXELMAP | DataType::RESOURCE)
    {
    }

    explicit ImageData(std::string url) : ImageData()
    {
        SetString(url);
    }

    explicit ImageData(const char* url) : ImageData(std::string(url))
    {
    }

    explicit ImageData(OH_PixelmapNative *pixelMap) : ImageData()
    {
        SetPixelMap(pixelMap);
    }

    explicit ImageData(Resource resource) : ImageData()
    {
        SetResource(resource);
    }
};

struct ImageDataBack : public OptionData {
    ImageDataBack() : OptionData(DataType::STRING | DataType::PIXELMAP | DataType::UNDEFINED)
    {
    }

    explicit ImageDataBack(std::string base64) : ImageDataBack()
    {
        SetString(base64);
    }

    explicit ImageDataBack(OH_PixelmapNative *pixelMap) : ImageDataBack()
    {
        SetPixelMap(pixelMap);
    }
};
} // end of namespace

#endif // IMAGE_KNIFE_C_IMAGEKNIFE_DATA_H
