#!/bin/sh

# ---
# File: osg-cmake-config.sh
#
# OpenSceneGraph
# ---

build_dir=$OSSIM_DEV_HOME

# Edit, uncomment one...
# install_dir=$build_dir/deps
# install_dir=/usr/local/ossim
# install_dir=/opt/radiantblue/ossim
install_dir=$OSSIM_INSTALL_PREFIX

cmake -G "Unix Makefiles" \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_LIBRARY_PATH=$install_dir/lib \
-DCMAKE_INCLUDE_PATH=$install_dir/include \
-DCMAKE_INSTALL_PREFIX=$install_dir \
$build_dir/osg/latest

