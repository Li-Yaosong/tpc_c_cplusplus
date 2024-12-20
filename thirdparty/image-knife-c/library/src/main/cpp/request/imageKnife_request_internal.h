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

#ifndef IMAGE_KNIFE_C_KEKE_IMAGEKNIFE_REQUEST_INTERNAL_H
#define IMAGE_KNIFE_C_KEKE_IMAGEKNIFE_REQUEST_INTERNAL_H

#include "imageKnife_request.h"

namespace ImageKnifeC {

class ImageKnifeRequestInternal : public ImageKnifeRequest {
public:
    void InsertTask(ImageKnifeTask *taskId, ImageKnifeRequestSource type);

    ImageKnifeTask *GetTask(ImageKnifeRequestSource type);

    void RemoveTask(ImageKnifeRequestSource type);

    void MarkDisplayStartTime(ImageKnifeRequestSource type, bool fromMemory);

    void MarkDisplayEndTime();

    void MarkStatusComplete();

    void MarkStatusError();

    void SyncLoad(bool syncLoad);

    void Destroy();

    bool IsMainSrcComplete() const;

    bool IsDestroy() const;

    bool IsSyncLoad() const;

    std::string GetNodeId() const override;

    int GetVersion() const override;

    Status GetStatus() const override;

    std::shared_ptr<ImageKnifeOption> GetImageKnifeOption() const override;

    bool IsPreload();

    void SetEnv(napi_env env);

    void SetJsRequestRef(napi_ref jsRequestRef);

    ImageKnifeRequestInternal(std::string componentId, std::shared_ptr<ImageKnifeOption> option, int componentVersion)
        : componentId_(componentId), imageKnifeOption_(option), componentVersion_(componentVersion), isPreload_(false)
    {
        start_ = std::chrono::high_resolution_clock::now();
    }

    eplicit ImageKnifeRequestInternal(std::shared_ptr<ImageKnifeOption> option)
        :imageKnifeOption_(option), isPreload_(true)
    {
    }

    ~ImageKnifeRequestInternal() override
    {
        if (isPreload_) {
            napi_value jsRequest = nullptr;
            napi_status status = napi_get_reference_value(env_, jsRequestRef_, &jsRequest);
            if (status != napi_ok) {
                OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                             "ImageKnifeRequestInternal::~ImageKnifeRequestInternal",
                             "Failed to get jsRequest");
            }

            void *nativeObjectResult = nullptr;
            status = napi_remove_wrap(env_, jsRequest, &nativeObjectResult);
            if (status != napi_ok) {
                OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN,
                             "ImageKnifeRequestInternal::~ImageKnifeRequestInternal",
                             "Failed to remove wrap in destructor");
            }
        }
    }

private:
    Status status_ = Status::PROGRESS;
    std::shared_ptr<ImageKnifeOption> imageKnifeOption_;
    std::string componentId_ = "Preload";
    int componentVersion_;
    bool syncLoad_ = false;
    std::unordered_map<ImageKnifeRequestSource, ImageKnifeTask*> taskIdMap_;

    // ---- 性能日志相关 ----
    typedef std::chrono::time_point<std::chrono::high_resolution_clock> TimePoint;
    // 总开始时间
    TimePoint start_;
    // 渲染开始时间
    TimePoint displayStart_;
    // 显示图片类型, 不同类型不会同时渲染
    ImageKnifeRequestSource displayType_;
    // 标记当前显示的图片是否来自内存缓存
    bool fromMemory_;

    // 判断是否显示图片
    bool isPreload_;
    napi_env env_ = nullptr;
    napi_ref jsRequestRef_ = nullptr;
};

} // end of namespace

#endif // IMAGE_KNIFE_C_KEKE_IMAGEKNIFE_REQUEST_INTERNAL_H
