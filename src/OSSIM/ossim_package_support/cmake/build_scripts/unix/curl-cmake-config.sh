#!/bin/sh

# ---
# File: curl-cmake-config.sh
# ---

build_dir="/work/osgeo";

cmake -G "Unix Makefiles" \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INSTALL_PREFIX=${build_dir}/local \
-DCMAKE_MODULE_PATH=${build_dir}/curl/curl-7.23.1/CMake \
../curl-7.23.1
