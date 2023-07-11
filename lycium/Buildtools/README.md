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

## 设置编译机cmake识别OHOS系统
由于sdk中的cmake版本过低, 导致很多开源库在cmake阶段报错. 这个时候就需要用户在编译机上安装一个高版本的cmake(推荐使用3.26版本). 但是
cmake官方是不支持OHOS的. 解决方案:
```
cp $OHOS_SDK/native/build-tools/cmake/share/cmake-3.16/Modules/Platform/OHOS.cmake xxx(代表你编译机安装的cmake的路径)/cmake-3.26.3-linux-x86_64/share/cmake-3.26/Modules/Platform
```
