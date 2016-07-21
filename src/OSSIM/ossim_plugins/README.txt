$Id: README.txt 10595 2007-03-07 15:26:48Z dburken $

README file for "Open Source Software Image Map (OSSIM) plugins module".

Web site:  http://www.ossim.org/

Mailing list: http://lists.sourceforge.net/lists/listinfo/ossim-developer 

//---
// SVS access via http:
// This will check out ossim_plugins to current working directory.
// You can use "http" or if you are a committer you should use "https".
//---
svn co http://svn.osgeo.org/ossim/trunk/ossim_plugins

Build instructions:

NOTE:  You must have "gdal" built for the gdal plugin.  There are brief
       instructions for that below.

Asuming you have a gdal library built, for most unix type platforms via 
command line:
1) If you do not have make.opt or a make_platform.opt you should run
"gmake make_options" to create them.

2) Edit the "make.opt" file.  This has specific details for different plugins.

3) Edit the make_platform.opt file if needed.  This is for operating system
overrides.  Usually this shouldn't need to be touched.

4) cd to the plugin directory you witch to build.

5) Type "gmake".


To add your plugin via the ossim preferences:

Note:  
There is an ossim preferences file template in our source code at:
ossim/etc/templates/ossim_preferences_template


1) Set the environment variable "OSSIM_PREFS_FILE".
   tcsh:     setenv OSSIM_PREFS_FILE your_path_to/ossim_preferences
     or
   bash/sh:  export OSSIM_PREFS_FILE=your_path_to/ossim_preferences

2) In your preferences file add the line for your plugin:
plugin.file1: /work/drb/ossim_plugins/lib/libgdalplugin.so


To build gdal here is what I do:

In this example code in parallel in "/work/drb/" directory.

// If you haven't got the code already login to maptools cvs pserver:
cvs -d :pserver:cvsanon@cvs.maptools.org:/cvs/maptools/cvsroot login
password:  <hit return for password>

// Get libtiff
cvs -d :pserver:cvsanon@cvs.maptools.org:/cvs/maptools/cvsroot co libtiff

// Get geotiff
cvs -d :pserver:cvsanon@cvs.maptools.org:/cvs/maptools/cvsroot co geotiff

// Get gdal
cvs -d :pserver:cvsanon@cvs.maptools.org:/cvs/maptools/cvsroot co gdal

// Build libtiff
cd /work/drb/libtiff
cvs -z 3 update -d -P
./configure --prefix=/work/drb/libtiff
make
make install

// Build geotiff
cd /work/drb/geotiff/libgeotiff
cvs -z 3 update -d -P
./configure --prefix=/work/drb/geotiff --with-libtiff=/work/drb/libtiff
make
make install
cd ../..

// Build gdal
cd /work/drb/gdal
cvs -z 3 update -d -P
./configure --prefix=/work/drb/geotiff --with-libtiff=/work/drb/libtiff
make
make install
cd ../..
./configure --prefix=/work/drb/gdal --without-libtool --with-libtiff=/work/drb/libtiff --with-geotiff=/work/drb/geotiff
make
make install

