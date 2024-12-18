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

#ifndef IMAGE_KNIFE_C_RESOURCE_INTERCEPTOR_OH_H
#define IMAGE_KNIFE_C_RESOURCE_INTERCEPTOR_OH_H

#include "interceptor.h"
#include "imageKnife_request.h"
#include <cstdint>

namespace ImageKnifeC {
class ResourceInterceptorOH : public DownloadInterceptor {
public:
    ResourceInterceptorOH()
    {
        name = "Default ResourceInterceptor";
    }

    bool Resolve(ImageKnifeTask *task) override
    {
        Resource resource;
        if (task->imageSrc->GetResource(resource)) {
            return LoadImageFromResource(resource, task);
        }

        return false;
    }

private:
    bool LoadImageFromResource(Resource resource, ImageKnifeTask *task)
    {
        auto option =  task->request->GetImageKnifeOption();
        if (option->context.resourceManager == nullptr) {
            task->EchoError("Get context.resourceManager failed");
            return false;
        }

        uint64_t length = 0;
        ResourceManager_ErrorCode errCode;
        uint8_t *buffer = nullptr;
        errCode = OH_ResourceManager_GetMedia(option->context.resourceManager,
                                              resource.id,
                                              &buffer,
                                              &length);
        if (errCode != ResourceManager_ErrorCode::SUCCESS) {
            // 获取图片资源失败
            task->EchoError("Get image from resource failed, error code: " + std::to_string(errCode));
            return false;
        }

        if (length > UINT32_MAX) {
            // 默认拦截器支持的length类型为uint32_t
            // TODO 这里使用delete 还是free ？
            free(buffer);
            task->EchoError("Resource Image size out of range!");
            return false;
        }

        task->product.imageBuffer = std::shared_ptr<uint8_t[]>(buffer);
        task->product.imageLength = length;
        return true;
    }
};

};

#endif // IMAGE_KNIFE_C_RESOURCE_INTERCEPTOR_OH_H
