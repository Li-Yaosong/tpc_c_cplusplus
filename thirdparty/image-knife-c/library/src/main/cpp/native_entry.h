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

#ifndef IMAGE_KNIFE_C_NATIVE_ENTRY_H
#define IMAGE_KNIFE_C_NATIVE_ENTRY_H

#include "arkUI_base_node.h"
#include <arkui/native_node.h>
#include <arkui/native_type.h>
#include <js_native_api_types.h>
#include <unordered_map>
#include <string>

namespace ImageKnifeC {
// 管理Native组件的生命周期和内存。
class NativeEntry {
public:
    static NativeEntry *GetInstance()
    {
        static NativeEntry nativeEntry;
        return &nativeEntry;
    }

    void SetRootNode(std::string id, ArkUI_NodeContentHandle handle, const std::shared_ptr<ArkUIBaseNode> &baseNode)
    {
        contentHandleMap_[id] = contentHandle({.imageNode = baseNode, .rootNode = handle});
        // 添加Native组件到NodeContent上用于挂载显示。
        OH_ArkUI_NodeContent_AddNode(handle, baseNode->GetHandle());
    }

    void DisposeRootNode(std::string id)
    {
        // 从NodeContent上卸载组件并销毁Native组件。
        OH_ArkUI_NodeContent_RemoveNode(contentHandleMap_[id].rootNode, contentHandleMap_[id].imageNode->GetHandle());
        contentHandleMap_[id].imageNode.reset();

        // contentHandleMap_[id].rootNode 是js层传下来占位的contentSlot无需处理
        // contentHandleMap_ 移除item
        contentHandleMap_.erase(id);
    }

    std::shared_ptr<ArkUIBaseNode> GetArkUIBaseNode(std::string id)
    {
        if (contentHandleMap_.find(id) != contentHandleMap_.end()) {
            return contentHandleMap_[id].imageNode;
        }

        return nullptr;
    }

    ~NativeEntry()
    {
    }

private:
    typedef struct {
        std::shared_ptr<ArkUIBaseNode> imageNode;
        ArkUI_NodeContentHandle rootNode;
    } contentHandle;

    std::unordered_map<std::string, contentHandle> contentHandleMap_;
};

} // namespace ImageKnifeC

#endif //IMAGE_KNIFE_C_NATIVE_ENTRY_H
