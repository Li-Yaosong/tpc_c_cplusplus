#!/bin/sh

TOP_DIR=`pwd`

SRC_DIR=$1
echo "SRC_DIR:"$SRC_DIR
if [ -z "$SRC_DIR" ]; then
echo "must set the param dir!"
exit 1
fi

cd $SRC_DIR

ICONV=libiconv-1.17
TEST_DIR=${SRC_DIR}/${ICONV}/srclib/

cp Makefile ${TEST_DIR}

cd ${TEST_DIR}
make
if [ $? -ne 0 ]; then
echo "generate test config file failed!"
else
echo "generate test config file success!!"
fi

cd ${TOP_DIR}

#EOF

