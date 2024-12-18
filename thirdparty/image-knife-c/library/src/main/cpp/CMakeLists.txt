# the minimum version of CMake.
cmake_minimum_required(VERSION 3.5.0)
project(image-knife-c)

option(IMAGEKNIFEC_ENABLE_INSTALL "" ON)

set(NATIVERENDER_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

if(DEFINED PACKAGE_FIND_FILE)
    include(${PACKAGE_FIND_FILE})
endif()

# Specify the C++ standard to use
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${NATIVERENDER_ROOT_PATH}
                    ${NATIVERENDER_ROOT_PATH}/include
                    ${NATIVERENDER_ROOT_PATH}/node
                    ${NATIVERENDER_ROOT_PATH}/cache
                    ${NATIVERENDER_ROOT_PATH}/interceptor)

add_library(imageknifec SHARED napi_init.cpp
                        imageKnife_napi.cpp
                        imageKnife_internal.cpp
                        imageKnife_dispatcher.cpp
                        request/imageKnife_request_internal.cpp
                        imageKnife_task_internal.cpp
                        interceptor/interceptor.cpp
                        cache/lru_cache.cpp
                        key/default_engine_key.cpp
                        cache/file_cache.cpp
                        loader/imageKnife_loader.cpp
                        loader/imageKnife_loader_internal.cpp
                        node/imageKnife_node.cpp
                        node/imageKnife_data.cpp
                        node/imageKnife_option.cpp
                        queue/default_job_queue.cpp)
target_link_libraries(imageknifec PUBLIC libace_napi.z.so libace_ndk.z.so librcp_c.so
                             libimage_source.so libpixelmap.so librawfile.z.so libohresmgr.so libhilog_ndk.z.so
                             libohcrypto.so)

target_link_directories(imageknifec PUBLIC ${HMOS_SDK_NATIVE}/sysroot/usr/lib/aarch64-linux-ohos)

if (IMAGEKNIFEC_ENABLE_INSTALL)
set(IMAGEKNIFEC_INSTALL_PATH "${CMAKE_SOURCE_DIR}/../../../")
add_custom_command(TARGET imageknifec POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/include ${IMAGEKNIFEC_INSTALL_PATH}/include)
endif()
