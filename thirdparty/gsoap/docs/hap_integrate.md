# gSOAP Toolkit集成到应用hap

本库是在RK3568开发板上基于OpenHarmony3.2 Release版本的镜像验证的，如果是从未使用过RK3568，可以先查看[润和RK3568开发板标准系统快速上手](https://gitee.com/openharmony-sig/knowledge_demo_temp/tree/master/docs/rk3568_helloworld)。

## 开发环境

- [开发环境准备](../../../docs/hap_integrate_environment.md)

## 编译三方库

- 下载本仓库

  ```shell
  git clone https://gitee.com/openharmony-sig/tpc_c_cplusplus.git --depth=1
  ```

- 三方库目录结构

  ```shell
  tpc_c_cplusplus/thirdparty/gsoap      #三方库gsoap的目录结构如下
  ├── docs                              #三方库相关文档的文件夹
  ├── HPKBUILD                          #构建脚本
  ├── HPKCHECK                          #测试脚本
  ├── OAT.xml                           #扫描结果文件
  ├── SHA512SUM                         #三方库校验文件
  ├── README.OpenSource                 #说明三方库源码的下载地址，版本，license等信息
  ├── README_zh.md                      #三方库简介
  ├── gsoap_ohos_pkg.patch              #用于gsoap库编译的补丁
  ```

- 在lycium目录下编译三方库

  编译环境的搭建参考[准备三方库构建环境](../../../lycium/README.md#1编译环境准备)

  ```shell
  cd lycium
  ./build.sh gsoap openssl zlib
  ```

- 三方库头文件及生成的库

  在lycium目录下会生成usr目录，该目录下存在已编译完成的32位和64位三方库

  ```shell
  gsoap/arm64-v8a   gsoap/armeabi-v7a
  ```
  
- [测试三方库](#测试三方库)

## 应用中使用三方库

- 使用bin目录下的 wsdl2h 和 &nbsp;soapcpp2 二进制文件自动生成源代码
  
  ```shell
  #执行 wsdl2h 从本地文件读取 WSDL 并自动生成一个默认名称的输出文件
  wsdl2h calc.wsdl
  #执行 wsdl2h 从远程 URL 读取 WSDL 并自动生成一个具有指定名称的输出文件
  wsdl2h -o calc.h http://www.genivia.com/calc.wsdl
  
  #执行 soapcpp2 从 calc.h 文件中自动生成客户端的源代码，-c 选项指定了生成客户端代码
  soapcpp2 -c calc.h
  
  #自动生成客户端的源代码
  #calc.nsmap
  #soapC.cpp
  #soapcalcProxy.cpp
  #soapcalcProxy.h
  #soapH.h
  #soapStub.h
  ```
  
- 在IDE的cpp目录下新增thirdparty目录，将编译生成的头文件拷贝到该目录下；在IDE的cpp目录下新增lib目录,将编译生成的.a文件拷贝到该目录下；在IDE的cpp目录下将自动生成客户端的源代码文件拷贝到该目录下。如下图所示：

&nbsp;![thirdparty_install_dir](pic/gsoap_install_dir.png)

- 在最外层（cpp目录下）CMakeLists.txt中添加如下语句，libz.a需要自己编译仓库里面的

  ```cmake
  #将执行 soapcpp2 自动生成的 soapC.cpp 和 soapcalcProxy.cpp 依赖文件加入工程中
  add_library(entry SHARED soapC.cpp soapcalcProxy.cpp napi_init.cpp)
  
  #将三方库加入工程中
  target_link_libraries(entry PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/gsoap/${OHOS_ARCH}/lib/libgsoap++.a)
  #将三方库的头文件加入工程中
  target_include_directories(entry PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/gsoap/${OHOS_ARCH}/include)
  ```

  调用三方库须知

  ```c++
  #调用需注意
  #添加soapcpp2自动生成客户端的源代码头文件
  #include "soapcalcProxy.h"
  #include "calc.nsmap"

## 测试三方库

三方库的测试使用原库自带的测试用例来做测试，[准备三方库测试环境](../../../lycium/README.md#3ci环境准备)

进入到构建目录下的sample文件夹中运行测试用例，涉及客户端和服务端程序需要开两个终端窗口测试。

（gsoap-2.8-arm64-v8a-build/gsoap/samples为构建64位的目录，gsoap-2.8-armeabi-v7a-build/gsoap/samples为构建32位的目录）

&nbsp;![gsoap_test](pic/gsoap_test.png)

## 参考资料

- [OpenHarmony三方库地址](https://gitee.com/openharmony-tpc)
- [OpenHarmony知识体系](https://gitee.com/openharmony-sig/knowledge)
- [gsoap toolkit三方库地址](https://sourceforge.net/projects/gsoap2/files/)
