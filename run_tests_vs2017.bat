:: ========================================================================
:: (C) Copyright 2015,2017 Douglas B. Cuthbertson
::
:: This library is free software; you can redistribute it and/or modify
:: it under the terms of the MIT license. See LICENSE for details.
:: ========================================================================

@echo off

:: Ensure the environment has bee set
if "%PROJECT_PATH%" == "" goto errProjectPath

set BUILD_CONFIG=Debug
if    "%1" == ""          set BUILD_CONFIG=Debug
if /i "%1" == "Debug"     set BUILD_CONFIG=Debug
if /i "%1" == "Release"   set BUILD_CONFIG=Release

set BUILD_ROOT=obj\vs2017

if "%BUILD_CONFIG%" == "Debug" (
    set COMPILER_BUILD_FLAGS=/Od /D PROJECT_INTERNAL=1 ^
    /D PROJECT_SLOW=1 /MTd
)

if "%BUILD_CONFIG%" =="Release" (
    set COMPILER_BUILD_FLAGS=/Ox /D PROJECT_INTERNAL=0 /D PROJECT_SLOW=0 /MT
)

:: Architecture is set by the shell-vs*.bat scripts in misc\
if "%Architecture%" == "amd64" (
    set MACHINE_FLAG=/MACHINE:X64
    set PLATFORM=x64
)

if "%Architecture%" == "x86" (
    set MACHINE_FLAG=/MACHINE:X86
    set PLATFORM=x86
)

set BUILD_PATH=%BUILD_ROOT%\%PLATFORM%\%BUILD_CONFIG%

:: Run the test suites
echo.
echo Executing %BUILD_PATH%\win32_but_driver.exe %BUILD_PATH%\test_but_driver.dll
%BUILD_PATH%\win32_but_driver.exe %BUILD_PATH%\test_but_driver.dll

echo.
echo Executing %BUILD_PATH%\win32_but_driver.exe %BUILD_PATH%\test_ehm.dll
%BUILD_PATH%\win32_but_driver.exe %BUILD_PATH%\test_ehm.dll

echo.
echo Executing %BUILD_PATH%\win32_but_driver.exe %BUILD_PATH%\test_ute_counter.dll
%BUILD_PATH%\win32_but_driver.exe %BUILD_PATH%\test_ute_counter.dll

echo.
echo Executing %BUILD_PATH%\win32_but_driver.exe %BUILD_PATH%\test_ute_driver.dll
%BUILD_PATH%\win32_but_driver.exe %BUILD_PATH%\test_ute_driver.dll

echo.
echo Executing %BUILD_PATH%\win32_but_driver.exe %BUILD_PATH%\test_mutex.dll
%BUILD_PATH%\win32_but_driver.exe %BUILD_PATH%\test_mutex.dll

goto :EOF

:errProjectPath
echo Error: The build environment is not set. Run either 'misc\shell-vs2015.bat' or 'misc\shell-vs2017.bat'?
