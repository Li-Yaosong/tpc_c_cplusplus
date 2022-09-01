# 三方库xerces-c适配脚本的使用

## 简介

目前三方库xerces-c已适配了OpenHarmony标准系统，本文介绍如何使用这些适配脚本，帮助开发者在Openharmony上快速适配和使用三方库xerces-c



## 使用场景

适用于需要在OpenHarmony源码下构建的工程，在工程中需要使用到三方库xerces-c接口能力时，可使用本目录下的适配脚本，将三方库引入自己的工程



## 三方库版本及Liense

- xerces-c：

  版本： tags/v3.2.3,[下载地址](https://github.com/apache/xerces-c)

  liense： Apache License V2.0



## 本仓库版本

- tpc_c_cplusplus：master,[下载地址](https://gitee.com/openharmony-sig/tpc_c_cplusplus)



## 工程环境

- OpenHarmony源码版本：tags/OpenHarmony-v3.2-Beta2
- ubuntu 20.04



## 下载三方库源码

- 下载三方库xerces-c

  ```
  #在OpenHarmony源码根目录下的third_party目录下获取源库
  git clone https://github.com/apache/xerces-c.git -b v3.2.3
  ```

- 下载本仓库适配脚本

  ```
  #在OpenHarmony源码根目录下的third_party目录下下载设配脚本仓库
  git clone https://gitee.com/openharmony-sig/tpc_c_cplusplus.git
  ```

  

## 适配步骤

```
#将库的配置文件拷贝到三方库中，注意config.h文件中，如果系统是32位，SIZEOF_LONG的值修改为4，如果系统是64为，值修改为8
cp tpc_c_cplusplus/xerces-c/gn/xerces-c/config.h.in xerces-c/config.h

#将构建脚本拷贝到三方库目录下
cp tpc_c_cplusplus/xerces-c/gn/xerces-c/src/BUILD.gn xerces-c/src
cp tpc_c_cplusplus/xerces-c/gn/xerces-c/tests/BUILD.gn xerces-c/tests
cp tpc_c_cplusplus/xerces-c/gn/xerces-c/samples/BUILD.gn xerces-c/samples
```



## 引入三方库

- [将三方库加入GN工程](../../common/gn_build.md)



## 编译工程

- [工程加入OpenHarmony构建体系](../../common/gn_compile.md)



