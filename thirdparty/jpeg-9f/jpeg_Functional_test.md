# jpeg测试说明

## 已测试功能

-图像数据转换为 JPEG 

-将 JPEG 格式解码为其他格式

## 生成库与测试文件

该库基于OpenHarmony5.0.0 Release版本进行编译测试。

1. 添加部件

   本项目在系统源码third_party目录下创建一个目录名为jpeg-9f

   在jpeg-9f目录下创建bundle.json文件，内容如下

   ```+shell
   {
       "name": "@ohos/jpeg-9f",
       "description": "",
       "version": "3.1",
       "license": "MIT",
       "publishAs": "code-segment",
       "segment": {
           "destPath": "third_party/jpeg-9f"
       },
       "dirs": {},
       "scripts": {},
       "licensePath": "LICENSE.md",
       "component": {
           "name": "jpeg-9f",
           "subsystem": "thirdparty",
           "syscap": [],
           "features": [],
           "adapted_system_type": [
               "standard"
           ],
           "rom": "",
           "ram": "",
           "deps": {
               "components": [],
               "third_party": []
           },
           "build": {
               "sub_component": [
   		    "//third_party/jpeg-9f:jpeg-9f"
               ],
               "inner_kits": [],
               "test": []
   	}
       }
   }
   
   ```

2. 加入编译中vendor/hihope/rk3568/config.json中的thirdparty子系统下组件（component）的信息，添加jpeg-9f部件

   ```+shell
           {
             "component": "jpeg-9f",
             "features": []
           },
   ```

3. 编译出动态链接库和测试用的可执行文件

   编译执行语句如下为：

   ```+shell
   ./build.sh --product-name rk3568 --ccache -T third_party/jpeg-9f:jpeg-9f
   ```

   生成的文件在//out/rk3568目录下。

## 逻辑测试

### 通过原生库示例程序测试

在jpeg原生库的jpeg目录下有原生的测试示例程序，在rk3568开发板上运行均成功，运行结果均符合示例程序要求，且运行效果与pc端运行效果一致。

测试脚本如下：

```+shell
#!/bin/sh
rm -f testout*
./djpeg -dct int -ppm -outfile testout.ppm testorig.jpg
./djpeg -dct int -gif -outfile testout.gif testorig.jpg
./djpeg -dct int -bmp -colors 256 -outfile testout.bmp estorig.jpg
./cjpeg -dct int -outfile testout.jpg testimg.ppm
./djpeg -dct int -ppm -outfile testoutp.ppm testprog.jpg
./cjpeg -dct int -progressive -opt -outfile testoutp.jpg testimg.ppm
./jpegtran -outfile testoutt.jpg testprog.jpg


cmp testimg.ppm testout.ppm
cmp testimg.gif testout.gif
cmp testimg.bmp testout.bmp
cmp testimg.jpg testout.jpg
cmp testimg.ppm testoutp.ppm
cmp testimgp.jpg testoutp.jpg
cmp testorig.jpg testoutt.jpg
```

### 测试步骤

1. 通过与ohos版本匹配的hdc_std工具,将编译生成的库以及测试用的可执行文件推送到开发板上

   ```+shell
   hdc_std shell               
   mount -o remount,rw /	    ## 重新加载系统为可读写
   mkdir jpeg		            ## 创建jpeg目录存放测试用例
   exit                        ## 退出开发板系统
   ```

2. 将压缩包push到开发板

   ```+shell
   hdc_std file jpeg.tar /jpeg
   ```

3. 解压压缩包并将库文件拷贝到对应的目录

   ```+shell
   hdc_std shell              ## 进入开发板系统
   cd jpeg
   tar -xvf jpeg.tar
   cp libjpeg.z.so /system/lib64/
   ```

4. 执行测试程序jpeg-9f_test.sh

   执行语句如下

   ```+shell
   chmod +x jpeg-9f_test.sh
   ./jpeg-9f_test.sh
   ```

   测试结果：所有图片都按照预定效果转变，和pc端效果相同

   

