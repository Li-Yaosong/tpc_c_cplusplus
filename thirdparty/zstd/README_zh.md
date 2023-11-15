# zstd 三方库说明
## 功能简介
zstd 是一种快速的无损压缩算法，是针对 zlib 级别的实时压缩方案，以及更好的压缩比。
## 使用约束
- ROM版本：OpenHarmony3.2 Beta1
- IDE版本：DevEco Studio 3.1 Release
- SDK版本：ohos_sdk_public 4.0.8.1 (API Version 10 Release)
- 三方库版本：v1.5.4
- 当前适配的功能：完成了生成和解码 .zst 格式以及字典压缩、解压缩
## 集成方式
+ [系统Rom包集成](docs/rom_integrate.md)
+ [应用hap包集成](docs/hap_integrate.md)