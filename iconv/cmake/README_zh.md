# libiconv适配脚本使用

## 简介

目前三方库libiconv已适配了OpenHarmony标准系统，本文介绍如何使用这些适配脚本，帮助开发者在Openharmony上快速适配和使用三方库libiconv。

### 使用场景

在IDE上开发NAPI接口，工程中需要使用到三方库libiconv接口能力时，可使用本目录下的适配脚本，将三方库引入自己的工程。

### 工程版本

- API版本:   OpenHarmony SDK API 9
- IDE版本：DevEco Studio 3.0 Beta4
- 三方库版本：libiconv 1.17

## 快速适配

### 准备工程

- 工程配置
  1. 工程配置为Openharmony工程。
  2. API版本选在 API 9
  3. model选择Stage。
- 下载三方库libiconv源码:[下载地址]( https://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.17.tar.gz)。
- 下载本仓库适配脚本：[下载地址](https://gitee.com/openharmony-sig/tpc_c_cplusplus/repository/blazearchive/master.zip?Expires=1662019402&Signature=ZNj1BicLLs%2FKnw%2F5CEZVY427MI6CNVtlkxoLBzLiqAg%3D)。



### 适配步骤

- 将下载libiconv源码解压并拷贝到工程的cpp目录下(原库解压后文件夹名字是libiconv-1.17，可将文件夹重命名为libiconv)。
- 将下载的适配脚本中的[libiconv](libiconv)/include下的所有文件拷贝到源码目录的libiconv/include中。
- 将下载的适配脚本中的[libiconv](libiconv)/CMakelists.txt下的所有文件拷贝到源码目录的libiconv中。
- 将下载的适配脚本中的[libiconv](libiconv)/lib/CMakelists.txt下的所有文件拷贝到源码目录的libiconv/lib/中。
- 将下载的适配脚本中的[libiconv](libiconv)/libcharset/CMakelists.txt下的所有文件拷贝到源码目录的libiconv/libcharset/中。

### 引入三方库

- [将三方库加入IDE（DevEco Studio）工程](../../common/cmake_build.md)。



## 参考资料

- [如何将将三方库加入IDE（DevEco Studio）工程](../../common/cmake_build.md)。
- [知识体系](https://gitee.com/openharmony-sig/knowledge)。