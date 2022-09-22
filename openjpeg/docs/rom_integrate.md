# openjpeg如何集成到系统Rom



## 准备源码工程

本库是基于OpenHarmony 3.2 beta1版本，在润和RK3568开发板上验证的。如果是从未使用过RK3568，可以先查看[润和RK3568开发板标准系统快速上手](https://gitee.com/openharmony-sig/knowledge_demo_temp/tree/master/docs/rk3568_helloworld)。



### 准备系统Rom源码

源码获取方法请参照：[OpenHarmony3.2beta1源码下载](https://gitee.com/openharmony/docs/blob/OpenHarmony-v3.2-Beta1/zh-cn/release-notes/OpenHarmony-v3.2-beta1.md#源码获取)



### 增加构建脚本及配置文件

- 下载本仓库代码

  ```
  # cd ~/
  # git clone git@gitee.com:openharmony-sig/tpc_c_cplusplus.git --depth=1
  ```

- 仓库代码目录结构说明

  ```
  # cd  ~/tpc_c_cplusplus/openjpeg/	# 进入到仓库代码库目录
  ```

  ```
  openjpeg
  	|-- BUILD.gn			# 编译构建脚本
  	|-- ohos.build			# 组件定义文件
  	|-- README_zh.md		# openjpeg 说明文档
  	|-- README.OpenSource	# openjpeg 开源信息说明文档
  	|-- docs				# 存放 openjpeg 相关文档
  	|-- media				# 存放图片的文件夹
  	|-- adapter
  			|-- include		# 包含库相关配置头文件的文件夹
  			|		|-- opj_apps_config.h
  			|		|-- opj_config.h
  			|		|-- opj_config_private.h
  			|
  			|-- script		# 存放库测试脚本的文件夹
  			        |-- test_open_jpeg.sh
  ```

- 将本仓库文件夹拷贝到third_party下

  ```
  # cp -arf ~/tpc_c_cplusplus/openjpeg ~/OpenHarmony/third_party
  ```



### 准备三方库源码

- 四方库依赖

  openjpeg三方库的测试依赖[libtiff](../../libtiff)，点击链接跳转至libtiff的Rom集成方法。

- 三方库源码下载

  将openjpeg-2.5.0源码下载到openjpeg目录并将其解压出来。

  ```
  # cd ~/OpenHarmony/third_party/openjpeg								         # 进入到openjpeg目录
  # wget https://github.com/uclouvain/openjpeg/archive/refs/tags/v2.5.0.zip    # 下载三方库
  # unzip v2.5.0.zip				                                             # 解压库
  # mv openjpeg-2.5.0 openjpeg                                                 # 修改三方库源码文件夹名称
  # rm v2.5.0.zip	                                                             # 删除压缩包
  ```



## 系统Rom中引入三方库

准备完三方库代码后，我们需要将三方库加入到编译构建体系中。标准系统编译构建可以参考文档[标准系统编译构建指导](https://gitee.com/openharmony/docs/blob/OpenHarmony-3.2-Beta1/zh-cn/device-dev/subsystems/subsys-build-standard-large.md)。

这里我们只需要将三方库作为组件添加到三方库子系统中即可（详细信息参照文档[如何为三方库组件中添加一个三方库](./add_thirdparty_component.md)）

添加步骤：

- 三方库子系统中添加组件

  打开//vendor/hihope/rk356/config.json文件，找到thirdparty子系统并添加一个openjpeg的组件

  ```
  {
    "subsystem": "thirdparty",
    "components": [
      {
        "component": "musl",
        "features": []
      },
      {
        "component": "openjpeg",
        "features": []
      }
    ]
  },
  ```

- 三方库目录下添加ohos.build文件

  ```
  {
    "subsystem": "thirdparty",
    "parts":{
      "openjpeg": {
        "module_list": [
          "//third_party/openjpeg:openjp2"
        ]
      }
    }
  }
  ```

- 三方库编译脚本中添加子系统组件名

  ```
  ohos_shared_library("openjp2"){
      ...
    part_name = "openjpeg"				
  }
  ```



## 系统Rom中引入三方库测试程序

openjpeg原生库提供了测试程序且已完成了gn的适配，我们只需要在三方库目录下的ohos.build中添加测试程序的依赖即可，如：

```
{
  "subsystem": "thirdparty",
  "parts":{
    "openjpeg": {
      "module_list": [
        "//third_party/openjpeg:openjpeg_test"
      ]
    }
  }
}
```



## 编译工程

- 选择产品

  ```
  # hb set	## 运行hb set后会出现产品列表，在此我们选择 rk3568
  ```

- 运行编译

  ```
  # hb build --target-cpu arm -f
  
  参数释义：
  --target-cpu arm 编译32位系统(未配置默认编译32位)，如果需要编译64位的需要改为--target-cpu arm64
  -f 全量编译，不加-f则为增量编译。每次设置完产品后建议进行全量编译。
  ```

- 验证结果

  正常编译完后会在out/rk3568/packages/phone/system/bin下生成的测试用可执行文件。以及out/rk3568/packages/phone/system/lib下生成libopenjp2.z.so动态库。

  ```
  # ls out/rk3568/packages/phone/system/lib | grep libopenjp2.z.so                                   ## 确认是否成功编译 openjp2 动态库
  # ls out/rk3568/packages/phone/system/bin | grep -E 'testemp|compare_|test_|j2k_ran|pdf2|ppm2'     ## 确认是否成功编译 openjpeg_test 包含的可执行文件
  ```



## 安装应用程序

1.安装应用的方式有两种：

- 方式一：烧录带有应用程序的镜像包

  烧录固件请参照[rk3568镜像烧录](https://gitee.com/openharmony-sig/knowledge_demo_temp/blob/master/docs/rk3568_helloworld/镜像烧录.md)。

- 方式二：将应用直接推送到开发板

  我们可以将需要推送到开发板的文件全部打包成一个压缩文件(openjpeg.tar)，然后通过[hdc_std工具](https://gitee.com/isrc_ohos/hdc-tool)推送到开发板，在推送前需要确保一点，板子的系统为可读写，如果出现推送失败且提示无权限，则需要重新加载系统权限

  ```
  # hdc_std.exe shell 						     ## 进入开发板系统
  # mount -o remount,rw /						     ## 修改系统权限
  # exit										     ## 退出开发板系统
  # hdc_std.exe file send .\openjpeg.tar /data/    ## 推送文件到开发板系统
  # hdc_std.exe shell
  # cd /data/
  # tar -xvf openjpeg.tar						     ## 解压文件包
  # cd openjpeg
  # cp libopenjp2.z.so	/system/lib/			 ## 将库文件拷贝到系统lib目录
  ```



## 运行效果

将[adapter/script/test_open_jpeg.sh](../adapter/script/test_open_jpeg.sh)脚本使用[hdc_std工具](https://gitee.com/isrc_ohos/hdc-tool)推送到开发板（应用安装若使用方式二，需确保脚本与应用位于同一目录下），执行如下命令运行脚本。

```
# chmod 777 test_open_jpeg.sh                    ## 修改脚本文件权限，防止因权限不够导致执行失败
# sh test_open_jpeg.sh                           ## 执行测试脚本
```

运行中的日志信息会实时打印在终端，若日志信息中所有测试项目为test_ok，则说明通过了全部的测试项目。

![image-20220922143536085](../media/running_result.png)

## 参考资料

- [润和RK3568开发板标准系统快速上手](https://gitee.com/openharmony-sig/knowledge_demo_temp/tree/master/docs/rk3568_helloworld)。
- [OpenHarmony3.2beta1](https://gitee.com/openharmony/docs/blob/OpenHarmony-v3.2-Beta1/zh-cn/release-notes/OpenHarmony-v3.2-beta1.md)。
- [OpenHarmony三方库组织地址](https://gitee.com/openharmony-tpc)。
- [知识体系](https://gitee.com/openharmony-sig/knowledge)。

