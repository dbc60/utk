:: ========================================================================
:: (C) Copyright 2015,2017 Douglas B. Cuthbertson
::
:: This library is free software; you can redistribute it and/or modify
:: it under the terms of the MIT license. See LICENSE for details.
:: ========================================================================

@echo off

set BUILD_CONFIG=Debug
if    "%1" == ""          set BUILD_CONFIG=Debug
if /i "%1" == "Debug"     set BUILD_CONFIG=Debug
if /i "%1" == "Release"   set BUILD_CONFIG=Release

if    "%2" == ""        set Architecture=amd64
if /i "%2" == "x86"     set Architecture=x86
if /i "%2" == "amd64"   set Architecture=amd64
if /i "%2" == "x64"     set Architecture=amd64
if /i "%2" == "arm"     set Architecture=arm
if /i "%2" == "arm64"   set Architecture=arm64
if "%Architecture%" == "" goto :errArch

set BUILD_ROOT=build\win

:: Architecture is set by the shell-vs*.bat scripts in misc\
if "%Architecture%" == "amd64" (
    set PLATFORM=x64
)

if "%Architecture%" == "x86" (
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

echo.
echo Executing %BUILD_PATH%\win32_but_driver.exe %BUILD_PATH%\test_rgn.dll
%BUILD_PATH%\win32_but_driver.exe %BUILD_PATH%\test_rgn.dll

echo.
echo Executing %BUILD_PATH%\win32_but_driver.exe %BUILD_PATH%\test_dlist.dll
%BUILD_PATH%\win32_but_driver.exe %BUILD_PATH%\test_dlist.dll

goto :EOF

:errArch
echo Error: "%2" is not a valid platform architecture. Please select amd64 (default), x86, arm64, or arm.
goto :EOF
