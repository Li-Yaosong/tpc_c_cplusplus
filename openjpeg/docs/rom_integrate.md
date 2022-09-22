# openjpeg如何集成到系统Rom

## 准备源码工程

本库是基于OpenHarmony 3.2 beta1版本，在润和RK3568开发板上验证的。如果是从未使用过RK3568，可以先查看[润和RK3568开发板标准系统快速上手](https://gitee.com/openharmony-sig/knowledge_demo_temp/tree/master/docs/rk3568_helloworld)。



### 准备系统Rom源码

源码获取方法请参照：[OpenHarmony3.2beta1源码下载](https://gitee.com/openharmony/docs/blob/OpenHarmony-v3.2-Beta1/zh-cn/release-notes/OpenHarmony-v3.2-beta1.md#源码获取)



### 增加构建脚本及配置文件

- 下载本仓库代码

  ```
  cd ~/
  git clone git@gitee.com:openharmony-sig/tpc_c_cplusplus.git --depth=1
  ```

- 仓库代码目录结构说明

  ```
  cd  ~/tpc_c_cplusplus/openjpeg/	# 进入到仓库代码库目录
  ```

  ```
  openjpeg
  	|-- BUILD.gn			# 编译构建脚本
  	|-- ohos.build			# 组件定义文件
  	|-- README_zh.md		# openjpeg 主页
  	|-- README.OpenSource	# openjpeg 源码的下载地址，版本，license等信息
  	|-- docs				# 存放 openjpeg 相关文档的文件夹
  	|-- adapter             # 存放三方库适配需要的代码文件
  ```
  
- 将本仓库文件夹拷贝到third_party下

  ```
  cp -arf ~/tpc_c_cplusplus/openjpeg ~/OpenHarmony/third_party
  ```



### 准备三方库源码

- 四方库依赖

  openjpeg三方库的测试依赖[libtiff](../../libtiff)，点击链接跳转至libtiff的Rom集成方法。

- 三方库源码下载

  将openjpeg-2.5.0源码下载到openjpeg目录并将其解压出来。

  ```
  # 进入到openjpeg目录
  cd ~/OpenHarmony/third_party/openjpeg
  
  # 下载三方库
  wget https://github.com/uclouvain/openjpeg/archive/refs/tags/v2.5.0.zip    
  
  # 解压库
  unzip v2.5.0.zip
  
  # 修改三方库源码文件夹名称
  mv openjpeg-2.5.0 openjpeg
  	                                                             
  # 删除压缩包
  rm v2.5.0.zip
  ```



## 系统Rom中引入三方库

准备完三方库代码后，我们需要将三方库加入到编译构建体系中。标准系统编译构建可以参考文档[标准系统编译构建指导](https://gitee.com/openharmony/docs/blob/OpenHarmony-3.2-Beta1/zh-cn/device-dev/subsystems/subsys-build-standard-large.md)。

我们默认三方库是属于OpenHarmony的thirdparty子系统，如果需要自己定义子系统参考文档[如何为三方库组件中添加一个三方库](./add_thirdparty_component.md)。

三方库子系统中添加组件

```
# 打开//vendor/hihope/rk356/config.json文件，找到thirdparty子系统并添加一个openjpeg的组件
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

- 进入源码根目录

  ```
  cd ~/openharmony
  ```

- 选择产品

  ```
  # 运行hb set后会出现产品列表，在此我们选择 rk3568
  hb set
  ```

- 运行编译

  ```
  # --target-cpu arm 编译32位系统(未配置默认编译32位)，如果需要编译64位的需要改为--target-cpu arm64
  # -f 全量编译，不加-f则为增量编译。每次设置完产品后建议进行全量编译。
  hb build --target-cpu arm -f
  ```
  
- 查证编译结果

  正常编译完后会在out/rk3568/thirdparty/openjpeg/目录下生成 libopenjp2.z.so 动态库 和 test_tile_encoder 等 openjpeg_test 所包含的测试用可执行程序。同时这些程序也会打包到镜像中。

  ```
  # 确认是否成功编译 openjp2 动态库 和 openjpeg_test 包含的可执行文件
  ls out/rk3568/thirdparty/openjpeg/
  ```



## 运行效果

- PC端hdc_std工具编译和使用

  将编译生成的库和测试文件推送到板子上运行，为避免每次将文件推入设备都烧录整个镜像，我们使用hdc_std工具将文件推送到开发板上。

  1. hdc_std工具编译

     成功执行如下指令后，hdc_std工具的路径为：out/sdk/ohos-sdk/windows/toolchains/hdc_std.exe。

     ```
     # 源码根目录下使用hb set 选择产品ohos-sdk
     hb set
     
     # 执行编译
     hb build
     ```

  2. hdc_std工具使用

     将工具拷贝到Windows，可以为工具目录配置环境变量，也可以在工具所在目录打开windows命令行。

     ```
     # 在工具所在目录打开windows命令行，输入.\hdc_std.exe -h查看工具使用说明
     D:\RKDevTool_Release_v2.94>.\hdc_std.exe -h
     
                              OpenHarmony device connector(HDC) ...
     
     ---------------------------------global commands:----------------------------------
      -h/help                               - Print hdc help
      -v/version                            - Print hdc version
      -l 0-5                                - Set runtime loglevel
      -t connectkey                         - Use device with given connect key
     ......
     ```

- 三方库运行环境打包和推送

  1. 将原生库测试需要的所有文件打包成openjpeg.tar.gz,并拷贝到windows系统下

     ```
     # 进入用户目录
     cd ~
     
     # 创建打包文件夹
     mkdir openjpeg
     
     # 复制测试脚本
     cp ~/openharmony/third_party/openjpeg/adapter/script/test_open_jpeg.sh openjpeg/
     
     # 复制可执行程序和动态库
     cp ~/openharmony/out/rk3568/thirdparty/openjpeg/* openjpeg/
     
     # 打包运行环境
     tar -zcvf openjpeg.tar.gz openjpeg/
     ```

  2. 将文件推送到开发板，在windows命令行进行如下操作

     ```
     # 修改开发板系统文件读写权限
     hdc_std.exe shell mount -o remount,rw /
     
     # 推送文件到开发板系统
     hdc_std.exe file send .\openjpeg.tar.gz /data/
     
     # 进入开发板系统
     hdc_std.exe shell
     
     # 解压文件包
     tar -zxvf openjpeg.tar.gz
     
     # 修改文件权限
     chmod 777 -R openjpeg/
     
     # 将库文件拷贝到系统lib目录
     cp libopenjp2.z.so	/system/lib/
     ```

- 运行测试程序

  1. 执行如下命令运行测试脚本，测试脚本会调用测试程序并将运行信息打印出来。

     ```
     sh test_open_jpeg.sh
     ```

  2. 运行中的日志信息会实时打印在终端，若日志信息中所有测试项目为test_ok，则说明通过了全部的测试项目。

     ![image-20220922143536085](./media/running_result.png)



## 参考资料

- [润和RK3568开发板标准系统快速上手](https://gitee.com/openharmony-sig/knowledge_demo_temp/tree/master/docs/rk3568_helloworld)。
- [OpenHarmony3.2beta1](https://gitee.com/openharmony/docs/blob/OpenHarmony-v3.2-Beta1/zh-cn/release-notes/OpenHarmony-v3.2-beta1.md)。
- [OpenHarmony三方库组织地址](https://gitee.com/openharmony-tpc)。
- [知识体系](https://gitee.com/openharmony-sig/knowledge)。

