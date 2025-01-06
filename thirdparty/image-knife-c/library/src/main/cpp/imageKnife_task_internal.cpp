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

#include "imageKnife_task_internal.h"

namespace ImageKnifeC {

std::string ImageKnifeTaskInternal::GetTaskInfo(ImageKnifeRequestSource type, ImageKnifeRequest *request)
{
    if (request == nullptr) {
        return "[Null Request] ";
    }

    std::string source;
    switch (type) {
        case ImageKnifeRequestSource::MAINSRC:
            source = "MainSrc";
            break;
        case ImageKnifeRequestSource::PLACESRC:
            source = "PlaceSrc";
            break;
        case ImageKnifeRequestSource::ERRORSRC:
            source = "ErrorSrc";
            break;
        default:
            break;
    }
    return "[ID:" + request->GetNodeId() + " VersionID:" + std::to_string(request->GetVersion()) +
           " Type:" + source + "] ";
}

void ImageKnifeTaskInternal::QueryDisplayPixelmapInfo()
{
    if (product.pixelmap == nullptr || product.pixelmap->GetPixelMap() == nullptr) {
        return;
    }

    OH_Pixelmap_ImageInfo* imageInfo = nullptr;
    if (OH_PixelmapImageInfo_Create(&imageInfo) != IMAGE_SUCCESS) {
        return;
    }

    if (OH_PixelmapNative_GetImageInfo(product.pixelmap->GetPixelMap(), imageInfo) != IMAGE_SUCCESS) {
        return;
    }

    int32_t pixelFormat = PIXEL_FORMAT_UNKNOWN;
    OH_PixelmapImageInfo_GetWidth(imageInfo, &pixelmapInfo.width);
    OH_PixelmapImageInfo_GetHeight(imageInfo, &pixelmapInfo.height);
    OH_PixelmapImageInfo_GetPixelFormat(imageInfo, &pixelFormat);
    OH_PixelmapImageInfo_Release(imageInfo);
    const int half = 2;
    const int four = 4;
    if (pixelFormat == PIXEL_FORMAT_NV21 || pixelFormat == PIXEL_FORMAT_NV12) {
        pixelmapInfo.size = pixelmapInfo.width * pixelmapInfo.height +
                            ((pixelmapInfo.width + 1) / half) * ((pixelmapInfo.height + 1) / half) * half;
    } else {
        pixelmapInfo.size = pixelmapInfo.width * pixelmapInfo.height * four;
    }

    switch (pixelFormat) {
        case PIXEL_FORMAT_RGB_565 :
            pixelmapInfo.pixelFormat = "RGB_565";
            break;
        case PIXEL_FORMAT_RGBA_8888 :
            pixelmapInfo.pixelFormat = "RGBA_8888";
            break;
        case PIXEL_FORMAT_BGRA_8888 :
            pixelmapInfo.pixelFormat = "BGRA_8888";
            break;
        case PIXEL_FORMAT_RGB_888 :
            pixelmapInfo.pixelFormat = "RGB_888";
            break;
        case PIXEL_FORMAT_ALPHA_8 :
            pixelmapInfo.pixelFormat = "ALPHA_8";
            break;
        case PIXEL_FORMAT_RGBA_F16 :
            pixelmapInfo.pixelFormat = "RGBA_F16";
            break;
        case PIXEL_FORMAT_NV21 :
            pixelmapInfo.pixelFormat = "NV21";
            break;
        case PIXEL_FORMAT_NV12 :
            pixelmapInfo.pixelFormat = "NV12";
            break;
        case PIXEL_FORMAT_RGBA_1010102 :
            pixelmapInfo.pixelFormat = "RGBA_1010102";
            break;
        case PIXEL_FORMAT_YCBCR_P010 :
            pixelmapInfo.pixelFormat = "YCBCR_P010";
            break;
        case PIXEL_FORMAT_YCRCB_P010 :
            pixelmapInfo.pixelFormat = "YCRCB_P010";
            break;
        default :
            pixelmapInfo.pixelFormat = "Unknown";
            break;
    }
}

std::string ImageKnifeTaskInternal::GetDisplayPixelmapInfo() const
{
    return "Pixelmap Info: width:" + std::to_string(pixelmapInfo.width) +
           " height:" + std::to_string(pixelmapInfo.height) +
           " size:" + std::to_string(pixelmapInfo.size) +
           " format:" + pixelmapInfo.pixelFormat;
}
} // end of namespace
