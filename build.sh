#!/bin/bash

BUILD_CONFIG=debug
if [ -z "$1" ]; then
    BUILD_CONFIG=debug
    OPT1=empty
else
    OPT1=${1,,}
    if [ "$OPT1" = "debug" ]; then
        BUILD_CONFIG=$OPT1
    elif [ "$OPT1" = "release" ]; then
        BUILD_CONFIG=$OPT1
    fi
fi

BUILD_ROOT=build
ARCHITECTURE=x86_x64
BUILD_PATH=$BUILD_ROOT/$ARCHITECTURE/$BUILD_CONFIG
PROJECT_PATH=`pwd`
COMMON_COMPILER_FLAGS="-Wall -Werror -Iinclude"

if [ "$BUILD_CONFIG" == "debug" ]; then
    COMPILER_BUILD_FLAGS="-Og -g -DPROJECT_INTERNAL=1 -DPROJECT_SLOW=1"
elif  [ "$BUILD_CONFIG" == "release" ]; then
    COMPILER_BUILD_FLAGS="-Ofast -g -DPROJECT_INTERNAL=0 -DPROJECT_SLOW=0"
fi

## but_driver shared library
## Compile the driver library into Position Independent Code (PIC)
gcc $COMMON_COMPILER_FLAGS $COMPILER_BUILD_FLAGS -fpic -c src/but_driver.c \
    -o $BUILD_PATH/but_driver.o
    
gcc $COMMON_COMPILER_FLAGS $COMPILER_BUILD_FLAGS -fpic -c src/but_version.c \
    -o $BUILD_PATH/but_version.o

## Create a shared library from the object files
gcc -shared -o $BUILD_PATH/but_driver.so $BUILD_PATH/but_driver.o \
    $BUILD_PATH/but_version.o


## test_driver shared library to test the but driver
## Compile the driver library into Position Independent Code (PIC)
gcc $COMMON_COMPILER_FLAGS $COMPILER_BUILD_FLAGS -fpic -Isrc \
    -c but/but_basic_unit_test.c -o $BUILD_PATH/but_basic_unit_test.o
    
gcc $COMMON_COMPILER_FLAGS $COMPILER_BUILD_FLAGS -fpic -Isrc \
    -c but/but_test.c -o $BUILD_PATH/but_test.o

gcc $COMMON_COMPILER_FLAGS $COMPILER_BUILD_FLAGS -fpic -Isrc \
    -c but/test_driver.c -o $BUILD_PATH/test_driver.o
