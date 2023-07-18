# minizip-ng 三方库说明

## 功能简介

minizip是一个用C编写的zip文件操作库。

## 使用约束

- ROM版本：OpenHarmony3.2 Beta1
- IDE版本：DevEco Studio 3.1 Release
- SDK版本：ohos_sdk_public 4.0.8.1 (API Version 10 Release)
- 三方库版本：3.0.4
- 当前适配的功能：
  - 创建和解压缩zip存档。
  - 在zip存档中添加和删除条目。
  - 从内存中读取和写入压缩文件。
  - Zlib、BZIP2、LZMA、XZ和ZSTD压缩方法。
  - 跟踪并存储符号链接。
  - 通过UTF-8编码支持Unicode文件名。
  - 传统字符编码支持CP437、CP932、CP936、CP950。
  - 关闭压缩、解压缩或加密的编译
  - 将本地文件头信息归零。
  - 压缩/解压缩中心目录以减小大小
  - 如果中心目录损坏或丢失，则恢复该目录

## 集成方式

- [系统Rom包集成](docs/rom_integrate.md)
- [应用hap包集成](docs/hap_integrate.md)
