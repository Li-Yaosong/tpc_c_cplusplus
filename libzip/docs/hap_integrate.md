# libzip如何集成到应用hap
## 准备应用工程
本库是基于DevEco Studio 3.1 Beta1版本，在RK3568开发板上验证的，如果是从未使用过RK3568，可以先查看[润和RK3568开发板标准系统快速上手](https://gitee.com/openharmony-sig/knowledge_demo_temp/tree/master/docs/rk3568_helloworld) 。
### 准备应用开发环境
开发环境的准备参考：[开发环境准备](https://gitee.com/openharmony-sig/knowledge_demo_temp/blob/master/docs/napi_study/docs/hello_napi.md)
### 增加构建脚本及配置文件
- 下载本仓库，并解压
  ```
  cd ~/
  git clone git@gitee.com:openharmony-sig/tpc_c_cplusplus.git --depth=1
  ```
- 三方库目录结构
  ```
  tpc_c_cplusplus/libzip    #三方库libzip 的目录结构如下
  ├── adapted               #存放三方库相关文档的文件夹
  ├── docs                  #存放三方库相关文档的文件夹
  ├── test                  #存放三方库test的文件夹
  ├── CmakeLists.txt        #构建脚本，支持hap包集成
  ├── README.OpenSource     #说明三方库源码的下载地址，版本，license等信息
  ├── README_zh.md   
  ```
- 将 libzip 拷贝至工程xxxx/entry/src/main/cpp/thirdparty目录下
### 准备三方库源码
- 三方库下载地址：[libzip](https://github.com/nih-at/libzip), 版本：v1.9.2
  解压后修改库文件名为libzip，拷贝至工程xxxx/entry/src/main/cpp/thirdparty/libzip目录下 
## 应用中使用三方库
- 将三方库加入工程中，目录结构如下
  ```
  demo/entry/src/main/cpp
  ├── thirdparty              #三方库存放目录
  │   ├──libzip               #三方库libzip
  ├── CMakeLists.txt          #工程目录的构建脚本
  ├── .....                   #工程目录的其他文件
  ```
- 在工程顶级CMakeLists.txt中引入三方库，增加如下代码
  ```
  include_directories(thirdparty/libzip/libzip/lib)
  add_subdirectory(thirdparty/libzip)      #引入子目录下的CMakeLists.txt
  target_link_libraries(工程库名 PUBLIC zip)   #工程依赖三方库libzip
  ```
## 编译工程
编译工程，安装应用可以参考 [应用的安装和运行](https://gitee.com/openharmony-sig/knowledge_demo_temp/blob/master/docs/napi_study/docs/hello_napi.md#%E5%AE%89%E8%A3%85%E8%B0%83%E8%AF%95) 

## 运行效果
- 在 [libzip](https://gitee.com/openharmony-tpc/openharmony_tpc_samples/tree/master/libzip) 中，运行效果如下图
  ![演示1](pic/hap1.jpeg)
  ![演示2](pic/hap2.png)

## 参考资料
- [润和RK3568开发板标准系统快速上手](https://gitee.com/openharmony-sig/knowledge_demo_temp/tree/master/docs/rk3568_helloworld)
- [OpenHarmony三方库地址](https://gitee.com/openharmony-tpc)
- [OpenHarmony知识体系](https://gitee.com/openharmony-sig/knowledge)
- [通过DevEco Studio开发一个NAPI工程](https://gitee.com/openharmony-sig/knowledge_demo_temp/blob/master/docs/napi_study/docs/hello_napi.md#%E5%A6%82%E4%BD%95%E9%80%9A%E8%BF%87deveco-studio%E5%BC%80%E5%8F%91%E4%B8%80%E4%B8%AAnapi%E5%B7%A5%E7%A8%8B)

