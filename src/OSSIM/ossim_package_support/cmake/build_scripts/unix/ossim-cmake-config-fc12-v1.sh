#!/bin/sh

###
#
# $Id: ossim-cmake-config-fc12-v1.sh 19754 2011-06-14 13:57:02Z dburken $
#
# Convenience script for ossim cmake command.  Edit as needed.  
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
-DBUILD_OSSIM_MPI_SUPPORT=1 \
-DBUILD_OSSIM_TEST_APPS=1 \
-DBUILD_RUNTIME_DIR=bin \
-DBUILD_SHARED_LIBS=ON \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INCLUDE_PATH=${build_dir}/local/include \
-DCMAKE_INSTALL_PREFIX=${build_dir}/local \
-DCMAKE_LIBRARY_PATH=${build_dir}/local/lib \
-DCMAKE_MODULE_PATH=${build_dir}/ossim_package_support/cmake/CMakeModules \
-DBUILD_RUNTIME_DIR=bin \
-DMPI_INCLUDE_PATH=/usr/include/openmpi-x86_64 \
-DMPI_LIBRARY=/usr/lib64/openmpi/lib/libmpi.so.0 \
-DMPI_LIBRARY=/usr/lib64/openmpi/lib/libmpi_cxx.so.0 \
-DOSSIM_COMPILE_WITH_FULL_WARNING=ON;
