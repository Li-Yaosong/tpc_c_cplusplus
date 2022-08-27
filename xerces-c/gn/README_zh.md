#  三方库xerces-c适配脚本的使用

### 目的

目前三方库xerces-c已适配了OpenHarmony标准系统，本文介绍如何使用这些适配脚本，帮助开发者在Openharmony上快速适配和使用三方库xerces-c



### 下载三方库源码

- 下载三方库xerces-c

  ```
  #在OpenHarmony源码根目录下的third_party目录下获取源库
  git clone https://github.com/apache/xerces-c.git -b v3.2.3
  ```



### 适配步骤

- 将三方库xerces-c拷贝至Openharmony源码的third_party目录下

- 将[xerces-c](xerces-c)/config.h.in文件修改成config.h，然后拷贝到源库xerces-c根目录下；注意，config.h文件中，如果系统是32位，SIZEOF_LONG的值修改为4，如果系统是64为，值修改为8
- 将[xerces-c](xerces-c)/src/BUILD.gn文件拷贝到源库xerces-c/src目录下
- 将[xerces-c](xerces-c)/tests/BUILD.gn文件拷贝到源库xerces-c/tests目录下
- 将[xerces-c](xerces-c)/samples/BUILD.gn文件拷贝到源库xerces-c/samples目录下



### 引入三方库

- [将三方库加入GN工程](../../common/gn-build.md)



### 编译工程

- [工程加入OpenHarmony构建体系](../../common/gn-compile.md)



