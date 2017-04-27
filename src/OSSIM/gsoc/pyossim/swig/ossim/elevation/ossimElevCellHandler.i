/*-----------------------------------------------------------------------------
 * Filename        : ossimElevCellHandler.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimElevCellHandler
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/elevation/ossimElevCellHandler.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling operators */
%rename(__set__) ossimElevCellHandler::operator=;

%include "ossim/base/ossimConstants.h"

/* Wrapping class ossimElevCellHandler */
%include "ossim/elevation/ossimElevCellHandler.h"

#endif
