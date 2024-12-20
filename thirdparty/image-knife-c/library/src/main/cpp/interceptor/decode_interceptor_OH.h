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

#ifndef IMAGE_KNIFE_C_DECODE_INTERCEPTOR_OH_H
#define IMAGE_KNIFE_C_DECODE_INTERCEPTOR_OH_H

#include "interceptor.h"
#include "imageKnife_request.h"
#include <multimedia/image_framework/image/image_source_native.h>

namespace ImageKnifeC {
class DecodeInterceptorOH : public DecodeInterceptor {
public:
    DecodeInterceptorOH()
    {
        name = "Default DecodeInterceptor";
    }

    bool Resolve(ImageKnifeTask *task) override
    {
        // 确定数据来源是网络下载response 还是本地文件buffer
        if (task->product.response != nullptr) {
            return Decode((uint8_t*)(task->product.response->body.buffer), task->product.response->body.length, task);
        } else if (task->product.imageBuffer != nullptr) {
            return Decode(task->product.imageBuffer.get(), task->product.imageLength, task);
        }
        return false;
    }

private:
    bool Decode(uint8_t *buffer, uint32_t length, ImageKnifeTask *task)
    {
        OH_ImageSourceNative *source = nullptr;
        Image_ErrorCode imgErrorCode = OH_ImageSourceNative_CreateFromData(buffer, length, &source);
        if (imgErrorCode != IMAGE_SUCCESS) {
            task->EchoError("create ImageSource failed, error code: " + std::to_string(imgErrorCode));
            return false;
        }

        OH_PixelmapNative *pixelmap = nullptr;
        imgErrorCode = OH_ImageSourceNative_CreatePixelmap(source, nullptr, &pixelmap);
        if (imgErrorCode != IMAGE_SUCCESS) {
            task->EchoError("decode image to pixelMap failed, error code: " + std::to_string(imgErrorCode));
            return false;
        }

        task->product.pixelmap = std::make_shared<PixelMapWrapper>(pixelmap);
        OH_ImageSourceNative_Release(source);
        return true;
    }
};

};

#endif // IMAGE_KNIFE_C_DECODE_INTERCEPTOR_OH_H
