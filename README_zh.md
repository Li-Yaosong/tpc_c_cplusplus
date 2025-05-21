# tpc_c_cplusplus

## 简介

本仓库主要用于存放已经适配OpenHarmony的C/C++三方库的适配脚本和OpenHarmony三方库适配指导文档、三方库适配工具[lycium](#lycium-交叉编译框架)。

## 三方库适配

本仓库的三方库主要是通过[OpenHarmony SDK进行交叉编译](lycium/doc/ohos_use_sdk/OHOS_SDK-Usage.md)适配的，并集成到应用端进行使用。

在使用OpenHarmony的SDK进行交叉编译的过程中较关注的问题是：不同编译构建方式如何进行交叉编译、不同的编译构建平台如何配置交叉编译的环境、不同的交叉编译架构如何配置以及交叉编译后的产物如何进行测试验证。

![交叉编译](./docs/media/cross_compile_cpp.png)

### 编译构建方式

当前开源的C/C++三方库编译方式多样化，以下为主流的几种交叉编译方式：

- [cmake 编译构建](./docs/cmake_portting.md)。
- [configure 编译构建方式](./docs/configure_portting.md)。
- [make 编译构建](./docs/make_portting.md)。

为了帮助开发者快速便捷的完成C/C++三方库交叉编译，我们开发了一套交叉编译框架**lycium**，其涵盖了以上三种构建方式。

### lycium 交叉编译框架

#### 简介

lycium是一款协助开发者通过shell语言实现C/C++三方库快速交叉编译，并在OpenHarmony 系统上快速验证的编译框架工具。开发者只需要设置对应C/C++三方库的编译方式以及编译参数，通过lycium就能快速的构建出能在OpenHarmony 系统运行的二进制文件。

#### 原则

**移植过程，不可以改源码（即不patchc/cpp文件，不patch构建脚本）。如移植必须patch，patch必须评审，给出充分理由。（不接受业务patch）**

#### 目录结构介绍
```shell
lycium
├── doc                   # lycium工具相关说明文档
├── Buildtools            # 存放编译环境准备说明
├── CItools               # 测试环境搭建相关说明文档
├── script                # 项目依赖的脚本
├── template              # thirdparty 目录中库的构建/测试模板
├── build.sh              # 顶层构建脚本
├── test.sh               # 顶层测试脚本
```

#### 通过lycium工具快速共建C/C++三方库

##### 1. lycium框架下载
  ```shell
  git clone https://gitcode.com/openharmony-sig/tpc_c_cplusplus.git   # 下载框架代码
  cd tpc_c_cpluspluc/lycium                                           # 进入到框架目录
  ```
##### 2. 编译环境准备

  请阅读 [Buildtools README](./lycium/Buildtools/README.md)

##### 3. 修改三方库的编译方式以及编译参数

  lycium框架提供了[HPKBUILD](./lycium/template/HPKBUILD)文件供开发者对相应的C/C++三方库的编译配置。具体方法：

  - 在[thirdparty](./thirdparty/)目录下新建需要共建的三方库名字pkgname。
  - 将[HPKBUILD](./lycium/template/HPKBUILD)模板文件拷贝到新建三方库目录下。
  - 根据三方库实际情况修改[HPKBUILD](./lycium/template/HPKBUILD)模板，文件修改可参考[minizip共建](./docs/thirdparty_port.md#快速适配三方库实例)。

##### 4. 快速编译三方库

  配置完三方库的编译方式参数后，在[lycium](./lycium/)目录执行./build.sh pkgname，进行自动编译三方库，并打包安装到[lycium](./lycium/)目录的 usr/\$pkgname/$ARCH 目录

  ```shell
      ./build.sh # 默认编译 thirdparty 目录下的多有库
  ```
  
  ```shell
      ./build.sh aaa bbb ccc ... # 编译 thirdparty 目录下指定的 aaa bbb ccc ...库
  ```

  **lycium框架是通过linux shell脚本语言编写的，如果对shell语言不熟悉的开发者可以先学习[Linux Shell编程基础教程](https://blog.51cto.com/centos5/912584)。或者查找其他linux shell编程资料**

##### 5. 编译后三方库在DevEco Studio上使用

请阅读[北向应用如何使用三方库二进制文件](./lycium/doc/app_calls_third_lib.md)

##### 6. 在OpenHarmony设备上快速验证C/C++三方库

- CI环境准备

  业界内C/C++三方库测试框架多种多样，我们无法将其统一，因此为了保证原生库功能完整，我们基于原生库的测试用例进行测试验证。为此，我们需要集成了一套可以在OH环境上进行cmake, ctest等操作的环境，具体请阅读 [lycium CItools](./lycium/CItools/README_zh.md)。

- 测试脚本编写

  lycium框架提供了[HPKCHECK](./lycium/template/HPKCHECK)文件供开发者对相应的C/C++三方库的自动化测试，开发者只需在脚本中配置当前三方库需要测试的命令即可。

- 测试运行

  由于我们运行的是原生库的测试用例，因此我们需要将原生库的源码及生成文件都作为测试资源打包到开发板进行测试(直接将tpc_c_cplusplus直接打包成一个测试资源，并且保证测试资源在开发板的测试路径与编译路径保持一致，避免部分原生库因编译时对测试文件配置了路径而导致测试不过)，然后在[lycium](./lycium/)目录下执行脚本./test.sh,自动运行thridparty目录下已编译的三方库，并在终端显示已测试三方库总数以及通过和未通过的三方库。

   ```shell
   ./test.sh # 默认测试 thirdparty 目录下的所有已编译的三方库
   ```

   ```shell
    ./test.sh aaa bbb ccc ... # 测试 thirdparty 目录下指定的 aaa bbb ccc ...库 当指定的库未编译时不会进行测试
   ```

### 编译构建平台

当前大部分的三方库都是在linux环境下进行交叉编译构建的，除此外，我们也可能需要在windows、MacOS等平台进行构建：

- windows平台构建

 1. [windows平台交叉编译](./docs/adapter_windows.md)
 2. [IDE通过源码方式集成C/C++三方库](./docs/adapter_thirdlib.md)。

- MacOS平台构建指导

  请参考[Mac上使用OpenHarmony SDK交叉编译指导](./docs/adapter_mac.md)

### 添加不同CPU架构

当前`lycium`交叉编译适配的CPU架构只支持arm32位和arm64位的，如若需新增其他CPU架构，请参照[lycium上面适配OpenHarmony 不同架构的构建](./docs/adpater_architecture.md)

## 三方库测试验证

### 原生库测试用例验证

业界内C/C++三方库测试框架多种多样(ctest、make check以及原生库demo用例等)，我们无法将其统一，因此为了确保原生库功能的完整性，需基于原生库的测试用例进行测试验证。详细信息请参照[三方库快速验证指导](./docs/fast_verification.md)

### 北向应用调用

请阅读[北向应用如何使用三方库二进制文件](lycium/doc/app_calls_third_lib.md)

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

- [C/C++三方库常见FAQ](./docs/FAQ.md)。
