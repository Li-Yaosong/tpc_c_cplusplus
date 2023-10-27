# OpenHarmony交叉编译环境配置

## 简介

本文介绍在使用`lycium`框架快速交叉编译三方库前如何进行环境配置。

## 基本工具准备

`lycium`框架支持多种构建方式的三方库，为了保障三方库能正常编译，我们需要保证编译环境中包含以下几个基本编译命令：
`gcc`, `cmake`, `make`, `pkg-config`, `autoconf`, `autoreconf`, `automake`,
如若缺少相关命令，可通过官网下载对应版本的工具包，也可以在编译机上通过命令安装，如若`Ubuntu`系统上缺少`cmake`可以通过以下命令安装：

```shell
sudo apt install cmake
```

## 下载ohos sdk

[参考OHOS_SDK-Usage](../doc/ohos_use_sdk/OHOS_SDK-Usage.md)

## 配置环境变量

`lycium`支持的是C/C++三方库的交叉编译，SDK工具链只涉及到`native`目录下的工具，故OHOS_SDK的路径需配置成`native`工具的父目录，linux环境中配置SDK环境变量方法如下：

```shell
    export OHOS_SDK=/home/ohos/tools/OH_SDK/ohos-sdk/linux      # 此处SDK的路径使用者需配置成自己的sdk解压目录
```

## 拷贝编译工具

为了简化开发中命令的配置，我们针对arm架构以及aarch64架构集成了几个编译命令，存放在`lycium/Buildtools`目录下，在使用`lycium`工具前，需要将这些编译命令拷贝到SDK对应的目录下，具体操作如下：

```shell
    cd lycium/Buildtools                        # 进入到工具包目录
    sha512sum -c SHA512SUM                      # 可校验工具包是否正常, 若输出"toolchain.tar.gz: OK"则说明工具包正常，否则说明工具包异常，需重新下载
    tar -zxvf toolchain.tar.gz                  # 解压拷贝编译工具
    cp toolchain/* ${OHOS_SDK}/native/llvm/bin  # 将命令拷贝到工具链的native/llvm/bin目录下
```

## 设置编译机cmake识别OHOS系统

由于sdk中的cmake版本过低, 导致很多开源库在cmake阶段报错. 这个时候就需要用户在编译机上安装一个高版本的cmake(推荐使用3.26及以上版本). 但是
cmake官方是不支持OHOS的. 解决方案:

```shell
cp $OHOS_SDK/native/build-tools/cmake/share/cmake-3.16/Modules/Platform/OHOS.cmake xxx(代表你编译机安装的cmake的路径)/cmake-3.26.3-linux-x86_64/share/cmake-3.26/Modules/Platform
```
