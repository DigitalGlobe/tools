#!/bin/sh

###
#
# $Id: ossim_plugins-cmake-config-fc12-v1.sh 19754 2011-06-14 13:57:02Z dburken $
#
# Convenience script for ossim_plugins cmake command.  Edit as needed.  
# 
# After editing do:
#
# cd build_dir/ossim
# chmod 775 ossim-cmake-config-v1.sh
# ./ossim-cmake-config-v1.sh
#
# This was run on a machine with the following operating system:
# 
# Fedora release 12 (Constantine)
#
###

build_dir="/work/drb";

cmake -G "Unix Makefiles" \
-DBUILD_LIBRARY_DIR=lib \
-DBUILD_OSSIM_PLUGIN=ON  \
-DBUILD_OSSIMCONTRIB_PLUGIN=OFF \
-DBUILD_OSSIMCSM_PLUGIN=OFF \
-DBUILD_OSSIMGDAL_PLUGIN=ON \
-DBUILD_OSSIMKAKADU_PLUGIN=ON \
-DBUILD_OSSIMLIBRAW_PLUGIN=ON \
-DBUILD_OSSIMMRSID_PLUGIN=ON \
-DBUILD_OSSIMNDF_PLUGIN=OFF \
-DBUILD_OSSIMNUI_PLUGIN=OFF \
-DBUILD_OSSIMPNG_PLUGIN=ON \
-DBUILD_OSSIMREGISTRATION_PLUGIN=ON \
-DBUILD_RUNTIME_DIR=bin \
-DBUILD_SHARED_LIBS=ON \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INCLUDE_PATH=${build_dir}/local/include \
-DCMAKE_INSTALL_PREFIX=${build_dir}/local \
-DCMAKE_LIBRARY_PATH=${build_dir}/local/lib \
-DCMAKE_MODULE_PATH=${build_dir}/ossim_package_support/cmake/CMakeModules \
-DKAKADU_ROOT_SRC=${build_dir}/kakadu/latest \
-DKAKADU_LIBRARY=${build_dir}/local/lib/libkdu_v63R.so
