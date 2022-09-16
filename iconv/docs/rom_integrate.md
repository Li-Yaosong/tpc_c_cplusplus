# libiconv 如何集成到系统Rom
## 准备源码工程
本库是基于OpenHarmony 3.2 beta1版本，在润和RK3568开发板上验证的。如果是从未使用过RK3568，可以先查看[润和RK3568开发板标准系统快速上手](https://gitee.com/openharmony-sig/knowledge_demo_temp/tree/master/docs/rk3568_helloworld)。



### 准备系统Rom源码

源码获取方法请参照：[OpenHarmony3.2beta1源码下载](https://gitee.com/openharmony/docs/blob/OpenHarmony-v3.2-Beta1/zh-cn/release-notes/OpenHarmony-v3.2-beta1.md#%E6%BA%90%E7%A0%81%E8%8E%B7%E5%8F%96)



### 增加构建脚本及配置文件

- 下载本仓库代码

  ```
  # cd ~/
  # git clone git@gitee.com:openharmony-sig/tpc_c_cplusplus.git --depth=1
  ```

- 仓库代码目录结构说明

  ```
  # cd  ~/tpc_c_cplusplus/iconv/	# 进入到仓库代码库目录
  ```

  ```
  iconv
  	|-- BUILD.gn			# 编译构建脚本
  	|-- ohos.build			# 组件定义文件
  	|-- README_zh.md		# libiconv说明文档
  	|-- README.OpenSource	# libiconv 开源信息说明文档
  	|-- docs				# 存放libconv相关文档
  	|-- media				# 存放图片的文件夹
  	|-- adapter
  			|-- include		# 包含库相关配置头文件的文件夹
  					|-- config.h
  					|-- incov.h
  					|-- localcharset.h
  					|-- libcharset.h	
  					|-- samples 		# 包含测试用例相关配置头文件的文件夹
  ```

  

- 将本仓库文件夹拷贝到third_party下

  ```
  # cp -arf ~/tpc_c_cplusplus/iconv ~/OpenHarmony/third_party
  ```



### 准备三方库源码

+ 将源码下载到iconv目录并将其解压出来。

  ```
  # cd ~/OpenHarmony/third_party/iconv								# 进入到iconv目录
  # wget https://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.17.tar.gz    # 下载三方库
  # tar -zxvf libiconv-1.17.tar.gz									# 解压库
  ```

  


## 系统Rom中引入三方库

准备完三方库代码后，我们需要将三方库加入到编译构建体系中。标准系统编译构建可以参考文档[标准系统编译构建指导](https://gitee.com/openharmony/docs/blob/OpenHarmony-3.2-Beta1/zh-cn/device-dev/subsystems/subsys-build-standard-large.md)。

这里我们只需要将三方库作为组件添加到三方库子系统中即可(详细信息参照文档[如何为三方库组件中添加一个三方库](./add_thirdparty_component.md))

添加步骤：

- 三方库子系统中添加组件

  打开//vendor/hihope/rk356/config.json文件，找到thirdparty子系统并添加一个iconv的组件

  ```
  {
        "subsystem": "thirdparty",
        "components": [
          {
            "component": "musl",
            "features": []
          },
          {
            "component": "iconv",
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
        "iconv": {
          "module_list": [
            "//third_party/iconv:libiconv"
          ]
        }
      }
  }
  ```

  

- 三方库编译脚本中添加子系统组件名

  ```
  ohos_shared_library("iconv"){		
      ...								
    part_name = "iconv"				
  }
  ```
  
  

## 系统Rom中引入三方库测试程序

libiconv原生库提供了测试程序且已完成了gn的适配，我们只需要在三方库目录下的ohos.build中添加测试程序的依赖即可，如：

```
{
  "subsystem": "thirdparty",
    "parts":{
      "iconv": {
        "module_list": [
          "//third_party/iconv:iconv_test"
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

- 正常编译完后会在out/rk3568/packages/phone/system/bin下生成iconv_test以及out/rk3568/packages/phone/system/lib下生成libiconv.z.so。

## 安装应用程序

安装应用的方式有两种：

- 烧录带有应用程序的镜像包

  烧录固件请参照[rk3568镜像烧录](https://gitee.com/openharmony-sig/knowledge_demo_temp/blob/master/docs/rk3568_helloworld/%E9%95%9C%E5%83%8F%E7%83%A7%E5%BD%95.md)。

- 将应用直接推送到开发板

  我们可以将需要推送到开发板的文件全部打包成一个压缩文件(iconv.tar)，然后通过[hdc_std工具](https://gitee.com/isrc_ohos/hdc-tool)推送到开发板，在推送前需要确保一点，板子的系统为可读写，如果出现推送失败且提示无权限，则需要重新加载系统权限

  ```
  # hdc_std.exe shell 						## 进入开发板系统
  # mount -o remount,rw /						## 修改系统权限
  # exit										## 退出开发板系统
  # hdc_std.exe file send .\iconv.tar /data/  ## 推送文件到开发板系统
  # hdc_std.exe shell
  # cd /data/
  # tar -xvf iconv.tar						## 解压文件包
  # cd iconv
  # cp libiconv.z.so	/lib/					## 将库文件拷贝到系统lib目录
  ```

  

## 运行效果

程序安装完后，可以通过命令行直接运行测试程序，在这里我们准备了2个测试文件，一个是GBK编码文件，一个是UTF-8的文件，：

- UTF-8转成GBK

  转换前：

  &nbsp;![UTF-8](../media/UTF-8.png)

  转换后：

  &nbsp;![UTF8_2_GBK](../media/UTF8_2_GBK.png)

  终端只能显示UTF-8的编码，所以转换GBK后，中文会显示乱码。将终端输出导入到文件(tmp.txt)后通过cmp命令与test_GBK文件进行对比，无差异

  ![1663034095540](../media/cmp_GBK.png)

- GBK转成UTF-8

  转换前：

  &nbsp;![GBK](../media/GBK.png)

  转换后：

  &nbsp;![GBK_2_UTF8](../media/GBK_2_UTF8.png)

  终端显示正常，且将终端输出导入到文件(tmp.txt)后通过cmp命令与test_UTF8.txt文件对比，无差异
  
  &nbsp;![1663034192353](../media/cmp_UTF8.png)

## 参考资料

- [润和RK3568开发板标准系统快速上手](https://gitee.com/openharmony-sig/knowledge_demo_temp/tree/master/docs/rk3568_helloworld)。
- [OpenHarmony3.2beta1](https://gitee.com/openharmony/docs/blob/OpenHarmony-v3.2-Beta1/zh-cn/release-notes/OpenHarmony-v3.2-beta1.md)。
- [OpenHarmony三方库组织地址](https://gitee.com/openharmony-tpc)。 
- [知识体系](https://gitee.com/openharmony-sig/knowledge)。

