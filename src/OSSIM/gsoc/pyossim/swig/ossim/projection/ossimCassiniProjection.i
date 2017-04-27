/*-----------------------------------------------------------------------------
 * Filename        : ossimCassiniProjection.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimMapProjection
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimCassiniProjection.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

%include "ossim/base/ossimConstants.h"

/* Wrapping the class ossimMapProjection */
%include "ossim/projection/ossimCassiniProjection.h"

#endif
