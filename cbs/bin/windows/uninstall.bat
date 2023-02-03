@echo off
SET SAVE_DIR_CBS_UNINSTALL=%CD%

set INPUT-CMDTOOLS=y
set INPUT-CBS-ENVIRONMENT-VARS=y
set INPUT-CMAKE=n
set INPUT-PYTHON=n

SET /P INPUT-CMDTOOLS=(1)  UnInstall CBS needed cmdtools (setenv.exe, sed.exe, wget.exe) [Y]/N : 
SET /P INPUT-CBS-ENVIRONMENT-VARS=(2)  UnInstall CBS environment variables [Y]/N : 
rem SET /P INPUT-CMAKE=(3)  UnInstall cmake [Y]/N : 
rem SET /P INPUT-PYTHON=(4)  UnInstall python [Y]/N : 

rem echo you typed %INPUT-CMDTOOLS%
echo .
echo You chose:
echo CMDTOOLS             : %INPUT-CMDTOOLS%
echo CBS-ENVIRONMENT-VARS : %INPUT-CBS-ENVIRONMENT-VARS%
rem echo CMAKE            : %INPUT-CMAKE%
rem echo PYTHON           : %INPUT-PYTHON%
echo .


IF "%INPUT-CBS-ENVIRONMENT-VARS%" == "n"  GOTO CMDTOOLS
echo (2)  UnInstalling CBS environment vars ...
call setenv -u CBS_ROOT_DIR -delete
call setenv -u CBS_ROOT_DIR_BACKSLASH -delete

:CMDTOOLS
IF "%INPUT-CMDTOOLS%" == "n"  GOTO CMAKE
echo (1)  UnInstalling command tools: setenv.exe, sed.exe, wget.exe ...
del %WINDIR%\setenv.exe
del %WINDIR%\wget.exe
del %WINDIR%\sed.exe


:CMAKE
IF "%INPUT-CMAKE%" == "n"  GOTO PYTHON
echo (3)  UnInstalling cmake [ NOT IMPLEMENTED YET ] ...

:PYTHON
IF "%INPUT-PYTHON%" == "n"  GOTO END-CBS
echo (4)  UnInstalling python [ NOT IMPLEMENTED YET ] ...

:END-CBS
echo Done!

chdir /d %SAVE_DIR_CBS_UNINSTALL%


