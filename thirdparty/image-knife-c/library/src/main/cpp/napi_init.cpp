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

#include "napi/native_api.h"
#include "include/imageKnife.h"

#include "imageKnife_napi.h"
#include "key/default_engine_key.h"

EXTERN_C_START
static napi_value init(napi_env env, napi_value exports)
{
    ImageKnifeC::ImageKnifeNapi::Init(env, exports);
    auto imageLoader = ImageKnifeC::ImageKnifeLoader::CreateDefaultImageLoader();
    ImageKnifeC::ImageKnife::GetInstance().SetDefaultImageKnifeLoader(imageLoader);

    ImageKnifeC::DefaultEngineKey *keyPtr = new ImageKnifeC::DefaultEngineKey();
    ImageKnifeC::ImageKnife::GetInstance().SetEngineKeyImpl(keyPtr);
    return exports;
}
EXTERN_C_END

static napi_module demoModule = {
    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = init,
    .nm_modname = "imageknifec",
    .nm_priv = ((void*)0),
    .reserved = { 0 },
};

extern "C" __attribute__((constructor)) void registerEntryModule(void)
{
    napi_module_register(&demoModule);
}
