#!/bin/sh

# File: build-sandbox.sh

if [ -z "$OSSIM_DEV_HOME" ]; then
   echo "OSSIM_DEV_HOME environment variable is not set!  Exiting..."
   exit 1
fi

dev=$OSSIM_DEV_HOME
prefix=$OSSIM_DEV_HOME/deps

answer=
command=
threadCount=4

# automake ./configure build:
function buildPackage()
{ 
   if [ -n $1 ]; then
      pkg=$1 # package
   
      if [ -d $dev/$pkg/latest ]; then

         cd $dev/$pkg/latest
         
         if [ -f $dev/$pkg/latest/Makefile ]; then
            command="make clean"
            echo $command
            $command
         fi
         
         if [ -f $dev/$pkg/latest/my${pkg}config.sh ]; then
	    
            ./my${pkg}config.sh

            command="make -j $threadCount"
            echo $command
            $command

            command="make install"
            echo $command
            $command

            cd $dev
         else
            echo "Missing file: $dev/$pkg/my${pkg}config.sh"
         exit 1 
      fi
      else
         echo "No directory: $dev/$pkg/latest"
         exit 1
      fi
   fi
}

function buildCmakePackage()
{
   if [ -n $1 ]; then
      pkg=$1 # package

      if [ ! -d $dev/$pkg/build ]; then
         command="mkdir -p $dev/$pkg/build"
         echo $command
         $command
      fi
         
      if [ -d $dev/$pkg/build ]; then

         cd $dev/$pkg/build

         if [ -f $dev/$pkg/build/Makefile ]; then
            command="make clean"
            echo $command
            $command
         fi

         if [ -f $dev/$pkg/build/CMakeCache.txt ]; then
            command="rm $dev/$pkg/build/CMakeCache.txt"
            echo $command
            $command
         fi
      
         if [ -f $dev/$pkg/$pkg-cmake-config.sh ]; then

            ../$pkg-cmake-config.sh

            command="make -j $threadCount"
            echo $command
            $command

            command="make install"
            echo $command
            $command

            cd $dev
         else
            echo "Missing file: $dev/$pkg/$pkg-cmake-config.sh"
            exit 1 
         fi
      else
         echo "No directory: $dev/$pkg/build"
         exit 1
      fi
   else
      echo "buildCmakePackage ERROR no package arg!!!"
   fi
}

function buildOssim()
{
   if [ -d $dev/build ]; then

      cd $dev/build      

      if [ -f $dev/$pkg/build/Makefile ]; then
         command="make clean"
         echo $command
         $command
      fi
      
      if [ -f $dev/build/CMakeCache.txt ]; then
         rm $dev/build/CMakeCache.txt
      fi

      if [ -f $dev/ossim-cmake-config.sh ]; then

         ../ossim-cmake-config.sh

         command="make -f $threadCount"
         echo $command
         $command

         command="make install"
         echo $command
         $command

         cd $dev
      else
         echo "Missing file: $dev/ossim-cmake-config.sh"
         exit 1 
      fi
   else
      echo "No directory: $dev/build"
      exit 1
   fi
}


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
fi

echo -n "Build geotiff[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
   buildCmakePackage geotiff
fi

# Must build before hdf code:
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

echo -n "Build ossim[y/n]: "
read answer
if [ "$answer" == 'y' ]; then
   buildOssim
fi

exit 0
# End
