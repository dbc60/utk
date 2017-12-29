#!/bin/bash
###############################################################################
# (C) Copyright 2015,2017 Douglas B. Cuthbertson
#
# This library is free software; you can redistribute it and/or modify
# it under the terms of the MIT license. See LICENSE for details.
###############################################################################


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

BUILD_ROOT=obj-linux
PLATFORM=x64
BUILD_PATH=$BUILD_ROOT/$PLATFORM/$BUILD_CONFIG

## Ensure the build path exists
[ -d "$BUILD_PATH" ] || mkdir -p "$BUILD_PATH"

PROJECT_PATH=`pwd`
COMMON_COMPILER_FLAGS="-Wall -Werror -Iinclude"

if [ "$BUILD_CONFIG" == "debug" ]; then
    COMPILER_BUILD_FLAGS="-Og -g -DPROJECT_INTERNAL=1 -DPROJECT_SLOW=1"
elif  [ "$BUILD_CONFIG" == "release" ]; then
    COMPILER_BUILD_FLAGS="-Ofast -g -DPROJECT_INTERNAL=0 -DPROJECT_SLOW=0"
fi

COMPILER_FLAGS="$COMMON_COMPILER_FLAGS $COMPILER_BUILD_FLAGS"

## but_driver static library. Need to compile with Position Independent Code
## (PIC) so it can be linked into the test suite shared library as well
## as the test driver.
gcc $COMPILER_FLAGS -c -fpic src/but_driver.c -o $BUILD_PATH/but_driver.o
gcc $COMPILER_FLAGS -c -fpic src/but_version.c -o $BUILD_PATH/but_version.o

## Create a static library from the object files
ar rcs $BUILD_PATH/but_driver.a $BUILD_PATH/but_driver.o \
    $BUILD_PATH/but_version.o

## -ldl will link against libdl to resolve dlopen, dlclose, etc.
## Note that "-ldl" must trail the file names for gcc.
gcc $COMPILER_FLAGS src/linux_but_driver.c src/but_test_driver.c \
    -o $BUILD_PATH/linux_but_driver $LINKER_FLAGS $BUILD_PATH/but_driver.a -ldl

## test_driver shared library to test the but driver
gcc $COMMON_COMPILER_FLAGS $COMPILER_BUILD_FLAGS -fpic -Isrc \
    -c but/but_test.c -o $BUILD_PATH/but_test.o

gcc $COMMON_COMPILER_FLAGS $COMPILER_BUILD_FLAGS -fpic -Isrc \
    -c but/test_driver.c -o $BUILD_PATH/test_driver.o

gcc $COMMON_COMPILER_FLAGS $COMPILER_BUILD_FLAGS -fpic -Isrc \
    -c but/but_test_suite.c -o $BUILD_PATH/but_test_suite.o

## Create a shared library from the object files
gcc -shared -Wl,-soname,$BUILD_PATH/libtest_driver.so.1 \
    -o $BUILD_PATH/libtest_driver.so.1.0 $BUILD_PATH/but_test.o \
    $BUILD_PATH/test_driver.o $BUILD_PATH/but_test_suite.o \
    $BUILD_PATH/but_driver.a
