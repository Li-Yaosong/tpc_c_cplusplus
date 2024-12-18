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

#ifndef IMAGE_KNIFE_C_IMAGEKNIFE_TASK_H
#define IMAGE_KNIFE_C_IMAGEKNIFE_TASK_H

#include "RemoteCommunicationKit/rcp.h"
#include <multimedia/image_framework/image/pixelmap_native.h>
#include <memory>
#include <string>
#include "hilog/log.h"
#include "imageKnife_data.h"

namespace ImageKnifeC {
class ImageKnifeRequest;
class ImageData;
class Interceptor;
enum class ImageKnifeRequestSource {
    MAINSRC,
    PLACESRC,
    ERRORSRC
};

struct PixelMapWrapper {
public:
    PixelMapWrapper(OH_PixelmapNative *pixelmap) : pixelmap_(pixelmap)
    {
    }

    OH_PixelmapNative *GetPixelMap()
    {
        return pixelmap_;
    }

    ~PixelMapWrapper()
    {
        if (pixelmap_ == nullptr) {
            return;
        }

        OH_PixelmapNative_Release(pixelmap_);
    }
private:
    // 禁止修改pixelmap地址，防止错误析构和内存泄漏
    OH_PixelmapNative *pixelmap_ = nullptr;
};

class ImageKnifeTask {
public:
    struct Product {
        // 网络下载session，用于取消下载
        Rcp_Session *session = nullptr;
        // 网络下载request，用于取消下载
        Rcp_Request *request = nullptr;
        // 网络下载, http响应数据
        Rcp_Response *response = nullptr;
        // 原始图片buffer
        std::shared_ptr<uint8_t[]> imageBuffer = nullptr;
        // 原始图片长度
        uint32_t imageLength = 0;
        // pixelmap 图片
        std::shared_ptr<PixelMapWrapper> pixelmap = nullptr;
        // 标记当前是需要写缓存还是读缓存
        bool writeCache = false;

        ~Product()
        {
            if (response != nullptr) {
                response->destroyResponse(response);
            }
        }
    };

    const std::shared_ptr<ImageKnifeRequest> request;
    const ImageKnifeRequestSource type;
    const ImageData *imageSrc;
    Product product;
    std::string cancelInfo;

    virtual ~ImageKnifeTask() = default;
    virtual void EchoError(std::string errorInfo) = 0;
protected:
    ImageKnifeTask(ImageKnifeRequestSource type, std::shared_ptr<ImageKnifeRequest> request, ImageData *imageSrc)
        : type(type), request(request), imageSrc(imageSrc)
    {
    }
};

}

#endif // IMAGE_KNIFE_C_IMAGEKNIFE_TASK_H
