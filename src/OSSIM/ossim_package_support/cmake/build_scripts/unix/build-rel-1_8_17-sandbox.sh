#!/bin/bash

#---
# File: build-rel-sandbox.sh
#
# Notes: 
# 1) Relies on several environment variables:
#    OSSIM_BUILD_TYPE
#      Mapped to "CMAKE_BUILD_TYPE" 
#      Should be one of: DEBUG|RELEASE|RELWITHDEBINFO|MINSIZEREL
#    OSSIM_DEV_HOME
#    OSSIM_INSTALL_PREFIX
#---

if [ $# -ne 1 ]
then
  echo "Usage: `basename $0` <path_to_dev_root>"
  exit 1
fi

OSSIM_DEV_HOME=$1

if [ -z "$OSSIM_DEV_HOME" ]; then
   echo "OSSIM_DEV_HOME environment variable is not set!  Exiting..."
   exit 1
fi

OS=unix
CONFIG_DIR=$OSSIM_DEV_HOME/ossim_package_support/cmake/build_scripts/$OS
THREADS=4

#---
# Dependency links:
#---
FFMPEG_BASE=ffmpeg-2.0.1
FFMPEG_URL=http://ffmpeg.org/releases/$FFMPEG_BASE.tar.bz2

GDAL_BASE=gdal-1.10.0
GDAL_URL=http://download.osgeo.org/gdal/1.10.0/$GDAL_BASE.tar.gz

GEOS_BASE=geos-3.4.2
GEOS_URL=http://download.osgeo.org/geos/$GEOS_BASE.tar.bz2

GEOTIFF_BASE=libgeotiff-1.4.0
GEOTIFF_URL=http://download.osgeo.org/geotiff/libgeotiff/$GEOTIFF_BASE.tar.gz

HDF4_BASE=hdf-4.2.9
HDF4_URL=http://www.hdfgroup.org/ftp/HDF/HDF_Current/src/$HDF4_BASE.tar.gz

HDF5_BASE=hdf5-1.8.11
HDF5_URL=http://www.hdfgroup.org/ftp/HDF5/current/src/$HDF5_BASE.tar.gz

OSG_BASE=OpenSceneGraph-3.0.1
OSG_URL=http://www.openscenegraph.org/downloads/stable_releases/OpenSceneGraph-3.0.1/source/$OSG_BASE.zip

# OSSIM_BASE_URL=https://svn.osgeo.org/ossim/trunk
OSSIM_BASE_URL=https://svn.osgeo.org/ossim/branches/v1.8.16

PODOFO_BASE=podofo-0.9.2
PODOFO_URL=http://downloads.sourceforge.net/podofo/$PODOFO_BASE.tar.gz

PROJ_BASE=proj-4.8.0
PROJ_URL=http://download.osgeo.org/proj/$PROJ_BASE.tar.gz

SZIP_BASE=szip-2.1
SZIP_URL=http://www.hdfgroup.org/ftp/lib-external/szip/2.1/src/$SZIP_BASE.tar.gz

TIFF_BASE=tiff-4.0.3
TIFF_URL=http://download.osgeo.org/libtiff/$TIFF_BASE.tar.gz

#---
# Functions:
#----
# automake ./configure build:
function buildPackage()
{ 
   if [ -n $1 ]; then
      pkg=$1 # package
   
      if [ -d $OSSIM_DEV_HOME/$pkg/latest ]; then

         cd $OSSIM_DEV_HOME/$pkg/latest            
         
         # Check for previous build:
         if [ -f $OSSIM_DEV_HOME/$pkg/latest/Makefile ]; then
            command="make clean"
            echo "clean disabled..."
            # echo $command
            # $command
         fi

         CONFIG_FILE=$pkg-automake-config.sh

         if [ ! -f $OSSIM_DEV_HOME/$pkg/latest/$CONFIG_FILE ]; then
            command="cp $CONFIG_DIR/$CONFIG_FILE $OSSIM_DEV_HOME/$pkg/latest/."
            echo $command
            $command
         fi
         
         if [ -f $CONFIG_FILE ]; then
	    
            ./$CONFIG_FILE

            # Some packages getting bad builds with threads...
            # command="make -j $THREADS"
            command="make"
            echo $command
            $command

            command="make install"
            echo $command
            $command

         else
            echo "Missing file: $OSSIM_DEV_HOME/$pkg/${pkg}-automake-config.sh"
            exit 1
         fi

         cd $OSSIM_DEV_HOME 

      else
         echo "No directory: $OSSIM_DEV_HOME/$pkg/latest"
         exit 1
      fi

   fi
}

function buildCmakePackage()
{
   if [ -n $1 ]; then

      pkg=$1 # package
      build_dir=$OSSIM_DEV_HOME/build/build_$pkg
      
      if [ ! -d $build_dir ]; then
         command="mkdir -p $build_dir"
         echo $command
         $command
      fi
         
      if [ -d $build_dir ]; then

         cd $build_dir

         if [ -f $build_dir/Makefile ]; then
            command="make clean"
            echo "$command commented out..."
            # $command
         fi

         if [ -f $build_dir/CMakeCache.txt ]; then
            command="rm $build_dir/CMakeCache.txt"
            echo $command
            $command
         fi
      
         if [ -f $CONFIG_DIR/$pkg-cmake-config.sh ]; then

            command="$CONFIG_DIR/$pkg-cmake-config.sh"
            echo $command
            $command

            command="make -j $THREADS"
            echo $command
            $command

            command="make install"
            echo $command
            $command

            cd $OSSIM_DEV_HOME
         else
            echo "Missing file: $CONFIG_DIR/$pkg-cmake-config.sh"
            exit 1 
         fi
      else
         echo "No directory: $OSSIM_DEV_HOME/$pkg/build"
         exit 1
      fi
   else
      echo "buildCmakePackage ERROR no package arg!!!"
   fi
}

answer=
echo -n "Install common rpms[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
   command="sudo yum install aspell automake cmake cvs expat-devel freeglut-devel freetype-devel gcc-c++ gcc-gfortran git libcurl-devel libjpeg-turbo-devel libxml2-devel minizip-devel qt-devel subversion xemacs xemacs-common xemacs-info xemacs-muse xemacs-packages-base xemacs-packages-extra yasm zlib-devel"
   echo $command
   $command
fi

echo -n "Check out ffmpeg[y/n]: "
read answer
if [ "$answer" == 'y' ]; then

   echo "ffmpeg..."
   cd $OSSIM_DEV_HOME
   mkdir -p $OSSIM_DEV_HOME/ffmpeg
   # git clone git://source.ffmpeg.org/ffmpeg.git ffmpeg-git
   cd $OSSIM_DEV_HOME/ffmpeg
   wget $FFMPEG_URL
   bunzip2 $FFMPEG_BASE.tar.bz2
   tar xvf $FFMPEG_BASE.tar
   ln -s  $FFMPEG_BASE latest
   cd $OSSIM_DEV_HOME
fi

echo -n "Check out gdal[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
   echo "gdal..."
   mkdir -p $OSSIM_DEV_HOME/gdal
   cd $OSSIM_DEV_HOME/gdal
   # svn co  http://svn.osgeo.org/gdal/trunk/gdal gdal-svn
   wget $GDAL_URL
   tar xvzf $GDAL_BASE.tar.gz
   ln -s $GDAL_BASE latest
   cd $OSSIM_DEV_HOME
fi

echo -n "Check out geos[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
   echo "geos..."
   mkdir -p $OSSIM_DEV_HOME/build/build_geos
   mkdir -p $OSSIM_DEV_HOME/geos
   cd $OSSIM_DEV_HOME/geos
   wget $GEOS_URL
   bunzip2 $GEOS_BASE.tar.bz2
   tar xvf $GEOS_BASE.tar
   ln -s $GEOS_BASE latest
   cd $OSSIM_DEV_HOME
fi

echo -n "Check out geotiff[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
   echo "geotiff..."
   mkdir -p $OSSIM_DEV_HOME/build/build_geotiff
   mkdir -p $OSSIM_DEV_HOME/geotiff
   cd $OSSIM_DEV_HOME/geotiff
   wget $GEOTIFF_URL
   tar xvzf $GEOTIFF_BASE.tar.gz
   ln -s $GEOTIFF_BASE latest
   cd $OSSIM_DEV_HOME
fi

echo -n "Check out hdf4[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
   echo "hdf4..."
   mkdir -p $OSSIM_DEV_HOME/build/build_hdf4
   mkdir -p $OSSIM_DEV_HOME/hdf4
   cd $OSSIM_DEV_HOME/hdf4
   wget $HDF4_URL
   tar xvzf $HDF4_BASE.tar.gz
   ln -s $HDF4_BASE latest
   cd $OSSIM_DEV_HOME
fi

echo -n "Check out hdf5[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
   echo "hdf5..."
   mkdir -p $OSSIM_DEV_HOME/build/build_hdf5
   mkdir -p $OSSIM_DEV_HOME/hdf5
   cd $OSSIM_DEV_HOME/hdf5
   wget $HDF5_URL
   tar xvzf $HDF5_BASE.tar.gz
   ln -s $HDF5_BASE latest
   cd $OSSIM_DEV_HOME
fi

answer=
echo -n "Check out libwms[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
   echo "svn co ossimwms..."
   svn co $OSSIM_BASE_URL/libwms $OSSIM_DEV_HOME/libwms
fi

answer=
echo -n "Check out omar[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
    echo "git clone omar..."
    cd $OSSIM_DEV_HOME
    git clone https://github.com/radiantbluetechnologies/omar.git omar
    cd $OSSIM_DEV_HOME
fi

answer=
echo -n "Check out oms[y/n]: "
read answer
if [ "$answer" == 'y' ]; then  
    echo "svn co oms..."
    svn co $OSSIM_BASE_URL/oms $OSSIM_DEV_HOME/oms
fi

echo -n "Check out OpenSceneGraph[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
   echo "geos..."
   mkdir -p $OSSIM_DEV_HOME/build/build_osg
   mkdir -p $OSSIM_DEV_HOME/osg
   cd $OSSIM_DEV_HOME/osg
   wget $OSG_URL
   unzip $OSG_BASE.zip
   ln -s $OSG_BASE latest
   cd $OSSIM_DEV_HOME
fi

answer=
echo -n "Check out ossim[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
    echo "svn co ossim..."
    svn co $OSSIM_BASE_URL/ossim $OSSIM_DEV_HOME/ossim
fi

answer=
echo -n "Check out ossimGui[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
    echo "svn co ossimGui..."
    svn co $OSSIM_BASE_URL/ossimGui $OSSIM_DEV_HOME/ossimGui
fi

answer=
echo -n "Check out ossimjni[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
    echo "svn co ossimjni..."
    svn co $OSSIM_BASE_URL/ossimjni $OSSIM_DEV_HOME/ossimjni
fi

answer=
echo -n "Check out ossim_junkyard[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
    echo "svn co ossimjunkyard..."
    svn co https://svn.osgeo.org/ossim/ossim_junkyard $OSSIM_DEV_HOME/ossim_junkyard
fi

answer=
echo -n "Check out ossimPlanet[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
    echo "svn co ossimPlanet..."
    svn co $OSSIM_BASE_URL/ossimPlanet $OSSIM_DEV_HOME/ossimPlanet
fi

answer=
echo -n "Check out ossimPlanetQt[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
    echo "svn co ossimPlanetQt..."
    svn co $OSSIM_BASE_URL/ossimPlanetQt $OSSIM_DEV_HOME/ossimPlanetQt
fi

answer=
echo -n "Check out ossimPredator[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
    echo "svn co ossimPredator..."
    svn co $OSSIM_BASE_URL/ossimPredator $OSSIM_DEV_HOME/ossimPredator
fi

answer=
echo -n "Check out ossim_package_support[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
    echo "svn co ossim_package_support..."
    svn co $OSSIM_BASE_URL/ossim_package_support $OSSIM_DEV_HOME/ossim_package_support
fi

answer=
echo -n "Check out ossim_plugins[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
    echo "svn co ossim_plugins..."
    svn co $OSSIM_BASE_URL/ossim_plugins $OSSIM_DEV_HOME/ossim_plugins
fi

answer=
echo -n "Check out ossim_qt4[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
    echo "svn co ossim_qt4..."
    svn co $OSSIM_BASE_URL/ossim_qt4 $OSSIM_DEV_HOME/ossim_qt4
fi

echo -n "Check out podofo[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
   echo "szip..."
   mkdir -p $OSSIM_DEV_HOME/build/build_podofo
   mkdir -p $OSSIM_DEV_HOME/podofo
   cd $OSSIM_DEV_HOME/podofo
   wget $PODOFO_URL
   tar xvzf $PODOFO_BASE.tar.gz
   ln -s $PODOFO_BASE latest
   cd $OSSIM_DEV_HOME
fi

answer=
echo -n "Check out proj[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
   echo "svn co proj..."
   mkdir -p $OSSIM_DEV_HOME/proj
   cd $OSSIM_DEV_HOME/proj
   wget $PROJ_URL
   tar xvzf $PROJ_BASE.tar.gz
   ln -s $PROJ_BASE latest
   cd $OSSIM_DEV_HOME
fi

echo -n "Check out szip[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
   echo "szip..."
   mkdir -p $OSSIM_DEV_HOME/build/build_szip
   mkdir -p $OSSIM_DEV_HOME/szip
   cd $OSSIM_DEV_HOME/szip
   wget $SZIP_URL
   tar xvzf $SZIP_BASE.tar.gz
   ln -s $SZIP_BASE latest
   cd $OSSIM_DEV_HOME
fi

answer=
echo -n "Check out tiff[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
   echo "tiff..."
   mkdir -p $OSSIM_DEV_HOME/tiff
   cd $OSSIM_DEV_HOME/tiff
   wget $TIFF_URL
   tar xvzf $TIFF_BASE.tar.gz
   ln -s $TIFF_BASE latest
   cd $OSSIM_DEV_HOME
fi
#---
# End of get code section:
#---

#---
# Build section:
# 
# Note: Order dependent...
#---

echo -n "Build ffmpeg[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
   buildPackage ffmpeg
fi

echo -n "Build geos[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
   buildCmakePackage geos
fi

# Must build before hdf5 code:
echo -n "Build szip[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
   buildCmakePackage szip
fi

echo -n "Build hdf4[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
   buildCmakePackage hdf4
fi

echo -n "Build hdf5[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
   buildCmakePackage hdf5
fi

echo -n "Build tiff[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
   buildPackage tiff
fi

# Must build before geotiff:
echo -n "Build proj[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
   buildPackage proj
   command="cp $OSSIM_DEV_HOME/proj/latest/src/projects.h $OSSIM_INSTALL_PREFIX/include/."
   echo $command
   $command
fi

echo -n "Build geotiff[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
   buildCmakePackage geotiff
fi

echo -n "Build gdal[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
   buildPackage gdal
fi

echo -n "Build OpenSceneGraph[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
   buildCmakePackage osg
fi

echo -n "Build podofo[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
   buildCmakePackage podofo
fi

echo -n "Build ossim[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
   buildCmakePackage ossim
fi


#---
# End of build section:
#---

exit 0
# End


