# minizip-ng 如何集成到应用hap
## 准备应用工程
​	本次验证所用的开发板型号为：rk3568。其中，系统源码版本为：openharmony 3.2 Beta1。
### 准备应用开发环境
- 开发工具：DevEco Studio 3.0 Beta4
- SDK版本： openharmony API 9

### 增加构建脚本及配置文件

- 下载本仓库代码

  ```
  cd ~/
  git clone git@gitee.com:openharmony-sig/tpc_c_cplusplus.git --depth=1      # 将本仓库下载到本地
  ```

- 仓库代码目录结构说明

  ```
  tpc_c_cplusplus/minizip-ng
  ├── docs			        # 存放三方库相关文档的文件夹
  ├── BUILD.gn		        # 构建脚本，支持rom包集成
  ├── bundle.json			    # 三方库组件定义文件
  ├── CMakeLists.txt          # 构建脚本，支持hap包集成
  ├── README.OpenSource       # 说明三方库源码的下载地址，版本，license等信息
  ├── README_zh.md
  ```

- 将上面的minizip-ng目录及其下面所有文件放在“工程名称\entry\src\main\cpp\third_party”文件夹下。

### 准备三方库源码

使用如下命令下载三方库minizip-ng源码。
```
cd ~/
git clone -b 3.0.4 https://github.com/zlib-ng/minizip-ng.git     
```

将下载的三方库minizip-ng的源码放在“工程名称\entry\src\main\cpp\third_party\minizip-ng”文件夹，如下：

```
工程名称/entry/src/main/cpp/third_party/minizip-ng
├── docs			        # 存放三方库相关文档的文件夹
├── BUILD.gn		        # 构建脚本，支持rom包集成
├── bundle.json			    # 三方库组件定义文件
├── minizip-ng              # 三方库minizip-ng源码目录
├── CMakeLists.txt          # 构建脚本，支持hap包集成
├── README.OpenSource       # 说明三方库源码的下载地址，版本，license等信息
├── README_zh.md
```

添加其它依赖的三方库
由于minizip-ng依赖iconv、openssl、zlib、bzip2、zstd和lzma。另外，DevEco已经支持zlib库，只需引用库即可，所以，需要在工程中和minizip-ng源码平行的目录中添加以下库代码。

- [工程中添加iconv代码的方法]()
- [工程中添加openssl代码的方法]()
- [工程中添加bzip2代码的方法]()
- [工程中添加zstd代码的方法]()
- [工程中添加lzma代码的方法]()

## 应用中使用三方库
- 接口声明：
  ```
  // 操作命令描述
  interface miniOptions {
    include_path?:number        // 包含全路径的方式压缩或者解压
    compress_level?:number      // 压缩级别
    compress_method?:number     // 压缩方式
    overwrite?:number           // 以覆盖方式压缩或者解压
    append?:number              // 以追加方式压缩或者解压
    disk_size?:number           // 磁盘大小
    follow_links?:number        // 跟踪符号链接
    store_links?:number         // 存储符号链接
    zip_cd?:number              // 压缩中心目录
    encoding?:number            // 文件使用编码方式
    verbose?:number             // 打印当前压缩/解压缩操作的进度
  }
  // 接口参数描述
  interface miniProperty {
    path:string                 // 压缩生成的文件/需要被解压的文件路径
    password?:string            // 密码（可选）
    operate?:string             // 调用的接口名称（可选）
    file_extract?:string        // 保存解压后文件的文件夹（可选）
    option:miniOptions          // 操作命令描述
    directory?:string           // 文件所在路径（可选）
    files?:Array<string>        // 需要被压缩的文件列表（可选）
  }
  export const create : () => minizipNapi;
  export class minizipNapi {
      compress(data:object):number;            
      decompress(data:object):number;           
  }
  ```
- 接口使用：<br />
  如下代码为ets中对minizip库压缩功能的使用。分别将file1.txt文件压缩成result.zip文件和将该result.zip文件中内容解压到out目录。
  
  ```
  // 压缩文件
  compress({path:this.dir + "result.zip", operate:"compress",
                    option:{append:1, compress_level:9 }, files:[this.dir + "file1.txt"]});
  // 解压缩文件
  decompress({path:this.dir + "result.zip", operate:"decompress",   
                  option:{overwrite:1, compress_level:9}, directory:this.dir + "out"});
  // 生成带密码，带全路径的压缩文件
  compress({path:this.dir + "result1.zip", password:"test", operate:"compress",
                  option:{overwrite:1,compress_level:9}, files:[this.dir + "file1.txt"]});
  // 解压带密码，带全路径的压缩文件 
  decompress({path:this.dir + "result1.zip", password:"test", operate:"decompress",
                  option:{overwrite:1, compress_level:9}, directory:this.dir + "out"})
  ```
- 应用中链接三方库
  打开“工程名称\entry\src\main\cpp\CMakeLists.txt”，添加如下字段：
  
  ```
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/minizip-ng)
  add_library(entry SHARED minizipNapi.cpp minizip.cpp)
  target_link_libraries(entry PUBLIC libace_napi.z.so minizip_shared)
  ```
## 编译工程
- 连接上设备后，DevEco Studio就会显示被发现设备。然后，点击“运行”，即可依次完成该应用“编译”和“安装”的过程，如图：
  &nbsp;![install](pic/install.png)
## 运行效果
安装hap应用后，在ets界面依次完成：压缩文件、解压缩文件、生成带密码和带全路径的压缩文件、解压带密码和带全路径的压缩文件操作后的结果，如下图：<br />
&nbsp;![ets_results](pic/ets_results.png)

## 参考资料
- [本文档涉及的技术补充说明](rom_integrate.md)
- [本文档引用的相关参考](rom_integrate.md)
- [相关案例参考](rom_integrate.md)











