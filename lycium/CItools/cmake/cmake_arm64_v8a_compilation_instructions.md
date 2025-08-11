# cmake工具arm64-v8a交叉编译说明

## 简介
CMake是一个跨平台的编译安装工具。

本文档主要介绍其arm64位交叉编译步骤

## 编译步骤

### 环境准备
- [交叉编译环境准备](../../../lycium/Buildtools/README.md)

### 下载解压cmake源码

- 下载最新版本的cmake源码,[下载链接](https://cmake.org/files/v3.27/cmake-3.27.0-rc2.tar.gz) (以3.27-rc2为例)
- 通过tar命令解压源码 tar -xzvf cmake-3.27.0-rc2.tar.gz

### 创建编译目录

- 进入源码解压后目录 cd cmake-3.27.0-rc2
- 创建64位编译目录 mkdir arm64_v8a

### 设置交叉编译环境

- [设置交叉编译环境变量](../../../lycium/doc/ohos_use_sdk/OHOS_SDK-Usage.md#%E6%96%B9%E6%B3%952%E4%BD%BF%E7%94%A8%E9%A1%B9%E7%9B%AE%E5%8E%9F%E7%94%9F%E7%9A%84%E6%9E%84%E5%BB%BA%E5%B7%A5%E5%85%B7%E4%BE%8B%E5%A6%82-configuremakefile)


### 生成makefile

- 进入64位编译路径 cd arm64_v8a
- 执行如下命令生成makefile

```shell
cmake ../ -L -DCMAKE_USE_OPENSSL=OFF -DBUILD_TESTING=OFF
```

### 编译cmake源码

在对应的编译目录执行 make VERBOSE=1,编译成功截图如下

&nbsp;![file](media/build_success_64.png)

### 编译完成

查看编译生成的文件ls bin/, 编译出来cmake ctest 就可以，并可通过file cmake 看到文件属性，必须是静态链接

&nbsp;![64_file](media/64_file.png)

### 安装cmake

由于交叉编译不能make install安装cmake，故make install X86系统的cmake

- 重新开一个窗口，进入cmake源码目录 

- 创建一个host目录

```shell
mkdir host
```

- 进入host目录，执行 cmake生成makefile，命令如下

```shell
cmake ../ -L -DCMAKE_USE_OPENSSL=OFF -DCMAKE_INSTALL_PREFIX="${PWD}/install" 
```

- 执行make install  进行编译安装，完成后可以看到Modules目录下的文件

![insatll_file](media/install.png)

- 将./install/share/cmake-3.27/Modules/ 拷贝到交叉编译目录下

```shell
#重新进入交叉编译目录，新建share/cmake-3.27/目录
cd ../arme64_v8a
mkdir -p share/cmake-3.27/
cp ../host/install/share/cmake-3.27/Modules/ ./share/cmake-3.27/ -rf
```

### OHOS系统上运行

将编译目录文件压缩打包，发送到OHOS开发板中

```shell
#打包编译目录
tar -zcvf arme64_v8a.tar.gz arm64_build/
#发送到OHOS系统开发板上
hdc file send xxx\arme64_v8a.tar.gz /data   
#进入开发板/data，解压arme64_v8a.tar.gz
hdc shell
cd /data
tar -zxvf arme64_v8a.tar.gz
```

进入arme64_v8a/bin目录，执行./cmake -version 和 ./ctest -version 效果如下截图

&nbsp;![run_file](media/run.png)



