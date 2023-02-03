#!/bin/bash
cpaf_thirdparty_startup_dir=`pwd`

CPAF_VERSION_MAJOR=1
CPAF_VERSION_MINOR=0

cpaf_root_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cpaf_install_dir="${HOME}/cpaf/${CPAF_VERSION_MAJOR}.${CPAF_VERSION_MINOR}/install"
root_build_dir="${HOME}/cpaf/${CPAF_VERSION_MAJOR}.${CPAF_VERSION_MINOR}/build"
base_external_sources_dir="${HOME}/cpaf/${CPAF_VERSION_MAJOR}.${CPAF_VERSION_MINOR}/external_sources"
webcc_source_dir="${cpaf_root_dir}/cul/webcc"

if [ ! -d ${cpaf_install_dir} ];            then mkdir -p ${cpaf_install_dir}; fi
if [ ! -d ${root_build_dir} ];              then mkdir -p ${root_build_dir}; fi
if [ ! -d ${base_external_sources_dir} ];   then mkdir -p ${base_external_sources_dir}; fi


# mkdir -p ${cpaf_root_dir}/../../_build
# root_build_dir="$( cd ${cpaf_root_dir}/../../_build && pwd )"
#
# mkdir -p ${cpaf_root_dir}/../../_build/install/cpaf
# cpaf_install_dir="$( cd ${cpaf_root_dir}/../../_build/install/cpaf && pwd )"

# base_external_sources_dir=${HOME}/temp/cpaf_3rdparty

echo "cpaf_install_dir          : ${cpaf_install_dir}"
echo "webcc_source_dir          : ${webcc_source_dir}"
echo "base_external_sources_dir : ${base_external_sources_dir}"
echo "root_build_dir            : ${root_build_dir}"


# -----------------------------
# --- Detect system bitwith ---
# -----------------------------
BITWIDTH=64


LINUX_DIST="linux"
WIDTH=${BITWIDTH}
JOBS=6

# -------------------------------------
# --- Parse command line parameters ---
# -------------------------------------
for i in $*
do
	case $i in
        --jobs=*)
        JOBS=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
        ;;
    	-h|--help)
		echo "Options:"
        echo "  --jobs=[6]"
        echo "    Set to number of jobs to use when compiling"
        echo " "
		exit
		;;
    	--default)
		DEFAULT=YES
		;;
    	*)
                # unknown option
		;;
  	esac
done


echo "LINUX_DIST        : $LINUX_DIST"
echo "WIDTH             : $WIDTH"
echo "JOBS              : $JOBS"

# Ensure we have the external source directory created before we try to git clone 
# anything there.
if [ ! -d ${base_external_sources_dir} ]; then mkdir -p ${base_external_sources_dir}; fi

# -------------
# --- webcc ---
# -------------
function build_webcc()
{
    build_dir=${root_build_dir}/webcc
    mkdir -p ${build_dir}

    cd ${build_dir}
    rm -rf *

    echo cmake -G"Unix Makefiles" -DCMAKE_INSTALL_PREFIX=${cpaf_install_dir} -DCMAKE_BUILD_TYPE=MinSizeRel -DWEBCC_ENABLE_LOG=1  -DWEBCC_LOG_LEVEL=0 -DWEBCC_ENABLE_SSL=1 -DWEBCC_ENABLE_GZIP=1 -DWEBCC_ENABLE_AUTOTEST=OFF -DWEBCC_ENABLE_UNITTEST=OFF -DWEBCC_ENABLE_EXAMPLES=ON ${webcc_source_dir}

    cmake -G"Unix Makefiles" -DCMAKE_CXX_FLAGS=-fPIC -DCMAKE_INSTALL_PREFIX=${cpaf_install_dir} -DCMAKE_BUILD_TYPE=MinSizeRel -DWEBCC_ENABLE_LOG=1  -DWEBCC_LOG_LEVEL=0 -DWEBCC_ENABLE_SSL=1 -DWEBCC_ENABLE_GZIP=1 -DWEBCC_ENABLE_AUTOTEST=OFF -DWEBCC_ENABLE_UNITTEST=OFF -DWEBCC_ENABLE_EXAMPLES=ON ${webcc_source_dir} 

    VERBOSE=0 make -j ${JOBS}
    make install
}

# ------------------
# --- libtorrent ---
# ------------------
function build_libtorrent()
{
    source_dir="${base_external_sources_dir}/libtorrent"
    build_dir=${root_build_dir}/libtorrent
    mkdir -p ${build_dir}

    if [ ! -d ${source_dir} ];
    then
        cd ${base_external_sources_dir}
        git clone --recurse-submodules https://github.com/arvidn/libtorrent.git
    else
        cd ${source_dir}
        git pull --recurse-submodules
    fi

    cd ${build_dir}
    rm -rf *
    pwd
    echo cmake -G"Unix Makefiles" -DCMAKE_INSTALL_PREFIX=${cpaf_install_dir} -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DCMAKE_CXX_STANDARD=20 ${source_dir}
    
    cmake -G"Unix Makefiles" -DCMAKE_INSTALL_PREFIX=${cpaf_install_dir} -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DCMAKE_CXX_STANDARD=20 ${source_dir}

    VERBOSE=1 make -j ${JOBS}
    make install
}


build_webcc
build_libtorrent


cd ${cpaf_thirdparty_startup_dir}

