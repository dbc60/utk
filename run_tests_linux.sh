## ========================================================================
## (C) Copyright 2015,2017 Douglas B. Cuthbertson
##
## This library is free software; you can redistribute it and/or modify
## it under the terms of the MIT license. See LICENSE for details.
## ========================================================================

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

BUILD_ROOT=build/linux
PLATFORM=x64
BUILD_PATH=$BUILD_ROOT/$PLATFORM/$BUILD_CONFIG

## Ensure the build path exists
[ -d "$BUILD_PATH" ] || mkdir -p "$BUILD_PATH"

## Run the test suites
echo
echo Executing $BUILD_PATH/but_driver $BUILD_PATH/libtest_but_driver.so
$BUILD_PATH/but_driver $BUILD_PATH/libtest_but_driver.so

echo
echo Executing $BUILD_PATH/but_driver $BUILD_PATH/libtest_ehm.so
$BUILD_PATH/but_driver $BUILD_PATH/libtest_ehm.so

echo
echo Executing $BUILD_PATH/but_driver $BUILD_PATH/libtest_ute_counter.so
$BUILD_PATH/but_driver $BUILD_PATH/libtest_ute_counter.so

echo
echo Executing $BUILD_PATH/but_driver $BUILD_PATH/libtest_ute_driver.so
$BUILD_PATH/but_driver $BUILD_PATH/libtest_ute_driver.so

echo
echo Executing $BUILD_PATH/but_driver $BUILD_PATH/libtest_mutex.so
$BUILD_PATH/but_driver $BUILD_PATH/libtest_mutex.so

echo
echo Executing $BUILD_PATH/but_driver $BUILD_PATH/libtest_dlist.so
$BUILD_PATH/but_driver $BUILD_PATH/libtest_dlist.so
