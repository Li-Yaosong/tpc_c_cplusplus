# 三方库xerces-c适配脚本的使用

## 简介

目前三方库xerces-c已适配了OpenHarmony标准系统，本文介绍如何使用这些适配脚本，帮助开发者在Openharmony上快速适配和使用三方库xerces-c



## 使用场景

在IDE上开发NAPI接口，工程中需要使用到三方库xerces-c接口能力时，可使用本目录下的适配脚本，将三方库引入自己的工程



## 三方库版本及license

- xerces-c： 

  版本：tags/v3.2.3,[下载地址](https://github.com/apache/xerces-c)

  license：Apache License V2.0

- icu：

  版本：tags/OpenHarmony-v3.2-Beta2,[下载地址](https://gitee.com/openharmony/third_party_icu)
  
  license：ICU License, BSD 3-Clause License

## 本仓库版本

- tpc_c_cplusplus：master,[下载地址](https://gitee.com/openharmony-sig/tpc_c_cplusplus)




## 工程环境

- IDE版本：DevEco Studio 3.0 Beta3 
- OpenHarmony SDK: 3.2.2.5
- API版本：API9
- Model：Stage



## 适配步骤

- 将下载的三方库xerces-c压缩包解压，并将库目录名修改为xerces-c

- 将下载的三方库icu压缩包解压，并将库目录名修改为third_party_icu

- 将下载的本仓库压缩包解压，并将库目录名修改为tpc_c_cplusplus

- 将三方库third_party_icu拷贝至三方库xerces-c/src目录下

- 将tpc_c_cplusplus/xerces-c/cmake/xerces-c/config.h.in修改成config.h,然后拷贝到三方库xerces-c目录下；注意，config.h文件中，如果系统是32位，SIZEOF_LONG的值修改为4，如果系统是64为，值修改为8

- 将tpc_c_cplusplus/xerces-c/cmake/xerces-c/src/CMakeLists.txt文件替换三方库xerces-c/src/CMakeLists.txt文件

- 将tpc_c_cplusplus/xerces-c/cmake/xerces-c/src/third_party_icu/CMakeLists.txt文件替换三方库xerces-c/src/third_party_icu/CMakeLists.txt文件

  

## 使用三方库

[将三方库加入IDE（DevEco Studio）工程](../../common/cmake_build.md)