#!/bin/bash

unames=`uname -s`
osname=${unames:0:5}

# 根目录
LYCIUM_ROOT=
buildcheckflag=true
if [ "$osname" == "Linux" ]
then
    echo "Build OS linux"
    LYCIUM_ROOT=`pwd`
elif [ "$osname" == "CYGWI" ] # CYGWIN
then
    echo "Build OS CYGWIN"
    lyciumroot=`cygpath -w $PWD`
    LYCIUM_ROOT=${lyciumroot//\\/\/}
    buildcheckflag=false
else
    echo "System cannot recognize, exiting"
    exit 0
fi
export LYCIUM_BUILD_CHECK=$buildcheckflag
export LYCIUM_BUILD_OS=$osname
export LYCIUM_ROOT=$LYCIUM_ROOT
if [ -z ${OHOS_SDK} ]
then
    echo "OHOS_SDK 未设置, 请先下载安装ohos SDK, 并设置OHOS_SDK环境变量. "
    exit 1
fi
echo "OHOS_SDK="${OHOS_SDK}
CLANG_VERSION_STR=$(echo __clang_version__ | $OHOS_SDK/native/llvm/bin/clang -E -xc - | tail -n 1)
CLANG_VERSION_ARR=(${CLANG_VERSION_STR//,/ })
CLANG_VERSION_1=${CLANG_VERSION_ARR[0]}
CLANG_VERSION=${CLANG_VERSION_1: 1}
echo "CLANG_VERSION="${CLANG_VERSION}
export CLANG_VERSION=${CLANG_VERSION}
jobFlag=true

checkbuildenv(){
    which gcc >/dev/null 2>&1
    if [ $? -ne 0 ]
    then
        echo "gcc 命令未安装, 请先安装 gcc 命令"
        exit 1
    fi
    echo "gcc 命令已安装"
    which cmake >/dev/null 2>&1
    if [ $? -ne 0 ]
    then
        echo "cmake 命令未安装, 请先安装 cmake 命令"
        exit 1
    fi
    echo "cmake 命令已安装"
    which make >/dev/null 2>&1
    if [ $? -ne 0 ]
    then
        echo "make 命令未安装. 请先安装 make 命令"
        exit 1
    fi
    echo "make 命令已安装"
    which pkg-config >/dev/null 2>&1
    if [ $? -ne 0 ]
    then
        echo "pkg-config 命令未安装, 请先安装 pkg-config 命令"
        exit 1
    fi
    echo "pkg-config 命令已安装"
    which autoreconf >/dev/null 2>&1
    if [ $? -ne 0 ]
    then
        echo "autoreconf 命令未安装, 请先安装 autoreconf 命令"
        exit 1
    fi
    echo "autoreconf 命令已安装"
    which patch >/dev/null 2>&1
    if [ $? -ne 0 ]
    then
        echo "patch 命令未安装, 请先安装 patch 命令"
        exit 1
    fi
    echo "patch 命令已安装"
    if [ ! -d `pwd`/usr ]
    then
        echo "创建 `pwd`/usr 目录"
        mkdir -p `pwd`/usr
    fi

}

hpkPaths=()
donelist=()
donelibs=()
readdonelibs() {
    if [ -f $1 ]
    then
        count=0
        while read line
        do
            doneflags=false
            libinfos=(${line//,/ })
            libname=${libinfos[0]}
            for lib in ${donelibs[@]}
            do
                if [ $lib == $libname ]
                then
                    doneflags=true
                fi
            done
            if ! $doneflags
            then
                donelibs[$count]=$libname
                count=$((count+1))
            fi
            
        done < $1
    fi
    donelist=(${donelibs[@]})
}

makelibsdir(){
    jobs=($*)
    for job in ${jobs[@]}
    do
        doneflags=false
        for donelib in ${donelibs[@]}
        do
            if [ $donelib == $job ]
            then
                doneflags=true
            fi
        done
        if $doneflags
        then
            continue
        fi
        tmppath=`pwd`/main/$job
        if [ -d $tmppath ]
        then
            hpkPaths[${#hpkPaths[@]}]=$tmppath
        fi
    done
}

# 找到main目录下的所有目录
# 参数1 为项目根路径
findmainhpkdir(){
    # hpkPaths=`find $1 -maxdepth 1 -type d`
    # # echo $hpkPaths
    # # remove root dir
    # hpkPaths=(${hpkPaths[*]/$1})

    tmplibs=()
    for file in $(ls $1)
    do
        if [ -d $1/$file ]
        then
            tmplibs[${#tmplibs[@]}]=$file
        fi
    done
    makelibsdir ${tmplibs[@]}
}

# 进入每一个目录 将script 目录下的脚本都链接过去
prepareshell(){
    for hpkdir in ${hpkPaths[@]}
    do
        cd $hpkdir
        ln -fs `pwd`/../../script/build_hpk.sh build_hpk.sh
        ln -fs `pwd`/../../script/envset.sh envset.sh
        cd ${OLDPWD}
    done
}

# 恢复脚本
cleanhpkdir(){
    for hpkdir in ${hpkPaths[@]}
    do
        cd $hpkdir
        rm build_hpk.sh envset.sh
        cd ${OLDPWD}
    done
}

# 编译库本身
nextroundlist=()
notdonelist=()
buildhpk(){
    nextroundlist=(${hpkPaths[*]})

    while $jobFlag
    do
        lastroundlen=${#nextroundlist[*]}
        notdonelist=(${nextroundlist[*]})
        nextroundlist=()
        len=${#notdonelist[*]}
        for ((i=0; i < $len; i=i+1))
        do
            cd ${notdonelist[$i]}
            bash ${PWD}/build_hpk.sh "${donelist[*]}" # > blackhole.log 2>&1  #入参已经完成的list
            res=$?
            if [ $res -eq 0 ]
            then
                donelist[${#donelist[@]}]=${notdonelist[$i]##*/}
            elif [ $res -eq 101 ]
            then
                nextroundlist[${#nextroundlist[@]}]=${notdonelist[$i]}
            else
                echo "${notdonelist[$i]} build ERROR"
            fi
            cd ${OLDPWD}
        done
        if [ ${#nextroundlist[*]} -eq 0 ]
        then
            echo "ALL JOBS DONE!!!"
            break
        fi
        if [ $lastroundlen -eq ${#nextroundlist[*]} ]
        then
            echo "Please check the dependencies of these items:"
            echo " "${nextroundlist[*]}
            jobFlag=false
        fi
    done
}

main(){
    checkbuildenv

    readdonelibs "`pwd`/usr/hpk_build.csv"

    if [ $# -ne 0 ]
    then
        makelibsdir $*
    else
        findmainhpkdir ${PWD}/main
        # exit 2
    fi

    prepareshell
    buildhpk

    cleanhpkdir
    unset LYCIUM_BUILD_OS LYCIUM_ROOT LYCIUM_BUILD_CHECK CLANG_VERSION
}

main $*
