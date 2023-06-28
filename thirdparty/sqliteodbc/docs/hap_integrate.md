# sqliteodbc集成到应用hap
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
  tpc_c_cplusplus/thirdparty/sqliteodbc #三方库sqliteodbc的目录结构如下
  ├── docs                              #三方库相关文档的文件夹
  ├── HPKBUILD                          #构建脚本
  ├── SHA512SUM                         #三方库校验文件
  ├── README.OpenSource                 #说明三方库源码的下载地址，版本，license等信息
  ├── README_zh.md   
  ```
  
- 将sqliteodbc拷贝至tools/main目录下
  ```
  cd tpc_c_cplusplus
  cp thirdparty/sqliteodbc tools/main -rf
  ```
- 在tools目录下编译三方库
  sqliteodbc库依赖sqlite、unixODBC、libxml2、tcl、xz、zlib这6个库，所以在build时需要将依赖库一起编译进来。
  编译环境的搭建参考[准备三方库构建环境](../../../tools/README.md#编译环境准备)
  ```
  cd tools
  ./build.sh sqliteodbc sqlite unixODBC libxml2 tcl xz zlib
  ```
- 三方库头文件及生成的库
  在tools目录下会生成usr目录，该目录下存在已编译完成的32位和64位三方库
  ```
  sqliteodbc/arm64-v8a  sqliteodbc/armeabi-v7a
  sqlite/arm64-v8a  sqlite/armeabi-v7a
  unixODBC/arm64-v8a  unixODBC/armeabi-v7a
  libxml2/arm64-v8a  libxml2/armeabi-v7a
  tcl/arm64-v8a  tcl/armeabi-v7a
  xz/arm64-v8a  xz/armeabi-v7a
  zlib/arm64-v8a  zlib/armeabi-v7a
  ```

- [测试三方库](#测试三方库)

## 应用中使用三方库

- 在IDE的cpp目录下新增thirdparty目录，将编译生成的库拷贝到该目录下，如下图所示
&nbsp;![thirdparty_install_dir](pic/sqliteodbc_install_dir.png)
- 在最外层（cpp目录下）CMakeLists.txt中添加如下语句
  ```
  #将三方库加入工程中
  target_link_libraries(entry PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sqliteodbc/${OHOS_ARCH}/lib/libsqlite3_mod_blobtoxy-0.9998.so
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sqliteodbc/${OHOS_ARCH}/lib/libsqlite3_mod_csvtable-0.9998.so
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sqliteodbc/${OHOS_ARCH}/lib/libsqlite3_mod_impexp-0.9998.so
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sqliteodbc/${OHOS_ARCH}/lib/libsqlite3_mod_xpath-0.9998.so
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sqliteodbc/${OHOS_ARCH}/lib/libsqlite3_mod_zipfile-0.9998.so
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sqliteodbc/${OHOS_ARCH}/lib/libsqlite3odbc-0.9998.so
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/unixODBC/${OHOS_ARCH}/lib/libodbc.a
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/unixODBC/${OHOS_ARCH}/lib/libodbccr.a
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/unixODBC/${OHOS_ARCH}/lib/libodbcinst.a
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sqlite/${OHOS_ARCH}/lib/libsqlite3.so.0.8.6
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/libxml2/${OHOS_ARCH}/lib/libxml2.so.2.11.3
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/tcl/${OHOS_ARCH}/lib/libtclsqlite3.so
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/tcl/${OHOS_ARCH}/lib/tdbc1.1.5/libtdbc1.1.5.so
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/tcl/${OHOS_ARCH}/lib/thread2.8.8/libthread2.8.8.so
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/tcl/${OHOS_ARCH}/lib/sqlite3.40.0/libsqlite3.40.0.so
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/tcl/${OHOS_ARCH}/lib/itcl4.2.3/libitcl4.2.3.so
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/tcl/${OHOS_ARCH}/lib/tdbcodbc1.1.5/libtdbcodbc1.1.5.so
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/tcl/${OHOS_ARCH}/lib/tdbcmysql1.1.5/libtdbcmysql1.1.5.so
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/tcl/${OHOS_ARCH}/lib/libtcl8.6.so
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/tcl/${OHOS_ARCH}/lib/tdbcpostgres1.1.5/libtdbcpostgres1.1.5.so
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/tcl/${OHOS_ARCH}/lib/tdbc1.1.5/libtdbcstub1.1.5.a
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/tcl/${OHOS_ARCH}/lib/libtclstub8.6.a
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/tcl/${OHOS_ARCH}/lib/itcl4.2.3/libitclstub4.2.3.a
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/xz/${OHOS_ARCH}/lib/liblzma.so.5.4.1
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/zlib/${OHOS_ARCH}/lib/libz.so.1.2.13
    )
  #将三方库的头文件加入工程中
  target_include_directories(entry PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/unixODBC/${OHOS_ARCH}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sqlite/${OHOS_ARCH}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/libxml2/${OHOS_ARCH}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/tcl/${OHOS_ARCH}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/xz/${OHOS_ARCH}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/zlib/${OHOS_ARCH}/include
  )
  ```
  ![sqliteodbc_usage](pic/sqliteodbc_usage.png)
## 测试三方库
sqliteodbc三方库的测试使用unixODBC库中的isql命令来做测试，步骤如下：

- 填写unixODBC安装目录下面的etc目录下的配置文件odbcinst.ini，其中Driver和Setup需要根据libsqlite3odbc.so实际路径填写（armeabi-v7a为安装32位的目录，arm64-v8a为安装64位的目录），以下经供参考
```shell
[SQL3]
Description     = unixODBC for sqlite3
Driver          = /data/local/tmp/lycium/usr/sqliteodbc/armeabi-v7a/lib/libsqlite3odbc.so
Setup           =/data/local/tmp/lycium/usr/sqliteodbc/armeabi-v7a/lib/libsqlite3odbc.so
FileUsage       = 1
```
- 填写unixODBC安装目录下面的etc目录下的配置文件odbc.ini
```shell
[myDB]
Driver  = SQL3
```
- 进入到unixODBC安装目录下面的bin目录执行命令./isql -v myDB，运行测试（usr/unixODBC/arm64-v8a为安装64位的目录，usr/unixODBC/armeabi-v7a为安装32位的目录）

&nbsp;![sqliteodbc_test_result](pic/sqliteodbc_test_result.png)

测试结果显示Connected!即表示已经连接上sqlite数据库，测试成功！

## 参考资料
- [润和RK3568开发板标准系统快速上手](https://gitee.com/openharmony-sig/knowledge_demo_temp/tree/master/docs/rk3568_helloworld)
- [OpenHarmony三方库地址](https://gitee.com/openharmony-tpc)
- [OpenHarmony知识体系](https://gitee.com/openharmony-sig/knowledge)
- [通过DevEco Studio开发一个NAPI工程](https://gitee.com/openharmony-sig/knowledge_demo_temp/blob/master/docs/napi_study/docs/hello_napi.md)
