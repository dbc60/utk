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

BUILD_ROOT=obj/linux
PLATFORM=x64
BUILD_PATH=$BUILD_ROOT/$PLATFORM/$BUILD_CONFIG

## Ensure the build path exists
[ -d "$BUILD_PATH" ] || mkdir -p "$BUILD_PATH"

## Run the test suites
echo
echo Executing $BUILD_PATH/linux_but_driver $BUILD_PATH/libtest_but_driver.so.1.0
$BUILD_PATH/linux_but_driver $BUILD_PATH/libtest_but_driver.so.1.0

echo
echo Executing $BUILD_PATH/linux_but_driver $BUILD_PATH/libtest_ehm.so.1.0
$BUILD_PATH/linux_but_driver $BUILD_PATH/libtest_ehm.so.1.0

echo
echo Executing $BUILD_PATH/linux_but_driver $BUILD_PATH/libtest_ute_counter.so.1.0
$BUILD_PATH/linux_but_driver $BUILD_PATH/libtest_ute_counter.so.1.0

echo
echo Executing $BUILD_PATH/linux_but_driver $BUILD_PATH/libtest_ute_driver.so.1.0
$BUILD_PATH/linux_but_driver $BUILD_PATH/libtest_ute_driver.so.1.0
