# C/C++三方库常见 FAQ

- Q1: 交叉编译完后在测试设备上运行测试用例失败，如测试bzip2时提示`/bin/sh: ./bzip2: No such file or directory`？

1. 确保文件已经推送到开发板
2. 确保文件的CPU架构和测试设备系统架构是一致的，可通过`file`查看文件属性，通过`getconf LONG_BIT`查看系统位数，如本错误示例中:

   ```shell
   # file bzip2                                                                           # 查看文件架构时arm64位的
   bzip2: ELF shared object, 64-bit LSB arm64, dynamic (/lib/ld-musl-aarch64.so.1)
   #
   # getconf LONG_BIT                                                                     # 查看系统位数
   32
   #
   ```

   文件属性与系统位数不匹配，故提示无法找到该文件。

- Q2: 当前`lycium`交叉编译适配的CPU架构只支持armeabi-v7a和aarch64的，如若需新增其他CPU架构该如何操作?
  
  本仓库中适配的三方库当前都是通过[lycium](../lycium/)适配且验证过的arm32位以及arm64位架构的三方库，如若需要添加其他CPU架构的编译适配，请参考文档[lycium上面适配OpenHarmony 不同架构的构建](./adpater_architecture.md)。
