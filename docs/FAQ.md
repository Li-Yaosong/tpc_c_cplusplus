# C/C++三方库常见 FAQ

- #### Q1: 交叉编译完后在测试设备上运行测试用例失败，如测试bzip2时提示`/bin/sh: ./bzip2: No such file or directory`？

1. 确保文件已经推送到开发板
2. 确保文件的CPU架构和测试设备系统架构是一致的，可通过`file`查看文件属性，通过`getconf LONG_BIT`查看系统位数，如本错误示例中:

   ```shell
   # file bzip2                                                                           # 查看文件架构时arm64位的
   bzip2: ELF shared object, 64-bit LSB arm64, dynamic (/lib/ld-musl-aarch64.so.1)
   #
   # getconf LONG_BIT                                                                     # 查看系统位数
   32
   #
   ```

   文件属性与系统位数不匹配，故提示无法找到该文件。

- #### Q2: 当前`lycium`交叉编译适配的CPU架构只支持arm32位和arm64位的，如若需新增其他CPU架构该如何操作?
  
  本仓库中适配的三方库当前都是通过[lycium工具](../lycium/)适配且验证过的arm32位以及arm64位架构的三方库，如若需要添加其他CPU架构的编译适配，请参考文档[lycium上面适配OpenHarmony 不同架构的构建](./adpater_architecture.md)。
  
- #### Q3: 交叉编译时出现ld.lld error：undefined symbol xxxx 以及 error: use of undeclared identifier 'XXXX'的情况

  情况一：系统本身的接口(SDK接口不支持的情况，通常在编译过程就会失败，如果成功编译出文件则注重检查交叉编译是否链接了SDK的静态库，存在此情况则可以尝试从将此静态库路径导入工程中与三方库一同编译) ，系统接口具体资料请参考下方系统符号参考中的 "附录"部分。

  [系统符号参考](https://gitee.com/openharmony/docs/blob/master/zh-cn/application-dev/reference/native-lib/Readme-CN.md)

  情况二：三方库的依赖库接口，可能是由于依赖库文件没有和三方库文件一同导入，需要将依赖库文件一同添加到工程下。如果确认三方库及依赖库已经存在工程中，则请核对cmake等编译工具路径、库名等配置选项。

  情况三：三方库本身的接口，可能是因为三方库的相关编译宏没有打开，需要打开相关编译宏，如果该功能需要其它依赖库则需要补全依赖库。

- #### Q4：lycium编译失败的日志文件查看路径

  ##### 情况一：查找需要的日志文件，目前三方库目录下存在两种日志路径，参考以下路径和示例图
  
  ​	三方库对应ARCH的编译文件夹(通常以ARCH-build命名，在源码目录下或者三方库目录下)下会生成build.log日志文件。
  
  <img src="./media/buildlog1.png" alt="buildlog1" width = 8000 height = 100/>
  
  ​	三方库目录下会生成一个库名+版本+ARCH+build.log命令的日志文件，文件中会记录具体的编译日志。
  
  <img src="./media/buildlog2.png" alt="buildlog2" width = 8000 height = 100/>
  
  
  
  ##### 情况二：想要快速查找所有的日志文件，可以在三方库目录下使用以下命令,执行效果如图所示
  
  ```
  find . -name "*build.log"
  ```
  
  <img src="./media/findbuildlog.png" alt="findbuildlog" width = 8000 height = 100/>



- ##### Q5 FFmpeg在MAC环境编译可能遇到的问题

  以下措施sed语句请注意修改文件所在的实际路径，如果修改错误可以尝试从同名+.bak的文件中恢复内容，并且sed修改的主要目的是为了通过编译，对于实际使用可能存在影响，需要仔细分辨。

  ##### static declaration of 'xxx' follows non-static declaration

  ```
  sed -i.bak 's/#define HAVE_LLRINT 0/#define HAVE_LLRINT 1/g' config.h
  sed -i.bak 's/#define HAVE_LLRINTF 0/#define HAVE_LLRINTF 1/g' config.h
  sed -i.bak 's/#define HAVE_LRINT 0/#define HAVE_LRINT 1/g' config.h
  sed -i.bak 's/#define HAVE_LRINTF 0/#define HAVE_LRINTF 1/g' config.h
  sed -i.bak 's/#define HAVE_ROUND 0/#define HAVE_ROUND 1/g' config.h
  sed -i.bak 's/#define HAVE_ROUNDF 0/#define HAVE_ROUNDF 1/g' config.h
  sed -i.bak 's/#define HAVE_CBRT 0/#define HAVE_CBRT 1/g' config.h
  sed -i.bak 's/#define HAVE_CBRTF 0/#define HAVE_CBRTF 1/g' config.h
  sed -i.bak 's/#define HAVE_COPYSIGN 0/#define HAVE_COPYSIGN 1/g' config.h
  sed -i.bak 's/#define HAVE_TRUNC 0/#define HAVE_TRUNC 1/g' config.h
  sed -i.bak 's/#define HAVE_TRUNCF 0/#define HAVE_TRUNCF 1/g' config.h
  sed -i.bak 's/#define HAVE_RINT 0/#define HAVE_RINT 1/g' config.h
  sed -i.bak 's/#define HAVE_HYPOT 0/#define HAVE_HYPOT 1/g' config.h
  sed -i.bak 's/#define HAVE_ERF 0/#define HAVE_ERF 1/g' config.h
  sed -i.bak 's/#define HAVE_GMTIME_R 0/#define HAVE_GMTIME_R 1/g' config.h
  sed -i.bak 's/#define HAVE_LOCALTIME_R 0/#define HAVE_LOCALTIME_R 1/g' config.h
  sed -i.bak 's/#define HAVE_INET_ATON 0/#define HAVE_INET_ATON 1/g' config.h
  ```

  可以在编译编译脚本中添加对应选项的sed命令，或者执行完./configure命令后，执行make之前在终端手动执行对应选项的sed命令。脚本添加位置以及手动执行顺序参考如下示例

  ```
  #configure检查和生成配置之后使用sed替换配置内容再编译
  ./configure
  sed -i.bak 's/#define HAVE_LLRINT 0/#define HAVE_LLRINT 1/g' config.h
  make
  ```

  ##### xxxxxxxxxx error: expected ')'

  ./config.h:17:19: error: expected ')' before numeric constant
  \#define getenv(x) NULL

  ```
  sed -i.bak 's|#define getenv(x) NULL||g' config.h
  ```

  ##### ld：-shared -Wl,- soname,xxxx.so unknown option
  使用clang编译时需要将config.mak文件中的 **SHFLAGS=-shared -Wl,-soname** 修改为 **SHFLAGS=- shared -soname**，config.mak文件是configure后生成的文件，可以参考此语句修改
  
  ```
  sed -i.bak 's|SHFLAGS=-shared -Wl,-soname|SHFLAGS=- shared -soname|g' 实际路径/config.mak
  ```
  
  

