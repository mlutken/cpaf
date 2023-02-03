#!/bin/bash

# --------------------------------------
# --- Detect linux distribution name ---
# --------------------------------------
APT_GET=`which apt-get`
DNF=`which dnf`
SYSNAME="Suse"
if [ "${APT_GET}" != "" ]; then  SYSNAME="Ubuntu";
elif [ "${DNF}" != "" ]; then  SYSNAME="Fedora";
fi

echo -n $SYSNAME
