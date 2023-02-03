#!/bin/sh 

# You must be in the unpacked source's root directory
# E.G:
# png-1.2.6 $ import_new_3rdparty_source.sh SOURCE_NAME VERSION_NUMBER CPAF_SUBDIR
# EXAMPLE :
# png-1.2.6 $ import_new_3rdparty_source.sh png 1.2.6 cul
 

if [ $# -lt 3 ]
# Test number of arguments to script (always a good idea).
then
    echo
    echo "You must be in the unpacked source's root directory"
    echo
    echo "Usage:   `basename $0` SOURCE_NAME VERSION_NUMBER CPAF_SUBDIR"
    echo "Example: png-1.2.6 $ `basename $0` png 1.2.6 cul"
    echo
    exit 0
fi

echo "Source Name       : $1"
echo "Source Version    : $2"
echo "CPAF Subdirectory : $3"

svn import . https://cpaf.svn.sourceforge.net/svnroot/cpaf/vendor/$1/current -m "Initial $1 vendor drop version $2"
svn copy https://cpaf.svn.sourceforge.net/svnroot/cpaf/vendor/$1/current https://cpaf.svn.sourceforge.net/svnroot/cpaf/vendor/$1/$2 -m "Tagging $1 version $2"  
svn copy https://cpaf.svn.sourceforge.net/svnroot/cpaf/vendor/$1/$2 https://cpaf.svn.sourceforge.net/svnroot/cpaf/trunk/cpaf/$3/$1 -m "Bringing $1 version $2 into main branch" 

echo
echo "svn import . https://cpaf.svn.sourceforge.net/svnroot/cpaf/vendor/$1/current -m \"Initial $1 vendor drop version $2\""
echo 
echo "svn copy https://cpaf.svn.sourceforge.net/svnroot/cpaf/vendor/$1/current https://cpaf.svn.sourceforge.net/svnroot/cpaf/vendor/$1/$2 -m \"Tagging $1 version $2\""  
echo
echo "svn copy https://cpaf.svn.sourceforge.net/svnroot/cpaf/vendor/$1/$2 https://cpaf.svn.sourceforge.net/svnroot/cpaf/trunk/cpaf/$3/$1 -m \"Bringing $1 version $2 into main branch\"" 
echo

