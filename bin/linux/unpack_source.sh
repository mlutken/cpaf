#!/bin/sh 

unpacked_src_DIR="unpacked_src"

SAVE_DIR_CPAF_BIN_LINUX=`pwd`
cd ../..

if [ ! -e $unpacked_src_DIR ]
then
	mkdir $unpacked_src_DIR
fi

cd src_zips

# # list=`echo *`
# # for file in $list
# # do
# #  	echo "Unpacking ${file} ..."
# #  	unzip -q ${file} -d ../unpacked_src/ 
# # done

#unzip -q zlib-1.2.8.zip -d ../unpacked_src/
#unzip -q RCF-1.2.zip -d ../unpacked_src/ 
#7z x boost_1_55_0.7z -o../unpacked_src/

# ..\bin\windows\7za.exe x ..\src_zips\lpng1610.7z
# ..\bin\windows\7za.exe x ..\src_zips\zlib-1.2.8.zip
# ..\bin\windows\7za.exe x ..\src_zips\bzip2-1.0.6.zip
# ..\bin\windows\7za.exe x ..\src_zips\jpegsr9a.zip
# ..\bin\windows\7za.exe x ..\src_zips\openssl-1.0.1f.7z
# ..\bin\windows\7za.exe x ..\src_zips\curl-7.36.0.7z
# ..\bin\windows\7za.exe x ..\src_zips\boost_1_55_0.7z
# ..\bin\windows\7za.exe x ..\src_zips\RCF-1.2.zip



cd ${SAVE_DIR_CPAF_BIN_LINUX}


# ----------------------------------------
# --- Install PATH and enviroment vars ---
# ----------------------------------------

