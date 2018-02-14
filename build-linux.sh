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

BUILD_ROOT=obj/linux
PLATFORM=x64
BUILD_PATH=$BUILD_ROOT/$PLATFORM/$BUILD_CONFIG

## Ensure the build path exists
[ -d "$BUILD_PATH" ] || mkdir -p "$BUILD_PATH"

COMMON_COMPILER_FLAGS="-Wall -Werror -Iinclude"

if [ "$BUILD_CONFIG" == "debug" ]; then
    COMPILER_BUILD_FLAGS="-Og -g -DPROJECT_INTERNAL=1 -DPROJECT_SLOW=1"
elif  [ "$BUILD_CONFIG" == "release" ]; then
    COMPILER_BUILD_FLAGS="-Ofast -g -DPROJECT_INTERNAL=0 -DPROJECT_SLOW=0"
fi

COMPILER_FLAGS="$COMMON_COMPILER_FLAGS $COMPILER_BUILD_FLAGS"


##
## but_driver.a
##

## Build the static library for the BUT driver: but_driver.a.
## Need to compile with Position Independent Code (PIC) so it can be linked
## into the test suite shared library as well as the test driver.
echo Building static library but_driver.a
gcc $COMPILER_FLAGS -c -fpic src/but_driver.c -o $BUILD_PATH/but_driver.o
gcc $COMPILER_FLAGS -c -fpic src/but_version.c -o $BUILD_PATH/but_version.o

## Create a static library from the object files
ar rcs $BUILD_PATH/but_driver.a $BUILD_PATH/but_driver.o \
    $BUILD_PATH/but_version.o


##
## ute_driver.a
##

## Build the static library for the UTE driver: ute_driver.a
echo Building static library ute_driver.a
gcc $COMPILER_FLAGS -c -fpic src/ute_driver.c -o $BUILD_PATH/ute_driver.o
gcc $COMPILER_FLAGS -c -fpic src/ute_version.c -o $BUILD_PATH/ute_version.o
gcc $COMPILER_FLAGS -c -fpic src/ute_counter.c -o $BUILD_PATH/ute_counter.o
ar rcs $BUILD_PATH/ute_driver.a $BUILD_PATH/ute_driver.o \
   $BUILD_PATH/ute_version.o  $BUILD_PATH/ute_counter.o


##
## Exception Handling Module (EHM)
##

## We build EHM as a shared library so there's only one instance of ehm_stack.
## libehm.so is the shared and import library
##
echo Building shared lib libehm.so
gcc $COMPILER_FLAGS -c -fpic -Isrc src/linux_ehm.c -o $BUILD_PATH/linux_ehm.o
gcc $COMPILER_FLAGS -c -fpic src/ehm.c -o $BUILD_PATH/ehm.o
gcc $COMPILER_FLAGS -c -fpic src/ehm_assert.c -o $BUILD_PATH/ehm_assert.o
gcc -shared -Wl,-soname,$BUILD_PATH/libehm.so -o $BUILD_PATH/libehm.so \
    $BUILD_PATH/linux_ehm.o $BUILD_PATH/ehm.o $BUILD_PATH/ehm_assert.o

##
## but_driver application
##

## -ldl will link against libdl to resolve dlopen, dlclose, etc.
## Note that "-l<library>" must trail the file names for gcc.
echo Building executable but_driver
gcc $COMPILER_FLAGS src/but_driver_linux.c src/but_test_driver.c \
    -o $BUILD_PATH/but_driver $LINKER_FLAGS $BUILD_PATH/but_driver.a \
    -L$BUILD_PATH -lehm -ldl

##
## libtest_but_driver.so test suite
##

## Compile the components of libtest_but_driver.so
echo Building the test suite libtest_but_driver.so
gcc $COMPILER_FLAGS -c -fpic -Isrc test/test_but_driver.c -o \
    $BUILD_PATH/test_but_driver.o

## Link the components and libraries to create the  test suite
gcc -shared -Wl,-soname,$BUILD_PATH/libtest_but_driver.so.1 \
    -o $BUILD_PATH/libtest_but_driver.so $BUILD_PATH/test_but_driver.o \
    $BUILD_PATH/but_driver.a


##
## libtest_ehm.so.1 test suite
##

## compile the components of test_ehm.so that tests libehm.so.1
echo Building components of libtest_ehm.so
gcc $COMPILER_FLAGS -c -fpic -Isrc test/test_ehm.c -o $BUILD_PATH/test_ehm.o

## build libtest_ehm.so - the unit test for ehm.a
echo building shared library libtest_ehm.so
gcc -shared -Wl,-soname,$BUILD_PATH/libtest_ehm.so.1 \
    -o $BUILD_PATH/libtest_ehm.so $BUILD_PATH/test_ehm.o \
    -L$BUILD_PATH -lehm


##
## libtest_ute_driver.so test suite
##

## compile the components of test_ute_driver.so that tests ute_driver.a
echo Building components of shared library libtest_ute_driver.so
gcc $COMPILER_FLAGS -c -fpic -Isrc test/test_ute_driver.c -o \
    $BUILD_PATH/test_ute_driver.o

## build libtest_ute_driver.so - the unit test for ute_driver.a
echo Building shared library libtest_ute_driver.so
gcc -shared -Wl,-soname,$BUILD_PATH/libtest_ute_driver.so.1 \
    -o $BUILD_PATH/libtest_ute_driver.so $BUILD_PATH/test_ute_driver.o \
    $BUILD_PATH/ute_driver.a -L$BUILD_PATH -lehm


##
## libtest_ute_counter.so test suite
##

## compile the components of libtest_ute_counter.so
echo Building components of libtest_ute_counter.so
gcc $COMPILER_FLAGS -c -fpic -Isrc test/test_ute_counter.c -o \
    $BUILD_PATH/test_ute_counter.o

## build libtest_ute_counter.so - the unit tests for ute_counter.o
gcc -shared -Wl,-soname,$BUILD_PATH/libtest_ute_counter.so \
    -o $BUILD_PATH/libtest_ute_counter.so $BUILD_PATH/test_ute_counter.o \
    $BUILD_PATH/ute_counter.o $BUILD_PATH/ute_driver.a -L$BUILD_PATH -lehm


##
## libtest_mutex.so test suite
##

## compile the components of libtest_mutex.so
echo Building components of libtest_mutex.so
gcc $COMPILER_FLAGS -c -fpic -Isrc test/test_utk_mutex.c -o \
    $BUILD_PATH/test_utk_mutex.o

## build libtest_mutex.so - the unit tests for ute_counter.o
gcc -shared -Wl,-soname,$BUILD_PATH/libtest_mutex.so \
    -o $BUILD_PATH/libtest_mutex.so $BUILD_PATH/test_utk_mutex.o -lpthread


##
## libtest_dlist.so test suite
##

## compile the components of libtest_dlist.so
echo Building components of libtest_dlist.so
gcc $COMPILER_FLAGS -c -fpic -Isrc test/test_dlist.c -o \
    $BUILD_PATH/test_dlist.o

## build libtest_dlist.so
gcc -shared -Wl,-soname,$BUILD_PATH/libtest_dlist.so \
    -o $BUILD_PATH/libtest_dlist.so $BUILD_PATH/test_dlist.o


echo build complete
