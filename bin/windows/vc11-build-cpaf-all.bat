@echo off
SET SaveDirBuildAll=%CD%
call "%VS110COMNTOOLS%"\vsvars32.bat

rem ------------------------------
rem --- Set default values ---
rem ------------------------------
set bReleaseStatic=y
set bReleaseShared=n
set bDebugStatic=y
set bDebugShared=n

rem ------------------------------------------
rem --- Find script source directory ---
rem ------------------------------------------
echo %0%> sourcePath.txt
for /f "delims=" %%i in ('sed "s/vc11-build-cpaf-all.bat//g" sourcePath.txt') do set ScriptSourcePath=%%i
del sourcePath.txt
chdir /d %ScriptSourcePath%
SET ScriptSourcePath=%CD%
chdir /d %SaveDirBuildAll%
echo "ScriptSourcePath: %ScriptSourcePath%"



rem ------------------------------------------------------------------------------------
rem --- Debug Only: Unsetting CBS_ROOT_DIR and CPAF_ROOT_DIR ---
rem ------------------------------------------------------------------------------------
rem set CBS_ROOT_DIR=
rem set CPAF_ROOT_DIR=
rem echo "Before: CBS_ROOT_DIR : %CBS_ROOT_DIR%"
rem echo "Before: CPAF_ROOT_DIR: %CPAF_ROOT_DIR%"

rem ------------------------------------------------------------------------------------------------------------
rem --- Try auto detecting CBS_ROOT_DIR and CPAF_ROOT_DIR in case they are not set ---
rem ------------------------------------------------------------------------------------------------------------
IF "%CBS_ROOT_DIR%" NEQ "" GOTO LabelCpafRootDir
echo "Finding CBS_ROOT_DIR ... "
chdir /d %ScriptSourcePath%
cd ../..
cd cbs
SET CBS_ROOT_DIR_BACKSLASH=%CD%
echo %CD%> path.txt
for /f "delims=" %%i in ('sed "s/\\/\//g" path.txt') do set CBS_ROOT_DIR=%%i
del path.txt
SET PATH=%CBS_ROOT_DIR_BACKSLASH%;%PATH%
chdir /d %SaveDirBuildAll%


:LabelCpafRootDir
IF "%CPAF_ROOT_DIR%" NEQ "" GOTO LabelInteractiveMode
echo "Finding CPAF_ROOT_DIR ... "
chdir /d %ScriptSourcePath%
cd ../..
SET CPAF_ROOT_DIR_BACKSLASH=%CD%
echo %CD%> path.txt
for /f "delims=" %%i in ('sed "s/\\/\//g" path.txt') do set CPAF_ROOT_DIR=%%i
del path.txt
chdir /d %SaveDirBuildAll%

rem -----------------------------
rem --- Interactive Mode ---
rem -----------------------------
:LabelInteractiveMode
IF "%1%" NEQ "-i" GOTO LabelDisplayBuildConfigurations

SET /P bReleaseStatic=(1)  Build Release Static [y]/n : 
SET /P bReleaseShared=(2)  Build Release Shared [y]/n : 
SET /P bDebugStatic=(3)  Build Debug Static [y]/n : 
SET /P bDebugShared=(4)  Build Debug Shared [y]/n : 

rem ---------------------------------------------------
rem --- Display chosen build configurations ---
rem ---------------------------------------------------
:LabelDisplayBuildConfigurations
echo .
echo Building these configurations:
echo Release Static : %bReleaseStatic%
echo Release Shared : %bReleaseShared%
echo Debug Static   : %bDebugStatic%
echo Debug Shared   : %bDebugShared%
echo CBS_ROOT_DIR   : %CBS_ROOT_DIR%
echo CPAF_ROOT_DIR  : %CPAF_ROOT_DIR%
echo .


rem -------------------------
rem --- Release Static ---
rem -------------------------
IF "%bReleaseStatic%" == "n"  GOTO LabelReleaseShared
echo (1)  Building Release Static ...
SET BUILD_DIR=build_cbs\nmake_release_static
chdir /d %ScriptSourcePath%
cd ../..
mkdir  %BUILD_DIR%
chdir %BUILD_DIR%
del CMakeCache.txt
cmake -D CMAKE_BUILD_TYPE:STRING=Release -D CBS_BUILD=ON -D CBS_LIB_TYPE_TO_BUILD:STRING=STATIC -G "NMake Makefiles" ../..
nmake all
chdir /d %SaveDirBuildAll%


rem ---------------------------
rem --- Release Shared ---
rem ---------------------------
:LabelReleaseShared
IF "%bReleaseShared%%" == "n"  GOTO LabelDebugStatic
echo (2)  Building Release Shared ...
SET BUILD_DIR=build_cbs\nmake_release_shared
chdir /d %ScriptSourcePath%
cd ../..
mkdir  %BUILD_DIR%
chdir %BUILD_DIR%
del CMakeCache.txt
cmake -D CMAKE_BUILD_TYPE:STRING=Release -D CBS_BUILD=ON -D CBS_LIB_TYPE_TO_BUILD:STRING=SHARED -G "NMake Makefiles" ../..
nmake all
chdir /d %SaveDirBuildAll%


rem ------------------------
rem --- Debug Static ---
rem ------------------------
:LabelDebugStatic
IF "%bDebugStatic%" == "n"  GOTO LabelDebugShared
echo (3)  Building Debug Static ...
SET BUILD_DIR=build_cbs\nmake_debug_static
chdir /d %ScriptSourcePath%
cd ../..
mkdir  %BUILD_DIR%
chdir %BUILD_DIR%
del CMakeCache.txt
cmake -D CMAKE_BUILD_TYPE:STRING=Debug -D CBS_BUILD=ON -D CBS_LIB_TYPE_TO_BUILD:STRING=STATIC -G "NMake Makefiles" ../..
nmake all
chdir /d %SaveDirBuildAll%

rem -------------------------
rem --- Debug Shared ---
rem -------------------------
:LabelDebugShared
IF "%bDebugShared%" == "n"  GOTO LabelEndBuildAll
echo (4)  Building Debug Shared ...
SET BUILD_DIR=build_cbs\nmake_debug_shared
chdir /d %ScriptSourcePath%
cd ../..
mkdir  %BUILD_DIR%
chdir %BUILD_DIR%
del CMakeCache.txt
cmake -D CMAKE_BUILD_TYPE:STRING=Debug -D CBS_BUILD=ON -D CBS_LIB_TYPE_TO_BUILD:STRING=SHARED -G "NMake Makefiles" ../..
nmake all
chdir /d %SaveDirBuildAll%

:LabelEndBuildAll
echo Done!

chdir /d %SaveDirBuildAll%

