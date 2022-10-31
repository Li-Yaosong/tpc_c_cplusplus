# Leptonica如何集成到系统Rom

## 准备源码工程

本库是基于OpenHarmony-v3.2-Beta1版本，在润和RK3568开发板上验证的。如果是从未使用过RK3568，可以先查看[润和RK3568开发板标准系统快速上手](https://gitee.com/openharmony-sig/knowledge_demo_temp/tree/master/docs/rk3568_helloworld)。

## 准备系统Rom源码

系统源码获取方法请参照：[OpenHarmony源码下载](https://gitee.com/openharmony/docs/blob/OpenHarmony-v3.2-Beta1/zh-cn/release-notes/OpenHarmony-v3.2-beta1.md#%E6%BA%90%E7%A0%81%E8%8E%B7%E5%8F%96)

## 增加构建脚本及配置文件

- 下载本仓库代码

  ```shell
  cd ~/
  git clone git@gitee.com:openharmony-sig/tpc_c_cplusplus.git --depth=1    # 将本仓库下载到本地
  ```

- 仓库代码目录结构说明

  ```shell
  tpc_c_cplusplus/leptonica
  |-- docs                # 存放三方库相关文档的文件夹
  |-- adapted             # 存放三方库适配需要的代码文件
  |-- BUILD.gn            # 构建脚本，支持rom包集成
  |-- bundle.json         # 三方库组件定义文件
  ├── README.OpenSource   # 说明三方库源码的下载地址，版本，license等信息
  ├── README_zh.md  
  ```

- 将本仓库文件夹拷贝到third_party下

  ```shell
  cp ~/tpc_c_cplusplus/leptonica   ~/openharmony/third_party/ -rf
  ```

## 准备三方库依赖的四方库

本三方库依赖了libgif, libjpg, libpng, openjpeg, zlib, libtiff, libwebp等四方库，除了openjpeg与libtiff外，其他四方库OpenHarmony都已支持。
openjpeg与libtiff的适配参考以下方法：

- [openjpeg的适配](https://gitee.com/openharmony-sig/tpc_c_cplusplus/openjpeg)
- [libtiff的适配](https://gitee.com/openharmony-sig/tpc_c_cplusplus/libtiff)

## 准备三方库源码

- 将源码下载到leptonica目录。
  
  ```shell
  cd ~/OpenHarmony/third_party/leptonica                                # 进入到leptonica目录
  git clone -b v1.74.3 https://github.com/DanBloomberg/leptonica.git    # 下载三方库
  ```

## 系统Rom中引入三方库

  准备完三方库代码后，我们需要将三方库加入到编译构建体系中。标准系统编译构建可以参考文档[标准系统编译构建指导](https://gitee.com/openharmony/docs/blob/OpenHarmony-3.2-Beta1/zh-cn/device-dev/subsystems/subsys-build-standard-large.md)。 <br />
  我们默认三方库是属于OpenHarmony的thirdparty子系统，如果需要自己定义子系统参考文档[如何为三方库组件中添加一个三方库](https://gitee.com/openharmony-sig/knowledge/blob/master/docs/openharmony_getstarted/port_thirdparty/README.md)  <br />
  在OpenHarmony源码的vendor/hihope/rk3568/config.json文件，新增需要编译的组件，如下：

  ```json
  {
     "subsystem": "thirdparty",
     "components": [
         {
         "component": "musl",
         "features": []
      },
      {
         "component": "leptonica",
         "features": []
      }
     ]
  }
  ```

## 系统Rom中引入三方库测试程序

  在OpenHarmony源码的vendor/hihope/rk356/config.json文件,对应组件的features中打开编译选项，如下：

  ```json
  {
     "subsystem": "thirdparty",
     "components": [
         {
         "component": "musl",
         "features": []
      },
      {
         "component": "leptonica",
         "features": ["enable_leptonica_test=true"]
      }
     ]
  }
  ```

## 编译工程

- 进入OpenHarmony源码根目录下

  ```shell
  cd ~/openharmony
  ```

- 选择产品

  ```shell
  hb set                          # 该命令会列出所有可选产品，这里我们选择rk3568
  ```

- 运行编译

  ```shell
  hb build --target-cpu arm      #编译32位系统
  hb build --target-cpu arm64    #编译64位系统
  ```

- 生成的可执行文件和库文件都在out/rk3568/thirdparty/leptonica目录下，同时也打包到了镜像中

## 运行效果

将编译生成的库和测试文件放到板子上运行，为避免每次将文件推入设备都需要烧录整个镜像，我们使用hdc_std工具将文件推到开发板上

- 首先将hdc_std工具编译出来

  ```shell
  hb set             # 首先，源码根目录下使用hb set 选择产品ohos-sdk
  hb build           # 然后，编译。最后，工具编译出来在out/sdk/ohos-sdk/windows/toolchains/hdc_std.exe
  ```

- 将工具拷贝到Windows，可以为工具目录配置环境变量，也可以在工具所在目录打开windows命令行
- 将原生库测试需要的所有文件打包成leptonica.tar,并拷贝到windows下
- 将文件推送到开发板，在windows命令行进行如下操作

  ```shell
  hdc_std shell mount -o remount,rw /          # 修改系统权限为可读写
  hdc_std file send leptonica.tar /            # 将文件包推入开发板
  hdc_std shell                                # 进入开发板
  tar xvf leptonica.tar                        # 解压
  cd leptonica                                 # 进入leptonica目录
  mv libleptonica.z.so /system/lib64/          # 64位系统需要将库文件拷贝到系统lib64目录, 32位系统则是lib目录
  ```

- 运行测试程序

  测试用例较多，我们只演示部分用例：adaptmap_dark,功能是扫描的图片去污、提色功能和图片转pdf功能 <br />
  &nbsp;![run_adaptmap_dark](pic/run_adaptmap_dark.png)

  结果验证：
  1. 去污、提色功能
     将'/tmp/lept/adapt/'下的图片'adapt_000.jpg'和'adapt_001.jpg' 拷贝到windows下面并打开对比，效果如图： <br />
     &nbsp;![color_lifting](pic/color_lifting.png) <br />
  2. 图片转pdf功能
     将'/tmp/lept/adapt/'下的图片'cleaning.pdf'拷贝到windows下面并打开，效果如图：<br />
     &nbsp;![generate_pdf](pic/generate_pdf.png)

## 参考资料

- [润和RK3568开发板标准系统快速上手](https://gitee.com/openharmony-sig/knowledge_demo_temp/tree/master/docs/rk3568_helloworld)
- [OpenHarmony源码下载](https://gitee.com/openharmony/docs/blob/OpenHarmony-v3.2-Beta1/zh-cn/release-notes/OpenHarmony-v3.2-beta1.md#%E6%BA%90%E7%A0%81%E8%8E%B7%E5%8F%96)
- [标准系统编译构建指导](https://gitee.com/openharmony/docs/blob/OpenHarmony-3.2-Beta1/zh-cn/device-dev/subsystems/subsys-build-standard-large.md)
- [如何为三方库组件中添加一个三方库](https://gitee.com/openharmony-sig/knowledge/blob/master/docs/openharmony_getstarted/port_thirdparty/README.md)
- [OpenHarmony三方库地址](https://gitee.com/openharmony-tpc)
- [OpenHarmony知识体系](https://gitee.com/openharmony-sig/knowledge)
  