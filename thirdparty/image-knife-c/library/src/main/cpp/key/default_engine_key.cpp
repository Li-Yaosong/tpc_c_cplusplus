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
#include "default_engine_key.h"
#include <sstream>
#include <iomanip>

namespace ImageKnifeC {
std::string DefaultEngineKey::GenerateFileKey(const ImageData *imageData, const std::string &signature, bool isAnimator)
{
    std::ostringstream src;
    std::string fileName;
    std::string fileKey;

    imageData->GetString(fileKey);
    src << (isAnimator ? "Animator=" : "loadSrc==") << fileKey << ";";

    if (!signature.empty()) {
        src << "signature=" << signature << ";";
    }

    DoMd5Hash(src.str(), fileName);
    return fileName;
}

std::string DefaultEngineKey::GenerateMemoryKey(const ImageData *imageData,
                                                ImageKnifeC::ImageKnifeRequestSource type,
                                                const ImageKnifeOption *const imageKnifeOption,
                                                const std::string &signature,
                                                bool isAnimator,
                                                int width,
                                                int height)
{
    std::string memoryKey;
    Resource resource;
    // 拼接 loadSrc 类型
    std::ostringstream key;
    if (imageData->GetString(memoryKey)) {
        key << "loadSrc=" << memoryKey << ";";
    } else {
        imageData->GetResource(resource);
        key << "loadSrc=" << resource.bundleName << "/"
        << resource.moduleName << "/"
        << resource.id << ";";
    }

    if (type == ImageKnifeC::ImageKnifeRequestSource::MAINSRC) {
        if (!signature.empty()) {
            key << "signature=" << signature << ";";
        }
    }
    return key.str();
}

OH_Crypto_ErrCode DefaultEngineKey::DoMd5Hash(const std::string &url, std::string &fileName)
{
    OH_Crypto_ErrCode ret;
    OH_CryptoDigest *ctx = nullptr;
    Crypto_DataBlob in = {.data = (uint8_t *)(url.c_str()), .len = strlen(url.c_str())};
    Crypto_DataBlob out = {.data = nullptr, .len = 0};
    int mdLen = 0;
    ret = OH_CryptoDigest_Create("MD5", &ctx);
    if (ret != CRYPTO_SUCCESS) {
        return ret;
    }

    do {
        ret = OH_CryptoDigest_Update(ctx, &in);
        if (ret != CRYPTO_SUCCESS) {
            break;
        }
        ret = OH_CryptoDigest_Final(ctx, &out);
        if (ret != CRYPTO_SUCCESS) {
            break;
        }
        mdLen = OH_CryptoDigest_GetLength(ctx);
    } while (0);
    const size_t defaultW = 2;
    for (int i = 0; i < out.len; ++i) {
        std::ostringstream oss;
        oss << std::setw(defaultW) << std::setfill('0') << std::hex << static_cast<int>(out.data[i]);
        fileName += oss.str();
    }
    OH_LOG_Print(LOG_APP, LOG_DEBUG, LOG_DOMAIN,
                 "ImageKnifeNode",
                 "The gethashFileName of data are : %{public}s",
                 fileName.c_str());
    OH_Crypto_FreeDataBlob(&out);
    OH_DigestCrypto_Destroy(ctx);
    return ret;
}
}
