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

#ifndef IMAGEKNIFEC_IMAGEKNIFE_H
#define IMAGEKNIFEC_IMAGEKNIFE_H

#include "imageKnife_request.h"
#include "key/IEngine_key.h"

namespace ImageKnifeC {
class ImageKnife {
public:
    static ImageKnife& GetInstance();
    virtual void Execute(std::shared_ptr<ImageKnifeRequest> request) = 0;
    virtual void CancelRequest(std::shared_ptr<ImageKnifeRequest> request) = 0;
    virtual void InitFileCache(std::string, int size, int memory, std::string path) = 0;
    virtual void SetDefaultImageKnifeLoader(std::shared_ptr<ImageKnifeLoader> imageLoader) = 0;
    virtual std::shared_ptr<ImageKnifeLoader> GetDefaultImageKnifeLoader() const = 0;
    virtual void SetMaxRequests(int concurrency) = 0;
    virtual void SetEngineKeyImpl(IEngineKey *keyPtr) = 0;
    virtual IEngineKey *GetEngineKeyImpl() const = 0;
    virtual ~ImageKnife() = default;
};

}

#endif // IMAGEKNIFEC_IMAGEKNIFE_H
