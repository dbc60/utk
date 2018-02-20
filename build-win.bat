:: ========================================================================
:: (C) Copyright 2015,2017 Douglas B. Cuthbertson
::
:: This library is free software; you can redistribute it and/or modify
:: it under the terms of the MIT license. See LICENSE for details.
:: ========================================================================

@echo off

:: Added _UNICODE and UNICODE so I can use Unicode strings in structs and
:: whatnot.
:: Need user32.lib if you link MessageBox()
:: Need gdi32.lib to link PatBlt()
:: /FC to get full path names in diagnostics. It's helpful when using Emacs to
:: code and launch the debugger and executable

:: /GS- turn off security checks because that compile-time option relies on
:: the C runtime library, which we are not using.

:: /Gs[size] The number of bytes that local variables can occupy before
:: a stack probe is initiated. If the /Gs option is specified without a
:: size argument, it is the same as specifying /Gs0

:: /Gm- disable minimal rebuild. We want to build everything. It won't
:: take long.

:: /GR- disable C++ RTTI. We don't need runtime type information.

:: /EHsc enable C++ EH (no SEH exceptions) (/EHs),
:: and  extern "C" defaults to nothrow (/EHc)

:: /EHa- disable C++ Exception Handling, so we don't have stack unwind code.

:: /W3 set warning level 3.
:: /W4 set warning level 4. It's better
:: /WX warnings are errors
:: /wd turns off a particular warning
::   /wd4201 - nonstandard extension used : nameless struct/union
::   /wd4100 - 'identifier' : unreferenced formal parameter (this happens a lot while developing code)
::   /wd4189 - 'identifier' : local variable is initialized but not referenced
::   /wd4127 - conditional expression is constant ("do {...} while (0)" in macros)

:: /FC use full pathnames in diagnostics

:: /Od - disable optimizations. The debug mode is good for development

:: /Oi Generate intrinsic functions. Replaces some function calls with
:: intrinsic or otherwise special forms of the function that help your
:: application run faster.

:: /GL whole program optimization. Use the /LTCG linker option to create the
:: output file. /ZI cannot be used with /GL.

:: /I<dir> add to include search path

:: /Fe:<file> name executable file

:: /D<name>{=|#}<text> define macro

:: /Zi enable debugging information
:: /Z7 enable debugging information

:: /link [linker options and libraries] The linker options are
:: documented here: https://msdn.microsoft.com/en-us/library/y0zzbyt4.aspx

:: /nodefaultlib t

:: Note that subsystem version number 5.1 only works with 32-bit builds.
:: The minimum subsystem version number for 64-bit buils is 5.2.
:: /subsystem:windows,5.1 - enable compatibility with Windows XP (5.1)

:: /LTCG link time code generation

:: /STACK:reserve[,commit] stack allocations. The /STACK option sets the size
:: of the stack in bytes. Use this option only when you build an .exe file.

:: DEFINITIONS
::   _UNICODE - 16-bit wide characters
::   UNICODE  - 16-bit wide characters
::   PROJECT_WIN32     - 0 or not defiend = building for other than Windows
::                     - 1 = building for Windows
::   PROJECT_INTERNAL  - 0 = build for public release
::                     - 1 = build for developers only
::   PROJECT_SLOW      - 0 = No slow code (like assertion checks) allowed!,
::                       1 = Slow code welcome
::   __ISO_C_VISIBLE   - the version of C we are targeting for the math library.
::                       1995 = C95, 1999 = C99, 2011 = C11.
:: Optimization switches /O2 /Oi /fp:fast

:: Ensure the environment has bee set
if "%PROJECT_PATH%" == "" goto errProjectPath

set BUILD_CONFIG=Debug
if    "%1" == ""          set BUILD_CONFIG=Debug
if /i "%1" == "Debug"     set BUILD_CONFIG=Debug
if /i "%1" == "Release"   set BUILD_CONFIG=Release

set BUILD_ROOT=build\win

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

:: C4456: "declaration of 'identifier' hides previous local declaration". This
:: happens when exceptions from the EHM module are nested.
set SUPPRESSED_COMPILER_WARNINGS=/wd4456
set COMMON_COMPILER_FLAGS=/nologo /Zc:wchar_t /fp:fast /Gm- /GR- /GS /EHa- ^
    /WX /W4 %SUPPRESSED_COMPILER_WARNINGS% /Zc:inline /FC /Z7 /Oi /D _UNICODE ^
    /D UNICODE /D PROJECT_WIN32=1 /Iinclude /Fa%BUILD_PATH%\ /Fo%BUILD_PATH%\

set COMPILER_FLAGS=%COMMON_COMPILER_FLAGS% %COMPILER_BUILD_FLAGS%

set LINKER_FLAGS=/nologo /incremental:no /opt:ref kernel32.lib user32.lib gdi32.lib winmm.lib


if /i "%1" == "clean" (
    call :DoClean
    goto :EOF
)
if /i "%2" == "clean" (
    call :DoClean
    goto :EOF
)

if /i "%1" == "cleanall" (
    call :DoCleanAll
    goto :EOF
)
if /i "%2" == "cleanall" (
    call :DoCleanAll
    goto :EOF
)

if /i "%1" == "rebuild" (
    call :DoClean
    call :BuildAll
    goto :EOF
)
if /i "%2" == "rebuild" (
    call :DoClean
    call :BuildAll
    goto :EOF
)

:: Build all the components
CALL :BuildAll
goto :EOF

::--------------------------------------------------------
::-- Function section starts below here
::--------------------------------------------------------

:BuildAll
:: clean up the symbol files from the previous build so we will have fresh
:: ones for live-load debugging in VS
IF NOT EXIST "%BUILD_PATH%" md "%BUILD_PATH%"

del /q "%BUILD_PATH%"\*.pdb >nul 2>&1


::
:: but_driver.lib
::

:: build the static library for the BUT driver: but_driver.lib
echo ** Building static library but_driver.lib
cl %COMPILER_FLAGS% /c /Fp%BUILD_PATH%\but_driver.pch ^
   /Fd%BUILD_PATH%\but_driver.pdb src\but_driver.c src\but_version.c

lib /OUT:"%BUILD_PATH%\but_driver.lib" %MACHINE_FLAG% /NOLOGO ^
    %BUILD_PATH%\but_driver.obj %BUILD_PATH%\but_version.obj
echo.


::
:: ute_driver.lib
::

:: build the static library for the UTE driver: ute_driver.lib
echo ** Building static library ute_driver.lib
cl %COMPILER_FLAGS% /c /Fp%BUILD_PATH%\ute_driver.pch ^
   /Fd%BUILD_PATH%\ute_driver.pdb src\ute_driver.c src\ute_version.c ^
   src\ute_counter.c

lib /OUT:"%BUILD_PATH%\ute_driver.lib" %MACHINE_FLAG% /NOLOGO ^
    %BUILD_PATH%\ute_driver.obj %BUILD_PATH%\ute_version.obj ^
    %BUILD_PATH%\ute_counter.obj
echo.


::
:: Exception Handling Module (EHM)
::

:: We build EHM as a shared library so there's only one instance of ehm_stack.
:: Building ehm.dll and ehm.lib (the import link-library)
::
echo ** Building shared library ehm.dll
cl %COMPILER_FLAGS% /D PROJECTLIBRARY_EXPORTS /c /Fp%BUILD_PATH%\ehm.pch ^
   /Fd%BUILD_PATH%\ehm.pdb src\win32_ehm.c src\ehm.c

link %LINKER_FLAGS% /DLL %MACHINE_FLAG% /OUT:"%BUILD_PATH%\ehm.dll" ^
     /PDB:%BUILD_PATH%\win32_ehm.pdb "%BUILD_PATH%\win32_ehm.obj" ^
     "%BUILD_PATH%\ehm.obj"
echo.


::
:: but_driver.exe
::

:: build but_driver.exe from but_driver_win32.c, but_test_driver.c,
:: but_driver.lib, and ehm.lib
echo ** Building application but_driver.exe
cl %COMPILER_FLAGS% "src\but_driver_win32.c" ^
   "src\but_test_driver.c" /Fe%BUILD_PATH%\but_driver.exe ^
   /Fm%BUILD_PATH%\but_driver.map  ^
   /link %LINKER_FLAGS% %BUILD_PATH%\but_driver.lib %BUILD_PATH%\ehm.lib
echo.


::
:: test_but_driver.dll test suite
::

:: compile the components of test_but_driver.dll
echo ** Building shared test library test_but_driver.dll
cl %COMPILER_FLAGS% /c /Isrc /D _LIB /D PROJECTLIBRARY_EXPORTS ^
   /Fp%BUILD_PATH%\test_but_driver.pch /Fd%BUILD_PATH%\test_but_driver.pdb ^
   "test\test_but_driver.c"

:: Link the components and libraries to create the  test suite
link %LINKER_FLAGS% /DLL %MACHINE_FLAG% ^
     /OUT:"%BUILD_PATH%\test_but_driver.dll" ^
     /PDB:%BUILD_PATH%\test_but_driver.pdb "%BUILD_PATH%\test_but_driver.obj" ^
     "%BUILD_PATH%\but_driver.lib"
echo.


::
:: test_ehm.dll test suite
::

:: compile the components of test_ehm.dll that tests ehm.lib
echo ** Building shared test library test_ehm.dll
cl %COMPILER_FLAGS% /c /Isrc /D _LIB /D PROJECTLIBRARY_EXPORTS ^
   /Fp%BUILD_PATH%\test_ehm.pch /Fd%BUILD_PATH%\test_ehm.pdb "test\test_ehm.c"

:: build test_ehm.dll - the unit test for ehm.lib
link %LINKER_FLAGS% /DLL %MACHINE_FLAG% /OUT:"%BUILD_PATH%\test_ehm.dll" ^
     /PDB:%BUILD_PATH%\test_ehm.pdb "%BUILD_PATH%\test_ehm.obj" ^
     "%BUILD_PATH%\ehm.lib"
echo.


::
:: test_ute_driver.dll test suite
::

:: compile the components of test_ute_driver.dll that tests ute_driver.lib
echo ** Building shared test library test_ute_driver.dll
cl %COMPILER_FLAGS% /c /Isrc /D _LIB /D PROJECTLIBRARY_EXPORTS ^
   /Fp%BUILD_PATH%\test_ute_driver.pch /Fd%BUILD_PATH%\test_ute_driver.pdb ^
   "test\test_ute_driver.c"

:: build test_ute_driver.dll - the unit test for ute_driver.lib
link %LINKER_FLAGS% /DLL %MACHINE_FLAG% /OUT:"%BUILD_PATH%\test_ute_driver.dll" ^
     /PDB:%BUILD_PATH%\test_ute_driver.pdb "%BUILD_PATH%\test_ute_driver.obj" ^
     "%BUILD_PATH%\ute_driver.lib" "%BUILD_PATH%\ehm.lib"
echo.


::
:: test_ute_counter.dll test suite
::

:: compile the components of test_ute_counter.dll
echo ** Building shared test library test_ute_counter.dll
cl %COMPILER_FLAGS% /c /Isrc /D _LIB /D PROJECTLIBRARY_EXPORTS ^
   /Fp%BUILD_PATH%\test_ute_counter.pch /Fd%BUILD_PATH%\test_ute_counter.pdb ^
   "test\test_ute_counter.c"

:: build test_ute_driver.dll - the unit test for ute_driver.lib
link %LINKER_FLAGS% /DLL %MACHINE_FLAG% /OUT:"%BUILD_PATH%\test_ute_counter.dll" ^
     /PDB:%BUILD_PATH%\test_ute_counter.pdb "%BUILD_PATH%\test_ute_counter.obj" ^
     "%BUILD_PATH%\ute_driver.lib" "%BUILD_PATH%\ehm.lib"
echo.


::
:: test_mutex.dll test suite
::

:: compile the components of test_mutex.dll
echo ** Building shared test library test_mutex.dll
cl %COMPILER_FLAGS% /c /Isrc /D _LIB /D PROJECTLIBRARY_EXPORTS ^
   /Fp%BUILD_PATH%\test_mutex.pch /Fd%BUILD_PATH%\test_utk_mutex.pdb ^
   "test\test_utk_mutex.c"

:: build test_mutex.dll
link %LINKER_FLAGS% /DLL %MACHINE_FLAG% /OUT:"%BUILD_PATH%\test_mutex.dll" ^
     /PDB:%BUILD_PATH%\test_mutex.pdb "%BUILD_PATH%\test_utk_mutex.obj"
echo.


::
:: test_rgn.dll test suite
::

:: compile the components of test_rgn.dll
echo ** Building shared test library test_rgn.dll
cl %COMPILER_FLAGS% /c /Isrc /D _LIB /D PROJECTLIBRARY_EXPORTS ^
   /Fp%BUILD_PATH%\test_rgn.pch /Fd%BUILD_PATH%\test_rgn.pdb ^
   "test\test_rgn.c"

:: build test_rgn.dll
link %LINKER_FLAGS% /DLL %MACHINE_FLAG% /OUT:"%BUILD_PATH%\test_rgn.dll" ^
     /PDB:%BUILD_PATH%\test_rgn.pdb "%BUILD_PATH%\test_rgn.obj"
echo.


::
:: test_dlist.dll test suite
::

:: compile the components of test_dlist.dll
echo ** Building shared test library test_dlist.dll
cl %COMPILER_FLAGS% /c /Isrc /D _LIB /D PROJECTLIBRARY_EXPORTS ^
   /Fp%BUILD_PATH%\test_dlist.pch /Fd%BUILD_PATH%\test_dlist.pdb ^
   "test\test_dlist.c"

:: build test_dlist.dll
link %LINKER_FLAGS% /DLL %MACHINE_FLAG% /OUT:"%BUILD_PATH%\test_dlist.dll" ^
     /PDB:%BUILD_PATH%\test_dlist.pdb "%BUILD_PATH%\test_dlist.obj"
echo.


::
:: test_utk_malloc.dll test suite
::

:: compile the components of test_utk_malloc.dll
echo ** Building shared test library test_utk_malloc.dll
cl %COMPILER_FLAGS% /c /Isrc /D _LIB /D PROJECTLIBRARY_EXPORTS ^
   /Fp%BUILD_PATH%\test_utk_malloc.pch /Fd%BUILD_PATH%\test_utk_malloc.pdb ^
   "test\test_utk_malloc.c"

:: build test_utk_malloc.dll
link %LINKER_FLAGS% /DLL %MACHINE_FLAG% ^
     /OUT:"%BUILD_PATH%\test_utk_malloc.dll" ^
     /PDB:%BUILD_PATH%\test_utk_malloc.pdb "%BUILD_PATH%\test_utk_malloc.obj" ^
     "%BUILD_PATH%\ute_driver.lib" "%BUILD_PATH%\ehm.lib"
echo.


:: Build complete
goto :EOF


:DoClean
echo cleaning %BUILD_PATH%
rd /s /q %BUILD_PATH% 1>nul 2>&1
goto :EOF


:DoCleanAll
echo cleaning %BUILD_ROOT%
rd /s /q %BUILD_ROOT% 1>nul 2>&1
goto :EOF

:errProjectPath
echo Error: The build environment is not set. Run either 'misc\shell-vs2015.bat' or 'misc\shell-vs2017.bat'?
