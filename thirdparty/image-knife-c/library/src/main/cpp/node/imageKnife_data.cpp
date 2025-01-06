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


#include "imageKnife_data.h"
#include <cstdint>
#include "hilog/log.h"

namespace ImageKnifeC {
bool OptionData::GetString(std::string &result) const
{
    if ((DataType::STRING & availableType_) && (type_ == DataType::STRING)) {
        result = data_.str;
        return true;
    }
    return false;
}

std::string OptionData::ToString() const
{
    if ((DataType::STRING & availableType_) && (type_ == DataType::STRING)) {
        return data_.str;
    } else if ((DataType::NUMBER & availableType_) && (type_ == DataType::NUMBER)) {
        return std::to_string(data_.number);
    } else if ((DataType::RESOURCE & availableType_) && (type_ == DataType::RESOURCE)) {
        return "[Resource]: " + data_.resource.bundleName + "," +
               data_.resource.moduleName + "," +  std::to_string(data_.resource.id);
    } else if (type_ == DataType::UNDEFINED) {
        return "[Undefined]";
    } else {
        return "[Object]";
    }
}

bool OptionData::GetNumber(int& result) const
{
    if ((DataType::NUMBER & availableType_) && (type_ == DataType::NUMBER)) {
        result = data_.number;
        return true;
    }
    return false;
}

bool OptionData::GetPixelMap(OH_PixelmapNative*& result) const
{
    if ((DataType::PIXELMAP & availableType_) && (type_ == DataType::PIXELMAP) && (data_.pixelMap != nullptr)) {
        result = data_.pixelMap;
        return true;
    }
    return false;
}

bool OptionData::MovePixelMap(OH_PixelmapNative *&result)
{
    if ((DataType::PIXELMAP & availableType_) && (type_ == DataType::PIXELMAP) && (data_.pixelMap != nullptr)) {
        result = data_.pixelMap;
        data_.pixelMap = nullptr;
        type_ = DataType::UNDEFINED;
        return true;
    }
    return false;
}

bool OptionData::GetResource(Resource &result) const
{
    if ((DataType::RESOURCE & availableType_) && (type_ == DataType::RESOURCE)) {
        result = data_.resource;
        return true;
    }
    return false;
}

void OptionData::SetString(std::string str)
{
    if (!(DataType::STRING & availableType_)) {
        return;
    }
    CleanData();
    new (&data_.str) std::string(str);
    type_ = DataType::STRING;
}

void OptionData::SetNumber(int number)
{
    if (!(DataType::NUMBER & availableType_)) {
        return;
    }
    CleanData();
    data_.number = number;
    type_ = DataType::NUMBER;
}

void OptionData::SetPixelMap(OH_PixelmapNative *pixelMap)
{
    if (!(DataType::PIXELMAP & availableType_)) {
        return;
    }
    CleanData();
    data_.pixelMap = pixelMap;
    type_ = DataType::PIXELMAP;
}

// 没有提供直接的拷贝接口，需要自己实现
void OptionData::CopyFromPixelmap(OH_PixelmapNative *source)
{
    if (source == nullptr) {
        return;
    }

    if (!(DataType::PIXELMAP & availableType_)) {
        return;
    }
    CleanData();
    data_.pixelMap = nullptr;
    type_ = DataType::PIXELMAP;

    OH_Pixelmap_ImageInfo *imageInfo = nullptr;
    if (OH_PixelmapImageInfo_Create(&imageInfo) != IMAGE_SUCCESS) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                     "OptionData",
                     "CopyFromPixelmap create ImageInfo failed");
        return;
    }

    if (OH_PixelmapNative_GetImageInfo(source, imageInfo) != IMAGE_SUCCESS) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                     "OptionData",
                     "CopyFromPixelmap GetImageInfo failed");
        OH_PixelmapImageInfo_Release(imageInfo);
        return;
    }

    OH_Pixelmap_InitializationOptions *options;
    if (OH_PixelmapInitializationOptions_Create(&options) != IMAGE_SUCCESS) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                     "OptionData",
                     "CopyFromPixelmap create option failed");
        OH_PixelmapImageInfo_Release(imageInfo);
        return;
    }

    int32_t alphaType;
    OH_PixelmapImageInfo_GetAlphaType(imageInfo, &alphaType);
    OH_PixelmapInitializationOptions_SetAlphaType(options, alphaType);

    uint32_t height;
    OH_PixelmapImageInfo_GetHeight(imageInfo, &height);
    OH_PixelmapInitializationOptions_SetHeight(options, height);

    uint32_t width;
    OH_PixelmapImageInfo_GetWidth(imageInfo, &width);
    OH_PixelmapInitializationOptions_SetWidth(options, width);

    uint32_t rowStride;
    OH_PixelmapImageInfo_GetRowStride(imageInfo, &rowStride);
    OH_PixelmapInitializationOptions_SetRowStride(options, rowStride);

    int32_t pixelFormat;
    OH_PixelmapImageInfo_GetPixelFormat(imageInfo, &pixelFormat);
    OH_PixelmapInitializationOptions_SetPixelFormat(options, pixelFormat);
    OH_PixelmapInitializationOptions_SetSrcPixelFormat(options, pixelFormat);

    // RGBA格式的缓冲区大小等于width * height * 4
    // NV21与NV12格式的缓冲区大小等于width * height+((width+1)/2) * ((height+1)/2) * 2
    size_t bufferSize;
    const int half = 2;
    const int four = 4;
    if (pixelFormat == PIXEL_FORMAT_NV21 || pixelFormat == PIXEL_FORMAT_NV12) {
        bufferSize = width * height + ((width + 1) / half) * ((height + 1) / half) * half;
    } else {
        bufferSize = width * height * four;
    }
    
    OH_LOG_Print(LOG_APP, LOG_DEBUG, LOG_DOMAIN,
                 "OptionData",
                 "CopyFromPixelmap bufferSize: %{public}d",
                 bufferSize);
    uint8_t *buffer = static_cast<uint8_t*>(malloc(bufferSize));
    if (buffer == nullptr) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                     "OptionData",
                     "Failed to allocate memory, format:%{public}d , bufferSize:%{public}d",
                     pixelFormat,
                     bufferSize);
        OH_PixelmapInitializationOptions_Release(options);
        OH_PixelmapImageInfo_Release(imageInfo);
    }

    if (OH_PixelmapNative_ReadPixels(source, buffer, &bufferSize) != IMAGE_SUCCESS) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                     "OptionData",
                     "CopyFromPixelmap ReadPixels failed, format:%{public}d , bufferSize:%{public}ld",
                     pixelFormat,
                     bufferSize);
        OH_PixelmapInitializationOptions_Release(options);
        OH_PixelmapImageInfo_Release(imageInfo);
        free(buffer);
        return;
    }

    // 使用buffer 新建pixelMap 完成拷贝
    if (OH_PixelmapNative_CreatePixelmap(buffer, bufferSize, options, &data_.pixelMap) != IMAGE_SUCCESS) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                     "OptionData",
                     "CopyFromPixelmap Copy failed!");
    }

    OH_PixelmapInitializationOptions_Release(options);
    OH_PixelmapImageInfo_Release(imageInfo);
    if (buffer != nullptr) {
        free(buffer);
        buffer = nullptr;
    }
}

void OptionData::SetResource(Resource resource)
{
    if (!(DataType::RESOURCE & availableType_)) {
        return;
    }
    CleanData();
    new (&data_.resource) Resource(resource);
    type_ = DataType::RESOURCE;
}

void OptionData::SetValue(napi_env env, napi_value &value)
{
    napi_valuetype type;
    napi_status status = napi_typeof(env, value, &type);

    if (type == napi_string) {
        SetString(ParseNapiString(env, value));
    } else if (type == napi_number) {
        int num;
        status = napi_get_value_int32(env, value, &num);
        SetNumber(num);
    } else if (type == napi_object) {
        // 通过成员属性名判断是否为pixelMap
        bool hasProperty = false;
        status = napi_has_named_property(env, value, "createAlphaPixelmap", &hasProperty);
        if (status == napi_ok && hasProperty) {
            OH_PixelmapNative *pixelmap;
            if (OH_PixelmapNative_ConvertPixelmapNativeFromNapi(env, value, &pixelmap) == IMAGE_SUCCESS) {
                SetPixelMap(pixelmap);
            }
            return;
        }

        // 通过成员属性名判断是否为Resource
        status = napi_has_named_property(env, value, "id", &hasProperty);
        if (status == napi_ok && hasProperty) {
            Resource resource;
            napi_value id, bundleName, moduleName, number;
            status = napi_get_named_property(env, value, "id", &id);
            if (status == napi_ok) {
                napi_get_value_int32(env, id, &resource.id);
            }

            status = napi_get_named_property(env, value, "bundleName", &bundleName);
            if (status == napi_ok) {
                resource.bundleName = ParseNapiString(env, bundleName);
            }

            status = napi_get_named_property(env, value, "moduleName", &moduleName);
            if (status == napi_ok) {
                resource.moduleName = ParseNapiString(env, moduleName);
            }

            status = napi_get_named_property(env, value, "number", &number);
            if (status == napi_ok) {
                napi_get_value_int32(env, number, &resource.number);
            }

            SetResource(resource);
        }
    }
}

std::string OptionData::ParseNapiString(napi_env env, napi_value value)
{
    size_t length;
    napi_status status = napi_get_value_string_utf8(env, value, nullptr, 0, &length);
    char buf[length + 1];
    status = napi_get_value_string_utf8(env, value, buf, length + 1, nullptr);
    return buf;
}

bool OptionData::IsUndefined()
{
    if ((DataType::UNDEFINED & availableType_) && type_ == DataType::UNDEFINED) {
        return true;
    }
    return false;
}

void OptionData::SetUndefined()
{
    if (!(DataType::UNDEFINED & availableType_)) {
        return;
    }
    CleanData();
    type_ = DataType::UNDEFINED;
}

void OptionData::CleanData()
{
    if (type_ == DataType::UNDEFINED) {
        return;
    }
    switch (type_) {
        case DataType::STRING :
            data_.str.~basic_string();
            break;
        case DataType::RESOURCE :
            data_.resource.~Resource();
            break;
        case DataType::PIXELMAP :
            if (data_.pixelMap != nullptr) {
                OH_PixelmapNative_Release(data_.pixelMap);
                data_.pixelMap = nullptr;
            }
            break;
        default:
            return;
    }
}

void Context::SetContext(napi_env env, napi_value value)
{
    bool hasProperty = false;
    napi_status status = napi_has_named_property(env, value, "resourceManager", &hasProperty);
    if (status == napi_ok && hasProperty) {
        napi_value jsResMgr;
        status = napi_get_named_property(env, value, "resourceManager", &jsResMgr);
        if (status == napi_ok) {
            resourceManager = OH_ResourceManager_InitNativeResourceManager(env, jsResMgr);
        }
    }
}

void BorderOption::SetBorderOption(napi_env env, napi_value &value)
{
    napi_status status = napi_has_named_property(env, value, "width", &hasWidth);
    if (status == napi_ok && hasWidth) {
        napi_value width;
        napi_get_named_property(env, value, "width", &width);
        ParseWidth(env, width);
    }

    status= napi_has_named_property(env, value, "color", &hasColor);
    if (status == napi_ok && hasColor) {
        napi_value color;
        napi_get_named_property(env, value, "color", &color);
        ParseColor(env, color);
    }

    status= napi_has_named_property(env, value, "radius", &hasRadius);
    if (status == napi_ok && hasRadius) {
        napi_value radius;
        napi_get_named_property(env, value, "radius", &radius);
        ParseRadius(env, radius);
    }

    status= napi_has_named_property(env, value, "style", &hasStyle);
    if (status == napi_ok && hasStyle) {
        napi_value style;
        napi_get_named_property(env, value, "style", &style);
        ParseStyle(env, style);
    }
}

void BorderOption::ParseWidth(napi_env env, napi_value &value)
{
    bool hasProperty = false;
    bool typeMatched = false;
    napi_status status;

    // LocalizedEdgeWidths
    std::vector<std::string> names = {"top", "end", "bottom", "start"};
    const int namesSize = 4;
    for (int i = 0; i < namesSize; i++) {
        napi_value lengthMetrics;
        status = napi_has_named_property(env, value, names[i].c_str(), &hasProperty);
        if (status == napi_ok && hasProperty) {
            napi_get_named_property(env, value, names[i].c_str(), &lengthMetrics);
            status = napi_has_named_property(env, lengthMetrics, "value", &hasProperty);
            if (status == napi_ok && hasProperty) {
                typeMatched = true;
            }
        }
    }

    if (typeMatched) {
        OH_LOG_Print(LOG_APP, LOG_WARN, LOG_DOMAIN,
                     "BorderOption",
                     "Border width only support number for now");
        hasWidth = false;
        return;
    }

    // EdgeWidths
    names = {"top", "right", "bottom", "left"};

    for (int i = 0; i < namesSize; i++) {
        napi_value length;
        status = napi_has_named_property(env, value, names[i].c_str(), &hasProperty);
        if (status == napi_ok && hasProperty) {
            typeMatched = true;
            napi_get_named_property(env, value, names[i].c_str(), &length);
            width[i] = ParseLength(env, length);
        }
    }

    // Length for four edge
    if (!typeMatched) {
        width[0] = ParseLength(env, value);
    }
}

void BorderOption::ParseRadius(napi_env env, napi_value &value)
{
    bool hasProperty = false;
    bool typeMatched = false;
    napi_status status;

    std::vector<std::string> names = {"topLeft", "topRight", "bottomLeft", "bottomRight"};
    const int namesSize = 4;
    for (int i = 0; i < namesSize; i++) {
        napi_value lengthValue;
        status = napi_has_named_property(env, value, names[i].c_str(), &hasProperty);
        if (status == napi_ok && hasProperty) {
            typeMatched = true;
            napi_get_named_property(env, value, names[i].c_str(), &lengthValue);
            // LocalizedBorderRadiuses
            status = napi_has_named_property(env, lengthValue, "value", &hasProperty);
            if (status == napi_ok && hasProperty) {
                OH_LOG_Print(LOG_APP, LOG_WARN, LOG_DOMAIN,
                             "BorderOption",
                             "Border radius only support number for now");
                hasRadius = false;
            } else { //  BorderRadiuses
                radius[i] = ParseLength(env, lengthValue);
            }
        }
    }

    if (!typeMatched) {
        radius[0] = ParseLength(env, value);
    }
}

void BorderOption::ParseColor(napi_env env, napi_value &value)
{
    bool hasProperty = false;
    bool typeMatched = false;
    napi_status status;

    // LocalizedEdgeColors |  EdgeColors
    napi_value resourceColor;
    std::vector<std::string> names = {"top", "end", "bottom", "start"};
    const int namesSize = 4;
    for (int i = 0; i < namesSize; i++) {
        status = napi_has_named_property(env, value, names[i].c_str(), &hasProperty);
        if (status == napi_ok && hasProperty) {
            typeMatched = true;
            napi_get_named_property(env, value, names[i].c_str(), &resourceColor);
            color[i] = ParseResourceColor(env, resourceColor);
        }
    }

    status = napi_has_named_property(env, value, "right", &hasProperty);
    if (status == napi_ok && hasProperty) {
        typeMatched = true;
        napi_get_named_property(env, value, "right", &resourceColor);
        color[1] = ParseResourceColor(env, resourceColor);
    }

    status = napi_has_named_property(env, value, "left", &hasProperty);
    const int colorLeftIndex = 3;
    if (status == napi_ok && hasProperty) {
        typeMatched = true;
        napi_get_named_property(env, value, "left", &resourceColor);
        color[colorLeftIndex] = ParseResourceColor(env, resourceColor);
    }

    // ResourceColor
    if (!typeMatched) {
        color[0] = ParseResourceColor(env, value);
    }
}

void BorderOption::ParseStyle(napi_env env, napi_value &value)
{
    bool hasProperty = false;
    bool typeMatched = false;
    napi_status status;

    // EdgeStyles
    napi_value styleValue;
    std::vector<std::string> names = {"top", "right", "bottom", "left"};
    const int namesSize = 4;
    for (int i = 0; i < namesSize; i++) {
        status = napi_has_named_property(env, value, names[i].c_str(), &hasProperty);
        if (status == napi_ok && hasProperty) {
            typeMatched = true;
            napi_get_named_property(env, value, names[i].c_str(), &styleValue);
            style[i] = parseBorderStyle(env, styleValue);
        }
    }

    if (!typeMatched) {
        style[0] = parseBorderStyle(env, value);
    }
}

float BorderOption::ParseLength(napi_env env, napi_value &value)
{
    // string | number | Resource
    napi_valuetype type;
    if (napi_typeof(env, value, &type) == napi_ok && type == napi_string) {
        size_t length;
        napi_get_value_string_utf8(env, value, nullptr, 0, &length);
        char buf[length + 1];
        napi_get_value_string_utf8(env, value, buf, length+1, nullptr);
        return std::atof(buf);
    } else if (napi_typeof(env, value, &type) == napi_ok && type == napi_number) {
        double num;
        napi_get_value_double(env, value, &num);
        return num;
    } else {
        OH_LOG_Print(LOG_APP, LOG_WARN, LOG_DOMAIN,
                     "BorderOption",
                     "Border only support Length:number for now");
        return 0;
    }
}

uint32_t BorderOption::ParseResourceColor(napi_env env, napi_value &value)
{
    OH_LOG_Print(LOG_APP, LOG_WARN, LOG_DOMAIN,
                 "BorderOption",
                 "Border only support width radius for now");
    return 0; // black
}

int32_t BorderOption::parseBorderStyle(napi_env env, napi_value &value)
{
    // C: ARKUI_BORDER_STYLE_SOLID = 0, ARKUI_BORDER_STYLE_DASHED, ARKUI_BORDER_STYLE_DOTTED
    // JS: Dotted, Dashed, Solid
    OH_LOG_Print(LOG_APP, LOG_WARN, LOG_DOMAIN,
                 "BorderOption",
                 "Border only support width radius for now");
    return 0;
}

} // end of namespace