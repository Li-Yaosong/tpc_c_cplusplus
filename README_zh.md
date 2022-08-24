# tpc_c_cplusplus

#### 简介
本仓库主要用于存放已经适配OpenHarmony的C/C++三方库的适配脚本，三方库的适配/使用指导；每个三方库目录下包含了GN化的适配指导、编译脚本和包含了cmake的适配指导、编译脚本。



#### 目录

```
tpc_c_cplusplus
├── README_zh.md
├── common           #通用说明文档的文件夹
├── 三方库1
│   ├──gn            #gn构建脚本、适配指导的文件夹
│   ├──cmake         #cmake构建脚本、适配指导的文件夹
│   ├──README_zh.md  #注意readme中要表明三方库的license
├── 三方库2
│   ├──gn            #gn构建脚本、适配指导的文件夹
│   ├──cmake         #cmake构建脚本、适配指导的文件夹
│   ├──README_zh.md  #注意readme中要表明三方库的license
......
```



#### 如何贡献

- 为了更好的维护仓库，提交的PR请保持仓库的目录结构，不符合仓库目录结构的PR可能不被合入
- [遵守OpenHarmony编码贡献规范](https://gitee.com/openharmony-sig/knowledge_demo_smart_home/blob/master/dev/docs/contribute/README.md)



#### 三方库列表

工具类

- [xerces-c](xerces-c)

