@echo off

echo ------
echo ------

set wildcard=*.h *.cpp *.inl *.c

echo STATICS FOUND:
findstr -s -n -i -l /C:"static " %wildcard%

echo ------
echo ------

echo.
echo GLOBALS FOUND:
findstr -s -n -i -l "LOCAL_VARIABLE" %wildcard%
findstr -s -n -i -l "GLOBAL_VARIABLE" %wildcard%

echo ------
echo ------

echo.
echo LOCAL FUNCTIONS FOUND:
findstr -s -n -i -l "LOCAL_VARIABLE" %wildcard%

echo ------
echo ------
