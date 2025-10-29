# libxnet/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

# 设置默认架构
if(NOT DEFINED TARGET_ARCH)
    set(TARGET_ARCH "x86_64")
endif()

# 根据目标架构设置编译器和标志
if(TARGET_ARCH STREQUAL "x86_64")
    set(ARCH_CXX_FLAGS "")
    message(STATUS "Building for x86_64 with default compiler")
    
elseif(TARGET_ARCH STREQUAL "arm64")
    set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
    set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
    set(ARCH_CXX_FLAGS "-mcpu=cortex-a53")
    message(STATUS "Building for ARM64 with cross-compiler")
    
elseif(TARGET_ARCH STREQUAL "arm32")
    set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
    set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
    set(ARCH_CXX_FLAGS "-march=armv7-a -mfpu=neon")
    message(STATUS "Building for ARM32 with cross-compiler")
    
else()
    message(FATAL_ERROR "Unsupported TARGET_ARCH: ${TARGET_ARCH}")
endif()

# 将架构特定的Flags添加到CMAKE_CXX_FLAGS
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ARCH_CXX_FLAGS}")

project(libxnet-headers LANGUAGES CXX)

# 调试信息
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ compiler version: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "Target architecture: ${TARGET_ARCH}")

# 设置 C++20 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 主库目标
add_library(xnet.headers INTERFACE)
target_include_directories(xnet.headers INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# 为接口目标也设置标准
target_compile_features(xnet.headers INTERFACE cxx_std_20)

enable_testing()
add_subdirectory(tests)  # 只在这里包含 tests 目录