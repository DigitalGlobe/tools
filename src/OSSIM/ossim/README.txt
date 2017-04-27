
OSSIM - Open Source Software Image Map

Introduction:

OSSIM, pronounced "awesome" is an open source project to
develop a state of the art set of libraries and tools for remote
sensing and geographic information systems.A test application,
OSSIM, is included in the distribution to showcase the
functionality of the libraries. For a complete list of OSSIM
functionality, point your Internet web browser to
http://www.ossim.org


Environmental Variables:

The various builds will use the following environmental variables, set them to point to the relevant paths in your development environment:

OSSIM_DEPENDENCIES // This should be one up from OSSIM_HOME.
OSSIM_HOME 
OSSIM_PREFS_FILE

for example, in .bashrc on the mac

export OSSIM_HOME=/Users/Shared/Development/ossim
export OSSIM_PREFS_FILE=/Users/mrl/ossim_preferences

Other useful Environmental variables you might want to set for OSSIM related builds:

QT_HOME



Binary distributions and source packages are available from sourceforge at:
http://sourceforge.net/project/showfiles.php?group_id=5891

Getting the source code via cvs, this includes get external packages that
we need/like to have:

//---
// cd into some work directory.  I like to put all packages parallel to 
// each other.
//---

//---
// Get external packages we like to have.  If you have these installed already
// you just need to tell ossim where they're at...  In most cases if you 
// install these parallel to ossim they will be found during a "./configure"
//---

// Get OpenThreads if not installed already (now a required library):
svn co http://www.openscenegraph.org/svn/osg/OpenThreads/trunk OpenThreads

// Login to maptools cvs pserver
cvs -d :pserver:cvsanon@cvs.maptools.org:/cvs/maptools/cvsroot login
password:  <hit return for password>

// Get libtiff
cvs -d :pserver:cvsanon@cvs.maptools.org:/cvs/maptools/cvsroot co libtiff

// The geotiff source code is available via svn (subversion) server on:
http://svn.osgeo.org/metacrs/geotiff/trunk/libgeotiff

// Get geotiff:
svn co http://svn.osgeo.org/metacrs/geotiff/trunk/libgeotiff

// The gdal source code is available via svn (subversion) server on
http://svn.osgeo.org/gdal/trunk/gdal

// Get gdal:
svn co http://svn.osgeo.org/gdal/trunk/gdal gdal

// Logout of cvs pserver
cvs -d :pserver:cvsanon@cvs.maptools.org:/cvs/maptools/cvsroot logout

// The ossim source code is available via svn (subversion) server on:
http://svn.osgeo.org/ossim

// Get ossim core
svn co http://svn.osgeo.org/ossim/trunk/ossim ossim

// Get ossim_qt - this is the old QT3.x gui module with imagelinker and iview
svn co http://svn.osgeo.org/ossim/trunk/ossim_qt ossim_qt

// Get ossim_qt4 - this is the QT4.x port of imagelinker and iview
svn co htts://svn.osgeo.org/ossim/trunk/ossim_qt4 ossim_qt4

// Get ossim_plugins
svn co http://svn.osgeo.org/ossim/trunk/ossim_plugins ossim_plugins

// Elevation geoid grid: egm96.grd (you will need this if you don't have)
svn co http://svn.osgeo.org/ossim/trunk/ossim_package_support/geoids geoids

//---
// Building on unix like machines, linux, sunos and so on.
// Note my workspace directory is "/work/drb".  You should change that out
// to be yours...
//---

//---
// Build libtiff:
// Note that ossim requires "libtiff", won't build without it.  Most systems
// have libtiff installed.  If you need to build it follow directions 
// below.
// 
// Official website at:  
// http://www.remotesensing.org/libtiff
// 
// LIBTIFF Notes:
// My libtiff is in "/work/drb/libiff".  The --prefix will tell it to install
// there rather than user local.
//---
cd /work/drb/libtiff
cvs -z 3 update -d -P
./configure --prefix=/work/drb/libtiff
make
make install

//---
// Build geotiff:
// Note that ossim does not require geotiff; however, it is nice to have.
// Without it none of you geotiffs will be placed on the ground correctly.
// Official website at:  
// http://www.remotesensing.org:16080/websites/geotiff/geotiff.html
//---
cd /work/drb/geotiff/libgeotiff
cvs -z 3 update -d -P
./configure --prefix=/work/drb/geotiff --with-libtiff=/work/drb/libtiff 
make 
make install
cd ../..

cd /work/drb/ossim
cvs -z 3 update -d -P
./configure
make depends;
make

//---
// Sample configure line to build single static libossim.a and static 
// applications.  (linux Fedora Core 3)
// Notes:
// 1) Mpi and freetype were shut off since there were no static 
//    libraries on my machine.
// 2) libtiff and geotiff packages were explicitly set since there were not
//    parallel to the static ossim build.
//---
./configure --enable-singleStaticOssimLibrary=yes --enable-singleSharedOssimLibrary=no --with-mpi=no --with-freetype=no --with-libtiff=/work/drb/libtiff --with-geotiff=/work/drb/geotiff --enable-staticOssimApps
