/*-----------------------------------------------------------------------------
 * Filename        : ossimObliqueMercatorProjection.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimObliqueMercatorProjection
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimObliqueMercatorProjection.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling operators */
%rename(__cmp__) ossimObliqueMercatorProjection::operator==;

%include "ossim/base/ossimConstants.h"

/* Wrapping class ossimObliqueMercatorProjection */
%include "ossim/projection/ossimObliqueMercatorProjection.h"

#endif
