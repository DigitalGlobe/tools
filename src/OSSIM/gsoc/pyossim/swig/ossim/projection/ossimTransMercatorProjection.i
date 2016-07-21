/*-----------------------------------------------------------------------------
 * Filename        : ossimTransMercatorProjection.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimTransMercatorProjection
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimTransMercatorProjection.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling operators */
%rename(__cmp__) ossimTransMercatorProjection::operator==;
%rename(ossimTransMercatorProjection_print) print;

%include "ossim/base/ossimConstants.h"

/* Wrapping class ossimTransMercatorProjection */
%include "ossim/projection/ossimTransMercatorProjection.h"

#endif
