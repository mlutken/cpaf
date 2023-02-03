@echo off
SET SaveDir=%CD%
call "%VS90COMNTOOLS%"\vsvars32.bat

rem ------------------------------------------
rem --- Find script source directory ---
rem ------------------------------------------
echo %0%> sourcePath.txt
for /f "delims=" %%i in ('sed "s/vc9-cpaf-release-static-solution.bat//g" sourcePath.txt') do set ScriptSourcePath=%%i
del sourcePath.txt
chdir /d %ScriptSourcePath%
SET ScriptSourcePath=%CD%
chdir /d %SaveDir%
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
chdir /d %SaveDir%


:LabelCpafRootDir
IF "%CPAF_ROOT_DIR%" NEQ "" GOTO LabelDisplayBuildConfigurations
echo "Finding CPAF_ROOT_DIR ... "
chdir /d %ScriptSourcePath%
cd ../..
SET CPAF_ROOT_DIR_BACKSLASH=%CD%
echo %CD%> path.txt
for /f "delims=" %%i in ('sed "s/\\/\//g" path.txt') do set CPAF_ROOT_DIR=%%i
del path.txt
chdir /d %SaveDir%

rem ---------------------------------------------------
rem --- Display chosen build configurations ---
rem ---------------------------------------------------
:LabelDisplayBuildConfigurations
echo .
echo Building with these vars:
echo CBS_ROOT_DIR   : %CBS_ROOT_DIR%
echo CPAF_ROOT_DIR  : %CPAF_ROOT_DIR%
echo .

rem -------------------------
rem --- Release Static ---
rem -------------------------
echo Creating Release Static VS2008 solution ...
SET BUILD_DIR=build_cbs\vc9_release_static
chdir /d %ScriptSourcePath%
cd ../..
mkdir  %BUILD_DIR%
chdir %BUILD_DIR%
cmake -D CMAKE_BUILD_TYPE:STRING=Release -D CBS_BUILD=ON -D CBS_LIB_TYPE_TO_BUILD:STRING=STATIC -G "Visual Studio 9 2008" ../..
chdir /d %SaveDir%


