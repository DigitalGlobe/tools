/*-----------------------------------------------------------------------------
 * Filename        : ossimImageElevationHandler.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimImageElevationHandler
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/elevation/ossimImageElevationHandler.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

%include "ossim/base/ossimConstants.h"

/* Wrapping class ossimImageElevationHandler */
%include "ossim/elevation/ossimImageElevationHandler.h"

#endif
