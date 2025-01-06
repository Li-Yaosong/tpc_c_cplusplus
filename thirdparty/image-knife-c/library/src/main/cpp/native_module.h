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

#ifndef IMAGE_KNIFE_C_NATIVE_MODULE_H
#define IMAGE_KNIFE_C_NATIVE_MODULE_H

#include <arkui/native_node.h>
#include <cassert>

#include <arkui/native_interface.h>
#include "napi/native_api.h"

namespace ImageKnifeC {

class NativeModuleInstance {
public:
    static NativeModuleInstance *GetInstance()
    {
        static NativeModuleInstance instance;
        return &instance;
    }

    void SetNapiEnv(napi_env env)
    {
        if (env_ == nullptr) {
            env_ = env;
        }
    }

    napi_env GetNapiEnv()
    {
        return env_;
    }

    NativeModuleInstance()
    {
        OH_ArkUI_GetModuleInterface(ARKUI_NATIVE_NODE, ArkUI_NativeNodeAPI_1, arkUINativeNodeApi_);
    }
    // 暴露给其他模块使用。
    ArkUI_NativeNodeAPI_1 *GetNativeNodeAPI()
    {
        return arkUINativeNodeApi_;
    }

private:
    ArkUI_NativeNodeAPI_1 *arkUINativeNodeApi_ = nullptr;
    napi_env env_ = nullptr;
};

} // namespace ImageKnifeC

#endif // IMAGE_KNIFE_C_NATIVE_MODULE_H
