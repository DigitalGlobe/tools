#!/bin/sh

# File: gdal-automake-config.sh
# Note: Python needed for QGIS.

prefix=$OSSIM_INSTALL_PREFIX

./configure --prefix=$prefix --with-geos=$prefix/bin/geos-config --with-libtiff=$prefix --with-geotiff=$prefix --with-mrsid=$OSSIM_DEV_HOME/mrsid/latest/Raster_DSDK

