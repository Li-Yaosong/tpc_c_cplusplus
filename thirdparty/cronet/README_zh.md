# cronet三方库说明

## 功能简介

cronet是chromium,项目的网络子模块。承接了chromium网络通信相关的能力，Cronet 原生支持 HTTP、HTTP/2 和 HTTP/3 over QUIC 协议。该库支持您为请求设置优先级标签。服务器可以使用优先级标记来确定处理请求的顺序。Cronet 可以使用内存缓存或磁盘缓存来存储网络请求中检索到的资源。后续请求会自动从缓存中传送。默认情况下，使用 Cronet 库发出的网络请求是异步的。在等待请求返回时，您的工作器线程不会被阻塞。Cronet 支持使用 Brotli 压缩数据格式进行数据压缩。

## 使用约束

- IDE版本：DevEco Studio 4.1.3.300
- SDK版本：apiVersion: 11, version: 4.1.3.5
- 三方库版本：107.0.5304.150
- 当前适配的功能：支持cronet http/https 通信能力

## 使用方式

由于cronet属于chromium的一部分，因此编译构建 cronet，需要先[下载chromium源码](https://chromium.googlesource.com/chromium/src/+/main/docs/linux/build_instructions.md)。
由于我们提供的移植cronet的patch是基于，chromium TAG 107.0.5304.150 因此获取源码后需要将代码, 切换到107.0.5304.150 tag点。

```bash
git checkout 107.0.5304.150
```

随后将我们的patch打入源码中,

```
git apply --check cronet_TAG_107.0.5304.150_oh_pkg.patch # 检查patch是否可用
# 如果可用打入patch，如果不可用确认下chromium分支是否切换ok
git apply cronet_TAG_107.0.5304.150_oh_pkg.patch
```

进入src目录,执行:

```bash
bash build.sh # 等待编译结果。
```

编译结束后可在out/cronet目录下获取libcronet.107.0.5304.150.so