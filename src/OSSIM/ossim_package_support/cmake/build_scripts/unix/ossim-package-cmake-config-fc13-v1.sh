#!/bin/sh

#---
# Sample cmake build script for linux for out of source cmake build.
#
# To run do:
# $ cp ossim-package-cmake-config-fc13.sh <your-build-dir>/ossim-cmake-config.sh
# $ cd <your-build-dir>
#
# Make executable:
# $ chmod 775 ossim-cmake-config.sh
#
# Edit as applicable...
#
# Run the cmake script:
# ./ossim-cmake-config.sh
#
# Compile:
# make
#
# make install (options)
#
# This script was ran on:
# $ uname -a
# Linux tazz 2.6.33.6-147.fc13.x86_64 #1 SMP 
# Tue Jul 6 22:32:17 UTC 2010 x86_64 x86_64 x86_64 GNU/Linux
#
# Any issues please email to mailing list at: 
# ossim-developer@lists.sourceforge.net
#---

#---
# This is the directory where everything is built.  Typically I will make a sub 
# directory to place this script in for my cmake build, e.g. /work/osgeo/build
#---
build_dir="/work/osgeo";


cmake -G "Unix Makefiles" \
-DBUILD_CSMAPI=OFF \
-DBUILD_LIBRARY_DIR=lib \
-DBUILD_OMS=OFF \
-DBUILD_OSSIM=ON \
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
-DBUILD_OSSIMQT4=ON \
-DBUILD_OSSIM_MPI_SUPPORT=0 \
-DBUILD_OSSIMPLANET=OFF \
-DBUILD_OSSIMPLANETQT=OFF \
-DBUILD_OSSIMPREDATOR=OFF \
-DBUILD_OSSIM_TEST_APPS=1 \
-DBUILD_RUNTIME_DIR=bin \
-DBUILD_SHARED_LIBS=ON \
-DBUILD_WMS=ON \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INCLUDE_PATH=${build_dir}/local/include \
-DCMAKE_INSTALL_PREFIX=${build_dir}/local \
-DCMAKE_LIBRARY_PATH=${build_dir}/local/lib \
-DCMAKE_MODULE_PATH=${build_dir}/ossim_package_support/cmake/CMakeModules \
-DKAKADU_ROOT_SRC=/work/drb/kakadu/latest \
-DKAKADU_AUX_LIBRARY=${build_dir}/local/lib64/libkdu_a64R.so \
-DKAKADU_LIBRARY=${build_dir}/local/lib64/libkdu_v64R.so  \
-DMPI_INCLUDE_PATH=/usr/include/openmpi-x86_64 \
-DMPI_LIBRARY=/usr/lib64/openmpi/lib/libmpi.so \
-DMRSID_LIBRARY=${build_dir}/local/lib/libltidsdk.a \
-DOSSIM_COMPILE_WITH_FULL_WARNING=ON \
-DOSSIM_DEPENDENCIES=${build_dir}/local \
-DOSSIM_DEV_HOME=${build_dir} \
-DBUILD_SHARED_LIBS=ON \
../ossim_package_support/cmake/

# -DCMAKE_CXX_FLAGS_RELEASE="-O2 -pg" \
# -DKAKADU_LIBRARY="${build_dir}/local/lib64/libkdu_a64R.so ${build_dir}/local/lib64/libkdu_v64R.so"  \
# -DMPI_LIBRARIES="/usr/lib64/openmpi/lib/libmpi_cxx.so /usr/lib64/openmpi/lib/libmpi.so"\


