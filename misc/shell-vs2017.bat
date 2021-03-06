:: ========================================================================
:: (C) Copyright 2015,2017 Douglas B. Cuthbertson
::
:: This library is free software; you can redistribute it and/or modify
:: it under the terms of the MIT license. See LICENSE for details.
:: ========================================================================


@echo off

:: Set the PROJECT_PATH, and PROJECT_NAME. Run this script from anywhere.

:: Build the 64-bit version by default
if    "%1" == ""        set Architecture=amd64
if /i "%1" == "x86"     set Architecture=x86
if /i "%1" == "amd64"   set Architecture=amd64
if /i "%1" == "x64"     set Architecture=amd64
if /i "%1" == "arm"     set Architecture=arm
if /i "%1" == "arm64"   set Architecture=arm64
if "%Architecture%" == "" goto :errArch

if    "%2" == ""        set HostArchitecture=amd64
if /i "%2" == "x86"     set HostArchitecture=x86
if /i "%2" == "amd64"   set HostArchitecture=amd64
if "%HostArchtecture" == "" goto errHostArch

:: get the command-line options for the script to set environment variables
:: call "E:\local\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat" -?

call "E:\local\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat" -arch=%Architecture% -host_arch=%HostArchitecture% -no_logo

:: verify the environment variables are set properly
:: call "E:\local\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat" -arch=%Architecture% -host_arch=%HostArchitecture% -no_logo -test

:: Find the project path. This script assumes it is one directory-level below
:: the project directory.
set HERE=%~dp0

:: Remove the trailing directory separator
IF %HERE:~-1%==\ SET PROJECT_PATH=%HERE:~0,-1%

:: This script is one level below the project directory, so remove the
:: subdirectory from the path.
FOR /f "delims=" %%F in ("%PROJECT_PATH%") do (
  set PROJECT_PATH=%%~dpF
)

:: Remove the trailing directory separator
IF %PROJECT_PATH:~-1%==\ SET PROJECT_PATH=%PROJECT_PATH:~0,-1%

FOR %%f in ("%PROJECT_PATH%") DO SET PROJECT_NAME=%%~nxf
TITLE %PROJECT_NAME% Project: Build %Architecture%
goto :EOF

:errArch
echo Error: "%1" is not a valid platform architecture. Please select amd64 (default), x86, arm64, or arm.
goto :EOF

:errHostArch
echo Error: "%2" is not a valid host architecture. Please select amd64 (default), or x86.
goto :EOF
