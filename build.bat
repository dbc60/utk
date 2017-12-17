@echo off

REM Run this script from the build directory, that is, the directory where you
REM want all the build artifacts to be placed.

REM Added _UNICODE and UNICODE so I can use Unicode strings in structs and whatnot.
REM Need user32.lib to link MessageBox(), which es used on branch DAY001
REM Need gdi32.lib to link PatBlt(), which es used on branch DAY002
REM 2015.01.25 (Day004) I added /FC to get full path names in diagnostics. It's
REM helpful when using Emacs to code and launch the debugger and executable

REM /GS- turn off security checks because that compile-time option relies on
REM the C runtime library, which we are not using.

REM /Gs[size] The number of bytes that local variables can occupy before
REM a stack probe is initiated. If the /Gs option is specified without a
REM size argument, it is the same as specifying /Gs0

REM /Gm- disable minimal rebuild. We want to build everything. It won't
REM take long.

REM /GR- disable C++ RTTI. We don't need runtime type information.

REM /EHsc enable C++ EH (no SEH exceptions) (/EHs),
REM and  extern "C" defaults to nothrow (/EHc)

REM /EHa- disable C++ Exception Handling, so we don't have stack unwind code.
REM Casey says we don't need it.


REM /W3 set warning level 3.
REM /W4 set warning level 4. It's better
REM /WX warnings are errors
REM /wd turns off a particular warning
REM   /wd4201 - nonstandard extension used : nameless struct/union
REM   /wd4100 - 'identifier' : unreferenced formal parameter (this happens a lot while developing code)
REM   /wd4189 - 'identifier' : local variable is initialized but not referenced
REM   /wd4127 - conditional expression is constant ("do {...} while (0)" in macros)

REM /FC use full pathnames in diagnostics

REM /Od - disable optimizations. The debug mode is good for development

REM /Oi Generate intrinsic functions. Replaces some function calls with
REM intrinsic or otherwise special forms of the function that help your
REM application run faster.

REM /GL whole program optimization. Use the /LTCG linker option to create the
REM output file. /ZI cannot be used with /GL.

REM /I<dir> add to include search path

REM /Fe:<file> name executable file

REM /D<name>{=|#}<text> define macro

REM /Zi enable debugging information
REM /Z7 enable debugging information

REM /link [linker options and libraries] The linker options are
REM documented here: https://msdn.microsoft.com/en-us/library/y0zzbyt4.aspx

REM /nodefaultlib t

REM Note that subsystem version number 5.1 only works with 32-bit builds.
REM The minimum subsystem version number for 64-bit buils is 5.2.
REM /subsystem:windows,5.1 - enable compatibility with Windows XP (5.1)

REM /LTCG link time code generation

REM /STACK:reserve[,commit] stack allocations. The /STACK option sets the size
REM of the stack in bytes. Use this option only when you build an .exe file.

REM DEFINITIONS
REM   _UNICODE - 16-bit wide characters
REM   UNICODE  - 16-bit wide characters
REM   PROJECT_INTERNAL  - 0 = build for public release, 1 = build for developers only
REM   PROJECT_SLOW      - 0 = No slow code (like assertion checks) allowed!,
REM                       1 = Slow code welcome
REM   __ISO_C_VISIBLE   - the version of C we are targeting for the math library.
REM                       1995 = C95, 1999 = C99, 2011 = C11.
REM Optimization switches /O2 /Oi /fp:fast

set BUILD_PATH=%cd%

set CommonCompilerFlags=/GS /Zc:wchar_t /MTd /nologo /fp:fast /Gm- /GR- /EHsc ^
    /WX /W4 /wd4201 /Zc:inline ^
    /wd4100 /wd4189 /wd4127 /wd4505 /FC /Z7 /Od /Oi /D _UNICODE /D UNICODE ^
    /D _DEBUG /D PROJECT_INTERNAL=1 /D PROJECT_SLOW=1 /D PROJECT_WIN32=1 ^
    /Wv:18 /Iinclude /Fabuild\x64\Debug\ /Fobuild\x64\Debug\

set CommonLinkerFlags=/incremental:no /opt:ref user32.lib gdi32.lib winmm.lib

REM clean up the symbol files from the previous build so we will have fresh
REM ones for live-load debugging in VS
del *.pdb > NUL 2> NUL

REM old way to create a new shared library. Use %random% instead
REM set datetime=%date:~-4,4%%date:~-10,2%%date:~-7,2%_%time:~0,2%%time:~3,2%%time:~6,2%
REM set datetime=%datetime: =0%

REM build the live-loadable shared library

REM echo WAITING FOR PDB > lock.tmp
REM set random_value=%random%
REM cl %CommonCompilerFlags% "%PROJECT_PATH%\src\%PROJECT_NAME%.cpp" ^
REM    /Fm%PROJECT_NAME%.map /LD /link /incremental:no /opt:ref ^
REM    /PDB:%PROJECT_NAME%_%random_value%.pdb
REM del lock.tmp

REM build the executable that will live-load a shared library
REM cl %CommonCompilerFlags% "%PROJECT_PATH%\src\win32_%PROJECT_NAME%.cpp" ^
REM   /Fmwin32_%PROJECT_NAME%.map  /link %CommonLinkerFlags%

REM build the static library for the BUT driver: but_driver.lib
REM the '/c' flag means 'compile only, do not link'
cl %CommonCompilerFlags% /c /D _LIB /Fpbuild\x64\Debug\but_driver.pch /Fdbuild\x64\Debug\but_driver.pdb src\but_driver.c src\but_version.c src\but.c

lib /OUT:"%PROJECT_PATH%\build\x64\Debug\but_driver.lib" /MACHINE:X64 /NOLOGO build\x64\Debug\but_driver.obj build\x64\Debug\but_version.obj build\x64\Debug\but.obj
