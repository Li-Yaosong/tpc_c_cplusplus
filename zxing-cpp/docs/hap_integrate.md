# zxing-cpp如何集成到应用hap
## 准备应用工程
本库是基于DevEco Studio 3.1 Beta1版本，在RK3568开发板上验证的，如果是从未使用过RK3568，可以先查看[润和RK3568开发板标准系统快速上手](https://gitee.com/openharmony-sig/knowledge_demo_temp/tree/master/docs/rk3568_helloworld)。
### 准备应用开发环境
开发环境的准备参考：[开发环境准备](https://gitee.com/openharmony-sig/knowledge_demo_temp/blob/master/docs/napi_study/docs/hello_napi.md#%E5%B7%A5%E7%A8%8B%E5%87%86%E5%A4%87)
### 增加构建脚本及配置文件
- 下载本仓库，并解压
- 三方库目录结构
  ```
  tpc_c_cplusplus/zxing-cpp  #三方库zxing-cpp的目录结构如下
  ├── adapted               #存放三方库适配需要的代码文件
  ├── docs                  #存放三方库相关文档的文件夹
  ├── CmakeLists.txt        #构建脚本，支持hap包集成
  ├── bundle.json           #三方库组件定义文件
  ├── README.OpenSource     #说明三方库源码的下载地址，版本，license等信息
  ├── README_zh.md   
  ```
- 将zxing-cpp拷贝至工程xxxx/entry/src/main/cpp/thirdparty目录下
### 准备三方库源码
- 三方库下载地址：[zxing-cpp](https://github.com/zxing-cpp/zxing-cpp), 版本：v2.10.1
  解压后修改库文件名为zxing-cpp，拷贝至工程xxxx/entry/src/main/cpp/thirdparty/zxing-cpp目录下
- 依赖库地址：[stb](https://github.com/nothings/stb),版本:master
  解压后把文件stb/deprecated/stb_image.c、stb/stb_image.h、stb/stb_image_write.h拷贝到zxing-cpp/core/src目录  
## 应用中使用三方库
- 将三方库加入工程中，目录结构如下
  ```
  demo/entry/src/main/cpp
  ├── thirdparty         #三方库存放目录
  │   ├──  zxing-cpp      #三方库zxing-cpp
  ├── CMakeLists.txt     #工程目录的构建脚本
  ├── .....              #工程目录的其他文件
  ```
- 在工程顶级CMakeLists.txt中引入三方库，增加如下代码
  ```
  add_subdirectory(thirdparty/zxing-cpp)      #引入子目录下的CMakeLists.txt
  target_link_libraries(工程库名 PUBLIC zxing-cpp)   #工程依赖三方库zxing-cpp
  target_include_directories(工程库名 PRIVATE thirdparty/zxing-cpp/include)
  ```
## 编译工程
编译工程，安装应用可以参考 [应用的安装和运行](https://gitee.com/openharmony-sig/knowledge_demo_temp/blob/master/docs/napi_study/docs/hello_napi.md#%E5%AE%89%E8%A3%85%E8%B0%83%E8%AF%95)

## 运行效果
- 在 [zxing-cpp](https://gitee.com/openharmony-tpc/openharmony_tpc_samples/tree/master/zxing-cpp)中，运行效果如下图
  &nbsp;![演示](pic/hap.jpg)
## 参考资料
- [润和RK3568开发板标准系统快速上手](https://gitee.com/openharmony-sig/knowledge_demo_temp/tree/master/docs/rk3568_helloworld)
- [OpenHarmony三方库地址](https://gitee.com/openharmony-tpc)
- [OpenHarmony知识体系](https://gitee.com/openharmony-sig/knowledge)
- [通过DevEco Studio开发一个NAPI工程](https://gitee.com/openharmony-sig/knowledge_demo_temp/blob/master/docs/napi_study/docs/hello_napi.md)

