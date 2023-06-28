# cmake 工具交叉编译说明

## 简介
CMake是一个跨平台的编译安装工具。

## 编译步骤

### 编译工具链下载

- 64位编译工具：gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu.tar  [下载链接](https://releases.linaro.org/components/toolchain/binaries/7.5-2019.12/aarch64-linux-gnu/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu.tar.xz)

- 32位编译工具：gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabi.tar [下载链接](https://releases.linaro.org/components/toolchain/binaries/7.5-2019.12/arm-linux-gnueabi/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabi.tar.xz)

### 解压编译工具链

- 解压64位 tar -xvf gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu.tar
- 解压32位 tar -xvf gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabi.tar

- 进入解压后的文件夹，查看bin目录下就有我们编译用到的工具链

### 下载解压cmake源码

- 下载最新版本的cmake源码,[下载链接](https://cmake.org/download/) (以3.27-rc2为例)
- 通过tar命令解压源码 tar -xzvf cmake-3.27.0-rc2.tar.gz

### 创建编译目录

- 进入源码解压后目录  cd cmake-3.27.0-rc2
- 创建64位编译目录  mkdir arm64_build 
- 创建32位编译目录 mkdir arm32_build 

### 设置交叉编译环境

- 设置64位交叉编译环境, xxx 是表示工具链存放的目录路径


```shell
export TOOLS=/xxx/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin
export AS=${TOOLS}/aarch64-linux-gnu-as
export CC=${TOOLS}/aarch64-linux-gnu-gcc
export CXX=${TOOLS}/aarch64-linux-gnu-g++
export LD=${TOOLS}/aarch64-linux-gnu-ld
export STRIP=${TOOLS}/aarch64-linux-gnu-strip
export RANLIB=${TOOLS}/aarch64-linux-gnu-ranlib
export OBJDUMP=${TOOLS}/aarch64-linux-gnu-objdump
export OBJCOPY=${TOOLS}/aarch64-linux-gnu-objcopy
export NM=${TOOLS}/aarch64-linux-gnu-gcc-nm
export AR=${TOOLS}/aarch64-linux-gnu-ar
export LDFLAGS="-static"
```

- 设置32位交叉编译环境, xxx 是表示工具链存放的目录路径

```shell

export TOOLS=/xxx/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabi/bin
export AS=${TOOLS}/arm-linux-gnueabi-as
export CC=${TOOLS}/arm-linux-gnueabi-gcc
export CXX=${TOOLS}/arm-linux-gnueabi-g++
export LD=${TOOLS}/arm-linux-gnueabi-ld
export STRIP=${TOOLS}/arm-linux-gnueabi-strip
export RANLIB=${TOOLS}/arm-linux-gnueabi-ranlib
export OBJDUMP=${TOOLS}/arm-linux-gnueabi-objdump
export OBJCOPY=${TOOLS}/arm-linux-gnueabi-objcopy
export NM=${TOOLS}/arm-linux-gnueabi-gcc-nm
export AR=${TOOLS}/arm-linux-gnueabi-ar
export LDFLAGS="-static"
```

### 生成makefile

- 进入64位编译路径 cd  arm64_build
- 执行如下命令生成makefile

```shell
cmake  ../  -L   -DCMAKE_USE_OPENSSL=OFF 
```

-  进入32位为 cd arm32_build ，由于编译测试是有一个动态库依赖，故关闭变成测试用例开关
- 执行如下命令生成makefile

```shell
cmake  ../  -L    -DCMAKE_USE_OPENSSL=OFF -DBUILD_TESTING=OFF
```

### 编译cmake源码

在对应的编译目录执行 make  VERBOSE=1,编译成功截图如下，以64位为例

&nbsp;![make_file](media/make_success.png)

### 编译完成

查看编译生成的文件ls bin/, 编译出来cmake ctest 就可以，并可通过file cmake 看到文件属性，必须是静态链接，不然OHOS系统会因为找不到动态链接库而报错

&nbsp;![32_file](media/32_file.png)

&nbsp;![64_file](media/64_file.png)

### 安装cmake

由于交叉编译不能make install安装cmake，故只能通过make install X86系统的cmake，这一步操作是为了解决在OHOS系统上执行cmake命令有提示报错的问题

&nbsp;![error](media/error.png)

- 重新开一个窗口，进入cmake源码目录 

- 创建一个host目录

```shell
mkdir host
```

- 进入host目录，执行 cmake生成makefile，命令如下

```shell
cmake  ../  -L   -DCMAKE_USE_OPENSSL=OFF -DCMAKE_INSTALL_PREFIX="${PWD}/install" 
```

- 执行make install  进行编译安装，完成后可以看到Modules目录下的文件

![insatll_file](media/install.png)

- 将./install/share/cmake-3.27/Modules/ 拷贝到交叉编译目录下

```shell
#重新进入交叉编译目录，新建share/cmake-3.27/目录
cd ../arm64_build
mkdir -p share/cmake-3.27/
cp ../host/install/share/cmake-3.27/Modules/ ./share/cmake-3.27/ -rf
```

### OHOS系统上运行

将编译目录文件压缩打包，发送到OHOS开发板中，进入bin路径，执行./ctest -version，截图如下

```shell
#打包64编译目录
tar -zcvf arm64_build.tar.gz arm64_build/
#发送到OHOS系统开发板上
hdc file send xxx\arm64_build.tar.gz /data   
#进入开发板/data，解压arm64_build.tar.gz
hdc shell
cd /data
tar -zxvf arm64_build.tar.gz
```

进入arm64_build/bin目录，执行./cmake -version 和 ./ctest -version 效果如下截图

&nbsp;![run_file](media/run.png)



