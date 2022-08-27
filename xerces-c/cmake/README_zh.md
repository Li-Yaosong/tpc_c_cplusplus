# 三方库xerces-c适配脚本的使用

### 目的

目前三方库xerces-c已适配了OpenHarmony标准系统，本文介绍如何使用这些适配脚本，帮助开发者在Openharmony上快速适配和使用三方库xerces-c



### 下载三方库源码

- 在github上下载xerces-c源码，版本取Tags/v3.2.3 ,[源库地址](https://github.com/apache/xerces-c)

- 下载xerces-c的依赖库icu,[下载地址](https://gitee.com/openharmony/third_party_icu)



### 适配步骤

- 将三方库third_party_icu拷贝至三方库xerces-c/src下

- 将[xerces-c](xerces-c)/config.h.in修改成config.h,然后拷贝到源库xerces-c目录下；注意，config.h文件中，如果系统是32位，SIZEOF_LONG的值修改为4，如果系统是64为，值修改为8

- 将[xerces-c](xerces-c)/src/CMakeLists.txt文件替换源库xerces-c/src/CMakeLists.txt文件

- 将[xerces-c](xerces-c)/src/third_party_icu/CMakeLists.txt文件替换源库xerces-c/src/third_party_icu/CMakeLists.txt文件



### 引入三方库

[将三方库加入IDE（DevEco Studio）工程](../../common/cmake-build.md)