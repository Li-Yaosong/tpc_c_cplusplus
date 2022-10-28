# tpc_c_cplusplus

## 简介
本仓库主要用于存放已经适配OpenHarmony的C/C++三方库的适配脚本，三方库的适配/使用指导；每个三方库目录下包含了GN化的适配指导、编译脚本和包含了cmake的适配指导、编译脚本。

## 本仓库目录
```
tpc_c_cplusplus
├── README_zh.md            #仓库主页
├── common                  #仓库通用说明文档的文件夹
├── thirdparty_template     #三方库模板文件夹
│   ├──README_zh.md         #三方库简介
│   ├──README.OpenSource    #说明三方库源码的下载地址，版本，license等信息
│   ├──bundle.json          #三方库组件定义文件
│   ├──CMakeLists.txt       #构建脚本，支持hap包集成
│   ├──BUILD.gn             #构建脚本，支持rom包集成
│   ├──adapted              #该目录存放三方库适配需要的代码文件
│   │   ├──config.h         #例如配置文件
│   │   ├──...              #其他适配文件
│   ├──docs                 #存放三方库相关文档的文件夹
│   │   ├──rom_integrate.md #rom集成说明文档
│   │   ├──hap_integrate.md #hap集成说明文档
│   │   ├── ...             #其他说明文档
├── thirdparty1             #三方库文件夹，内容和thirdparty_template模板的格式一样	
├── thirdparty2             #三方库文件夹，内容和thirdparty_template模板的格式一样
......
```

## 如何贡献
- [遵守仓库目录结构](#本仓库目录)
- [遵守OpenHarmony编码贡献规范](https://gitee.com/openharmony-sig/knowledge_demo_smart_home/blob/master/dev/docs/contribute/README.md)
- [三方库模板目录](thirdparty_template)

## 三方库列表
### 工具类
#### 数据压缩
- [lzma](lzma/README_zh.md) - 是2001年以来得到发展的一个数据压缩算法，它是一种高压缩比的传统数据压缩软件
- [minizip-ng](minizip-ng/README_zh.md) - 一个用C编写的zip文件操作库
- [zstd](zstd/README_zh.md) - 一种快速的无损压缩算法，是针对 zlib 级别的实时压缩方案，以及更好的压缩比
#### XML解析
- [xerces-c](xerces-c/README_zh.md) - 一个开放源代码的XML语法分析器，它提供了SAX和DOM API
#### 编码转换
- [iconv](iconv/README_zh.md) - 一个实现字符集转换的库，用于没有Unicode或无法从其他字符转换为Unicode的系统