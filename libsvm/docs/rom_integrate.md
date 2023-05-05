# libsvm如何集成到系统Rom

## 准备源码工程

本库是基于OpenHarmony-v3.2-Beta2版本，在RK3568开发板上验证的，如果是从未使用过RK3568，可以先查看[润和RK3568开发板标准系统快速上手](https://gitee.com/openharmony-sig/knowledge_demo_temp/tree/master/docs/rk3568_helloworld)。

### 准备系统Rom源码

系统源码获取请参考：[OpenHarmony源码下载](https://gitee.com/openharmony/docs/blob/OpenHarmony-v3.2-Beta3/zh-cn/release-notes/OpenHarmony-v3.2-beta2.md)

### 增加构建脚本及配置文件

- 下载本仓库代码

  ```
  cd ~
  git clone git@gitee.com:openharmony-sig/tpc_c_cplusplus.git --depth=1
  ```

- 三方库目录结构

  ```
  tpc_c_cplusplus/libsvm  #三方库libsvm的目录结构如下
  ├── adapted               #存放三方库适配需要的代码文件
  ├── docs                  #存放三方库相关文档的文件夹
  ├── BUILD.gn              #构建脚本，支持rom包集成
  ├── bundle.json           #三方库组件定义文件
  ├── README.OpenSource     #说明三方库源码的下载地址，版本，license等信息
  ├── README_zh.md        
  ```

- 将三方库拷贝到OpenHarmony源码的third_party目录下

  ```
  cp ~/tpc_c_cplusplus/libsvm ~/openharmony/third_party -rf
  ```

### 准备三方库源码

```
cd ~/openharmony/third_party/libsvm    #进入三方库目录
git clone https://github.com/cjlin1/libsvm.git   #下载三方库源码
```

## 系统Rom中引入三方库

准备完三方库代码后，我们需要将三方库加入到编译构建体系中。标准系统编译构建可以参考文档[标准系统编译构建指导](https://gitee.com/openharmony/docs/blob/OpenHarmony-3.2-Beta1/zh-cn/device-dev/subsystems/subsys-build-standard-large.md) 我们默认三方库是属于OpenHarmony的thirdparty子系统，如果需要自己定义子系统参考文档[如何为三方库组件中添加一个三方库](https://gitee.com/openharmony-sig/knowledge/blob/master/docs/openharmony_getstarted/port_thirdparty/README.md) 新增需要编译的组件,在OpenHarmony源码的vendor/hihope/rk3568/config.json文件中，新增需要编译的组件，如下代码段所示，在thirdparty子系统下面新增libsvm组件

```
    {
      "subsystem": "thirdparty",
      "components": [
        {
          "component": "musl",
          "features": []
        },
        {
          "component": "libsvm",
          "features": []
        }
      ]
    }
```

## 系统Rom中引入三方库测试程序

libsvm原生库提供了测试用例，如需要引入测试程序，在OpenHarmony源码的vendor/hihope/rk3568/config.json文件,对应组件的features中打开编译选项：

```
{
	"subsystem": "thirdparty",
	"components": [
	 {
		"component": "musl",
		"features": []
	 },
	 {
		"component": "libsvm",
		"features": [ "enable_svm_test=true" ]
	 }
    ]
}
```

## 编译工程

在OpenHarmony源码根目录下

```
cd ~/openharmony
```

- 执行编译

  ```
  ./build.sh --product-name rk3568 --ccache
  ```
  
- 生成文件的路径

  可执行文件和库生成在out/rk3568/thirdparty/libsvm

## 运行效果

将编译生成的库和测试文件放到板子上运行，为避免每次将文件推入设备都烧录整个镜像，我们使用hdc_std工具将文件推到开发板上

- 首先将hdc_std工具编译出来 工具编译出来所在路径out/sdk/ohos-sdk/windows/toolchains/hdc_std.exe

  ```
  hb set     #源码根目录下使用hb set 选择产品ohos-sdk
  hb build   #然后编译
  ```

- 将工具拷贝到Windows，可以为工具目录配置环境变量，也可以在工具所在目录打开windows命令行

- 将编译生成的svm-scale、svm-train、svm-predict可执行文件和训练数据[test.data](https://www.csie.ntu.edu.tw/~cjlin/papers/guide/data/)准备好

- 将准备好的文件推送到开发板，在windows命令行进行如下操作

  ```
  hdc_std shell mount -o remount,rw /     #修改系统权限为可读写
  hdc_std file send svm-scale /data         #将可执行文件推入开发板data目录
  hdc_std file send svm-train /data         #将可执行文件推入开发板data目录
  hdc_std file send svm-predict /data         #将可执行文件推入开发板data目录

  hdc_std file send libsvm.z.so /system/lib64
  hdc_std file send libsvm.z.so /system/lib
  hdc_std file send test.data /data  #将测试文件推入开发板data目录
  hdc_std shell                          #进入开发板
  chmod 777 svm-scale              #添加权限
  chmod 777 svm-train              #添加权限
  chmod 777 svm-predict              #添加权限

  ./svm-scale  /data/test.data  >  /data/test.scale #scale
  ./svm-train  /data/test.scale  /data/test.model #train
  ./svm-predict  /data/test.scale  /data/test.model  /data/test.predict #predict
  ```

- 运行测试程序 效果如下图

-  ![result](pic/result.png)

## 参考资料

- [润和RK3568开发板标准系统快速上手](https://gitee.com/openharmony-sig/knowledge_demo_temp/tree/master/docs/rk3568_helloworld)
- [OpenHarmony三方库地址](https://gitee.com/openharmony-tpc)
- [OpenHarmony知识体系](https://gitee.com/openharmony-sig/knowledge)
- [如何为三方库组件中添加一个三方库](https://gitee.com/openharmony-sig/knowledge/blob/master/docs/openharmony_getstarted/port_thirdparty/README.md)
- [标准系统编译构建指导](https://gitee.com/openharmony/docs/blob/OpenHarmony-3.2-Beta1/zh-cn/device-dev/subsystems/subsys-build-standard-large.md)