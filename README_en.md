# tpc_c_cplusplus

## 简介

本仓库主要用于存放已经适配OpenHarmony的C/C++三方库的适配脚本和OpenHarmony三方库适配指导文档、三方库适配相关的工具。

## 三方库适配

本仓库的三方库主要是通过OpenHarmony SDK进行交叉编译适配的，并集成到应用端进行使用。

在使用OpenHarmony的SDK进行交叉编译的过程中较关注的问题是：不同编译构建方式如何进行交叉编译、不同的编译构建平台如何配置交叉编译的环境、不同的交叉编译架构如何配置以及交叉编译后的产物如何进行测试验证。

### 编译前准备

#### OpenHarmony SDK 准备

OpenHarmony同时提供 linux/windwos以及mac平台的几种SDK，开发者可以在linux、windwos以及mac平台上进行交叉编译。本文以linux平台为主进行交叉编译的讲解。

1. 从 OpenHarmony SDK [官方发布渠道](https://gitcode.com/openharmony/docs/blob/master/zh-cn/release-notes/OpenHarmony-v5.0.2-release.md#%E4%BB%8E%E9%95%9C%E5%83%8F%E7%AB%99%E7%82%B9%E8%8E%B7%E5%8F%96) 下载对应版本的SDK。
2. 解压SDK

   ```shell
   owner@ubuntu:~/workspace$ tar -zxvf ohos-sdk-windows_linux-public.tar.tar.gz
   ```

3. 进入到sdk的linux目录，解压工具包：

   ```shell
   owner@ubuntu:~/workspace$ cd ohos_sdk/linux
   owner@ubuntu:~/workspace/ohos-sdk/linux$ for i in *.zip;do unzip ${i};done                   # 通过for循环一次解压所有的工具包
   owner@ubuntu:~/workspace/ohos-sdk/linux$ ls
   ets                                native                                   toolchains
   ets-linux-x64-4.0.1.2-Canary1.zip  native-linux-x64-4.0.1.2-Canary1.zip     toolchains-linux-x64-4.0.1.2-Canary1.zip
   js                                 previewer
   js-linux-x64-4.0.1.2-Canary1.zip   previewer-linux-x64-4.0.1.2-Canary1.zip
   ```

#### 三方库源码准备

开源的三方库编译构建方式多种多样，当前以支持构建的方式包含cmake、configure、make等。本文以主流的cmake构建方式的三方库cJSON为例进行讲解。

适配三方库如果没有指定版本，我们一般取三方库最新版本，不建议使用master的代码，这里我们下载cJSON v1.7.15 版本的源码：

```shell
   owner@ubuntu:~/workspace$ git clone https://github.com/DaveGamble/cJSON.git -b v1.7.15       # 通过git下载指定版本的源码
   Cloning into 'cJSON'...
   remote: Enumerating objects: 4545, done.
   remote: Total 4545 (delta 0), reused 0 (delta 0), pack-reused 4545
   Receiving objects: 100% (4545/4545), 2.45 MiB | 1.65 MiB/s, done.
   Resolving deltas: 100% (3026/3026), done.
   Note: switching to 'd348621ca93571343a56862df7de4ff3bc9b5667'.

   You are in 'detached HEAD' state. You can look around, make experimental
   changes and commit them, and you can discard any commits you make in this
   state without impacting any branches by switching back to a branch.

   If you want to create a new branch to retain commits you create, you may
   do so (now or later) by using -c with the switch command. Example:

   git switch -c <new-branch-name>

   Or undo this operation with:

   git switch -

   Turn off this advice by setting config variable advice.detachedHead to false

   owner@ubuntu:~/workspace$
   ```

### 编译构建 && 安装

1. 新建编译目录

  为了不污染源码目录文件，我们推荐在三方库源码目录新建一个编译目录，用于生成需要编译的配置文件，本用例中我们在cJSON目录下新建一个build目录：

   ```shell
   owner@ubuntu:~/workspace$ cd sJSON                                   # 进入cJSON目录
   owner@ubuntu:~/workspace/cJSON$ mkdir build && cd build              # 创建编译目录并进入到编译目录
   owner@ubuntu:~/workspace/cJSON/build$
   ```

2. 配置交叉编译参数，生成Makefile

   ```shell
   owner@ubuntu:~/workspace/cJSON/build$ /home/owner/workspace/ohos-sdk/linux/native/build-tools/cmake/bin/cmake -DCMAKE_TOOLCHAIN_FILE=//home/owner/workspace/ohos-sdk/linux/native/build/cmake/ohos.toolchain.cmake -DCMAKE_INSTALL_PREFIX=/home/owner/workspace/usr/cJSON -DOHOS_ARCH=arm64-v8a .. -L             # 执行cmake命令
   -- The C compiler identification is Clang 12.0.1
   -- Check for working C compiler: /home/ohos/tools/OH_SDK/ohos-sdk/linux/native/llvm/bin/clang # 采用sdk内的编译器
   -- Check for working C compiler: /home/ohos/tools/OH_SDK/ohos-sdk/linux/native/llvm/bin/clang -- works
   ...
   删除大量 cmake 日志
   ...
   ENABLE_PUBLIC_SYMBOLS:BOOL=ON
   ENABLE_SAFE_STACK:BOOL=OFF
   ENABLE_SANITIZERS:BOOL=OFF
   ENABLE_TARGET_EXPORT:BOOL=ON
   ENABLE_VALGRIND:BOOL=OFF
   owner@ubuntu:~/workspace/cJSON/build$ ls                                                     # 执行完cmake成功后在当前目录生成Makefile文件
   cJSONConfig.cmake  cJSONConfigVersion.cmake  CMakeCache.txt  CMakeFiles  cmake_install.cmake  CTestTestfile.cmake  fuzzing  libcjson.pc  Makefile  tests
   ```

   **注意这里执行的 cmake 必须是 SDK 内的 cmake，不是你自己系统上原有的 cmake 。否则会不识别参数OHOS_ARCH。**

   参数说明：
   1) CMAKE_TOOLCHAIN_FILE: 交叉编译置文件路径，必须设置成工具链中的配置文件。
   2) CMAKE_INSTALL_PREFIX: 配置安装三方库路径。
   3) OHOS_ARCH: 配置交叉编译的CPU架构，一般为arm64-v8a(编译64位的三方库)、armeabi-v7a(编译32位的三方库)，本示例中我们设置编译64位的cJSON库。
   4) -L: 显示cmake中可配置项目

3. 执行编译

   cmake执行成功后，在build目录下生成了Makefile，我们就可以直接执行make对cJSON进行编译了：

   ```shell
   owner@ubuntu:~/workspace/cJSON/build$ make                  # 执行make命令进行编译
   Scanning dependencies of target cjson
   [  2%] Building C object CMakeFiles/cjson.dir/cJSON.c.o
   clang: warning: argument unused during compilation: '--gcc-toolchain=//home/owner/workspace/ohos-sdk/linux/native/llvm' [-Wunused-command-line-argument]
   /home/owner/workspace/cJSON/cJSON.c:561:9: warning: 'long long' is an extension when C99 mode is not enabled [-Wlong-long]
   ...
   删除大量 make 日志
   ...
   clang: warning: argument unused during compilation: '--gcc-toolchain=//home/owner/workspace/ohos-sdk/linux/native/llvm' [-Wunused-command-line-argument]
   [ 97%] Building C object fuzzing/CMakeFiles/fuzz_main.dir/cjson_read_fuzzer.c.o
   clang: warning: argument unused during compilation: '--gcc-toolchain=//home/owner/workspace/ohos-sdk/linux/native/llvm' [-Wunused-command-line-argument]
   [100%] Linking C executable fuzz_main
   [100%] Built target fuzz_main
   owner@ubuntu:~/workspace/cJSON/build$
   ```

4. 查看编译后文件属性

   编译成功后我们可以通过file命令查看文件的属性，以此判断交叉编译是否成功，如下信息显示libcjson.so为aarch64架构文件，即交叉编译成功：

   ```shell
   owner@ubuntu:~/workspace/cJSON/build$ file libcjson.so.1.7.15     # 查看文件属性命令
   libcjson.so.1.7.15: ELF 64-bit LSB shared object, ARM aarch64, version 1 (SYSV), dynamically linked, BuildID[sha1]=c0aaff0b401feef924f074a6cb7d19b5958f74f5, with debug_info, not stripped
   ```

5. 执行安装命令

   编译成功后，我们可以执行make install将编译好的二进制文件以及头文件安装到cmake配置的安装路径下：

   ```shell
   owner@ubuntu:~/workspace/cJSON/build$ make install                # 执行安装命令
   [  4%] Built target cjson
   [  8%] Built target cJSON_test
   ...
   删除大量make install信息
   ...
   -- Installing: /home/owner/workspace/usr/cJSON/lib/cmake/cJSON/cJSONConfig.cmake
   -- Installing: /home/owner/workspace/usr/cJSON/lib/cmake/cJSON/cJSONConfigVersion.cmake
   owner@ubuntu:~/workspace/cJSON/build$
   owner@ubuntu:~/workspace/cJSON/build$ ls /home/owner/workspace/usr/cJSON                  # 查看安装文件
   include  lib
   owner@ubuntu:~/workspace/cJSON/build$ ls /home/owner/workspace/usr/cJSON/lib/
   cmake  libcjson.so  libcjson.so.1  libcjson.so.1.7.15  pkgconfig
   ```

### configure 编译构建

configure 是一个由 GNU Autoconf 提供的脚本用于自动生成 Makefile。在此以`jpeg`库来展示如何将一个configure构建方式的三方库在linux环境上通过`OpenHarmony SDK`进行交叉编译。

SDK和源码准备请参考上面的[SDK准备](#openharmony-sdk-准备)

1. 准备源码
   ```shell
   owner@ubuntu:~/workspace$ wget http://www.ijg.org/files/jpegsrc.v9e.tar.gz       ## 下载指定版本的源码包
   owner@ubuntu:~/workspace$ tar -zxvf jpegsrc.v9e.tar.gz                           ## 解压源码包
   ```
2. 查看configure配置
  进入到jpeg目录执行configure配置，如若对configure配置项不熟悉，我们可以通过运行`configure --help`查看：

   ```shell
   owner@ubuntu:~/workspace/jpeg-9e$ ./configure --help
   `configure` configures libjpeg 9.5.0 to adapt to many kinds of systems.
   Usage: ./configure [OPTION]... [VAR=VALUE]...
   ...
   # 去除大量不必要信息
   ...
   # 配置安装选项
   Installation directories:
     --prefix=PREFIX         install architecture-independent files in PREFIX
                          [/usr/local]
   ...
   # 去除大量不必要信息
   ...
   # 配置编译的主机选项(--host)，默认配置为linux
   System types:
   --build=BUILD     configure for building on BUILD [guessed]
   --host=HOST       cross-compile to build programs to run on HOST [BUILD]
   --target=TARGET   configure for building compilers for TARGET [HOST]
   # cJSON库配置可选项
   Optional Features:
   --disable-option-checking  ignore unrecognized --enable/--with options
   --disable-FEATURE       do not include FEATURE (same as --enable-FEATURE=no)
   --enable-FEATURE[=ARG]  include FEATURE [ARG=yes]
   --enable-silent-rules   less verbose build output (undo: "make V=1")
   --disable-silent-rules  verbose build output (undo: "make V=0")
   --enable-maintainer-mode
                          enable make rules and dependencies not useful (and
                          sometimes confusing) to the casual installer
   --enable-dependency-tracking
                          do not reject slow dependency extractors
   --disable-dependency-tracking
                          speeds up one-time build
   --enable-ld-version-script
                          enable linker version script (default is enabled
                          when possible)
   --enable-shared[=PKGS]  build shared libraries [default=yes]
   --enable-static[=PKGS]  build static libraries [default=yes]
   --enable-fast-install[=PKGS]
                          optimize for fast installation [default=yes]
   --disable-libtool-lock  avoid locking (might break parallel builds)
   --enable-maxmem=N     enable use of temp files, set max mem usage to N MB
   ...
   # 去除大量不必要信息
   ...
   # 配置编译命令(默认使用linux gcc相关配置)
   Some influential environment variables:
     CC          C compiler command
     CFLAGS      C compiler flags
     LDFLAGS     linker flags, e.g. -L<lib dir> if you have libraries in a
                 nonstandard directory <lib dir>
     LIBS        libraries to pass to the linker, e.g. -l<library>
     CPPFLAGS    (Objective) C/C++ preprocessor flags, e.g. -I<include dir> if
                 you have headers in a nonstandard directory <include dir>
     CPP         C preprocessor
     LT_SYS_LIBRARY_PATH
                 User-defined run-time library search path.
   
   Use these variables to override the choices made by `configure` or to help
   it to find libraries and programs with nonstandard names/locations.
   Report bugs to the package provider.
   ```

   由configure的帮助信息我们可以知道，jpeg交叉编译需要配置主机(编译完后需要运行的系统机器), 需要配置交叉编译命令以以及配置安装路径等选项。

3. 配置交叉编译命令,在命令行输入以下命令：
   ```shell
   export OHOS_SDK=/home/owner/tools/OHOS_SDK/ohos-sdk/linux/                   ## 配置SDK路径，此处需配置成自己的sdk解压目录
   export AS=${OHOS_SDK}/native/llvm/bin/llvm-as
   export CC="${OHOS_SDK}/native/llvm/bin/clang --target=aarch64-linux-ohos"    ## 32bit的target需要配置成 --target=arm-linux-ohos
   export CXX="${OHOS_SDK}/native/llvm/bin/clang++ --target=aarch64-linux-ohos" ## 32bit的target需要配置成 --target=arm-linux-ohos
   export LD=${OHOS_SDK}/native/llvm/bin/ld.lld
   export STRIP=${OHOS_SDK}/native/llvm/bin/llvm-strip
   export RANLIB=${OHOS_SDK}/native/llvm/bin/llvm-ranlib
   export OBJDUMP=${OHOS_SDK}/native/llvm/bin/llvm-objdump
   export OBJCOPY=${OHOS_SDK}/native/llvm/bin/llvm-objcopy
   export NM=${OHOS_SDK}/native/llvm/bin/llvm-nm
   export AR=${OHOS_SDK}/native/llvm/bin/llvm-ar
   export CFLAGS="-fPIC -D__MUSL__=1"                                            ## 32bit需要增加配置 -march=armv7a
   export CXXFLAGS="-fPIC -D__MUSL__=1"                                          ## 32bit需要增加配置 -march=armv7a
   ```

4. 执行configure命令

  安装路劲以及host配置可以在configure时执行，此处以配置arm64位为例，如若需要配置32位，将`aarch64-arm`替换成`arm-linux`即可。

   ```shell
   owner@ubuntu:~/workspace/jpeg-9e$ ./configure --prefix=/home/owner/workspace/usr/jpeg --host=aarch64-linux       # 执行configure命令配置交叉编译信息
   checking build system type... x86_64-pc-linux-gnu
   checking host system type... x86_64-pc-linux-gnu
   checking target system type... x86_64-pc-linux-gnu
   ...
   # 删除大量configure信息
   ...
   configure: creating ./config.status
   config.status: creating Makefile
   config.status: creating libjpeg.pc
   config.status: creating jconfig.h
   config.status: executing depfiles commands
   config.status: executing libtool commands
   ```

   执行完confiure没有提示任何错误，即说明confiure配置成功，在当前目录会生成Makefile文件。

5. 执行make编译命令
  configure执行成功后，在当前目录会生成Makefile文件，直接运行make即可进行交叉编译：

   ```shell
   owner@ubuntu:~/workspace/jpeg-9e$ make                       # 执行make编译命令
   make  all-am
   make[1]: Entering directory '/home/owner/workspace/jpeg-9e'
     CC       cjpeg.o
     CC       rdppm.o
     ...
     # 删除大量make信息
     ...
     CC       rdcolmap.o
     CCLD     djpeg
     CC       jpegtran.o
     CC       transupp.o
     CCLD     jpegtran
     CC       rdjpgcom.o
     CCLD     rdjpgcom
     CC       wrjpgcom.o
     CCLD     wrjpgcom
   make[1]: Leaving directory '/home/owner/workspace/jpeg-9e'
   ```

6. 执行安装命令
   ```shell
   owner@ubuntu:~/workspace/jpeg-9e$ make install
   ```
  
  执行完后对应的文件安装到prefix配置的路径`/home/owner/workspace/usr/jpeg`, 查看对应文件属性：

  ```shell
   owner@ubuntu:~/workspace/jpeg-9e$ cd /home/owner/workspace/usr/jpeg
   owner@ubuntu:~/workspace/usr/jpeg$ ls
   bin  include  lib  share
   owner@ubuntu:~/workspace/usr/jpeg$ ls lib
   libjpeg.a  libjpeg.la  libjpeg.so  libjpeg.so.9  libjpeg.so.9.5.0  pkgconfig
   owner@ubuntu:~/workspace/usr/jpeg$ ls include/
   jconfig.h  jerror.h  jmorecfg.h  jpeglib.h
   owner@ubuntu:~/workspace/usr/jpeg$ file lib/libjpeg.so.9.5.0
   lib/libjpeg.so.9.5.0: ELF 64-bit LSB shared object, ARM aarch64, version 1 (SYSV), dynamically linked, with debug_info, not stripped
   ```

## 三方库使用

1. 将三方库生成的二进制文件拷贝到应用工程目录

   为了更好的管理应用集成的三方库，在应用工程的cpp目录新建一个thirdparty目录，将生成的二进制文件以及头文件拷贝到该目录下，如下图所示,xxx代表的是三方库名称，xxx文件夹下包含了arm架构以后aarch64架构2种方式生成的二进制文件，每种架构目录下包含了该库的头文件(include)以及二进制文件(lib)：

   ![lib location](./lycium/doc/media/lib_location.png)

   如果该三方库二进制文件为so文件，还需要将so文件拷贝到工程目录的`entry/libs/${OHOS_ARCH}/`目录下,如下图:

   ![so localtion](./lycium/doc/media/so_location.png)

   **动态库引用事项注意: 1、应用在引用动态库的时候是通过soname来查找的，所以我们需要将名字为soname的库文件拷贝到entry/libs/${OHOS_ARCH}/目录下(soname查看方法：` $OHOS_SDK/native/llvm/bin/llvm-readelf -d libxxx.so`)。**

   &nbsp;![soname](./lycium/doc/media/soname.png)

   **2、 要正确的拷贝SO文件，如下图所示：** <br>
    原库大小：<br>
    ![lib size](./lycium/doc/media/so_size.png)

    正确拷贝so文件后so文件大小应该与原库实体文件大小一致，如下图所示：(拷贝方法：不通过压缩直接将so文件拷贝到windows或将so文件压缩成.zip格式拷贝到windwos) <br>
    ![corect size](./lycium/doc/media/so_correct_size.png)

    如果将so文件以tar,gz,7z,bzip2等压缩方式拷贝到windwos后在解压，其文件是实体库的软连接，其大小和实体库大小不一致，文件也不能正常使用：<br>
    ![wrong size](./lycium/doc/media/so_wrong_size.png)

2. 配置对应链接

   配置链接只需要在cpp目录的CMakeLists.txt文件中添加对应`target_link_libraries`即可：
   - 配置静态库链接

    ```cmake
    target_link_libraries(entry PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/xxx/${OHOS_ARCH}/lib/libxxx.a)
    ```

   - 配置动态库链接

    ```cmake
    target_link_libraries(entry PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/xxx/${OHOS_ARCH}/lib/libxxx.so)
    ```

    ![so link](./lycium/doc/media/so_link.png)

3. 配置头文件路径

   配置链接只需要在cpp目录的CMakeLists.txt文件中添加对应`target_include_directories`

    ```cmake
    target_include_directories(entry PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/xxx/${OHOS_ARCH}/include)
    ```

4. 编写NAPI接口

   配置完三方库的链接和头文件路径后，可以根据各自的业务逻辑调用三方库对应的接口完成NAPI接口的编写，NAPI接口开发可以参照文档[NAPI学习](docs/thirdparty_knowledge.md###北向应用中使用).

5. 编译构建

   请参考文档[DevEco Studio编译构建指南](https://developer.harmonyos.com/cn/docs/documentation/doc-guides-V3/build_overview-0000001055075201-V3?catalogVersion=V3)

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
