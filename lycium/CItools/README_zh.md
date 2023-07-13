# 三方库测试验证工具说明文档

## 背景
业界内C/C++三方库测试框架多种多样，我们无法将其统一，为了保证三方库功能完整，我们基于原生库的测试用例进行测试验证。为此，我们需要集成了一套可以在OHOS上进行make test等操作的环境；有如下两种集成方式。

## 集成方式

### 二进制文件集成

我们已经编译出测试验证工具的二进制文件，可以直接参考[CItools README](https://gitee.com/han_jin_fei/lycium-citools)文档，进行环境搭建；如需了解编译过程，请查看源码编译集成段落。

### 源码编译集成

cmake、make等工具源码集成指导文档，如下列表所示，可按照指导文档编译出二进制文件，再拷贝到OHOS上，进行环境搭建。

| 工具名称   | 编译指导文档路径32位                                         | 编译指导文档路径64位                                         |
| :--------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| cmake 32位 | [cmake_32位](./cmake/cmake_armeabi_v7a_compilation_instructions.md) | [cmake_64位](./cmake/cmake_arm64_v8a_compilation_instructions.md) |
| make       | [make_32位](./make/make_armeabi-v7a_Compilation_instructions.md) | [make_64位](./make/make_arm64_v8a_Compilation_instructions.md) |
| busybox    | [busybox_32位](./busybox/busybox_armeabi-v7a_Compilation_instructions.md) | [busybox_64位](./busybox/busybox_arm64_v8a_Compilation_instructions.md) |
| perl       | [perl_32位](./perl/perl_armeabi-v7a_Compilation_instructions.md) | [perl_64位](./perl/perl_arm64_v8a_Compilation_instructions.md) |
| shell_cmd  | [shell_cmd_32位](./shell_cmd/shell_cmd_armeabi_v7a_Compilation_instructions.md) | [shell_cmd_64位](./shell_cmd/shell_cmd_arm64_v8a_Compilation_instructions.md) |

