#!/usr/bin/bash

function build_busybox() {
    
    local file=.config

    make defconfig

    sed -i 's/# CONFIG_STATIC is not set/CONFIG_STATIC=y/' $file

    make -j8
    if [ $? -ne 0 ]; then
        echo "build busybox failed!"
        return 1
    fi
    
    cp ./busybox ../

    echo "build busybox success!"

    ## make test   ## for gen test cmd

    return 0
}

function download_busybox() {
    local retry=0

    while [ $retry -le 5 ]
    do
        if [ ! -z "$2" ]; then
            git clone $1 -b $2
        else
            git clone $1
        fi

        if [ $? -eq 0 ]; then
            break;
        fi
    done

    if [ $retry -eq 5 ]; then
        echo "download busybox failed!"
        return 1
    else
        echo "download busybox success!"
        return 0
    fi
}

ERR_MSG="BUILD busybox ERROR!!!!!!!!!!!!!!!!!!!!!"
target_name=busybox
curdir=`pwd`
srcdir=$1
url="https://github.com/mirror/busybox.git"
ver=1_36_0
PATHCH_FILE=`pwd`/../../third_party/busybox/busybox_config.patch
source_dir=$target_name-$ver

echo "curdir=$curdir"
echo "srcdir=$srcdir"

if [ -z "$srcdir" ];then
    echo "must set the param dir!"
    exit 1
fi

cp $PATHCH_FILE $srcdir/

cd $srcdir

if [ ! -d "$source_dir" ]; then
    download_busybox $url $ver
    if [ $? -ne 0 ]; then
        echo $ERR_MSG
        exit 1
    fi

    mv ./$target_name ./$source_dir
    cd $source_dir
    patch -p0 < ../busybox_config.patch
    cd $srcdir
fi

cd $source_dir

build_busybox
if [ $? -ne 0 ]; then
    echo $ERR_MSG
    exit 1
fi

cd $curdir

#eof
