#!/bin/bash 


SAVE_DIR_STARTUP=`pwd`
USER_HOME=`cd && pwd`
CPAF_BIN_LINUX_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"


echo "USER_HOME: $USER_HOME"
echo "CPAF_BIN_LINUX_DIR $CPAF_BIN_LINUX_DIR"

cd ${CPAF_BIN_LINUX_DIR}

sudo ${CPAF_BIN_LINUX_DIR}/installNeededLibs_RunAsRoot.sh
cd ${CPAF_BIN_LINUX_DIR}

cd ../..
cd cbs/bin/linux
sh ./install.sh
cd ../../..
pwd
. ./env.cpaf
cd ${CPAF_BIN_LINUX_DIR}

# sh ${CPAF_BIN_LINUX_DIR}/unpack_source.sh

# --------------------------------
# --- Find bash file to modify ---
# --------------------------------
BASH_FILE=${USER_HOME}/.bashrc
if [[ ! -f ${BASH_FILE} ]]; then
	BASH_FILE=${USER_HOME}/.profile
fi
echo "BASH_FILE: $BASH_FILE"


# ----------------------------------------
# --- Install PATH and enviroment vars ---
# ----------------------------------------

##exit 0

# First get a copy of .profile with all webminer specific lines removed
cat ${BASH_FILE} | grep -v ADDED_BY_CPAF_INSTALLER > new.profile

# Add our enviroment vars + PATH extension line
echo 'export CPAF_ROOT_DIR='"${CPAF_ROOT_DIR}" '# ADDED_BY_CPAF_INSTALLER' >> new.profile
echo 'export PATH=${CPAF_ROOT_DIR}/bin/linux:${PATH} # ADDED_BY_CPAF_INSTALLER' >> new.profile

# overwrite original .profile with our new one and delete the new.profile
cp -f ./new.profile ${BASH_FILE}
rm ./new.profile

echo "CPAF_ROOT_DIR: '${CPAF_ROOT_DIR}'"
echo "Please run 'source ../../env.cpaf' or logout and login before continuing"

# Build CPAF libs in debug and release
# cd ${CPAF_ROOT_DIR}
# . env.cpaf
# cbmake.py -C d
# cbmake.py -c 1 -b debug -j 4
# cbmake.py -c 1 -b release -j 4
#
# cd ${SAVE_DIR_STARTUP}
