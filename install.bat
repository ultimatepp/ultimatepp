@echo off
REM ============================================================================
REM  U++ installer for Windows 10/11
REM
REM  Put this file (install.bat) in the root of an extracted U++ release
REM  archive or a git clone of ultimatepp, then double-click it.
REM
REM  It installs everything next to itself:
REM    bin\mingit    - MinGit (official git-for-windows release)
REM    bin\clang     - llvm-mingw toolchain (matches the host architecture)
REM    vcpkg         - official Microsoft vcpkg (git clone)
REM    vcpkg\installed - static libs for the selected triplet
REM    umk.exe, theide.exe - built and placed in this folder
REM
REM  Re-running the script safely skips steps that already finished
REM  (markers are stored in the .install folder).
REM
REM  Exit code: 0 on success, 1 on failure (for CI use).
REM  Non-interactive mode: "install.bat /q" or CI environment variable set
REM  (pauses suppressed, suitable for CI runners).
REM ============================================================================
setlocal EnableExtensions EnableDelayedExpansion
title U++ installer
cd /d "%~dp0"
set "ROOT=%CD%"
set "STATE=%ROOT%\.install"
if not exist "%STATE%" mkdir "%STATE%"

REM ----------------------------------------------------------------------------
REM Self-heal: cmd.exe batch label resolution (goto/call) is unreliable in
REM batch files saved with Unix (LF-only) line endings. If this file lacks
REM CR bytes, convert it to CRLF and restart the script fresh.
REM ----------------------------------------------------------------------------
powershell -NoProfile -Command "$b=[IO.File]::ReadAllBytes('%~f0'); if(($b -eq 13).Count -lt 10){ [IO.File]::WriteAllText('%~f0', ([IO.File]::ReadAllText('%~f0') -replace \"`r?`n\",\"`r`n\")); exit 1 } else { exit 0 }"
if errorlevel 1 (
    echo Converting install.bat to Windows line endings and restarting ...
    %COMSPEC% /c "%~f0" %*
    exit /b %ERRORLEVEL%
)

REM Pause handling: silent when /q given or CI variable is set.
set "PAUSECMD=pause"
if /i "%~1"=="/q" set "PAUSECMD=rem"
if defined CI set "PAUSECMD=rem"

echo ============================================================
echo   U++ installer
echo   Root: %ROOT%
echo ============================================================
echo.

REM --- Sanity check: are we really in a U++ tree? -----------------------------
if not exist "%ROOT%\mkumk_blitz.cpp" goto :badtree
if not exist "%ROOT%\uppsrc" goto :badtree
goto :treeok
:badtree
echo ERROR: This does not look like a U++ source tree.
echo        install.bat must be placed next to mkumk_blitz.cpp and uppsrc\
echo        (i.e. in the root of an U++ archive or git clone).
echo.
%PAUSECMD%
exit /b 1
:treeok

REM ----------------------------------------------------------------------------
REM Detect architecture (x64 / arm64 / x86)
REM PROCESSOR_ARCHITEW6432 overrides PROCESSOR_ARCHITECTURE when a 32-bit cmd
REM runs on a 64-bit OS (possible if the bat is launched from a 32-bit shell).
REM ----------------------------------------------------------------------------
set "ARCH=%PROCESSOR_ARCHITECTURE%"
if defined PROCESSOR_ARCHITEW6432 set "ARCH=%PROCESSOR_ARCHITEW6432%"

set "LLVM_ARCH="
set "TRIPLET="
set "MINGIT_PAT="
set "BM=CLANG"

if /i "%ARCH%"=="AMD64" (
    set "LLVM_ARCH=x86_64"
    set "TRIPLET=x64-mingw-static-release"
    set "MINGIT_PAT=MinGit-*-64-bit.zip"
    set "BM=CLANGx64"
)
if /i "%ARCH%"=="ARM64" (
    set "LLVM_ARCH=aarch64"
    set "TRIPLET=arm64-mingw-static"
    set "MINGIT_PAT=MinGit-*-arm64.zip"
    set "BM=CLANGarm64"
)
if /i "%ARCH%"=="x86" (
    set "LLVM_ARCH=i686"
    set "TRIPLET=x86-mingw-static-release"
    set "MINGIT_PAT=MinGit-*-32-bit.zip"
    set "BM=CLANG"
)

if not defined LLVM_ARCH (
    echo ERROR: Unsupported architecture "%ARCH%".
    %PAUSECMD%
    exit /b 1
)

echo Detected architecture : %ARCH%
echo llvm-mingw variant    : ucrt-%LLVM_ARCH%
echo vcpkg triplet         : %TRIPLET%
echo U++ build method      : %BM%
echo.
echo NOTE: The first run downloads MinGit and the llvm-mingw toolchain and then
echo       builds ~10 libraries with vcpkg ^(OpenSSL is the slowest one^).
echo       Expect this to take a long while. Progress is resumable: simply
echo       re-run install.bat if anything fails.
echo.
%PAUSECMD%

REM ============================================================================
REM STEP 1: MinGit into .\bin\mingit
REM ============================================================================
echo.
echo === STEP 1/6: MinGit -------------------------------------------------
if exist "%STATE%\done-mingit" (
    echo Already installed, skipping.
    goto :step2
)

if not exist "%ROOT%\bin\mingit\cmd\git.exe" goto :getmingit
echo Found existing bin\mingit, marking as done.
echo done>"%STATE%\done-mingit"
goto :step2

:getmingit
echo Resolving latest MinGit release from github.com/git-for-windows/git ...
for /f "usebackq delims=" %%i in (`powershell -NoProfile -Command "[Net.ServicePointManager]::SecurityProtocol=[Net.SecurityProtocolType]::Tls12; $a=(Invoke-RestMethod 'https://api.github.com/repos/git-for-windows/git/releases/latest').assets; ($a | Where-Object { $_.name -like '%MINGIT_PAT%' -and $_.name -notlike '*busybox*' } | Select-Object -First 1).browser_download_url"`) do set "MINGIT_URL=%%i"

if not defined MINGIT_URL (
    echo ERROR: Could not find a MinGit asset matching "%MINGIT_PAT%".
    echo        Check https://github.com/git-for-windows/git/releases manually.
    goto :fail
)
echo Downloading: !MINGIT_URL!
powershell -NoProfile -Command "[Net.ServicePointManager]::SecurityProtocol=[Net.SecurityProtocolType]::Tls12; Invoke-WebRequest -Uri '!MINGIT_URL!' -OutFile '%STATE%\mingit.zip'" || goto :fail

if exist "%STATE%\mingit_stage" rd /s /q "%STATE%\mingit_stage"
mkdir "%STATE%\mingit_stage"
tar -xf "%STATE%\mingit.zip" -C "%STATE%\mingit_stage" || goto :fail

if exist "%ROOT%\bin\mingit" rd /s /q "%ROOT%\bin\mingit"
robocopy "%STATE%\mingit_stage" "%ROOT%\bin\mingit" /E /NFL /NDL /NJH /NJS
if errorlevel 8 goto :fail

if not exist "%ROOT%\bin\mingit\cmd\git.exe" (
    echo ERROR: MinGit extracted but bin\mingit\cmd\git.exe not found.
    goto :fail
)
del /q "%STATE%\mingit.zip" 2>nul
rd /s /q "%STATE%\mingit_stage" 2>nul
echo done>"%STATE%\done-mingit"
echo MinGit installed to bin\mingit

REM ============================================================================
REM STEP 2: llvm-mingw into .\bin\clang
REM ============================================================================
:step2
echo.
echo === STEP 2/6: llvm-mingw (ucrt-%LLVM_ARCH%) ---------------------------
if exist "%STATE%\done-clang" (
    echo Already installed, skipping.
    goto :step3
)

if not exist "%ROOT%\bin\clang\bin\clang++.exe" goto :getllvm
echo Found existing bin\clang, marking as done.
echo done>"%STATE%\done-clang"
goto :step3

:getllvm
set "LLVM_URL=https://github.com/User0755/llvm-mingw/releases/download/nightly/llvm-mingw-nightly-ucrt-%LLVM_ARCH%.zip"
echo Downloading: %LLVM_URL%
powershell -NoProfile -Command "[Net.ServicePointManager]::SecurityProtocol=[Net.SecurityProtocolType]::Tls12; Invoke-WebRequest -Uri '%LLVM_URL%' -OutFile '%STATE%\llvm-mingw.zip'" || goto :fail

if exist "%STATE%\llvm_stage" rd /s /q "%STATE%\llvm_stage"
mkdir "%STATE%\llvm_stage"
tar -xf "%STATE%\llvm-mingw.zip" -C "%STATE%\llvm_stage" || goto :fail

REM The archive contains one top-level folder (name depends on the build/date);
REM find it and move it to bin\clang.
set "CLANG_DIR="
for /d %%d in ("%STATE%\llvm_stage\*") do set "CLANG_DIR=%%~d"

if exist "%ROOT%\bin\clang" rd /s /q "%ROOT%\bin\clang"

if defined CLANG_DIR (
    move "%CLANG_DIR%" "%ROOT%\bin\clang" >nul || goto :fail
) else (
    REM No wrapper folder - archive extracted flat
    robocopy "%STATE%\llvm_stage" "%ROOT%\bin\clang" /E /NFL /NDL /NJH /NJS
    if errorlevel 8 goto :fail
)

if not exist "%ROOT%\bin\clang\bin\clang++.exe" (
    echo ERROR: llvm-mingw extracted but bin\clang\bin\clang++.exe not found.
    dir "%ROOT%\bin\clang" 2>nul
    goto :fail
)
del /q "%STATE%\llvm-mingw.zip" 2>nul
rd /s /q "%STATE%\llvm_stage" 2>nul
echo done>"%STATE%\done-clang"
echo llvm-mingw installed to bin\clang

REM ============================================================================
REM STEP 3: official Microsoft vcpkg into .\vcpkg
REM ============================================================================
:step3
echo.
echo === STEP 3/6: vcpkg ---------------------------------------------------
if exist "%STATE%\done-vcpkg" (
    echo Already installed, skipping.
    goto :step4
)

set "GIT=%ROOT%\bin\mingit\cmd\git.exe"

if not exist "%ROOT%\vcpkg\vcpkg.exe" goto :getvcpkg
echo Found existing vcpkg with bootstrapped vcpkg.exe, marking as done.
echo done>"%STATE%\done-vcpkg"
goto :step4

:getvcpkg
if exist "%ROOT%\vcpkg" (
    echo Removing incomplete vcpkg folder from a previous run ...
    rd /s /q "%ROOT%\vcpkg"
)
echo Cloning official Microsoft vcpkg (shallow) ...
"%GIT%" clone --depth 1 https://github.com/microsoft/vcpkg.git "%ROOT%\vcpkg" || goto :fail

echo Bootstrapping vcpkg ...
pushd "%ROOT%\vcpkg"
call bootstrap-vcpkg.bat -disableMetrics || (popd & goto :fail)
popd

if not exist "%ROOT%\vcpkg\vcpkg.exe" (
    echo ERROR: vcpkg bootstrap did not produce vcpkg.exe.
    goto :fail
)
echo done>"%STATE%\done-vcpkg"
echo vcpkg installed to vcpkg\

REM ============================================================================
REM STEP 4: build the required packages with vcpkg
REM ============================================================================
:step4
echo.
echo === STEP 4/6: vcpkg packages (%TRIPLET%) ------------------------------
if exist "%STATE%\done-pkgs" (
    echo Already installed, skipping.
    goto :step5
)

REM Make sure CMake picks OUR clang toolchain and OUR git:
set "PATH=%ROOT%\bin\clang\bin;%ROOT%\bin\mingit\cmd;%PATH%"

echo Building: bzip2 libjpeg-turbo liblzma libpng lz4 openssl pcre2 xxhash zlib zstd
echo Triplet : %TRIPLET%
echo This is the long step. Go get some coffee.
echo.
pushd "%ROOT%\vcpkg"
vcpkg.exe install bzip2 libjpeg-turbo liblzma libpng lz4 openssl pcre2 xxhash zlib zstd --triplet %TRIPLET% || (popd & goto :fail)
popd

echo done>"%STATE%\done-pkgs"
echo vcpkg packages installed.

REM ============================================================================
REM STEP 5: build umk
REM ============================================================================
:step5
echo.
echo === STEP 5/6: building umk --------------------------------------------
if exist "%STATE%\done-umk" (
    echo Already built, skipping.
    goto :step6
)

if not exist "%ROOT%\bin\clang\bin\clang++.exe" (
    echo ERROR: bin\clang\bin\clang++.exe missing - cannot build umk.
    goto :fail
)
if not exist "%ROOT%\vcpkg\installed\%TRIPLET%\lib" (
    echo ERROR: vcpkg packages for %TRIPLET% not found.
    goto :fail
)

set "CXX=%ROOT%\bin\clang\bin\clang++.exe"
set "LIBDIR=%ROOT%\vcpkg\installed\%TRIPLET%\lib"
set "INCDIR=%ROOT%\vcpkg\installed\%TRIPLET%\include"

REM Resolve static library file names (names can differ between vcpkg ports;
REM e.g. zlib may be libz.a, libzlib.a or libzs.a, libpng may be libpng.a or libpng16.a)
set "LIB_Z=%LIBDIR%\libz.a"
if not exist "%LIB_Z%" set "LIB_Z=%LIBDIR%\libzlib.a"
if not exist "%LIB_Z%" set "LIB_Z=%LIBDIR%\libzs.a"
if not exist "%LIB_Z%" goto :missinglibs

set "LIB_PNG=%LIBDIR%\libpng.a"
if not exist "%LIB_PNG%" set "LIB_PNG=%LIBDIR%\libpng16.a"
if not exist "%LIB_PNG%" goto :missinglibs

if not exist "%LIBDIR%\libbz2.a" goto :missinglibs
if not exist "%LIBDIR%\liblzma.a" goto :missinglibs
if not exist "%LIBDIR%\libpcre2-8.a" goto :missinglibs
if not exist "%LIBDIR%\libpcre2-32.a" goto :missinglibs
if not exist "%LIBDIR%\liblz4.a" goto :missinglibs
if not exist "%LIBDIR%\libzstd.a" goto :missinglibs
if not exist "%LIBDIR%\libxxhash.a" goto :missinglibs

if exist "%ROOT%\umk.exe" del /q "%ROOT%\umk.exe"

REM NOTE: no -DflagNO_FONTSYS here (unlike the POSIX makefile): on Windows the
REM Win32 font backend (FontWin32.cpp) uses only native GDI (usp10.h etc.) and
REM forcing NO_FONTSYS would collide with it - FontWin32.cpp ignores NO_FONTSYS.
echo Compiling umk ...
"%CXX%" -O2 -std=c++17 -DflagMAIN -DflagWIN32 ^
  -Iuppsrc -I. -I"%INCDIR%" ^
  mkumk_blitz.cpp ^
  uppsrc/Draw/FontCR.cpp ^
  uppsrc/Core/heaputil.cpp uppsrc/Core/hheap.cpp uppsrc/Core/sheap.cpp ^
  uppsrc/Core/lheap.cpp uppsrc/Core/heap.cpp uppsrc/Core/heapdbg.cpp ^
  uppsrc/Core/String.cpp uppsrc/Core/CharSet.cpp uppsrc/Core/CvFlt.cpp ^
  uppsrc/Core/xxHash_chk.cpp uppsrc/Core/Http.cpp ^
  -L"%LIBDIR%" ^
  -o "%ROOT%\umk.exe" ^
  -static ^
  "%LIB_Z%" "%LIB_PNG%" ^
  "%LIBDIR%\libbz2.a" "%LIBDIR%\liblzma.a" "%LIBDIR%\libpcre2-8.a" ^
  "%LIBDIR%\libpcre2-32.a" "%LIBDIR%\liblz4.a" "%LIBDIR%\libzstd.a" ^
  "%LIBDIR%\libxxhash.a" ^
  -lgdi32 -luser32 -lusp10 -lole32 -loleaut32 -luuid -lws2_32 -ladvapi32 ^
  -lmpr -lwinmm || goto :fail

if not exist "%ROOT%\umk.exe" (
    echo ERROR: umk.exe was not produced.
    goto :fail
)
echo done>"%STATE%\done-umk"
echo umk.exe built in installation root.
goto :step6

:missinglibs
echo ERROR: Expected static libraries not found in:
echo        %LIBDIR%
echo Present .a files:
dir /b "%LIBDIR%\*.a" 2>nul
goto :fail

REM ============================================================================
REM STEP 6: build theide
REM ============================================================================
:step6
echo.
echo === STEP 6/6: building theide -----------------------------------------
if exist "%STATE%\done-theide" (
    echo Already built, skipping.
    goto :finish
)

if not exist "%ROOT%\umk.exe" (
    echo ERROR: umk.exe not found - cannot build theide.
    goto :fail
)

REM Make sure umk's CLANG builder finds our toolchain:
set "PATH=%ROOT%\bin\clang\bin;%ROOT%\bin\mingit\cmd;%PATH%"

REM ---------------------------------------------------------------------------
REM Build a temporary build-method file for umk. On Windows umk does not
REM auto-create CLANG/GCC build methods (POSIX-only feature), so we generate
REM one that always uses the self-installed clang toolchain. ${METHOD_DIR}
REM is substituted by umk with the folder containing this .bm file, so the
REM U++ tree stays relocatable. The file is deleted after the build because
REM theide will create proper methods (CLANGx64 etc.) at first launch.
REM ---------------------------------------------------------------------------
set "BMFILE=%ROOT%\_TEMP_.bm"
set "CLANGTRIPLE=x86_64-w64-mingw32"
if "%LLVM_ARCH%"=="aarch64" set "CLANGTRIPLE=aarch64-w64-mingw32"
if "%LLVM_ARCH%"=="i686" set "CLANGTRIPLE=i686-w64-mingw32"

> "%BMFILE%" (
    echo BUILDER = "CLANG";
    echo COMPILER = "";
    echo COMMON_OPTIONS = "";
    echo COMMON_CPP_OPTIONS = "-std=c++17";
    echo COMMON_C_OPTIONS = "";
    echo COMMON_LINK = "";
    echo COMMON_FLAGS = "";
    echo DEBUG_INFO = "2";
    echo DEBUG_BLITZ = "1";
    echo DEBUG_LINKMODE = "0";
    echo DEBUG_OPTIONS = "";
    echo DEBUG_FLAGS = "";
    echo DEBUG_LINK = "-Wl,--stack,20000000";
    echo RELEASE_BLITZ = "1";
    echo RELEASE_LINKMODE = "0";
    echo RELEASE_OPTIONS = "-O3 ";
    echo RELEASE_FLAGS = "";
    echo RELEASE_LINK = "-Wl,--stack,20000000";
    echo DEBUGGER = "gdb";
    echo ALLOW_PRECOMPILED_HEADERS = "1";
    echo DISABLE_BLITZ = "0";
    echo PATH = "${METHOD_DIR}/bin/clang/bin;${METHOD_DIR}/bin/clang/%CLANGTRIPLE%/bin";
    echo INCLUDE = "${METHOD_DIR}/vcpkg/installed/%TRIPLET%/include";
    echo LIB = "${METHOD_DIR}/vcpkg/installed/%TRIPLET%/lib";
    echo LINKMODE_LOCK = "0";
)

if exist "%ROOT%\theide" rd /s /q "%ROOT%\theide"
echo Building TheIDE with umk ^(temporary build method _TEMP_.bm, this takes a while^) ...
echo ^(full umk output is captured in %STATE%\theide-build.log^)
REM NOTE: no -s (shared libraries)! On Windows we only build the static vcpkg
REM triplet; with -s, U++ packages select the "z" library name (-lz) instead of
REM the static one (libzs.a, linked as -lzs per Core.upp WIN32 !MSC !SHARED rule).
"%ROOT%\umk.exe" ./uppsrc ide _TEMP_ -brH ./theide > "%STATE%\theide-build.log" 2>&1
set "UMK_RC=%ERRORLEVEL%"
del /q "%BMFILE%" 2>nul

if not "%UMK_RC%"=="0" (
    echo.
    echo ERROR: umk failed. Last lines of the build log:
    echo ------------------------------------------------------------
    powershell -NoProfile -Command "Get-Content '%STATE%\theide-build.log' -Tail 50"
    echo ------------------------------------------------------------
    goto :fail
)

set "THEIDE_EXE="
if exist "%ROOT%\theide.exe" set "THEIDE_EXE=%ROOT%\theide.exe"
if exist "%ROOT%\theide\theide.exe" set "THEIDE_EXE=%ROOT%\theide\theide.exe"
if not defined THEIDE_EXE if exist "%ROOT%\theide\ide.exe" set "THEIDE_EXE=%ROOT%\theide\ide.exe"
if not defined THEIDE_EXE if exist "%ROOT%\out\ide\theide.exe" set "THEIDE_EXE=%ROOT%\out\ide\theide.exe"
if not defined THEIDE_EXE (
    echo ERROR: theide.exe not found after the build. Contents of root, .\theide, .\out:
    dir /b "%ROOT%\theide.exe" 2>nul
    dir /b "%ROOT%\theide\*.exe" 2>nul
    goto :fail
)
if not exist "%ROOT%\theide.exe" copy /y "%THEIDE_EXE%" "%ROOT%\theide.exe" >nul || goto :fail
echo done>"%STATE%\done-theide"

REM ============================================================================
:finish
echo.
echo ============================================================
echo   Installation finished successfully!
echo.
echo   umk.exe    : %ROOT%\umk.exe
echo   theide.exe : %ROOT%\theide.exe
echo   Toolchain  : %ROOT%\bin\clang
echo   Git        : %ROOT%\bin\mingit
echo   vcpkg      : %ROOT%\vcpkg (triplet %TRIPLET%)
echo.
echo   Re-running install.bat is safe; finished steps are skipped.
echo ============================================================
%PAUSECMD%
exit /b 0

:fail
echo.
echo ============================================================
echo   *** INSTALLATION FAILED ***
echo.
echo   Fix the problem (or just free disk space / fix network) and
echo   re-run install.bat - completed steps are remembered and skipped.
echo ============================================================
%PAUSECMD%
exit /b 1
