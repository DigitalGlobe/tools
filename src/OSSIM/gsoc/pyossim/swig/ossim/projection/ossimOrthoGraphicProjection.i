/*-----------------------------------------------------------------------------
 * Filename        : ossimOrthoGraphicProjection.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimOrthoGraphicProjection
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimOrthoGraphicProjection.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Wrapping class ossimOrthoGraphicProjection */
%include "ossim/projection/ossimOrthoGraphicProjection.h"

#endif
