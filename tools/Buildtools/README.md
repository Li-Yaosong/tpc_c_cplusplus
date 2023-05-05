## 下载ohos sdk
[参考OHOS_SDK-Usage](../doc/ohos_use_sdk/OHOS_SDK-Usage.md)

## 配置环境变量
```shell
                    # 此处是我的ohos sdk解压目录, 使用者需自行配置自己的目录
    export OHOS_SDK=/home/ohos/tools/OH_SDK/ohos-sdk/linux
```

## 拷贝编译工具
```
    # 校验压缩包
    sha512sum -c SHA512SUM
    #输出 toolchain.tar.gz: OK
    # 解压拷贝编译工具
    tar -zxvf toolchain.tar.gz
    cp toolchain/* ${OHOS_SDK}/native/llvm/bin
```

## 设置ohos编译宏
由于 ohos_sdk 没有为OpenHarmony提供系统识别的宏，因此我们需要在 ${OHOS_SDK}/native/build/cmake/ohos.toolchain.cmake 文件末尾，添加
```
    add_definitions(-DOHOS_NDK)
```
用于系统识别