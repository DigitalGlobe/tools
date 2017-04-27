/*-----------------------------------------------------------------------------
 * Filename        : ossimSrtmElevationDatabase.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimSrtmElevationDatabase
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/elevation/ossimSrtmElevationDatabase.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

%include "ossim/base/ossimConstants.h"

/* Wrapping class ossimSrtmElevationDatabase */
%include "ossim/elevation/ossimSrtmElevationDatabase.h"

#endif
