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

#ifndef IMAGE_KNIFE_C_DOWNLOAD_INTERCEPTOR_OH_H
#define IMAGE_KNIFE_C_DOWNLOAD_INTERCEPTOR_OH_H

#include "interceptor.h"
#include "imageKnife_request.h"

namespace ImageKnifeC {
class DownloadInterceptorOH : public DownloadInterceptor {
public:
    DownloadInterceptorOH()
    {
        name = "Default DownloadInterceptor";
    }

    bool Resolve(ImageKnifeTask *task) override
    {
        std::string url;
        if (task->imageSrc->GetString(url)) {
            return LoadImageFromUrl(url, task);
        }

        return false;
    }

    void Cancel(ImageKnifeTask *task) override
    {
        return;
        const uint64_t errorCode = 1007900993;
        if (task->product.request != nullptr && task->product.session != nullptr) {
            uint32_t errorCode = HMS_Rcp_CancelRequest(task->product.session, task->product.request);
            uint32_t errCodeSession = HMS_Rcp_CancelSession(task->product.session);
            if (!errorCode) {
                task->cancelInfo = "Cancel download success: " + std::to_string(errCodeSession);
            } else if (errorCode == errorCode) {
                task->cancelInfo = "Cancel Failed: Session is closed or not work," + std::to_string(errCodeSession);
            } else {
                task->cancelInfo = "Cancel Failed, error code: " +
                                    std::to_string(errorCode) + " , : " + std::to_string(errCodeSession);
            }
        }
    }

private:
    struct CallbackData {
        bool fetchFinished = false;
        std::string errorInfo;
        Rcp_Response* response = nullptr;
    };

    bool LoadImageFromUrl(std::string url, ImageKnifeTask *task)
    {
        task->product.request = HMS_Rcp_CreateRequest(url.c_str());
        ConfigHttpRequest(task->product.request);

        uint32_t errCode = 0;
        task->product.session = HMS_Rcp_CreateSession(NULL, &errCode);
        if (errCode) {
            task->EchoError("create Session error: " + std::to_string(errCode));
            HMS_Rcp_DestroyRequest(task->product.request);
            return false;
        }

        bool result = HttpFetchSync(task->product.session, task->product.request, task);
        HMS_Rcp_DestroyRequest(task->product.request);
        HMS_Rcp_CloseSession(&task->product.session);
        return result;
    }

    void ConfigHttpRequest(Rcp_Request *request)
    {
        // 设置为Get请求
        request->method = RCP_METHOD_GET;
        Rcp_EventsHandler evenHandler;
        Rcp_Configuration config;
        config.tracingConfiguration.httpEventsHandler = evenHandler;
    }

    static void ResponseCallback(void *usrCtx, Rcp_Response *response, uint32_t errCode)
    {
        CallbackData *data = static_cast<CallbackData*>(usrCtx);
        if (errCode) {
            // 1007900992 represent http download has been canceled
            if (errCode != 1007900992) {
                data->errorInfo = "HttpFetch HMS_Rcp_Fetch error:" + std::to_string(errCode);
            }
        }

        if (response != NULL) {
            if (response->statusCode != RCP_OK) {
                data->errorInfo = "HttpFetch response error, statusCode: " + std::to_string(response->statusCode);
                response->destroyResponse(response);
            } else {
                data->response = response;
            }
        }

        data->fetchFinished = true;
    }

    bool HttpFetchSync(Rcp_Session *session, Rcp_Request *request, ImageKnifeTask *task)
    {
        uint32_t errCode = 0;
        Rcp_Response *response = HMS_Rcp_FetchSync(session, request, &errCode);
        if (errCode) {
            task->EchoError("HttpFetch HMS_Rcp_Fetch error:" + std::to_string(errCode));
            return false;
        }

        if (response != NULL) {
            if (response->statusCode != RCP_OK) {
                task->EchoError("HttpFetch response error, statusCode: " + std::to_string(response->statusCode));
                response->destroyResponse(response);
            } else {
                task->product.response = response;
                return true;
            }
        }

        return false;
    }
};

};

#endif // IMAGE_KNIFE_C_DOWNLOAD_INTERCEPTOR_OH_H
