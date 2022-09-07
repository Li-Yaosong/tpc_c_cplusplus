# 三方库openjpeg基于gn构建版本使用指导

## 前提

根据本文档对该库进行GN构建时，默认你已经阅读了《[三方库[openjpeg]的构建与使用](../README_zh.md)》一文，并已经完成本库的构建。若未完成上述工作，请返回《[三方库[openjpeg]的构建与使用](../README_zh.md)》一文，并根据相关信息进行操作。




## 创建工程

参考《[工程加入OpenHarmony构建体系](../../common/gn_compile_build.md)》一文，创建开发者自己的工程。




## 三方库引入工程

参考《[三方库引入GN工程](../../common/gn_build.md)》一文，将三方库openjpeg引入开发者自己的工程。文中[thirdpartyA]对应本库库名*openjpeg*，[libA]对应本库动态库名*libjp2*，包含头文件路径需根据openjpeg三方库头文件实际路径进行调整。




## 在工程中使用本库

1. 在工程源码中包含本库对外暴露接口的头文件

   ```
   #include "openjpeg.h"
   ```
2. 在工程源码中直接调用本库接口，具体使用什么接口开发者可根据自身项目需要，下述伪代码仅供参考

   ```
   opj_codec_t * l_codec;
   opj_image_t * l_image;
   opj_stream_t * l_stream;
   ... ...
   opj_read_header(l_stream, l_codec, &l_image);
   ... ...
   ```



## 编译工程

开发者可参考《[三方库引入GN工程](../../common/gn_build.md)》、《[工程加入OpenHarmony构建体系](../../common/gn_compile_build.md)》文中编译步骤对工程进行编译，并检查编译输出是否正确。
