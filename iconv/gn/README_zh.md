# libiconv 如何集成到系统Rom
## 准备源码工程
本库是基于OpenHarmony 3.2 beta1版本，在润和RK3568开发板上验证的。如果是从未使用过RK3568，可以先查看[润和RK3568开发板标准系统快速上手](https://gitee.com/openharmony-sig/knowledge_demo_temp/tree/master/docs/rk3568_helloworld)。
### 准备系统Rom源码
源码获取方法请参照：[OpenHarmony3.2beta1源码下载](https://gitee.com/openharmony/docs/blob/OpenHarmony-v3.2-Beta1/zh-cn/release-notes/OpenHarmony-v3.2-beta1.md#%E6%BA%90%E7%A0%81%E8%8E%B7%E5%8F%96)

### 准备三方库源码
+ 通过[iconv下载地址]( https://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.17.tar.gz )下载库源码。

+ 将下载的压缩包解压到OpenHarmony源码的third_party/iconv目录下(解压前需要在OpenHarmony/third_party目录下创建iconv文件夹)。

+ 解压后的文件夹重命名为libiconv
  
  ```
  # mv libiconv-1.17 libiconv
  ```
  
  

## 增加构建脚本及配置文件

- 下载本仓库代码

  ```
  # cd ~/
  # git clone git@gitee.com:openharmony-sig/tpc_c_cplusplus.git --step=1
  ```

- 仓库代码目录结构说明

  ```
  # cd  ~/tpc_c_cplusplus/iconv/gn/libiconv/	# 进入到库目录
  ```

  ```
  libiconv
  	|-- BUILD.gn	# 编译构建脚本
  	|-- include		# 包含库相关配置头文件的文件夹
  			|-- config.h
  			|-- incov.h
  			|-- localcharset.h
  			|-- libcharset.h	
  			|-- samples 		# 包含测试用例相关配置头文件的文件夹
  ```

  

- 将构建脚本拷贝到third_party的iconv目录

  ```
  # cp ~/tpc_c_cplusplus/iconv/gn/libiconv/BUILD.gn ~/OpenHarmony/third_party/iconv/
  ```

  

- 将包含配置文件的include文件夹拷贝到third_party的iconv目录

  ```
  # cp -rf ~/tpc_c_cplusplus/iconv/gn/libiconv/include ~/OpenHarmony/third_party/iconv/
  ```

  

## 系统Rom中引入三方库

- 新建子系统(如果将库添加到已有的子系统中可跳过此步骤)

  在根目录下创建一个子系统目录，如demos

  ```
  # mkdir demos
  ```

  并在该文件夹下创建ohos.build文件，内容如下:

  ```
  {
    "subsystem": "demos",
      "parts":{
        "app_demo": {
          "module_list": [
            "//third_party/iconv:libiconv"
          ]
        }
      }
  }
  ```

  

- 将子系统添加到subsystem_config.json

  打开//build/subsystem_config.json文件，在最后加上(如果编译的子系统已在配置中可跳过此步骤)

  ```
  "demos":{
      "path":"demos",
      "name":"demos"
  }
  ```

  

- 将子系统添加到编译体系中

  打开文件//vendor/hihope/rk3568/config.json并在最后添加：

  ```
  {
    "subsystem": "demos",			## 子系统名字
    "components": [
      {
        "component": "app_demo",	## 子系统组件名(part_name名称)
        "features": []
      }
    ]
  }
  ```

  

- 三方库编译脚本中添加子系统组件名

  打开iconv下的BUILD.gn文件，将文件中的part_name值修改为对应工程依赖的子系统组件名字, 如:

  ```
  part_name = "app_demo"
  ```

  

## 系统Rom中引入三方库测试程序

libiconv原生库提供了测试程序且已完成了gn的适配，我们只需要在子系统目录下的ohos.build中添加测试程序的依赖即可，如：

```
{
  "subsystem": "demos",
    "parts":{
      "app_demo": {
        "module_list": [
          "//third_party/iconv:iconv_test"	# 编译生成iconv测试程序
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

## 固件烧录

- 烧录固件请参照[rk3568镜像烧录](https://gitee.com/openharmony-sig/knowledge_demo_temp/blob/master/docs/rk3568_helloworld/%E9%95%9C%E5%83%8F%E7%83%A7%E5%BD%95.md)。

- 除了烧录固件，如果我们开发板已烧录好了系统，我们可以通过[hdc_std工具](https://gitee.com/isrc_ohos/hdc-tool)将库文件与测试程序推送到开发板：

  ```
  hdc_std.exe file send .\iconv_test /			## 将测试程序推送到开发板的根目录
  hdc_std.exe file send .\libiconv.z.so /			## 将库文件推送到开发板的根目录
  ```

  其他资源文件参考类似方法推送。在推送前需要确保一点，板子的系统为可读写，如果出现推送失败且提示无权限，则需要重新加载系统权限

  ```
  hdc_std shell 				## 进入板子系统
  mount -o remount,rw /		## 修改系统权限
  ```

## 运行效果

固件烧录成功后，可以通过命令行直接运行测试程序，运行效果如下：

- UTF-8转成GBK

  转换前：

  &nbsp;![UTF-8](../media/UTF-8.png)

  转换后：

  &nbsp;![UTF8_2_GBK](../media/UTF8_2_GBK.png)

  终端只能显示UTF-8的编码，所以转换GBK后，中文会显示乱码。将终端输出导入到文件(tmp.txt)后与test_GBK.txt文件对比，内容是一致的。

- GBK转成UTF-8

  转换前：

  &nbsp;![GBK](../media/GBK.png)

  转换后：

  &nbsp;![GBK_2_UTF8](../media/GBK_2_UTF8.png)

  终端显示正常，且将终端输出导入到文件(tmp.txt)后与test_UTF8.txt文件对比，内容是一致的。

## 参考资料

- [润和RK3568开发板标准系统快速上手](https://gitee.com/openharmony-sig/knowledge_demo_temp/tree/master/docs/rk3568_helloworld)。
- [OpenHarmony3.2beta1](https://gitee.com/openharmony/docs/blob/OpenHarmony-v3.2-Beta1/zh-cn/release-notes/OpenHarmony-v3.2-beta1.md)。
- [OpenHarmony三方库组织地址](https://gitee.com/openharmony-tpc)。 
- [知识体系](https://gitee.com/openharmony-sig/knowledge)。

