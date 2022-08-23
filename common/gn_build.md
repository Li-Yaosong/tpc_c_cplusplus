# 三方库引入GN工程

这里以三方库thirdpartyA、动态库名libA的三方库引入到helloworld工程为例：

三方库thirdpartyA目录结构如下：

```
third_party/thirdpartyA
├── src
├── include
├── BUILD.gn
.....
```

helloworld工程目录结构如下：

```
third_party/helloworld
├── src
├── include
├── BUILD.gn
.....
```

在helloworld工程构建脚本中引入三方库thirdpartyA：

```
import("//build/ohos.gni")  # 导入编译模板
ohos_executable("helloworld") { # 可执行模块
  sources = [       # 模块源码
    "src/helloworld.c"
  ]
  include_dirs = [  # 模块依赖头文件目录
    "include" ,
    “//third_party/thirdpartyA/include”,  #依赖三方库头文件的目录
  ]
  cflags = []
  cflags_c = []
  cflags_cc = []
  ldflags = []
  configs = []
  deps =[]    # 部件内部依赖
  deps += [ “//third_party/thirdpartyA:libA” ] #依赖三方库库文件
  part_name = "hello"    # 所属部件名称，必选
  install_enable = true  # 是否默认安装（缺省默认不安装），可选
}
```

