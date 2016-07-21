#!/bin/sh

# ---
# File: hdf5-cmake-config.sh
# ---

build_dir=$OSSIM_DEV_HOME/
install_dir=$OSSIM_INSTALL_PREFIX

cmake -G "Unix Makefiles" \
-DBUILD_SHARED_LIBS=ON \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INSTALL_PREFIX=$install_dir \
-DHDF5_BUILD_CPP_LIB=ON \
$build_dir/hdf5/latest
