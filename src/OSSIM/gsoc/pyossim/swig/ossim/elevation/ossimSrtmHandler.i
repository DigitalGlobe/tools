/*-----------------------------------------------------------------------------
 * Filename        : ossimSrtmHandler.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimSrtmHandler
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/elevation/ossimSrtmHandler.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling operators */
%rename(__set__) ossimSrtmHandler::operator=;

%include "ossim/base/ossimConstants.h"

/* Wrapping class ossimSrtmHandler */
%include "ossim/elevation/ossimSrtmHandler.h"

#endif
