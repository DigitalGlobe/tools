#!/bin/sh

# ---
# File: geos-cmake-config.sh
# ---

build_dir=$OSSIM_DEV_HOME/

build_type=
if [ -z "$OSSIM_BUILD_TYPE" ]; then
   echo "NOTICE: OSSIM_BUILD_TYPE is unset.  Defaulting CMAKE_BUILD_TYPE to RELWITHDEBINFO."
   build_type=RELWITHDEBINFO
else
   build_type=$OSSIM_BUILD_TYPE
fi

install_dir=$OSSIM_INSTALL_PREFIX


cmake -G "Unix Makefiles" \
-DCMAKE_BUILD_TYPE=$build_type \
-DCMAKE_INSTALL_PREFIX=$install_dir \
-DCMAKE_MODULE_PATH=$build_dir/geos/latest/cmake/modules \
$build_dir/geos/latest

