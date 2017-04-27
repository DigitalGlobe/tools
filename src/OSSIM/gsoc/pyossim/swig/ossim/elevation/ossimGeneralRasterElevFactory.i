/*-----------------------------------------------------------------------------
 * Filename        : ossimGeneralRasterElevFactory.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimGeneralRasterElevFactory
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/elevation/ossimGeneralRasterElevFactory.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

%include "ossim/base/ossimConstants.h"

/* Wrapping class ossimGeneralRasterElevFactory */
%include "ossim/elevation/ossimGeneralRasterElevFactory.h"

#endif
