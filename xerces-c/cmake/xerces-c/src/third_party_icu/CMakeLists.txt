# Copyright (c) 2021 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
cmake_minimum_required(VERSION 3.4.1)
set(CMAKE_CXX_STANDARD 11)
project(icu)
#set(NATIVERENDER_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-ignored-attributes -std=c++11 -O3 -frtti -fexceptions -pedantic -Wno-unused-parameter -Wno-unused-const-variable -Wno-unneeded-internal-declaration")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ")
set(LibIcuSrc "${CMAKE_CURRENT_SOURCE_DIR}/icu4c/source")
include_directories(
                    ${LibIcuSrc}
                    ${LibIcuSrc}/common
                    ${LibIcuSrc}/i18n
                    )
add_definitions(
                -DU_ATTRIBUTE_DEPRECATED=
                -DU_COMMON_IMPLEMENTATION
                -DUPRV_BLOCK_MACRO_BEGIN=
                -DUPRV_BLOCK_MACRO_END=
                -DUCONFIG_USE_WINDOWS_LCID_MAPPING_API=0
                -D_REENTRANT
                )
set(icustubdata "icu4c/source/stubdata/stubdata.cpp")
aux_source_directory(icu4c/source/common icucommon)

set(src ${icucommon}
        ${icustubdata}
        )

add_library(icu SHARED ${src})
