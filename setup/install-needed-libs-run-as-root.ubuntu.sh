#!/bin/bash

SAVE_DIR_CPAF_SETUP=`pwd`

sudo apt-get install -y libx11-dev
sudo apt-get install -y libx11-xcb-dev
sudo apt-get install -y libfontenc-dev
sudo apt-get install -y libice-dev
sudo apt-get install -y libsm-dev
sudo apt-get install -y libxau-dev
sudo apt-get install -y libxaw7-dev
sudo apt-get install -y libxcb-cursor-dev
sudo apt-get install -y libvdpau-dev

cd $SAVE_DIR_CPAF_SETUP

