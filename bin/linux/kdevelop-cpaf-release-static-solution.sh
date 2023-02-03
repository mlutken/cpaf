#!/bin/sh

SAVE_DIR=`pwd`
pushd $SAVE_DIR

DIR_NAME=`dirname $0`
ABS_DIR_NAME=`cd $DIR_NAME && pwd`

cd $ABS_DIR_NAME

# In case CPAF_ROOT_DIR and/or CBS_ROOT_DIR are not set we try to find them
if [ "$CPAF_ROOT_DIR" == "" ] ; then
	echo "Finding CPAF_ROOT_DIR ... "
	CPAF_ROOT_DIR=`cd .. && cd .. && pwd`
fi
if [ "$CBS_ROOT_DIR" == "" ] ; then
	echo "Finding CBS_ROOT_DIR ... "
	CBS_ROOT_DIR=`cd .. && cd .. && cd cbs && pwd`
fi

echo CBS_ROOT_DIR : $CBS_ROOT_DIR
echo CPAF_ROOT_DIR: $CPAF_ROOT_DIR

BUILD_DIR="build_cbs/kdev_release_static"
cd ../..
mkdir -p $BUILD_DIR
cd $BUILD_DIR
CBS_ROOT_DIR=$CBS_ROOT_DIR CPAF_ROOT_DIR=$CPAF_ROOT_DIR cmake -D CMAKE_BUILD_TYPE:STRING=Release -D CBS_BUILD=ON -D CBS_LIB_TYPE_TO_BUILD:STRING=STATIC -G "KDevelop3" ../..
popd



#read varName
