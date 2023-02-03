#!/bin/bash

SAVE_DIR_CPAF_BIN=`pwd`

# --------------------------------------
# --- Detect linux distribution name ---
# --------------------------------------
APT_GET=`which apt-get`
DNF=`which dnf`
SYSNAME="Suse"
if [ "${APT_GET}" != "" ]; then  SYSNAME="Ubuntu";
elif [ "${DNF}" != "" ]; then  SYSNAME="Fedora";
fi

echo "System name: $SYSNAME"
if [ "$SYSNAME" == "Ubuntu" ]
then
    # ---------------------------
    # --- Ubuntu Install libs ---
    # ---------------------------
	echo "System is Ubuntu"
	sudo snap install ruby --classic
	sudo apt-get install -y ninja-build
	sudo apt-get install -y python2
	sudo apt-get install -y python3
	sudo apt-get install -y python-is-python3
	sudo apt-get install -y cmake
	sudo apt-get install -y build-essential
	sudo apt-get install -y libboost-all-dev
	sudo apt-get install -y libcurl4-gnutls-dev
    sudo apt-get install -y libssl-dev
    sudo apt-get install -y libbz2-dev
    sudo apt-get install -y libz-dev
    sudo apt-get install -y libpthread-*
    sudo apt-get install -y libqt5webchannel5*
    sudo apt-get install -y libqt5quick5
    sudo apt-get install -y qtmultimedia5-*
    sudo apt-get install -y qtquickcontrols*-
    sudo apt-get install -y libqt5svg*-dev
    sudo apt-get install -y libvlc*-dev
    sudo apt-get install -y vlc-plugin-qt
    sudo apt-get install -y libavfilter-dev
    sudo apt-get install -y libavdevice-dev
    sudo apt-get install -y libopenal-dev libpulse-dev libva-dev libxv-dev libass-dev libegl1-mesa-dev
    sudo apt install -y portaudio*-dev
    sudo apt install -y yasm nasm
    sudo apt install -y mercurial
    sudo apt install -y libsdl2-*dev
    sudo apt install -y libglew-dev
    
elif [ "$SYSNAME" == "Fedora" ]
then
    # -----------------------------------------
    # --- Fedora/CentOS/Redhat Install libs ---
    # -----------------------------------------
	echo "System is Fedora/Centos/Redhat"
	sudo dnf install -y redhat-lsb-core
	sudo dnf install -y make automake gcc gcc-c++ kernel-devel cmake clang ninja-build nano
	sudo dnf install -y libpng-devel zlib-devel boost-devel libcurl-devel curlpp
elif [ "$SYSNAME" == "Suse" ]
then
    # -------------------------
    # --- SuSe Install libs ---
    # -------------------------
	echo "System is Suse"
	sudo zypper install cmake libidn-devel libexpat-devel binutils-devel python-devel openldap2-devel SDL_sound-devel libgnomeprint-devel libgnomeprintui-devel
fi

cd ${SAVE_DIR_CPAF_BIN}


