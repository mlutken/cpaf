@echo off
SET SAVE_DIR_CPAF_INSTALL=%CD%

set INPUT-CPAF-ENVIRONMENT-VARS=y
set INPUT-UNPACK-SOURCE=y

SET /P INPUT-CPAF-ENVIRONMENT-VARS=(2)  Install CPAF environment variables [Y]/N : 
SET /P INPUT-UNPACK-SOURCE=(3)  Unpack source archives (boost etc.) [Y]/N : 

echo .
echo You chose:
echo CPAF-ENVIRONMENT-VARS : %INPUT-CPAF-ENVIRONMENT-VARS%
echo INPUT-UNPACK-SOURCE : %INPUT-UNPACK-SOURCE%
echo .

echo (1)  Running install in cbs directory ...
cd ..\..\cbs\bin\windows
call install.bat
chdir /d %SAVE_DIR_CPAF_INSTALL%


IF "%INPUT-CPAF-ENVIRONMENT-VARS%" == "n"  GOTO END-CPAF
echo (2)  Setting up CPAF environment vars ...
echo Setting CPAF_ROOT_DIR, CPAF_ROOT_DIR_BACKSLASH ...
cd ../..
SET CPAF_ROOT_DIR_BACKSLASH=%CD%
echo %CD%> path.txt
for /f "delims=" %%i in ('sed "s/\\/\//g" path.txt') do set CPAF_ROOT_DIR=%%i
del path.txt
REM SET PATH=%CPAF_ROOT_DIR_BACKSLASH%\bin\windows;%PATH%

chdir /d %SAVE_DIR_CPAF_INSTALL%

echo CPAF_ROOT_DIR           : %CPAF_ROOT_DIR%
echo CPAF_ROOT_DIR_BACKSLASH : %CPAF_ROOT_DIR_BACKSLASH%
call setenv -u CPAF_ROOT_DIR "%CPAF_ROOT_DIR%"
call setenv -u CPAF_ROOT_DIR_BACKSLASH "%CPAF_ROOT_DIR_BACKSLASH%"
REM call setenv -u PATH "%PATH%"
REM echo PATH: %PATH%
REM echo .

:END-CPAF
echo Done!

chdir /d %SAVE_DIR_CPAF_INSTALL%

IF "%INPUT-UNPACK-SOURCE%" == "n"  GOTO ENDEND-CPAF
call unpack_source.bat

:ENDEND-CPAF
chdir /d %SAVE_DIR_CPAF_INSTALL%




