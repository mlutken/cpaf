@echo off
SET SAVE_DIR_CPAF_UNINSTALL=%CD%

set INPUT-CPAF-ENVIRONMENT-VARS=y

SET /P INPUT-CPAF-ENVIRONMENT-VARS=(2)  UnInstall CPAF environment variables [Y]/N : 

echo .
echo You chose:
echo CPAF-ENVIRONMENT-VARS : %INPUT-CPAF-ENVIRONMENT-VARS%
echo .

IF "%INPUT-CPAF-ENVIRONMENT-VARS%" == "n"  GOTO UNINSTALL-CBS
echo (2)  UnInstalling CPAF environment vars ...
call setenv -u CPAF_ROOT_DIR -delete
call setenv -u CPAF_ROOT_DIR_BACKSLASH -delete


:UNINSTALL-CBS
echo (1)  Running UnInstall in cbs directory ...
cd ..\..\cbs\bin\windows
call uninstall.bat
chdir /d %SAVE_DIR_CPAF_UNINSTALL%

echo Done!
