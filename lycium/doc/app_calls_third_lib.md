# 北向应用使用三方库二进制文件

## 简介

北向应用集成三方库的方法除了通过[源码方式集成](../../docs/adapter_thirdlib.md)，也可以直接集成已交叉编译好的二进制文件。本文主要介绍如何将二进制文件集成到应用中。

**注意：集成到北向应用的二进制文件必须是通过OpenHarmony的linux下的SDK交叉编译生成的，不能是通过rom方式编译生成的。**

## 集成方法

### 交叉编译三方库

交叉编译的方法可以参照文档[OpenHarmony Linux 环境 SDK 使用说明](./ohos_use_sdk/OHOS_SDK-Usage.md),也可以直接使用[lycium工具](../)进行交叉编译.

### 应用中调用三方库

1. 将三方库生成的二进制文件拷贝到应用工程目录

   为了更好的管理应用集成的三方库，在应用工程的cpp目录新建一个thirdparty目录，将生成的二进制文件以及头文件拷贝到该目录下，如下图所示,xxx代表的是三方库名称，xxx文件夹下包含了arm架构以后aarch64架构2种方式生成的二进制文件，每种架构目录下包含了该库的头文件(include)以及二进制文件(lib)：

   ![lib location](./media/lib_location.png)

   如果该三方库二进制文件为so文件，还需要将so文件拷贝到工程目录的`entry/libs/${OHOS_ARCH}/`目录下,如下图:

   ![so localtion](./media/so_location.png)

   **动态库引用事项注意: IDE只识别不带版本号的库(即libxxx.so)，而一般的动态库的soname是带主版本号的（libxxx.so.1），所以我们需要同时将不带版本号以及带主版本号的so文件拷贝到entry/libs/${OHOS_ARCH}/`目录下。另外，不带版本号的动态库以及带主版本号的动态库在linux下是一个软连接，但拷贝到windows目录后变为实体，因此不需要做任何单独处理。**

2. 配置对应链接

   配置链接只需要在cpp目录的CMakeLists.txt文件中添加对应`target_link_libraries`即可：
   - 配置静态库链接

    ```cmake
    target_link_libraries(entry PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/xxx/${OHOS_ARCH}/lib/libxxx.a)
    ```

   - 配置动态库链接

    ```cmake
    target_link_libraries(entry PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/xxx/${OHOS_ARCH}/lib/libxxx.so)
    ```

    ![so link](./media/so_link.png)

3. 配置头文件路径

   配置链接只需要在cpp目录的CMakeLists.txt文件中添加对应`target_include_directories`

    ```cmake
    target_include_directories(entry PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/xxx/${OHOS_ARCH}/include)
    ```

4. 编写NAPI接口

   配置完三方库的链接和头文件路径后，可以根据各自的业务逻辑调用三方库对应的接口完成NAPI接口的编写，NAPI接口开发可以参照文档[NAPI学习](../../docs/thirdparty_knowledge.md###北向应用中使用).

5. 编译构建

   请参考文档[DevEco Studio编译构建指南](https://developer.harmonyos.com/cn/docs/documentation/doc-guides-V3/build_overview-0000001055075201-V3?catalogVersion=V3)

### 测试验证

请参照文档[在OpenHarmony开发板上验证hap包中集成的三方库](../../docs/test_hap.md)

## 参考文档

- [北向应用集成三方库](../../docs/thirdparty_knowledge.md)
- [DevEco Studio使用指南](https://developer.harmonyos.com/cn/docs/documentation/doc-guides-V3/build_overview-0000001055075201-V3?catalogVersion=V3)
