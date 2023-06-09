# libdash集成到应用hap
本库是在RK3568开发板上基于OpenHarmony3.2 Release版本的镜像验证的，如果是从未使用过RK3568，可以先查看[润和RK3568开发板标准系统快速上手](https://gitee.com/openharmony-sig/knowledge_demo_temp/tree/master/docs/rk3568_helloworld)。
## 开发环境
- ubuntu20.04
- [OpenHarmony3.2Release镜像](https://gitee.com/link?target=https%3A%2F%2Frepo.huaweicloud.com%2Fopenharmony%2Fos%2F3.2-Release%2Fdayu200_standard_arm32.tar.gz)
- [ohos_sdk_public 3.2.11.9 (API Version 9 Release)](https://gitee.com/link?target=https%3A%2F%2Frepo.huaweicloud.com%2Fopenharmony%2Fos%2F3.2-Release%2Fohos-sdk-windows_linux-public.tar.gz)
- [DevEco Studio 3.1 Beta2](https://gitee.com/link?target=https%3A%2F%2Fcontentcenter-vali-drcn.dbankcdn.cn%2Fpvt_2%2FDeveloperAlliance_package_901_9%2Ff3%2Fv3%2FuJyuq3syQ2ak4hE1QZmAug%2Fdevecostudio-windows-3.1.0.400.zip%3FHW-CC-KV%3DV1%26HW-CC-Date%3D20230408T013335Z%26HW-CC-Expire%3D315360000%26HW-CC-Sign%3D96262721EDC9B34E6F62E66884AB7AE2A94C2A7B8C28D6F7FC891F46EB211A70)
- [准备三方库构建环境](../../../tools/README.md#编译环境准备)
- [准备三方库测试环境](../../../tools/README.md#ci环境准备)
## 编译三方库
- 下载本仓库
  ```
  git clone https://gitee.com/openharmony-sig/tpc_c_cplusplus.git --depth=1
  ```
- 三方库目录结构
  ```
  tpc_c_cplusplus/thirdparty/libdash  #三方库libdash的目录结构如下
  ├── docs                              #三方库相关文档的文件夹
  ├── HPKBUILD                          #构建脚本
  ├── SHA512SUM                         #三方库校验文件
  ├── libdash_oh_pkg.patch              #补丁
  ├── README.OpenSource                 #说明三方库源码的下载地址，版本，license等信息
  ├── README_zh.md   
  ```
  
- 将libdash拷贝至tools/main目录下
  ```
  cd tpc_c_cplusplus
  cp thirdparty/libdash tools/main -rf
  cp thirdparty/libxml2 tools/main -rf
  cp thirdparty/curl tools/main -rf
  cp thirdparty/openssl tools/main -rf
  cp thirdparty/zstd tools/main -rf
  cp thirdparty/nghttp2 tools/main -rf
  cp thirdparty/CUnit tools/main -rf
  cp thirdparty/xz tools/main -rf
  cp thirdparty/openssl_quic tools/main -rf
  cp thirdparty/nghttp3 tools/main -rf
  ```
- 在tools目录下编译三方库
  编译环境的搭建参考[准备三方库构建环境](../../../tools/README.md#编译环境准备)
  ```
  cd tools
  ./build.sh CUnit openssl_quic nghttp3 openssl zstd nghttp2 xz libxml2 curl libdash
  ```
- 三方库头文件及生成的库
  在tools目录下会生成usr目录，该目录下存在已编译完成的32位和64位三方库
  ```
  libdash/arm64-v8a   libdash/armeabi-v7a
  libxml2/arm64-v8a   libxml2/armeabi-v7a
  curl/arm64-v8a      curl/armeabi-v7a
  openssl/arm64-v8a   openssl/armeabi-v7a
  zstd/arm64-v8a      zstd/armeabi-v7a
  nghttp2/arm64-v8a   nghttp2/armeabi-v7a
  CUnit/arm64-v8a     CUnit/armeabi-v7a
  nghttp3/arm64-v8a   nghttp3/armeabi-v7a
  xz/arm64-v8a        xz/armeabi-v7a
  openssl_quic/arm64-v8a  openssl_quic/armeabi-v7a
  ```

- [测试三方库](#测试三方库)

## 应用中使用三方库

- 拷贝动态库到`\\entry\libs\${OHOS_ARCH}\`目录：
  动态库需要在`\\entry\libs\${OHOS_ARCH}\`目录，才能集成到hap包中，所以需要将对应的so文件拷贝到对应CPU架构的目录

  &nbsp;![thirdparty_install_dir](pic/libdash_install_dir_1.jpg)
- 在IDE的cpp目录下新增thirdparty目录，将编译生成的库拷贝到该目录下，如下图所示

  &nbsp;![thirdparty_install_dir](pic/libdash_install_dir_2.jpg)
- 在最外层（cpp目录下）CMakeLists.txt中添加如下语句
  ```
  #将三方库加入工程中
  target_link_libraries(entry PRIVATE ${CMAKE_SOURCE_DIR}/../../../libs/${OHOS_ARCH}/libdash.so)
  target_link_libraries(entry PRIVATE ${CMAKE_SOURCE_DIR}/../../../libs/${OHOS_ARCH}/libcurl.so)
  target_link_libraries(entry PRIVATE ${CMAKE_SOURCE_DIR}/../../../libs/${OHOS_ARCH}/libcrypto.so.1.1)
  target_link_libraries(entry PRIVATE ${CMAKE_SOURCE_DIR}/../../../libs/${OHOS_ARCH}/libnghttp2.so.14)
  target_link_libraries(entry PRIVATE ${CMAKE_SOURCE_DIR}/../../../libs/${OHOS_ARCH}/libssl.so.1.1)
  target_link_libraries(entry PRIVATE ${CMAKE_SOURCE_DIR}/../../../libs/${OHOS_ARCH}/libxml2.so.2)
  target_link_libraries(entry PRIVATE ${CMAKE_SOURCE_DIR}/../../../libs/${OHOS_ARCH}/libzstd.so.1)
  #将三方库的头文件加入工程中
  target_include_directories(entry PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/libdash/${OHOS_ARCH}/include)
  target_include_directories(entry PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/curl/${OHOS_ARCH}/include)
  target_include_directories(entry PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/libxml2/${OHOS_ARCH}/include)
  target_include_directories(entry PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/nghttp2/${OHOS_ARCH}/include)
  target_include_directories(entry PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/openssl/${OHOS_ARCH}/include)
  target_include_directories(entry PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/zstd/${OHOS_ARCH}/include)
  ```
  ![libdash_usage](pic/libdash_usage.jpg)
## 测试三方库
三方库的测试使用原库自带的测试用例来做测试，[准备三方库测试环境](../../../tools/README.md#ci环境准备)

- 将编译生成的libdash_networkpart_test可执行文件准备好(注意：需要联网)

- 将准备好的文件推送到开发板，在windows命令行进行如下操作

  ```
  hdc_std shell mount -o remount,rw /         #修改系统权限为可读写
  hdc_std file send libdash_networkpart_test /data    #将可执行文件推入开发板data目录
  hdc_std file send libc++_shared.so /system/lib64    #将动态库推送开发板
  hdc_std file send libdash.so /system/lib64
  hdc_std file send libcurl.so /system/lib64
  hdc_std file send libcrypto.so.1.1 /system/lib64
  hdc_std file send libnghttp2.so.14 /system/lib64
  hdc_std file send libssl.so.1.1 /system/lib64
  hdc_std file send libxml2.so.2 /system/lib64
  hdc_std file send libzstd.so.1 /system/lib64

  hdc_std shell                          #进入开发板
  chmod 777 libdash_networkpart_test     #添加权限
  ./libdash_networkpart_test             #执行测试用例
  ```

&nbsp;![libdash_test](pic/libdash_test.jpg)

## 参考资料
- [润和RK3568开发板标准系统快速上手](https://gitee.com/openharmony-sig/knowledge_demo_temp/tree/master/docs/rk3568_helloworld)
- [OpenHarmony三方库地址](https://gitee.com/openharmony-tpc)
- [OpenHarmony知识体系](https://gitee.com/openharmony-sig/knowledge)
- [通过DevEco Studio开发一个NAPI工程](https://gitee.com/openharmony-sig/knowledge_demo_temp/blob/master/docs/napi_study/docs/hello_napi.md)
