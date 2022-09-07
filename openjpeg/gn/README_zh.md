# 三方库openjpeg GN构建指南

## 前提

根据本文档对该库进行GN构建时，默认你已经阅读了《[三方库openjpeg的构建与使用](../README_zh.md)》一文，并已经完成Openharmony的对应版本下载及依赖四方库的构建。若未完成上述工作，请返回《[三方库openjpeg的构建与使用](../README_zh.md)》一文，并根据相关信息进行操作。




## 源码下载

在OpenHarmony源码根目录下的third_party目录下执行如下指令，下载三方库openjpeg源码

```
git clone git@github.com:uclouvain/openjpeg.git -b v2.5.0
```



## 修改或添加编译文件

按照下述操作，将文件添加至工程路径，其中*.h.in文件为该库源码在Ubuntu20.04_x64环境下根据INSTALL.md指导文档执行cmake脚本获取的。经检查，这些文件中无CPU位数等硬件相关的配置，因此不需要修改，直接复制到工程路径中即可。

- 将[openjpeg](openjpeg)/BUILD.gn文件拷贝到源库openjpeg/目录下
- 将[openjpeg/src/bin/common](openjpeg/src/bin/common)/opj_apps_config.h.in文件修改成opj_apps_config.h，然后拷贝到源库openjpeg/src/bin/common/目录下
- 将[openjpeg/src/lib/openjp2](openjpeg/src/lib/openjp2)/opj_config_private.h.in文件修改成opj_config_private.h，然后拷贝到源库openjpeg/src/lib/openjp2/目录下
- 将[openjpeg/src/lib/openjp2](openjpeg/src/lib/openjp2)/opj_config.h.in文件修改成opj_config.h，然后拷贝到源库openjpeg/src/lib/openjp2/目录下




## 编译

完成上述步骤，三方库openjpeg就构建结束了。但若系统中没有组件依赖或调用，那么本库是不会参与编译的。开发者根据《[三方库openjpeg的构建与使用](../README_zh.md)》一文中三方库使用指导，将本库编译到系统中。

