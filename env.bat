@echo off

SET SAVE_DIR=%CD%
REM *** Get CBS_ROOT_DIR ***
echo Setting CBS_ROOT_DIR, CBS_ROOT_DIR_BACKSLASH ...
cd cbs
SET CBS_ROOT_DIR_BACKSLASH=%CD%
echo %CD%> path.txt
for /f "delims=" %%i in ('sed "s/\\/\//g" path.txt') do set CBS_ROOT_DIR=%%i
del path.txt
chdir /d %SAVE_DIR%

REM *** Get CPAF_ROOT_DIR ***
echo Setting CPAF_ROOT_DIR, CPAF_ROOT_DIR_BACKSLASH ...
SET CPAF_ROOT_DIR_BACKSLASH=%CD%
echo %CD%> path.txt
for /f "delims=" %%i in ('sed "s/\\/\//g" path.txt') do set CPAF_ROOT_DIR=%%i
del path.txt
chdir /d %SAVE_DIR%

SET PATH=%CBS_ROOT_DIR_BACKSLASH%;%PATH%

echo CBS_ROOT_DIR            : "%CBS_ROOT_DIR%"
echo CBS_ROOT_DIR_BACKSLASH  : "%CBS_ROOT_DIR_BACKSLASH%"
echo CPAF_ROOT_DIR           : "%CPAF_ROOT_DIR%"
echo CPAF_ROOT_DIR_BACKSLASH : "%CPAF_ROOT_DIR_BACKSLASH%"
