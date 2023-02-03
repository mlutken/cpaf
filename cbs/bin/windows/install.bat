@echo off
SET SAVE_DIR_CBS_INSTALL=%CD%

rem call install-cmdtools.bat
rem call install-environment-vars.bat
rem call install-cmake.bat
rem call install-python.bat

REM set INPUT-CMDTOOLS=y
set INPUT-CBS-ENVIRONMENT-VARS=y
set INPUT-CMAKE=y
set INPUT-PYTHON=y

REM SET /P INPUT-CMDTOOLS=(1)  Install CBS needed cmdtools (setenv.exe, sed.exe, wget.exe) [Y]/N : 
SET /P INPUT-CBS-ENVIRONMENT-VARS=(2)  Install CBS environment variables [Y]/N : 
SET /P INPUT-CMAKE=(3)  Install cmake 2.8.12.2 [Y]/N : 
SET /P INPUT-PYTHON=(4)  Install python 2.7 [Y]/N : 

rem echo you typed %INPUT-CMDTOOLS%
echo .
echo You chose:
echo CMDTOOLS             : %INPUT-CMDTOOLS%
echo CBS-ENVIRONMENT-VARS : %INPUT-CBS-ENVIRONMENT-VARS%
echo CMAKE                : %INPUT-CMAKE%
echo PYTHON               : %INPUT-PYTHON%
echo .

REM IF "%INPUT-CMDTOOLS%" == "n"  GOTO CBS-ENVIRONMENT-VARS
REM echo (1)  Installing CBS needed command tools: setenv.exe, sed.exe, wget.exe ...
REM copy /Y setenv.exe %WINDIR%
REM copy /Y wget.exe %WINDIR%
REM copy /Y sed.exe %WINDIR%


:CBS-ENVIRONMENT-VARS
IF "%INPUT-CBS-ENVIRONMENT-VARS%" == "n"  GOTO CMAKE
echo (2)  Setting up CBS environment variables ...
echo Setting CBS_ROOT_DIR, CBS_ROOT_DIR_BACKSLASH ...
cd ../..
SET CBS_ROOT_DIR_BACKSLASH=%CD%
echo %CD%> path.txt
for /f "delims=" %%i in ('bin\windows\sed.exe "s/\\/\//g" path.txt') do set CBS_ROOT_DIR=%%i
del path.txt
SET PATH=%CBS_ROOT_DIR_BACKSLASH%;%PATH%

chdir /d %SAVE_DIR_CBS_INSTALL%

echo CBS_ROOT_DIR           : %CBS_ROOT_DIR%
echo CBS_ROOT_DIR_BACKSLASH : %CBS_ROOT_DIR_BACKSLASH%
echo PATH: %PATH%
echo .
call setenv -u CBS_ROOT_DIR "%CBS_ROOT_DIR%"
call setenv -u CBS_ROOT_DIR_BACKSLASH "%CBS_ROOT_DIR_BACKSLASH%"
call setenv -u PATH "%PATH%"


:CMAKE
IF "%INPUT-CMAKE%" == "n"  GOTO PYTHON
echo (3)  Installing cmake 2.8.12.2 ...
echo *****************
echo *** IMPORTANT ***
echo *****************
echo .
echo You MUST Allow CMake installer to set up your PATH (not default).
echo Set eg. to CURRENT user.
echo .
echo *****************

call wget http://www.cmake.org/files/v2.8/cmake-2.8.12.2-win32-x86.exe
call cmake-2.8.12.2-win32-x86.exe
del cmake-2.8.12.2-win32-x86.exe
chdir /d %SAVE_DIR_CBS_INSTALL%

:PYTHON
IF "%INPUT-PYTHON%" == "n"  GOTO END-CBS
echo (4)  Installing python 2.7 ...
call wget http://www.python.org/ftp/python/2.7/python-2.7.msi
call python-2.7.msi
del python-2.7.msi
chdir /d %SAVE_DIR_CBS_INSTALL%

:END-CBS
echo Done!

chdir /d %SAVE_DIR_CBS_INSTALL%

REM C:\Program Files (x86)\CMake 2.8\bin;C:\Program Files (x86)\IDM Computer Solutions\UltraEdit\
