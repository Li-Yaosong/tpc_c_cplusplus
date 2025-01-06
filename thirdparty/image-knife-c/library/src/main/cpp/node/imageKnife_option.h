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

#ifndef IMAGE_KNIFE_C_IMAGEKNIFE_OPTION_H
#define IMAGE_KNIFE_C_IMAGEKNIFE_OPTION_H

#include "imageKnife_loader.h"
#include "napi/native_api.h"
#include "imageKnife_data.h"
#include <string>
#include <functional>
#include <arkui/native_type.h>
#include "transformation/multi_transformation.h"

namespace ImageKnifeC {

struct OnLoadCallBack {
    void SetCallBack(napi_env env, napi_value value);
    std::function<void()> onLoadStart = nullptr;
    std::function<void(ImageDataBack &data, ImageKnifeData &imageKnifeData)> onLoadSuccess = nullptr;
    std::function<void(std::string err)> onLoadFailed = nullptr;
    std::function<void(std::string reason)> onLoadCancel = nullptr;
    ~OnLoadCallBack();
private:
    napi_env env_;
    napi_ref jsThis_;
    napi_ref onLoadStart_;
    napi_ref onLoadSuccess_;
    napi_ref onLoadFailed_;
    napi_ref onLoadCancel_;

    // 标记使用的ArkTs传入的回调 还是C函数回调
    bool enableJsFunction_ = false;

    void CallJsOnLoadStart();

    void CallJsOnLoadSuccess(ImageDataBack &data, ImageKnifeData &imageKnifeData);

    void CallJsOnLoadFailed(std::string err);

    void CallJsOnLoadCancel(std::string reason);
};

class ImageKnifeOption {
public:
    // 主图资源: STRING | PIXELMAP | RESOURCE
    ImageData loadSrc;
    // 占位图: STRING | PIXELMAP | RESOURCE
    ImageData placeholderSrc;
    // error占位图: STRING | PIXELMAP | RESOURCE
    ImageData errorholderSrc;

    std::string signature;

    int priority;

    // 主图填充效果 默认为cover
    ArkUI_ObjectFit objectFit = ARKUI_OBJECT_FIT_COVER;
    // 占位图填充效果 默认为cover
    ArkUI_ObjectFit placeholderObjectFit = ARKUI_OBJECT_FIT_COVER;
    // 错误图填充效果 默认为cover
    ArkUI_ObjectFit errorholderObjectFit = ARKUI_OBJECT_FIT_COVER;

    BorderOption border;

    OnLoadCallBack onLoadListener;
    // 加载显示完成后的回调 可映射ArtTs的onComplete
    std::function<void(EventImage)> onComplete = nullptr;

    Context context;

    std::shared_ptr<ImageKnifeLoader> imageLoader = nullptr;

    std::shared_ptr<Transformation> transformation = nullptr;

    std::shared_ptr<MultiTransformation> multiTransformation = nullptr;
    // 用户自定义数据类型，可在自定义拦截器中取得使用
    void* customData = nullptr;
    // 用户自定义finalize回调，可用来析构用户自定义数据
    std::function<void(void* customData)> onFinalize = nullptr;

    ImageKnifeOption(napi_env env, napi_value& jsObject);

    ~ImageKnifeOption()
    {
        if (onFinalize != nullptr) {
            onFinalize(customData);
        }

        if (enableJsOption_) {
            if (onComplete != nullptr) {
                napi_delete_reference(env_, onComplete_);
            }

            napi_delete_reference(env_, jsThis_);
        }
    }

private:
    napi_env env_;
    napi_ref jsThis_;
    napi_ref onComplete_;

    // 标记使用的ArkTs传入option 还是C版本
    bool enableJsOption_ = false;

    ArkUI_ObjectFit ParseImagFit(napi_env env, napi_value &value);

    void CallJsOnComplete(EventImage &eventImage);
};

} // end of namespace

#endif // IMAGE_KNIFE_C_IMAGEKNIFE_OPTION_H
