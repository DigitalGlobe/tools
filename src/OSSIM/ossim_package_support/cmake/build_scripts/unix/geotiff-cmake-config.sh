#!/bin/sh

# File: geotiff-cmake-config.sh

build_dir=$OSSIM_DEV_HOME
install_dir=$OSSIM_INSTALL_PREFIX

cmake -G "Unix Makefiles" \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INCLUDE_PATH=$install_dir/include \
-DCMAKE_INSTALL_PREFIX=$install_dir \
-DCMAKE_LIBRARY_PATH=$install_dir/lib \
-DCMAKE_MODULE_PATH=$build_dir/ossim_package_support/cmake/CMakeModules \
-DGEOTIFF_ENABLE_INCODE_EPSG=ON \
-DWITH_JPEG=ON \
-DWITH_PROJ4=ON \
-DWITH_TIFF=ON \
-DWITH_ZLIB=ON \
$build_dir/geotiff/latest
