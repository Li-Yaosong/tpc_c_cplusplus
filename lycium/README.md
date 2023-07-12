# HPKBUILD build script!
协助开发者, 在 OpenHarmony 系统上快速编译、验证以及长期维护 c/c++ 库. 
## 使用
执行脚本build.sh,自动编译main目录下的所有开源库，并打包安装到 usr\/\$pkgname-$ARCH-install 目录
```shell
    ./build.sh # 默认编译 main 目录下的多有库
```
```shell
    ./build.sh aaa bbb ccc ... # 编译 main 目录下指定的 aaa bbb ccc ...库 当 aaa 库存在依赖时，必须保证入参中包含依赖，否则 aaa 库不会编译
```
## 原则
**移植过程，不可以改源码（即不patchc/cpp文件，不patch构建脚本）。如移植必须patch，patch必须评审，给出充分理由。（不接受业务patch）**

## 如何贡献
为 lycium 项目共享，开源三方库

### 1.编译环境准备
请阅读 [Buildtools README](./Buildtools/README.md)

### 2.HPKBUILD 编写说明 
请阅读 [template README](./template/README.md) 

### 3.CI环境准备
请查阅 [lycium CItools](https://gitee.com/han_jin_fei/lycium-citools)

### 4.测试通过即可提交PR，附带测试成功的截屏

## 介绍
Buildtools: 存放编译环境准备说明

main: 被移植构建的库信息存放的目录

script: 项目依赖的脚本

template: main 目录中库的构建模板

build.sh: 顶层构建脚本

## TODO
支持同一个库，不同版本的编译
    1.库的依赖也可添加依赖的版本，实际版本大于等于依赖时，才可以编译
