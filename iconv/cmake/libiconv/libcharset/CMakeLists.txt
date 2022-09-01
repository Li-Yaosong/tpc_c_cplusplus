
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
project(libcharset)

set(LIBCHARSET_DEF
        -DCVAPI_EXPORTS
        -D_USE_MATH_DEFINES
        -D__STDC_CONSTANT_MACROS
        -D__STDC_FORMAT_MACROS
        -D__STDC_LIMIT_MACROS
        -DHAVE_CONFIG_H)

set(LIBCHARSET_DIR ../include)

add_library(charset STATIC lib/localcharset.c lib/relocatable-stub.c)
target_compile_definitions(charset PRIVATE ${LIBCHARSET_DEF})
target_include_directories(charset PRIVATE ${LIBCHARSET_DIR})
