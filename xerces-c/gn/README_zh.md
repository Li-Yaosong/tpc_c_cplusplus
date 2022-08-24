#  三方库xerces-c适配脚本的使用

#### 三方库源码下载

- 下载三方库xerces-c

  ```
  #在OpenHarmony源码根目录下的third_party目录下获取源库
  git clone https://github.com/apache/xerces-c.git -b v3.2.3
  ```



#### 修改编译脚本

- 将[xerces-c](xerces-c)/config.h文件拷贝到源库xerces-c根目录下；注意，config.h文件中，如果系统是32位，SIZEOF_LONG的值修改为4，如果系统是64为，值修改为8
- 将[xerces-c](xerces-c)/src/BUILD.gn文件拷贝到源库xerces-c/src目录下
- 将[xerces-c](xerces-c)/tests/BUILD.gn文件拷贝到源库xerces-c/tests目录下
- 将[xerces-c](xerces-c)/samples/BUILD.gn文件拷贝到源库xerces-c/samples目录下



#### 相关资料

- [工程加入OpenHarmony构建体系](../../common/gn_compile_build.md)
- [三方库引入GN工程](../../common/gn_build.md)



