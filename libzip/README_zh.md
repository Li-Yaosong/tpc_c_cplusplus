# libzip三方库说明
## 功能简介
本软件是参照开源软件 [libzip](https://github.com/nih-at/libzip) 的CMakeLists.txt 重新编辑了适合在OpenHarmony上使用的CMakeLists.txt，
libzip是一个用于读取、创建和修改zip存档的C库。文件可以从数据缓冲区、文件或直接从其他zip档案复制的压缩数据中添加。

## 使用约束
- ROM版本：OpenHarmony-v3.2-Beta1

- 三方库版本：v1.9.2

- 当前适配的功能：读取，创建和修改的C库 zip 存档。

- [libzip License](https://github.com/nih-at/libzip/blob/main/LICENSE)
## 集成方式
+ [系统Rom包集成](docs/rom_integrate.md)
+ [系统Hap包集成](docs/hap_integrate.md)
