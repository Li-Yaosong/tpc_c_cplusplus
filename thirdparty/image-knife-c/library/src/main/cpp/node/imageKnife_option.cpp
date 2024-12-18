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

#include "napi/native_api.h"
#include "imageKnife_option.h"
#include "hilog/log.h"
#include "imageKnife.h"

namespace ImageKnifeC {
ImageKnifeOption::ImageKnifeOption(napi_env env, napi_value &jsObject)
{
    enableJsOption_ = true;
    env_ = env;
    napi_create_reference(env, jsObject, 1, &jsThis_);
    imageLoader =  ImageKnife::GetInstance().GetDefaultImageKnifeLoader();

    napi_value value;
    if (napi_get_named_property(env, jsObject, "loadSrc", &value) == napi_ok ) {
        loadSrc.SetValue(env, value);
    }

    if (napi_get_named_property(env, jsObject, "placeholderSrc", &value) == napi_ok) {
        placeholderSrc.SetValue(env, value);
    }

    if (napi_get_named_property(env, jsObject, "errorholderSrc", &value) == napi_ok) {
        errorholderSrc.SetValue(env, value);
    }

    if (napi_get_named_property(env, jsObject, "signature", &value) == napi_ok) {
        napi_valuetype type;
        napi_status status = napi_typeof(env, value, &type);
        if (type == napi_string) {
            size_t length;
            status = napi_get_value_string_utf8(env, value, nullptr, 0, &length);
            char buf[length + 1];
            status = napi_get_value_string_utf8(env, value, buf, length + 1, nullptr);
            signature = buf;
            OH_LOG_Print(LOG_APP, LOG_DEBUG, LOG_DOMAIN,
                         "ImageKnifeOption::ImageKnifeOption",
                         "signature is : %{public}s",
                         signature.c_str());
        }
    }

    if (napi_get_named_property(env, jsObject, "priority", &value) == napi_ok) {
        napi_valuetype type;
        napi_status status = napi_typeof(env, value, &type);
        if (type == napi_number) {
            int32_t priorityValue = 0;

            // 获取整数值
            status = napi_get_value_int32(env, value, &priorityValue);
            if (status == napi_ok) {
                priority = priorityValue;
                OH_LOG_Print(LOG_APP, LOG_DEBUG, LOG_DOMAIN,
                             "ImageKnifeOption::ImageKnifeOption",
                             "priority is : %{public}d",
                             priorityValue);
            } else {
                OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                             "ImageKnifeOption::ImageKnifeOption",
                             "Failed to get priority value as number.");
            }
        }
    }

    if (napi_get_named_property(env, jsObject, "onLoadListener", &value) == napi_ok) {
        onLoadListener.SetCallBack(env, value);
    }

    if (napi_get_named_property(env, jsObject, "context", &value) == napi_ok) {
        context.SetContext(env, value);
    }

    if (napi_get_named_property(env, jsObject, "objectFit", &value) == napi_ok) {
        objectFit = ParseImagFit(env, value);
    }

    if (napi_get_named_property(env, jsObject, "placeholderObjectFit", &value) == napi_ok) {
        placeholderObjectFit = ParseImagFit(env, value);
    }

    if (napi_get_named_property(env, jsObject, "errorholderObjectFit", &value) == napi_ok) {
        errorholderObjectFit = ParseImagFit(env, value);
    }

    if (napi_get_named_property(env, jsObject, "border", &value) == napi_ok) {
        border.SetBorderOption(env, value);
    }

    if (napi_get_named_property(env, jsObject, "onComplete", &value) == napi_ok) {
        napi_valuetype type;
        napi_typeof(env, value, &type);
        if (type != napi_undefined) {
            napi_create_reference(env, value, 1, &onComplete_);
            onComplete = [this](EventImage eventImage) {
                this->CallJsOnComplete(eventImage);
            };
        }
    }
}

ArkUI_ObjectFit ImageKnifeOption::ParseImagFit(napi_env env, napi_value &value)
{
#define DEFAULT_IMAGEFIT_VALUE 2
    // default is Cover
    int num = DEFAULT_IMAGEFIT_VALUE;
    napi_get_value_int32(env, value, &num);
    // js 侧的枚举 ImageFit 与侧对应的ArkUI_ObjectFit 不一致，需要映射
    ArkUI_ObjectFit jsEnum[] = {
        ARKUI_OBJECT_FIT_CONTAIN,
        ARKUI_OBJECT_FIT_COVER,
        ARKUI_OBJECT_FIT_AUTO,
        ARKUI_OBJECT_FIT_FILL,
        ARKUI_OBJECT_FIT_SCALE_DOWN,
        ARKUI_OBJECT_FIT_NONE
    };

    // 如果给了异常值，默认cover模式
    if (num < 1 || num > sizeof(jsEnum) / sizeof(jsEnum[0])) {
        num = DEFAULT_IMAGEFIT_VALUE;
    }

    return jsEnum[num - 1];
}

void ImageKnifeOption::CallJsOnComplete(EventImage &eventImage)
{
    // TODO 什么时候eventIamge触发时是undefined？
    napi_status status;
    napi_value thisValue, jsFunc, eventImageValue, number;

    napi_create_object(env_, &eventImageValue);

    napi_create_int32(env_, eventImage.loadingStatus, &number);
    napi_set_named_property(env_, eventImageValue, "loadingStatus", number);

    napi_create_double(env_, eventImage.width, &number);
    napi_set_named_property(env_, eventImageValue, "width", number);

    napi_create_double(env_, eventImage.height, &number);
    napi_set_named_property(env_, eventImageValue, "height", number);

    napi_create_double(env_, eventImage.componentWidth, &number);
    napi_set_named_property(env_, eventImageValue, "componentWidth", number);

    napi_create_double(env_, eventImage.componentHeight, &number);
    napi_set_named_property(env_, eventImageValue, "componentHeight", number);

    napi_create_double(env_, eventImage.contentOffsetX, &number);
    napi_set_named_property(env_, eventImageValue, "contentOffsetX", number);

    napi_create_double(env_, eventImage.contentOffsetY, &number);
    napi_set_named_property(env_, eventImageValue, "contentOffsetY", number);

    napi_create_double(env_, eventImage.contentWidth, &number);
    napi_set_named_property(env_, eventImageValue, "contentWidth", number);

    napi_create_double(env_, eventImage.contentHeight, &number);
    napi_set_named_property(env_, eventImageValue, "contentHeight", number);

    napi_get_reference_value(env_, jsThis_, &thisValue);
    napi_get_reference_value(env_, onComplete_, &jsFunc);
    status = napi_call_function(env_, thisValue, jsFunc, 1, &eventImageValue, nullptr);
    if ( status != napi_ok) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                     "ImageKnifeOption",
                     "CallJsOnComplete failed, napi status: %{public}d",
                     status);
    }
}

void OnLoadCallBack::SetCallBack(napi_env env, napi_value value)
{
    enableJsFunction_ = true;
    env_ = env;
    napi_status status;
    napi_create_reference(env, value, 1, &jsThis_);

    napi_valuetype type;
    napi_typeof(env, value, &type);
    if (type == napi_undefined) {
        return;
    }

    napi_value jsFunc;
    status = napi_get_named_property(env, value, "onLoadStart", &jsFunc);
    if (status == napi_ok) {
        napi_typeof(env, jsFunc, &type);
        if (type != napi_undefined) {
            napi_create_reference(env, jsFunc, 1, &onLoadStart_);
            onLoadStart = [this]() {
                this->CallJsOnLoadStart();
            };
        }
    }

    status = napi_get_named_property(env, value, "onLoadSuccess", &jsFunc);
    if (status == napi_ok) {
        napi_typeof(env, jsFunc, &type);
        if (type != napi_undefined) {
            napi_create_reference(env, jsFunc, 1, &onLoadSuccess_);
            onLoadSuccess = [this](ImageDataBack &data, ImageKnifeData &imageKnifeData) {
                this->CallJsOnLoadSuccess(data, imageKnifeData);
            };
        }
    }

    status = napi_get_named_property(env, value, "onLoadFailed", &jsFunc);
    if (status == napi_ok) {
        napi_typeof(env, jsFunc, &type);
        if (type != napi_undefined) {
            napi_create_reference(env, jsFunc, 1, &onLoadFailed_);
            onLoadFailed = [this](std::string err) {
                this->CallJsOnLoadFailed(err);
            };
        }
    }

    status = napi_get_named_property(env, value, "onLoadCancel", &jsFunc);
    if (status == napi_ok) {
        napi_typeof(env, jsFunc, &type);
        if (type != napi_undefined) {
            napi_create_reference(env, jsFunc, 1, &onLoadCancel_);
            onLoadCancel = [this](std::string reason) {
                this->CallJsOnLoadCancel(reason);
            };
        }
    }
}

void OnLoadCallBack::CallJsOnLoadStart()
{
    napi_status status;
    napi_value thisValue, jsFunc;
    napi_get_reference_value(env_, jsThis_, &thisValue);
    napi_get_reference_value(env_, onLoadStart_, &jsFunc);
    status = napi_call_function(env_, thisValue, jsFunc, 0, nullptr, nullptr);
    if (status != napi_ok) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                     "ImageKnifeNode",
                     "call js onLoadStart failed, napi status: %{public}d",
                     status);
    }
}

void OnLoadCallBack::CallJsOnLoadSuccess(ImageDataBack &data, ImageKnifeData &imageKnifeData)
{
    // for test
    std::string info;
    napi_value args[2];
    OH_PixelmapNative *pixelmap;
    if (data.GetString(info)) {
        napi_create_string_utf8(env_, info.c_str(), NAPI_AUTO_LENGTH, &args[0]);
    } else if (data.GetPixelMap(pixelmap)) {
        OH_PixelmapNative_ConvertPixelmapNativeToNapi(env_, pixelmap, &args[0]);
    } else {
        napi_get_undefined(env_, &args[0]);
    }

    // TODO: 暂未实现ImageKnifeData, 目前使用undefined测试
    napi_get_undefined(env_, &args[1]);

    napi_status status;
    napi_value thisValue, jsFunc;
    napi_get_reference_value(env_, jsThis_, &thisValue);
    napi_get_reference_value(env_, onLoadSuccess_, &jsFunc);
    status = napi_call_function(env_, thisValue, jsFunc, 2, args, nullptr);
    if (status != napi_ok) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                     "ImageKnifeNode",
                     "call js onLoadSuccess failed, napi status: %{public}d",
                     status);
    }
}

void OnLoadCallBack::CallJsOnLoadFailed(std::string err)
{
    napi_value errInfo;
    napi_create_string_utf8(env_, err.c_str(), NAPI_AUTO_LENGTH, &errInfo);
    napi_status status;
    napi_value thisValue, jsFunc;
    napi_get_reference_value(env_, jsThis_, &thisValue);
    napi_get_reference_value(env_, onLoadFailed_, &jsFunc);
    status = napi_call_function(env_, thisValue, jsFunc, 1, &errInfo, nullptr);
    if (status != napi_ok) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                     "ImageKnifeNode",
                     "call js onLoadFailed failed, napi status: %{public}d",
                     status);
    }
}

void OnLoadCallBack::CallJsOnLoadCancel(std::string reason)
{
    napi_value info;
    napi_create_string_utf8(env_, reason.c_str(), NAPI_AUTO_LENGTH, &info);
    napi_status status;
    napi_value thisValue, jsFunc;
    napi_get_reference_value(env_, jsThis_, &thisValue);
    napi_get_reference_value(env_, onLoadCancel_, &jsFunc);
    status = napi_call_function(env_, thisValue, jsFunc, 1, &info, nullptr);
    if (status != napi_ok) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                     "ImageKnifeNode",
                     "call js onLoadCancel failed, napi status: %{public}d",
                     status);
    }
}

OnLoadCallBack::~OnLoadCallBack()
{
    if (enableJsFunction_) {
        if (onLoadStart != nullptr) {
            napi_delete_reference(env_, onLoadStart_);
        }

        if (onLoadSuccess != nullptr) {
            napi_delete_reference(env_, onLoadSuccess_);
        }

        if (onLoadFailed != nullptr) {
            napi_delete_reference(env_, onLoadFailed_);
        }

        if (onLoadCancel != nullptr) {
            napi_delete_reference(env_, onLoadCancel_);
        }

        napi_delete_reference(env_, jsThis_);
    }
}

} // end of namespace