/*-----------------------------------------------------------------------------
 * Filename        : ossimGeneralRasterElevationDatabase.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimDataObject
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/elevation/ossimElevationDatabase.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/elevation/ossimGeneralRasterElevHandler.h>
#include <ossim/elevation/ossimGeneralRasterElevationDatabase.h>
#include <OpenThreads/Mutex>

%}

#ifndef TYPE_DATA
#define TYPE_DATA

%include "ossim/base/ossimConstants.h"

/* Wrapping class ossimGeneralRasterElevationDatabase */
%include "ossim/elevation/ossimGeneralRasterElevationDatabase.h"

#endif
