# 通过IDE集成非cmake方式构建的C/C++三方库

## 简介

DevEco Studio目前只支持cmake构建方式，对于非cmake构建方式的三方库需要通过DevEco Studio工具集成的话，我们需要对原生库编写一个cmake的构建脚本。本文主要介绍cmake构建脚本编写，包含的一些主要元素和命名规范。

## cmake构建脚本编写步骤

cmake的构建脚本的具体步骤如下：

### cmake构建工具版本要明确

```shell
# 命令名字要小写，这条语句要求构建工具至少需要版本为3.12或以上
cmake_minimum_required (VERSION 3.12)
```

### 工程名及库的版本号明确

在编写工程名以及版本号时有几点需要注意：

1. 工程名需要大写
2. 版本号需要标明主版本号,次版本号以及补丁版本号,如：

  ```shell
  project(PROJECT_NAME VERSION 0.0.0) 
  ```

  PROJECT_NAME工程的版本号时 0.0.0，该版本号会被三个cmake内置变量所继承，例如主版本号PROJECT_VERSION_MAJOR=0，次版本号PROJECT_VERSION_MINOR=0，补丁版本号PROJECT_VERSION_PATCH=0，后续可以直接使用这三个内置变量来使用库的版本号

### 配置构建语言

可以自己根据工程配置需要构建的语言，比如CXX表示可以编译C++文件；C表示可以编译c文件；ASM表示可以编译汇编文件,.

```shell
enable_language(CXX C ASM)
```

### 配置测试用例可选变量

配置自定义变量，默认不构建测试用例，可以由使用者通过传入参数打开测试用例构建选项.

```shell
option(BUILD_SAMPLE "Build tests" OFF) # 变量名BUILD_SAMPLE  变量说明"Build tests"  状态 OFF：表示不打开  ON：表示打开
```

### 配置打印调试信息

对于一些可能出现的错误或者警告，我们可以通过`message`函数给用户输出一些日志信息。

```shell
message(WARNING "message text")  # 构建时打印警告信息
message(FATAL_ERROR "message text") # 产生CMAKE Error时，会停止编译构建过程
message(STATUS "message text")  # 常用于查看变量值，类似于编程语言中的 DEBUG 级别信息.
```

### 配置生成动态库或者静态库

配置内置变量BUILD_SHARED_LIBS，设置脚本默认构建库的模式为动态库,用户可以通过传入参数来设置生成的是动态库还是静态库

```shell
set(BUILD_SHARED_LIBS TRUE CACHE BOOL "If TRUE, tinyxpath is built as a shared library, otherwise as a static library")
```

### 常用变量定义

配置一些后面使用比较频繁的变量，变量名需要大写，并且变量名不能以CMAKE开头

```shell
set(TARGET_NAME tinyxpath)      # 定义变量存放库名
set(TARGET_SAMPLE_NAME tinyxpath_test)      # 定义变量存放库测试用例名
set(TARGET_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME})     # 定义变量存放库路径
set(TARGET_SRC ${TARGET_SRC_PATH}/tinystr.cpp)  # 定义变量存放库源码
if(BUILD_TESTS)     # 判断是否需要编译测试用例
    set(TARGET_SAMPLE_SRC ${TARGET_SRC_PATH}/htmlutil.cpp ${TARGET_SRC_PATH}/main.cpp)  # 定义变量存放测试程序源码
endif()
set(TARGET_INCLUDE ${TARGET_SRC_PATH})  # 定义变量存放编译库或编译测试用例所需要的头文件的路径
set(TARGET_INSTALL_INCLUDEDIR include)  # 定义变量存放三方库安装时，头文件存放的路径
set(TARGET_INSTALL_BINDIR bin)  # 定义变量存放三方库安装时，可执行二进制文件存放的路径
set(TARGET_INSTALL_LIBDIR lib)  # 定义变量存放三方库安装时，库存放的路径
```

### 引用其他三方库

引用其他三方库的方式有2种

- 引用其他三方库的源码
  
  1. 源码有cmake构建脚本，直接通过`add_subdirectory()`引用该三方库

     ```shell
     add_subdirectory(xxx)  # xxx是需要引入的三方库源码文件夹名字
     ```

  2. 源码非cmake构建或者无法通过cmake方式构建的，可以将引用三方库的源码添加到本三方库的构建中具体方式:

     ```shell
     set(SOURCE_NAME    source1.cxx source2.cxx ...)    # SOURCE_NAME 引入三方库源码变量名称， source1.cxx 引入的源码文件(需要包含文件的路径) 
     add_library(${TARGET} ${SOURCE_NAME} ...)  # TARGET 本库生成的库名， 通过 add_library 添加 引入三方库源码以及本库源码等生成目标TARGET
     ```

- 引用其他三方库的二进制文件(.so)
  
  对于二进制库的引入，使用find_package方法来引用。

  ```shell
  find_package(XXX REQUIRED)  # xxx表示要引入的库名
  ```

tinyxpath库为依赖其他三方库，故不需要做引入设置。
