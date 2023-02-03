echo off
SET SAVE_DIR_CPAF_INSTALL=%CD%

SET unpacked_src_DIR=unpacked_src

cd ..\..

md %unpacked_src_DIR%
cd %unpacked_src_DIR%

rem ..\bin\windows\7za.exe x ..\src_zips\libpng-1.5.1.zip
..\bin\windows\7za.exe x ..\src_zips\lpng1610.7z
..\bin\windows\7za.exe x ..\src_zips\zlib-1.2.8.zip
..\bin\windows\7za.exe x ..\src_zips\bzip2-1.0.6.zip
rem ..\bin\windows\7za.exe x ..\src_zips\jpegsr6b.zip
..\bin\windows\7za.exe x ..\src_zips\jpegsr9a.zip
rem ..\bin\windows\7za.exe x ..\src_zips\openssl-0.9.8j.zip
..\bin\windows\7za.exe x ..\src_zips\openssl-1.0.1f.7z
rem ..\bin\windows\7za.exe x ..\src_zips\curl-7.19.4.zip
..\bin\windows\7za.exe x ..\src_zips\curl-7.36.0.7z
rem ..\bin\windows\7za.exe x ..\src_zips\boost_1_53_0.zip
..\bin\windows\7za.exe x ..\src_zips\boost_1_55_0.7z
..\bin\windows\7za.exe x ..\src_zips\RCF-1.2.zip


chdir /d %SAVE_DIR_CPAF_INSTALL%
