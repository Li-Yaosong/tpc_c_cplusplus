
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
project(libiconv)

set(ICONV_LIB_DEFINITIONS
        -DCVAPI_EXPORTS
        -D_USE_MATH_DEFINES
        -D__STDC_CONSTANT_MACROS
        -D__STDC_FORMAT_MACROS
        -D__STDC_LIMIT_MACROS
        -DHAVE_CONFIG_H
        -DENABLE_RELOCATABLE=1
        -DBUILDING_LIBICONV
        -DNO_XMALLOC)

set(ICONV_LIB_DIR ../ ../include)

add_library(iconv SHARED iconv.c relocatable.c)

target_link_libraries(iconv charset)
target_compile_definitions(iconv PRIVATE ${ICONV_LIB_DEFINITIONS})
target_include_directories(iconv PRIVATE ${ICONV_LIB_DIR})
