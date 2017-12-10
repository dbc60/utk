@echo off

REM Build the 64-bit version by default
if    "%1" == ""          set Platform=amd64
if /i "%1" == "x86"       set Platform=x86
if /i "%1" == "x86_amd64" set Platform=x86_amd64
if /i "%1" == "x86_x64"   set Platform=x86_amd64
if /i "%1" == "x86_arm"   set Platform=x86_arm
if /i "%1" == "amd64"     set Platform=amd64
if /i "%1" == "amd64_x86" set Platform=amd64_x86
if /i "%1" == "amd64_arm" set Platform=amd64_arm
if /i "%1" == "x64"       set Platform=amd64
if /i "%1" == "x64_x86"   set Platform=amd64_x86
if /i "%1" == "x64_arm"   set Platform=amd64_arm
if /i "%1" == "arm"       set Platform=arm

if "%Platform%" == "" goto :err

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" %Platform%

REM Find the project path. This script assumes it is one directory-level below
REM the project directory.
set HERE=%~dp0

REM Remove the trailing directory separator
IF %HERE:~-1%==\ SET HERE=%PROJECT_PATH:~0,-1%

REM This script is one level below the project directory, so remove the
REM subdirectory from the path.
FOR /f "delims=" %%F in ("%HERE%") do (
  set PROJECT_PATH=%%~dpF
)

REM Remove the trailing directory separator
IF %PROJECT_PATH:~-1%==\ SET PROJECT_PATH=%PROJECT_PATH:~0,-1%

FOR %%f in ("%PROJECT_PATH%") DO SET PROJECT_NAME=%%~nxf
TITLE %PROJECT_NAME% Project: Build %Platform%

set BUILD_PATH=%cd%

goto :EOF

:err
echo Error: "%1" is not a valid platform. Please select amd64 (default), or x86.
