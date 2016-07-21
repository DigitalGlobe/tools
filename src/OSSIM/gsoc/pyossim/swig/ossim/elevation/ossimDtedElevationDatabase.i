/*-----------------------------------------------------------------------------
 * Filename        : ossimDtedElevationDatabase.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimDtedElevationDatabase
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/elevation/ossimElevationDatabase.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/elevation/ossimDtedHandler.h>
#include <OpenThreads/Mutex>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Wrapping class ossimDtedElevationDatabase */
%include "ossim/base/ossimConstants.h"
%include "ossim/elevation/ossimDtedElevationDatabase.h"

#endif
