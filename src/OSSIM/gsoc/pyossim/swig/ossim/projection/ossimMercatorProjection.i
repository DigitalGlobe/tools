/*-----------------------------------------------------------------------------
 * Filename        : ossimMercatorProjection.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimMercatorProjection
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimMercatorProjection.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling operators */
%rename(__set__) ossimMercatorProjection::operator==;

%include "ossim/base/ossimConstants.h"

/* Wrapping class ossimMercatorProjection */
%include "ossim/projection/ossimMercatorProjection.h"

#endif
