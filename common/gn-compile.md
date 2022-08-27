# 工程加入OpenHarmony构建体系

#### 概述

本文介绍如何将自己的工程加入OpenHarmony进行编译



#### 开发流程

以helloworld工程为例，有如下步骤

###### 新增工程目录

```
#在源码根目录下创建工程目录
mkdir ~/openharmony/helloworld

#在helloworld工程目录下新建源码文件
touch hello.c

#编写源文件内容如下
#include <stdio.h>

void HelloPrint()
{
    printf("\n************************************************\n");
    printf("\n\t\tHello World!\n");
    printf("\n************************************************\n");
}

int main(int argc, char **argv)
{
    HelloPrint();
    return 0;
}


```



###### 新增工程构建脚本

```
#在helloworld工程目录下新增BUILD.gn脚本，内容如下

import("//build/ohos.gni")
ohos_executable("hello") {
  sources = [
        "helloworld.c"
  ]
  include_dirs = []
  deps = []
  part_name = "hello"
  install_enable = true
}
```



###### 新增组件定义脚本

```
#在helloworld工程目录下新增ohos.build脚本，内容如下
{
  "subsystem": "helloworld",
  "parts": {
    "hello": {
      "module_list": [
        "//helloworld:hello"
      ]
    }
  }
}
```



###### 新增产品定义

```
#在vendor目录下新增产品的定义，这里以rk3568为例
#在openharmony源码根目录下的vendor/hihope/rk3568/config.json,新增如下内容
    {
      "subsystem": "helloworld",
      "components": [
        {
          "component": "hello",
          "features": []
        }
      ]
    }

```



###### 新增子系统定义

```
#在openharmony源码根目录下的build/subsystem_config.json中新增子系统定义

 "helloworld": {
    "path": "helloworld",
    "name": "helloworld"
  }
```



###### 编译Openharmony镜像

```
#在OpenHarmony源码根目录
#编译32位镜像
hb build --target-cpu arm

#编译64位镜像
hb build --target-cpu arm64
```

