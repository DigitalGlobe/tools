/*-----------------------------------------------------------------------------
 * Filename        : ossimImageElevationDatabase.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimImageElevationDatabase
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/elevation/ossimImageElevationDatabase.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling operators */
%rename(__set__) ossimImageElevationDatabase::operator=;

%include "ossim/base/ossimConstants.h"

/* Wrapping class ossimImageElevationDatabase */
%include "ossim/elevation/ossimImageElevationDatabase.h"

#endif
