# lzo集成到应用hap
本库是在RK3568开发板上基于OpenHarmony3.2 Release版本的镜像验证的，如果是从未使用过RK3568，可以先查看[润和RK3568开发板标准系统快速上手](https://gitee.com/openharmony-sig/knowledge_demo_temp/tree/master/docs/rk3568_helloworld)。
## 开发环境
- ubuntu20.04
- [OpenHarmony3.2Release镜像](https://gitee.com/link?target=https%3A%2F%2Frepo.huaweicloud.com%2Fopenharmony%2Fos%2F3.2-Release%2Fdayu200_standard_arm32.tar.gz)
- [ohos_sdk_public 4.0.8.1 (API Version 10 Release)](http://download.ci.openharmony.cn/version/Master_Version/OpenHarmony_4.0.8.1/20230608_091016/version-Master_Version-OpenHarmony_4.0.8.1-20230608_091016-ohos-sdk-full.tar.gz)
- [DevEco Studio 3.1 Release](https://contentcenter-vali-drcn.dbankcdn.cn/pvt_2/DeveloperAlliance_package_901_9/81/v3/tgRUB84wR72nTfE8Ir_xMw/devecostudio-windows-3.1.0.501.zip?HW-CC-KV=V1&HW-CC-Date=20230621T074329Z&HW-CC-Expire=315360000&HW-CC-Sign=22F6787DF6093ECB4D4E08F9379B114280E1F65DA710599E48EA38CB24F3DBF2)
- [准备三方库构建环境](../../../lycium/README.md#1编译环境准备)
- [准备三方库测试环境](../../../lycium/README.md#3ci环境准备)
## 编译三方库
- 下载本仓库
   ```shell
   git clone https://gitee.com/openharmony-sig/tpc_c_cplusplus.git --depth=1
   ```
- 三方库目录结构
  ```shell
  tpc_c_cplusplus/thirdparty/lzo        #三方库lzo的目录结构如下
  ├── docs                              #三方库相关文档的文件夹
  ├── HPKBUILD                          #构建脚本
  ├── HPKCHECK                          #测试脚本
  ├── OAT.xml                           #扫描结果文件
  ├── SHA512SUM                         #三方库校验文件
  ├── README.OpenSource                 #说明三方库源码的下载地址，版本，license等信息
  ├── README_zh.md                      #三方库简介
  ```
- 在lycium目录下编译三方库 
  编译环境的搭建参考[准备三方库构建环境](../../../lycium/README.md#1编译环境准备)
  lzo库不需要依赖其它库，所以在build时只需要编译lzo库即可
  ```shell
  cd lycium
  ./build.sh lzo
  ```
- 三方库头文件及生成的库
  在lycium目录下会生成usr目录，该目录下存在已编译完成的32位和64位三方库
  ```shell
  lzo/lzo-arm64-v8a   lzo/lzo-armeabi-v7a
  ```
- [测试三方库](#测试三方库)
## 应用中使用三方库
- 在IDE的cpp目录下新增thirdparty目录，将编译生成的头文件拷贝到该目录下
- 在新增thirdparty/lzo/(arm64-v8a及armeabi-v7a)目录下新增lib目录，将编译生成的.a文件拷贝到该目录下

&nbsp;![lzo_install](pic/lzo_install.PNG)

- 在最外层（cpp目录下）CMakeLists.txt中添加如下语句:
  ```cmake
  #将三方静态库加入工程中
  target_link_libraries(entry PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/lzo/${OHOS_ARCH}/lib/liblzo2.a)
  #将三方库的头文件加入工程中
  target_include_directories(entry PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/lzo/${OHOS_ARCH}/include)
  ```
## 测试三方库
- [准备三方库测试环境](../../../lycium/README.md#3ci环境准备)
- 将编译生成的文件推送到开发板，进入到构建目录执行原库自带的测试用例(注意arm64-v8a为构建64位的目录，armeabi-v7a为构建32位的目录)
  ```
  cd data/thirdparty/lzo/lzo-armeabi-v7a-buil/examples
  ./simple
  ```
- 执行echo $? 返回结果为0执行成功，测试通过结果如图所示：

&nbsp;![lzo_tests](pic/lzo_tests.PNG)

## 参考资料
- [润和RK3568开发板标准系统快速上手](https://gitee.com/openharmony-sig/knowledge_demo_temp/tree/master/docs/rk3568_helloworld)
- [OpenHarmony三方库地址](https://gitee.com/openharmony-tpc)
- [OpenHarmony知识体系](https://gitee.com/openharmony-sig/knowledge)
- [通过DevEco Studio开发一个NAPI工程](https://gitee.com/openharmony-sig/knowledge_demo_temp/blob/master/docs/napi_study/docs/hello_napi.md)
