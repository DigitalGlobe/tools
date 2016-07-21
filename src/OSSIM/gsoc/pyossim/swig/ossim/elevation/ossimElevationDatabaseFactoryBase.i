/*-----------------------------------------------------------------------------
* Filename        : ossimElevationDatabaseFactoryBase.i
* Author          : Vipul Raheja
* License         : See top level LICENSE.txt file.
* Description     : Contains SWIG-Python of class ossimElevationDatabaseFactoryBase
* -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimObjectFactory.h>
#include <ossim/elevation/ossimElevationDatabase.h>
%}

/* Wrapping class ossimElevationDatabaseFactoryBase */   
%include "ossim/base/ossimObjectFactory.h"
%include "ossim/elevation/ossimElevationDatabaseFactoryBase.h"
