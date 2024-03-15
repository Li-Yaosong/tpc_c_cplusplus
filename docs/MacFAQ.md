#### LuaJIT编译问题
因为-m32在M1及以上版本的并不支持，所以如果需要编译32位的LuaJIT建议使用Ubuntu编译或者使用更早的Mac进行尝试，如果只需要编译64位可以参考如下修改

```
#文件路径：tpc_c_cplusplus/thirdparty/LuaJIT/HPKBUILD
#去除32位的编译
-archs=("armeabi-v7a" "arm64-v8a")
+archs=("arm64-v8a")

#修改build（）{}函数中的编译参数
-make HOST_CC="$host_gcc" CFLAGS="-fPIC" DYNAMIC_CC=${dynamic_cc} TARGET_LD=${target_ld} STATIC_CC=${static_cc} TARGET_AR="${target_ar}" TARGET_STRIP=${target_strip} -j4 > `pwd`/build.log 2>&1
+make TARGET_SYS=Linux HOST_CC="$host_gcc" CFLAGS="-fPIC" DYNAMIC_CC=${dynamic_cc} TARGET_LD=${target_ld} STATIC_CC=${static_cc} TARGET_AR="${target_ar}" TARGET_STRIP=${target_strip} -j4 > `pwd`/build.log 2>&1
```

以上方式验证平台为Mac M3 Ventura13.5

另有MAC M1平台解决方式可参考

(https://gitee.com/openharmony-sig/tpc_c_cplusplus/issues/I98FHG?from=project-issue)

#### 库中包含有cross-file的情况

此类库出错可以先查看cross-file中的pkg路径和当前设备的pkg是否一致，如果不一致则修改cross-file文件与设备中的pkg路径保持一致，重新编译。

```
#查看pkg路径的命令参考
which pkg-config
```

#### 平台命令差异修改参考



```
#文件路径：tpc_c_cplusplus/thirdparty/xxx/HPKBUILD

#sed -i命令报错可以参考如下修改
-sed -i 
+sed -i.bak 

#cp -arf命令报错可以参考如下修改
-cp -arf
+cp -rf

#cp xxx xxx -rf命令报错可以参考如下修改
#Mac环境下的cp命令可能不支持参数在后的格式
cp xxx xxx -rf
cp -rf xxx xxx
```

#### unable local ICU time报错

安装xcode command

#### libtoolize: command not found报错

Mac环境安装的libtoolize工具可能命名为glibtoolize，添加软链接即可解决，参考命令如下

```
#需要注意自身安装libtoolize的实际路径可以通过which libtoolize查看
ln -s /usr/local/bin/glibtoolize /usr/local/bin/libtoolize
```

 #### glib编译缺失distutil模块

python在3.10 by PEP 632 “Deprecate distutils module”中弃用distutil，需要使用该模块可以使用3.10之前的版本或者安装setuptools工具，它任然提供distutil的支持。

```
#安装命令参考
brew install python-setuptools
```
