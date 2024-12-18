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

#ifndef IMAGE_KNIFE_C_IMAGEKNIFE_NODE_H
#define IMAGE_KNIFE_C_IMAGEKNIFE_NODE_H

#include "imageKnife_option.h"
#include "arkUI_node.h"
#include <string>
#include "hilog/log.h"

namespace ImageKnifeC {

class ImageKnifeNode : public ArkUINode {
public:
    // 使用自定义组件类型ARKUI_NODE_IMAGE创建组件。
    ImageKnifeNode(std::string componentId, std::shared_ptr<ImageKnifeOption> option)
        : ArkUINode((NativeModuleInstance::GetInstance()->GetNativeNodeAPI())->createNode(ARKUI_NODE_IMAGE)),
        componentId_(componentId)
    {
        init(option);
    }

    ~ImageKnifeNode() override
    {
        // 反注册自定义事件监听器。
        nativeModule_->removeNodeCustomEventReceiver(handle_, OnCustomEvent);
        // 取消声明自定义事件。
        nativeModule_->unregisterNodeCustomEvent(handle_, ARKUI_NODE_CUSTOM_EVENT_ON_MEASURE);

        // 反注册NodeEvent
        if (onImageComplete_ != nullptr) {
            nativeModule_->unregisterNodeEvent(handle_, NODE_IMAGE_ON_COMPLETE);
        }
        nativeModule_->removeNodeEventReceiver(handle_, OnNodeEvent);
        // 移除当前可能在显示的图片
        ClearDisplayedImage();
    }

    // 必需在主线程中运行
    void Display(void *data) override
    {
        if (data != nullptr) {
            ArkUI_AttributeItem item = {.object = data};
            nativeModule_->setAttribute(this->handle_, NODE_IMAGE_SRC, &item);

            // 释放之前的图片描述内存, 并记录最新的imageDesc地址
            ReleaseDrawableDescriptor();
            imageDesc_ = static_cast<ArkUI_DrawableDescriptor*>(data);
        }
    }

    void FitImage(int fitEnum) override
    {
        imageFitEnum_.i32 = fitEnum;
        ArkUI_AttributeItem item = {.value = &imageFitEnum_, .size = 1};
        nativeModule_->setAttribute(this->handle_, NODE_IMAGE_OBJECT_FIT, &item);
    }

    void ClearDisplayedImage() override
    {
        ArkUI_AttributeItem item = {.object = nullptr};
        nativeModule_->setAttribute(this->handle_, NODE_IMAGE_SRC, &item);
        ReleaseDrawableDescriptor();
    }

    std::shared_ptr<ImageKnifeRequest> GetImageKnifeRequest()
    {
        return imageKnifeRequest_;
    }

    void UpdateImageKnifeRequest(std::shared_ptr<ImageKnifeOption> option);

    void SyncLoad(bool syncLoad);

    void init(std::shared_ptr<ImageKnifeOption> option);
private:
    std::string componentId_;
    // 保持request生命周期
    std::shared_ptr<ImageKnifeRequest> imageKnifeRequest_;

    ArkUI_DrawableDescriptor *imageDesc_ = nullptr;

    // ImageFit, ArkUI_ObjectFit 枚举入参类型
    ArkUI_NumberValue imageFitEnum_ = {.i32 = 1};

    ArkUI_NumberValue borderWidth_[4];
    ArkUI_NumberValue borderRadius_[4];
    ArkUI_NumberValue borderColor_[4];
    ArkUI_NumberValue borderStyle_[4];

    std::function<void(EventImage image)> onImageComplete_ = nullptr;

    void ReleaseDrawableDescriptor();

    void RegisterMeasureCallback();

    void RegisterNodeEvent();

    static void OnNodeEvent(ArkUI_NodeEvent *event);

    static void OnCustomEvent(ArkUI_NodeCustomEvent *event);

    void OnImageComplete(ArkUI_NodeEvent *event);
    // 测算事件回调
    void OnMeasure(ArkUI_NodeCustomEvent *event);
    void SetBorder();
};

} // namespace ImageKnifeC

#endif // IMAGE_KNIFE_C_IMAGEKNIFE_NODE_H
