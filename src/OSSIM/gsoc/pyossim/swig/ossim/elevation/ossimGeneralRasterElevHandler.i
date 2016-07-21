/*-----------------------------------------------------------------------------
 * Filename        : ossimGeneralRasterElevHandler.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimGeneralRasterElevHandler
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/elevation/ossimGeneralRasterElevHandler.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling operators */
%rename(__set__) ossimGeneralRasterElevHandler::operator=;

%include "ossim/base/ossimConstants.h"

/* Wrapping class ossimGeneralRasterElevHandler */
%include "ossim/elevation/ossimGeneralRasterElevHandler.h"

#endif
