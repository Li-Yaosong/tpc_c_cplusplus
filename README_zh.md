# tpc_c_cplusplus

## 简介

本仓库主要用于存放已经适配OpenHarmony的C/C++三方库的适配脚本和OpenHarmony三方库适配指导文档、三方库适配相关的工具。

## 三方库适配

本仓库的三方库主要是通过[OpenHarmony linux SDK进行交叉编译](lycium/doc/ohos_use_sdk/OHOS_SDK-Usage.md)适配的，并集成到应用端使用。当前已经适配交叉编译的构建方式有：

- [cmake 编译构建](./docs/cmake_portting.md)
- [configure 编译构建方式](./docs/configure_portting.md)
- [make 编译构建](./docs/make_portting.md)

为了帮助开发者快速便捷的完成C/C++三方库交叉编译，我们开发了一套交叉编译框架[lycium](./lycium/README.md)，其涵盖了以上三种构建方式。

## 已适配三方库

- [c/c++三方库](./thirdparty/)

## 应用端集成三方库知识赋能

- [应用端集成三方库知识赋能](docs/thirdparty_knowledge.md)

## 本仓库目录

```shell
tpc_c_cplusplus
├── README_zh.md            # 仓库主页
├── docs                    # 说明文档/三方库适配文档8
├── lycium                  # 三方库适配相关工具
├── thirdparty              # 已经适配OpenHarmony的三方库的构建脚本
├── LICENSE                 # 本仓库的开源license文件
......
```

## 如何贡献

- [遵守仓库目录结构](#本仓库目录)
- [通过lycium快速交叉编译C/C++三方库](./lycium/)

## FAQ

[C/C++三方库常见FAQ](./docs/FAQ.md)。
