# bctoolbox集成到应用hap

本库是在RK3568开发板上基于OpenHarmony3.2 Release版本的镜像验证的，如果是从未使用过RK3568，可以先查看[润和RK3568开发板标准系统快速上手](https://gitee.com/openharmony-sig/knowledge_demo_temp/tree/master/docs/rk3568_helloworld)。

## 开发环境

- ubuntu20.04
- [OpenHarmony3.2Release镜像](https://gitee.com/link?target=https%3A%2F%2Frepo.huaweicloud.com%2Fopenharmony%2Fos%2F3.2-Release%2Fdayu200_standard_arm32.tar.gz)
- [ohos_sdk_public 4.0.8.1 (API Version 10 Release)](https://gitee.com/link?target=http%3A%2F%2Fdownload.ci.openharmony.cn%2Fversion%2FMaster_Version%2FOpenHarmony_4.0.8.1%2F20230608_091058%2Fversion-Master_Version-OpenHarmony_4.0.8.1-20230608_091058-ohos-sdk-public.tar.gz)
- [DevEco Studio 3.1 Release](https://gitee.com/link?target=https%3A%2F%2Fcontentcenter-vali-drcn.dbankcdn.cn%2Fpvt_2%2FDeveloperAlliance_package_901_9%2F81%2Fv3%2FtgRUB84wR72nTfE8Ir_xMw%2Fdevecostudio-windows-3.1.0.501.zip%3FHW-CC-KV%3DV1%26HW-CC-Date%3D20230621T074329Z%26HW-CC-Expire%3D315360000%26HW-CC-Sign%3D22F6787DF6093ECB4D4E08F9379B114280E1F65DA710599E48EA38CB24F3DBF2)
- [准备三方库构建环境](../../../lycium/README.md#1编译环境准备)
- [准备三方库测试环境](../../../lycium/README.md#3ci环境准备)

## 编译三方库

- 下载本仓库

  ```shell
  git clone https://gitee.com/openharmony-sig/tpc_c_cplusplus.git --depth=1
  ```

- 三方库目录结构

  ```shell
  tpc_c_cplusplus/thirdparty/bctoolbox     #三方库的目录结构如下
  ├── docs                              #三方库相关文档的文件夹
  ├── HPKBUILD                          #构建脚本
  ├── HPKCHECK                          #测试脚本
  ├── SHA512SUM                         #三方库校验文件
  ├── README.OpenSource                 #说明三方库源码的下载地址，版本，license等信息
  ├── README_zh.md   
  ├── bctoolbox_oh_pkg.patch            #patch文档
  ```

  编译环境的搭建参考[准备三方库构建环境](../../../lycium/README.md#1编译环境准备)

- 进入lycium 目录编译

  ```shell
  cd lycium
  ./build.sh bctoolbox
  ```

- 三方库头文件及生成的库

  在lycium目录下会生成usr目录，该目录下存在已编译完成64位三方库

  ```shell
  bctoolbox/arm64-v8a bctoolbox/armeabi-v7a
  ```
- [测试三方库](#测试三方库)

## 
- 在IDE的libs目录下，要保证以下目录存在，否则就要创建，如下图所示：
  
  ![lib_dirs](pic/libs_dir.png)

  该目录将在编译之后，依赖的动态库将会自动拷贝到这里。

- 在IDE的cpp目录下新增thirdparty目录，将编译生成的库拷贝到该目录下，动态库文件会自动拷贝到libs目录下打包，如下图所示:
  
  ![thirdparty_install_dir](pic/bctoolbox_js.png)
 
- 在最外层（cpp目录下）CMakeLists.txt中添加如下语句

  ```shell
  #将三方库加入工程中
  target_link_libraries(entry PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/bctoolbox/${OHOS_ARCH}/lib/libbctoolbox.so)

  #将三方库的头文件加入工程中
  target_include_directories(entry PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/bctoolbox/${OHOS_ARCH}/include)

   #将动态库打包到libs
  file(GLOB allLibs "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/bctoolbox/${OHOS_ARCH}/lib/*.so*")
  add_custom_command(TARGET entry POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${allLibs}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/${OHOS_ARCH}/)
  ```
 修改IDE的entry/build-profile.json5 文件，增加编译架构过滤："abiFilters": ["arm64-v8a", "armeabi-v7a"], 如下图所示：

 ![bctoolbox_build](pic/bctoolbox_build.png)

 然后，执行编译运行.

## 测试三方库

三方库的测试使用原库自带的测试用例来做测试，[准备三方库测试环境](../../../lycium/README.md#3ci环境准备)

进入到构建目录,执行如下命令ctest（arm64-v8a-build为构建64位的目录, armeabi-v7a-build为构建32位的目录）

![bctoolbox_test](pic/bctoolbox-test.png)

## 参考资料

- [润和RK3568开发板标准系统快速上手](https://gitee.com/openharmony-sig/knowledge_demo_temp/tree/master/docs/rk3568_helloworld)
- [OpenHarmony三方库地址](https://gitee.com/openharmony-tpc)
- [OpenHarmony知识体系](https://gitee.com/openharmony-sig/knowledge)
- [通过DevEco Studio开发一个NAPI工程](https://gitee.com/openharmony-sig/knowledge_demo_temp/blob/master/docs/napi_study/docs/hello_napi.md)
