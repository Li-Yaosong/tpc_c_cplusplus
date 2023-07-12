#!/bin/bash

# 退出码检查
sure() {
    eval $*
    err=$?
    if [ "$err" != "0" ]
    then
        echo "ERROR during : $*"
        echo "ERROR during : $* $err" > last_error
        exit 1
    fi
}

PKGBUILD_ROOT=$(cd $(dirname ${BASH_SOURCE[0]}); pwd)

# 加载库信息
source ${PWD}/HPKBUILD

# 下载库压缩包
# 参数1 链接地址
# 参数2 压缩包名
download() {
    if [ -s ${PWD}/$2 ]
    then
        echo ${PWD}/$2"，存在"
    else
        curl -f -L -- "$1" > ${PWD}/$2
        return $?
    fi
}

# 库的完整性校验
checksum() {
    sha512sum -c ${PWD}/$1
    ret=$?
    if [ $ret -ne 0 ]
    then
        echo ${PWD}/$1" ERROR!"
        echo "请检查$pkgname SHA512SUM 文件, 并重新下载src压缩包."
        exit $ret
    fi
}

# 解压库
# 参数1 压缩包名
unpack() {
    if [ -f ${PWD}/$1 ]
    then
        if [[ "$1" == *.tar.gz ]]
        then
            echo ${PWD}/$1
            tar -zxvf ${PWD}/$1 > /dev/null
        elif [[ "$1" == *.tgz ]]
        then
            echo ${PWD}/$1
            tar -zxvf ${PWD}/$1 > /dev/null
        
        elif [[ "$1" == *.tar.xz ]]
        then
            echo ${PWD}/$1
            tar -xvJf ${PWD}/$1 > /dev/null
        elif [[ "$1" == *.tar.bz2 ]]
        then
            echo ${PWD}/$1
            tar -xvjf ${PWD}/$1 > /dev/null
        elif [[ "$1" == *.zip ]]
        then
            echo ${PWD}/$1
            unzip ${PWD}/$1 > /dev/null
        else
            echo "ERROR Package Format!"
            exit 2
        fi
    else
        echo "ERROR Package Not Found!"
        exit 2
    fi
}

newdeps=()
builddepends() {
    donelist=($*)
    # 如果有依赖没有编译完，则跳过编译后续再次编译
    deplen=${#depends[*]}
    count=0
    for depend in ${depends[@]}
    do
        doneflag=false
        for donelib in ${donelist[@]}
        do
            if [ $depend == $donelib ]
            then
                count=$((count+1))
                doneflag=true
            fi
        done
        # 记录未编译的依赖项
        # echo $doneflag
        if ! $doneflag
        then
            # echo -----------
            newdeps[${#newdeps[@]}]=$depend
        fi
    done
    if [ $count -ne $deplen ]
    then
        return 101
    fi
    return 0
}

recordbuildlibs() {
    echo $2,$3,$1>> $LYCIUM_ROOT/usr/hpk_build.csv
}

buildargs=
pkgconfigpath=
cmakedependpath() {
    buildargs="-DCMAKE_BUILD_TYPE=Release -DCMAKE_SKIP_RPATH=ON -DCMAKE_SKIP_INSTALL_RPATH=ON -DCMAKE_TOOLCHAIN_FILE=${OHOS_SDK}/native/build/cmake/ohos.toolchain.cmake -DCMAKE_INSTALL_PREFIX=$LYCIUM_ROOT/usr/$pkgname/$1 -G \"Unix Makefiles\" "
    pkgconfigpath=""
    if [ ${#depends[@]} -ne 0 ] 
    then
        tmppath="\""
        for depend in ${depends[@]}
        do
            dependpath=$LYCIUM_ROOT/usr/$depend/$1/
            tmppath=$tmppath"${dependpath};"

            dependpkgpath=$LYCIUM_ROOT/usr/$depend/$1/lib/pkgconfig
            if [ -d ${dependpkgpath} ]
            then
                pkgconfigpath=$pkgconfigpath"${dependpkgpath}:"
            fi
        done
        tmppath=${tmppath%;*}
        pkgconfigpath=${pkgconfigpath%:*}
        tmppath=$tmppath"\""
        buildargs=$buildargs"-DCMAKE_FIND_ROOT_PATH="$tmppath
    fi
}

configuredependpath() {
    pkgconfigpath=""
    buildargs="--prefix=$LYCIUM_ROOT/usr/$pkgname/$1"
    if [ ${#depends[@]} -ne 0 ] 
    then
        for depend in ${depends[@]}
        do
            dependpath=$LYCIUM_ROOT/usr/$depend/$1/lib/pkgconfig
            if [ ! -d ${dependpath} ]
            then
                continue
            fi
            pkgconfigpath=$pkgconfigpath"${dependpath}:"
        done
        pkgconfigpath=${pkgconfigpath%:*}
    fi
}

checkmakedepends() {
    ismakedependready=true
    for makedepend in ${makedepends[@]}
    do
        which $makedepend >/dev/null 2>&1
        if [ $? -ne 0 ]
        then
            echo "请先安装 $makedepend 命令, 才可以编译 $1"
            ismakedependready=false
        else
            echo "$makedepend 已安装"
        fi
    done
    if ! $ismakedependready
    then
        echo "!!! 退出 $1 编译 !!!"
        exit 1
    fi
}

builpackage() {
    donelist=($*)
    builddepends "${donelist[*]}"
    if [ $? -eq 101 ]
    then
        echo $pkgname" not ready. wait "${newdeps[*]}
        for dep in ${newdeps[@]}
        do
            echo $dep >> ${LYCIUM_DEPEND_PKGNAMES}
        done
        exit 101
    fi
    echo "Build $pkgname $pkgver strat!"
    if [ ! $downloadpackage ] || [ $downloadpackage != false ]
    then
        sure download $source $packagename
        if [ -f "SHA512SUM" ]
        then
            sure checksum SHA512SUM
        fi
    fi
    if [ ! $autounpack ] || [ $autounpack != false ]
    then
        sure unpack $packagename
    fi
    
    checkmakedepends $pkgname
    for arch in ${archs[@]}
    do
        # TODO archs1 编译失败，继续编译archs2
        echo "Compileing OpenHarmony $arch $pkgname $pkgver libs..." 
        ARCH=$arch
        sure prepare
        if [ ! $buildtools ] || [ $buildtools == "cmake" ]
        then
            sure cmakedependpath $ARCH
        elif [ $buildtools == "configure" ]
        then
            sure configuredependpath $ARCH
        else
            :
        fi
        sure build $buildargs
        sure package
        if $LYCIUM_BUILD_CHECK
        then
            sure check
        fi
        sure recordbuildlibs $ARCH $pkgname $pkgver
    done
    echo "Build $pkgname $pkgver end!"
}

cleanhpk() {
    sure cleanbuild
}

main() {
    # 清理上次的环境
    sure cleanhpk
    # 编译 PKG
    sure builpackage $*
}

main $*