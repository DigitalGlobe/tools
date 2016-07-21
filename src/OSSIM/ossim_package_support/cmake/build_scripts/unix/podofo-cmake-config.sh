#!/bin/sh

# ---
# File: podofo-cmake-config.sh
# ---

build_dir=$OSSIM_DEV_HOME
install_dir=$OSSIM_INSTALL_PREFIX

cmake -G "Unix Makefiles" \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INSTALL_PREFIX=$install_dir \
-DPODOFO_BUILD_SHARED=ON \
$build_dir/podofo/latest
