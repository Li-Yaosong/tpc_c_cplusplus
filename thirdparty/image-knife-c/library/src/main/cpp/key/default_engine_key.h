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

#ifndef IMAGE_KNIFE_C_DEFAULT_ENGINE_KEY_H
#define IMAGE_KNIFE_C_DEFAULT_ENGINE_KEY_H

#include "IEngine_key.h"
#include "CryptoArchitectureKit/crypto_common.h"
#include "CryptoArchitectureKit/crypto_digest.h"
#include "node/imageKnife_data.h"
#include "imageKnife_task.h"

namespace ImageKnifeC {
class DefaultEngineKey : public IEngineKey {
public:
    DefaultEngineKey() = default;
    ~DefaultEngineKey() override = default;

    // MD5哈希计算
    OH_Crypto_ErrCode DoMd5Hash(const std::string &url, std::string &fileName);

    // 实现纯虚函数
    std::string GenerateFileKey(const ImageData *imageData, const std::string& signature = "", bool isAnimator = false) override;
    std::string GenerateMemoryKey(const ImageData *imageData, ImageKnifeC::ImageKnifeRequestSource type, const ImageKnifeOption * const imageKnifeOption,
                                  const std::string &signature = "", bool isAnimator = false, int width = 0, int height = 0) override;
};

} // namespace ImageKnifeC

#endif // IMAGE_KNIFE_C_DEFAULT_ENGINE_KEY_H
