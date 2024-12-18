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


#include "imageKnife_node.h"
#include "native_entry.h"
#include "task_worker.h"
#include "hilog/log.h"
#include "request/imageKnife_request_internal.h"

namespace ImageKnifeC {

void ImageKnifeNode::OnMeasure(ArkUI_NodeCustomEvent *event)
{
    auto layoutConstrain = OH_ArkUI_NodeCustomEvent_GetLayoutConstraintInMeasure(event);
    nativeModule_->measureNode(handle_, layoutConstrain);
    auto size = nativeModule_->getMeasuredSize(handle_);
}

// static
void ImageKnifeNode::OnCustomEvent(ArkUI_NodeCustomEvent *event)
{
    // 获取组件实例对象，调用相关实例方法。
    auto customNode = reinterpret_cast<ImageKnifeNode *>(OH_ArkUI_NodeCustomEvent_GetUserData(event));
    auto type = OH_ArkUI_NodeCustomEvent_GetEventType(event);
    switch (type) {
        case ARKUI_NODE_CUSTOM_EVENT_ON_MEASURE:
            customNode->OnMeasure(event);
            break;
        default:
            break;
    }
}

void ImageKnifeNode::RegisterMeasureCallback()
{
    nativeModule_->addNodeCustomEventReceiver(handle_, OnCustomEvent);
    nativeModule_->registerNodeCustomEvent(handle_, ARKUI_NODE_CUSTOM_EVENT_ON_MEASURE, 0, this);
    // 自定义属性事件更新需要主动调用标记脏区接口。
    nativeModule_->markDirty(handle_, NODE_NEED_MEASURE);
}

// 需在ImageKnifeRequest 构造完以后调用
void ImageKnifeNode::RegisterNodeEvent()
{
    nativeModule_->setUserData(handle_, this);
    nativeModule_->addNodeEventReceiver(handle_, OnNodeEvent);

    onImageComplete_ = imageKnifeRequest_->GetImageKnifeOption()->onComplete;
    if (onImageComplete_ != nullptr) {
        nativeModule_->registerNodeEvent(handle_, NODE_IMAGE_ON_COMPLETE, 0, nullptr);
    }
}

// static 
void ImageKnifeNode::OnNodeEvent(ArkUI_NodeEvent *event)
{
    auto nodeHandle = OH_ArkUI_NodeEvent_GetNodeHandle(event);
    ImageKnifeNode *node = reinterpret_cast<ImageKnifeNode *>(NativeModuleInstance::
                           GetInstance()->GetNativeNodeAPI()->getUserData(nodeHandle));
    auto eventType = OH_ArkUI_NodeEvent_GetEventType(event);

    switch (eventType) {
        case NODE_IMAGE_ON_COMPLETE:
            node->OnImageComplete(event);
            break;
        default:
            return;
    }
}

void ImageKnifeNode::OnImageComplete(ArkUI_NodeEvent *event)
{
    if (onImageComplete_ != nullptr) {
        auto componetEvent = OH_ArkUI_NodeEvent_GetNodeComponentEvent(event);
        if (componetEvent->data[0].i32 == 1) {
            // 在渲染完成时标记时间
            static_cast<ImageKnifeRequestInternal*>(imageKnifeRequest_.get())->MarkDisplayEndTime();
        }
        onImageComplete_({.loadingStatus = componetEvent->data[0].i32,
                          .width = componetEvent->data[1].f32,
                          .height = componetEvent->data[2].f32,
                          .componentWidth = componetEvent->data[3].f32,
                          .componentHeight = componetEvent->data[4].f32,
                          .contentOffsetX = componetEvent->data[5].f32,
                          .contentOffsetY = componetEvent->data[6].f32,
                          .contentWidth = componetEvent->data[7].f32,
                          .contentHeight = componetEvent->data[8].f32});
    }
}

void ImageKnifeNode::ReleaseDrawableDescriptor()
{
    if (imageDesc_ != nullptr) {
        OH_ArkUI_DrawableDescriptor_Dispose(imageDesc_);
        imageDesc_ = nullptr;
    }
}

void ImageKnifeNode::SetBorder()
{
    BorderOption &border = imageKnifeRequest_->GetImageKnifeOption()->border;
    // TODO 先获取当前的border状态，把用户对应的设置项填上
    for (int i = 0; i < sizeof(border.width) / sizeof(float); i++) {
        borderWidth_[i].f32 = border.width[i];
        borderRadius_[i].f32 = border.radius[i];
        borderColor_[i].u32 = border.color[i];
        borderStyle_[i].i32 = border.style[i];
    }

    if (border.hasWidth) {
        ArkUI_AttributeItem item = { .value = borderWidth_, .size=1};
        nativeModule_->setAttribute(this->handle_, NODE_BORDER_WIDTH, &item);
    }

    if (border.hasRadius) {
        ArkUI_AttributeItem item = { .value = borderRadius_, .size=1};
        nativeModule_->setAttribute(this->handle_, NODE_BORDER_RADIUS, &item);
    }

    if (border.hasStyle) {
        ArkUI_AttributeItem item = { .value = borderStyle_, .size=1};
        nativeModule_->setAttribute(this->handle_, NODE_BORDER_STYLE, &item);
    }
}

void ImageKnifeNode::UpdateImageKnifeRequest(std::shared_ptr<ImageKnifeOption> option)
{
    imageKnifeRequest_ = std::make_shared<ImageKnifeRequestInternal>(componentId_, option, GetNodeVersion());

    if (option->onComplete == nullptr && onImageComplete_ != nullptr) {
        // 避免遗漏事件反注册
        nativeModule_->unregisterNodeEvent(handle_, NODE_IMAGE_ON_COMPLETE);
    } else if (option->onComplete != nullptr && onImageComplete_ == nullptr) {
        // 旧的option没启用OnComplete  新的启用了，需注册事件
        nativeModule_->registerNodeEvent(handle_, NODE_IMAGE_ON_COMPLETE, 0, nullptr);
    }
    // 更新OnComplete 回调状态
    onImageComplete_ = option->onComplete;
    SetBorder();
}

void ImageKnifeNode::init(std::shared_ptr<ImageKnifeOption> option)
{
    imageKnifeRequest_ = std::make_shared<ImageKnifeRequestInternal>(componentId_, option, GetNodeVersion());
    RegisterMeasureCallback();
    RegisterNodeEvent();
    SetBorder();
}

void ImageKnifeNode::SyncLoad(bool syncLoad)
{
    static_cast<ImageKnifeRequestInternal*>(imageKnifeRequest_.get())->SyncLoad(syncLoad);
}

} // end of namespace