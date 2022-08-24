# 三方库xerces-c适配脚本的使用



#### 三方库源码下载

- 下载三方库xerces-c

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;在github上下载xerces-c源码，版本取Tags/v3.2.3 ,[源库地址](https://github.com/apache/xerces-c)

- 下载三方库icu

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;下载xerces-c的依赖库icu,[下载地址](https://gitee.com/openharmony/third_party_icu)



#### 修改编译脚本

- 将三方库third_party_icu拷贝至三方库xerces-c/src下；

- 将[xerces-c](xerces-c)/config.h拷贝到源库xerces-c目录下；

- 将[xerces-c](xerces-c)/src/CMakeLists.txt文件替换源库xerces-c/src/CMakeLists.txt文件；

- 将[xerces-c](xerces-c)/src/third_party_icu/CMakeLists.txt文件替换源库xerces-c/src/third_party_icu/CMakeLists.txt文件；



#### 相关资料

[三方库引入IDE（DevEco Studio）工程](../../common/cmake_build.md)