#!/bin/sh 

SAVE_DIR_CBS_BIN=`pwd`
USER_HOME=`cd && pwd`

cd ../..
pwd
. ./env.cbs
cd ${SAVE_DIR_CBS_BIN}

# --------------------------------
# --- Find bash file to modify ---
# --------------------------------
BASH_FILE=${USER_HOME}/.bashrc
if [ ! -f ${BASH_FILE} ]; then
	BASH_FILE=${USER_HOME}/.profile
fi
echo "BASH_FILE: $BASH_FILE"

# ----------------------------------------
# --- Install PATH and enviroment vars ---
# ----------------------------------------

# First get a copy of .profile with all webminer specific lines removed
cat ${BASH_FILE} | grep -v ADDED_BY_CBS_INSTALLER > new.profile

# Add our enviroment vars + PATH extension line
echo 'export CBS_ROOT_DIR='"${CBS_ROOT_DIR}" '# ADDED_BY_CBS_INSTALLER' >> new.profile
echo 'export PATH=${CBS_ROOT_DIR}:${PATH} # ADDED_BY_CBS_INSTALLER' >> new.profile

# overwrite original .profile with our new one and delete the new.profile
cp -f ./new.profile ${BASH_FILE}
rm ./new.profile

echo "Please run 'source ../../env.cbs' or logout and login before continuing"
