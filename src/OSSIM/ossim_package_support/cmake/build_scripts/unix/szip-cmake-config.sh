#!/bin/sh

# ---
# File: szip-cmake-config.sh
# ---

build_dir=$OSSIM_DEV_HOME/
install_dir=$OSSIM_INSTALL_PREFIX

cmake -G "Unix Makefiles" \
-DBUILD_SHARED_LIBS=ON \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INSTALL_PREFIX=$install_dir \
$build_dir/szip/latest
