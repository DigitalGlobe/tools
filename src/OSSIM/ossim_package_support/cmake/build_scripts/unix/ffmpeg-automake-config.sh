#!/bin/sh

# File: ffmpeg-automake-config.sh

prefix=$OSSIM_INSTALL_PREFIX

./configure --prefix=$prefix --disable-yasm --enable-shared

