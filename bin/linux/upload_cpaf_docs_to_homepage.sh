#!/bin/sh 

# TODO: How to compare strings here ?? 
#if [ "$1" -eq "--help" ]
#then
#  echo
#  echo "Usage: `basename $0` username"
#  echo
#  echo Arg1   :  username on SourceForge
#  exit 0
#fi

echo Uploading CPAF documents/homepage to web server ...

TAR_FILE=cpaf_homepage.tar.bz2
cd ../../build_cbs/CPAF-Documentation
rm -f ${TAR_FILE}
tar cjf ${TAR_FILE} *
ssh ${1}@shell.sf.net "cd /home/groups/c/cp/cpaf/htdocs && rm -rf *"
scp ${TAR_FILE} ${1}@shell.sf.net:/home/groups/c/cp/cpaf/htdocs
ssh ${1}@shell.sf.net "cd /home/groups/c/cp/cpaf/htdocs && tar xjf ${TAR_FILE} && rm -f ${TAR_FILE}"
cd ../../bin/linux

