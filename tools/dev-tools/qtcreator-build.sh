#!/bin/bash

# Note: Tested on Ubuntu 22.04
# https://github.com/qt-creator/qt-creator
# See: https://wiki.qt.io/Building_Qt_Creator_from_Git_on_Ubuntu_22.04
#

startup_dir="$( pwd )"
this_script_source_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"


# --------------------------------
# --- Install needed libraries ---
# --------------------------------
# Qt libs
sudo apt install -y qt6-base-dev qt6-base-private-dev qt6-declarative-dev qt6-declarative-private-dev qt6-tools-dev qt6-tools-private-dev qt6-scxml-dev qt6-documentation-tools libqt6core5compat6-dev qt6-tools-dev-tools qt6-l10n-tools qt6-shader-baker libqt6shadertools6-dev qt6-quick3d-dev qt6-quick3d-dev-tools libqt6svg6-dev libqt6quicktimeline6-dev libqt6serialport6-dev

# Clang tools
sudo apt install -y clang clangd libclang-dev
sudo apt install -y clang-15 clangd-15 libclang-15-dev
#sudo apt install -y clang-14 clangd-14 libclang-14-dev

#  Install Qt Creator dependenciess
sudo apt install -y libgl1-mesa-dev libvulkan-dev libxcb-xinput-dev libxcb-xinerama0-dev libxkbcommon-dev libxkbcommon-x11-dev libxcb-image0 libxcb-keysyms1 libxcb-render-util0 libxcb-xkb1 libxcb-randr0 libxcb-icccm4

# Extra needed for generating package etc.
sudo apt install -y build-essential binutils lintian debhelper dh-make devscripts chrpath cmake


if [ -d /tmp/qtcreator ]; then
    rm -rf /tmp/qtcreator
fi

mkdir -p /tmp/qtcreator/qtcreator_build
cd /tmp/qtcreator

git clone https://code.qt.io/qt-creator/qt-creator.git
cd qt-creator
git submodule update --init --recursive

cd /tmp/qtcreator/qtcreator_build
cmake -D CMAKE_BUILD_TYPE=Release ../qt-creator
make -j6


# ------------------------------------------------------------
# --- Set up for debian package creation WITH dependencies ---
# ------------------------------------------------------------
#echo 'set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS "ON")' >> /tmp/qtcreator/qtcreator_build/CPackConfig.cmake
cpack -G DEB


# This is done by adding a line in CPackConfig.cmake : set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS "ON")
# if [[ -f /tmp/new.hosts ]]; then
#     rm -f /tmp/new.hosts
# fi
#
# cat /etc/hosts | grep -v ADDED_BY_AUTOMATED_INSTALLER > /tmp/new.hosts
#
# echo '# --- Create aliases for servers needed by build system ADDED_BY_AUTOMATED_INSTALLER' >> /tmp/new.hosts
# echo "${BUILD_ARCHIVE_IP}         build-archive   # ADDED_BY_AUTOMATED_INSTALLER" >> /tmp/new.hosts
# echo "${REPOSITORIES_IP}          cph1-eud-rep001 # ADDED_BY_AUTOMATED_INSTALLER" >> /tmp/new.hosts
#
# sudo cp -f /tmp/new.hosts /etc/hosts
# rm -f /tmp/new.hosts


# TODO:
# in file CPackConfig.cmake
# REPLACE
# set(CPACK_BINARY_DEB "OFF")
# WITH
# set(CPACK_BINARY_DEB "ON")
# set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS "ON")


# make package
#   842  make package
#   846  less CPackConfig.cmake
#   847  cpack -G DEB
#   849  dpkg -I QtCreator-11.0.82-Linux.deb
#   852  apt-cache depends qtcreator
#   851  apt-cache rdepends qtcreator
#  sudo apt install ./package.deb

cd ${startup_dir}
