/*-----------------------------------------------------------------------------
 * Filename        : ossimAlbersProjection.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimMapProjection
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimAlbersProjection.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling operators */
%rename(__cmp__) ossimAlbersProjection::operator==;

%include "ossim/base/ossimConstants.h"
%include "ossim/projection/ossimAlbersProjection.h"

#endif
