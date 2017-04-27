###########################################################################
#    install.sh
#    ---------------------
#    Date                 : March 2016
#    Copyright            : (C) 2016 by Matthias Kuhn
#    Email                : matthias at opengis dot ch
###########################################################################
#                                                                         #
#   This program is free software; you can redistribute it and/or modify  #
#   it under the terms of the GNU General Public License as published by  #
#   the Free Software Foundation; either version 2 of the License, or     #
#   (at your option) any later version.                                   #
#                                                                         #
###########################################################################

mkdir build
cd build

ln -s ${HOME}/osgeo4travis/bin/ccache ${HOME}/osgeo4travis/bin/clang++-${LLVM_VERSION}
ln -s ${HOME}/osgeo4travis/bin/ccache ${HOME}/osgeo4travis/bin/clang-${LLVM_VERSION}
ln -s ${HOME}/osgeo4travis/bin/ccache ${HOME}/osgeo4travis/bin/g++-6
ln -s ${HOME}/osgeo4travis/bin/ccache ${HOME}/osgeo4travis/bin/gcc-6

ccache -s

#export CXX="clang++-${LLVM_VERSION}"
#export CC="clang-${LLVM_VERSION}"
export CXX="g++-6"
export CC="gcc-6"

export PATH=${HOME}/osgeo4travis/bin:${PATH}

cmake --version
${CC} --version
${CXX} --version

# CLANG_WARNINGS="-Wimplicit-fallthrough"
CLANG_WARNINGS=""

# Include this line for debug reasons
#      -DCMAKE_BUILD_TYPE=RelWithDebInfo \
#
cmake \
      -DCMAKE_PREFIX_PATH=/home/travis/osgeo4travis \
      -DWITH_STAGED_PLUGINS=ON \
      -DWITH_GRASS=ON \
      -DSUPPRESS_QT_WARNINGS=ON \
      -DENABLE_MODELTEST=ON \
      -DENABLE_PGTEST=ON \
      -DWITH_QSPATIALITE=ON \
      -DWITH_QWTPOLAR=OFF \
      -DWITH_APIDOC=ON \
      -DWITH_ASTYLE=ON \
      -DWITH_SERVER=ON \
      -DWITH_PYSPATIALITE=ON \
      -DGRASS_PREFIX7=/usr/lib/grass70 \
      -DGRASS_INCLUDE_DIR7=/usr/lib/grass70/include \
      -DCXX_EXTRA_FLAGS="$CLANG_WARNINGS" \
      ..
