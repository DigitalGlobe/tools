/*-----------------------------------------------------------------------------
 * Filename        : ossimElevationDatabaseRegistry.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimElevationDatabaseRegistry
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/elevation/ossimElevationDatabaseFactoryBase.h>
#include <ossim/elevation/ossimElevationDatabase.h>
#include <ossim/base/ossimFactoryListInterface.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

%template(ossimFactoryListInterface_ossimElevationDatabaseFactoryBase_ossimElevationDatabase) ossimFactoryListInterface< ossimElevationDatabaseFactoryBase,ossimElevationDatabase >;

%include "ossim/base/ossimConstants.h"

/* Wrapping class ossimElevationDatabaseRegistry */
%include "ossim/elevation/ossimElevationDatabaseRegistry.h"

#endif
