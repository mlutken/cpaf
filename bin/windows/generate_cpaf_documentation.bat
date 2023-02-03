echo off
SET SAVE_DIR=%CD%

echo Generating Documentation for CPAF...
chdir /d %CPAF_ROOT_DIR_BACKSLASH%
%CPAF_ROOT_DIR_BACKSLASH%\bin\windows\doxys.exe
chdir /d %SAVE_DIR%

