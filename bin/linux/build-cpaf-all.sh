#!/bin/bash


# Initialize which libraries to build 
bReleaseStatic=y
bReleaseShared=n
bDebugStatic=y
bDebugShared=n

# -----------------------------------
# --- Read command line arguments ---
# -----------------------------------
bInteractive=
sJobs="-j 1"
while getopts 'ij:' OPTION
do
	case $OPTION in
	i)	bInteractive=1
		;;
	j)	sJobs="-j $OPTARG"
		;;
	?)	printf "Usage: %s: [-a] [-b value] args\n" $(basename $0) >&2
		exit 2
		;;
	esac
done
shift $(($OPTIND - 1))

if [ "$bInteractive" ]
then
	printf "Interactive mode requested\n\n"
	printf "Build Release Static [y]/n : "
	read bReleaseStatic	
	if [ "$bReleaseStatic" == "" ] ; then bReleaseStatic=y ; fi
 	
 	printf "\nBuild Release Shared [y]/n : "
	read bReleaseShared	
	if [ "$bReleaseShared" == "" ] ; then bReleaseShared=y ; fi
 	
 	printf "\nBuild Debug Static [y]/n : "
	read bDebugStatic	
	if [ "$bDebugStatic" == "" ] ; then bDebugStatic=y ; fi
 	
 	printf "\nBuild Debug Shared [y]/n : "
	read bDebugShared	
	if [ "$bDebugShared" == "" ] ; then bDebugShared=y ; fi
	
	printf "\n"
fi




# -----------------------------------------------------------------------------------
# --- Find script source dir and from that CPAF_ROOT_DIR, CBS_ROOT_DIR if not set ---
# -----------------------------------------------------------------------------------

SAVE_DIR=`pwd`
pushd $SAVE_DIR

DIR_NAME=`dirname $0`
ABS_DIR_NAME=`cd $DIR_NAME && pwd`

cd $ABS_DIR_NAME


# In case CPAF_ROOT_DIR and/or CBS_ROOT_DIR are not set we try to find them
if [ "$CPAF_ROOT_DIR" == "" ] ; then
	printf "Finding CPAF_ROOT_DIR ...\n"
	CPAF_ROOT_DIR=`cd .. && cd .. && pwd`
fi
if [ "$CBS_ROOT_DIR" == "" ] ; then
	printf "Finding CBS_ROOT_DIR ...\n"
	CBS_ROOT_DIR=`cd .. && cd .. && cd cbs && pwd`
fi

printf "Running with these settings:\n" 
printf "Release Static : $bReleaseStatic\n" 
printf "Release Shared : $bReleaseShared\n" 
printf "Debug   Static : $bDebugStatic\n" 
printf "Debug   Shared : $bDebugShared\n" 
printf "Number of jobs : $sJobs\n"
printf "CBS_ROOT_DIR   : $CBS_ROOT_DIR\n"
printf "CPAF_ROOT_DIR  : $CPAF_ROOT_DIR\n"
printf "\n"

##printf "Remaining arguments are: %s\n" "$*"
##exit

# ----------------------
# --- Debug / Static ---
# ----------------------
if [ "$bDebugStatic" == "y" ] ; then
	printf "\nBuilding Debug Static ...\n\n"
	BUILD_DIR="build_cbs/make_debug_static"
	pushd ../..
	mkdir -p $BUILD_DIR
	cd $BUILD_DIR
	if [ -e "CMakeCache.txt" ] ; then
		rm  "CMakeCache.txt"
	fi
	CBS_ROOT_DIR=$CBS_ROOT_DIR CPAF_ROOT_DIR=$CPAF_ROOT_DIR cmake -D CMAKE_BUILD_TYPE:STRING=Debug -D CBS_BUILD=ON -D CBS_LIB_TYPE_TO_BUILD:STRING=STATIC -G "Unix Makefiles" ../..
	CBS_ROOT_DIR=$CBS_ROOT_DIR CPAF_ROOT_DIR=$CPAF_ROOT_DIR make $sJobs
	popd
fi

# ----------------------
# --- Debug / Shared ---
# ----------------------
if [ "$bDebugShared" == "y" ] ; then
	printf "\nBuilding Debug Shared ...\n\n"
	BUILD_DIR="build_cbs/make_debug_shared"
	pushd ../..
	mkdir -p $BUILD_DIR
	cd $BUILD_DIR
	if [ -e "CMakeCache.txt" ] ; then
		rm  "CMakeCache.txt"
	fi
	CBS_ROOT_DIR=$CBS_ROOT_DIR CPAF_ROOT_DIR=$CPAF_ROOT_DIR cmake -D CMAKE_BUILD_TYPE:STRING=Debug -D CBS_BUILD=ON -D CBS_LIB_TYPE_TO_BUILD:STRING=SHARED -G "Unix Makefiles" ../..
	CBS_ROOT_DIR=$CBS_ROOT_DIR CPAF_ROOT_DIR=$CPAF_ROOT_DIR make $sJobs
	popd
fi

# ----------------------
# --- Release / Static ---
# ----------------------
if [ "$bReleaseStatic" == "y" ] ; then
	printf "\nBuilding Release Static ...\n\n"
	BUILD_DIR="build_cbs/make_release_static"
	pushd ../..
	mkdir -p $BUILD_DIR
	cd $BUILD_DIR
	if [ -e "CMakeCache.txt" ] ; then
		rm  "CMakeCache.txt"
	fi
	CBS_ROOT_DIR=$CBS_ROOT_DIR CPAF_ROOT_DIR=$CPAF_ROOT_DIR cmake -D CMAKE_BUILD_TYPE:STRING=Release -D CBS_BUILD=ON -D CBS_LIB_TYPE_TO_BUILD:STRING=STATIC -G "Unix Makefiles" ../..
	CBS_ROOT_DIR=$CBS_ROOT_DIR CPAF_ROOT_DIR=$CPAF_ROOT_DIR make $sJobs
	popd
fi


# ------------------------
# --- Release / Shared ---
# ------------------------
if [ "$bReleaseShared" == "y" ] ; then
	printf "\nBuilding Release Shared ...\n\n"
	BUILD_DIR="build_cbs/make_release_shared"
	pushd ../..
	mkdir -p $BUILD_DIR
	cd $BUILD_DIR
	if [ -e "CMakeCache.txt" ] ; then
		rm  "CMakeCache.txt"
	fi
	CBS_ROOT_DIR=$CBS_ROOT_DIR CPAF_ROOT_DIR=$CPAF_ROOT_DIR cmake -D CMAKE_BUILD_TYPE:STRING=Release -D CBS_BUILD=ON -D CBS_LIB_TYPE_TO_BUILD:STRING=SHARED -G "Unix Makefiles" ../..
	CBS_ROOT_DIR=$CBS_ROOT_DIR CPAF_ROOT_DIR=$CPAF_ROOT_DIR make $sJobs
	popd
fi


popd

