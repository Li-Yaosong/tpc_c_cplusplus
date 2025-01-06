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

#ifndef IMAGE_KNIFE_C_KEKE_IMAGEKNIFE_INTERNAL_H
#define IMAGE_KNIFE_C_KEKE_IMAGEKNIFE_INTERNAL_H

#include "imageKnife.h"
#include "imageKnife_dispatcher.h"

namespace ImageKnifeC {

class ImageKnifeInternal : public ImageKnife {
public:
    void Execute(std::shared_ptr<ImageKnifeRequest> request) override;

    void CancelRequest(std::shared_ptr<ImageKnifeRequest> request) override;

    std::shared_ptr<ImageKnifeLoader> GetDefaultImageKnifeLoader() const override;

    void SetDefaultImageKnifeLoader(std::shared_ptr<ImageKnifeLoader> imageLoader) override;

    virtual std::shared_future<std::string> InitFileCacheAsync(std::string cachePath, int size, int memory,
                                                               std::string path) override ;

    void InitFileCache(std::string cachePath = "", int size = 256,
                       int memory = 256 * 1024 * 1024, std::string path = "ImageKnife") override;

    void SetEngineKeyImpl(IEngineKey *keyPtr) override;

    IEngineKey *GetEngineKeyImpl() const override;

    std::shared_ptr<ImageKnifeRequest> Preload(std::shared_ptr<ImageKnifeOption> imageKnifeOption) override ;

    void Cancel(std::shared_ptr<ImageKnifeRequest> request) override ;

    void SetMaxRequests(int concurrency) override;
private:
    std::shared_ptr<ImageKnifeDispatcher> dispatcher = std::make_shared<ImageKnifeDispatcher>();

    std::shared_ptr<ImageKnifeLoader> imageLoader_ = nullptr;
    IEngineKey *keyPtr_ = nullptr;

    ImageKnifeInternal() = default;
    ImageKnifeInternal(const ImageKnifeInternal&) = delete;
    ImageKnifeInternal &operator = (const ImageKnifeInternal&) = delete;
    friend ImageKnife &ImageKnife::GetInstance();

    ~ImageKnifeInternal() override
    {
        if (keyPtr_ != nullptr) {
            delete keyPtr_;
            keyPtr = nullptr;
        }
    }
};

} // end of namespace

#endif // IMAGE_KNIFE_C_KEKE_IMAGEKNIFE_INTERNAL_H
