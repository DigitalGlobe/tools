/*-----------------------------------------------------------------------------
 * Filename        : ossimMapProjection.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimMapProjection
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimMapProjection.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling ossimHistogramRemapper assignment operator */
%rename(__cmp__) ossimMapProjection::operator==;

/* Handling the reserved function print */
%rename(ossimMapProjection_print) ossimMapProjection::print;

%include "ossim/base/ossimConstants.h"

/* Wrapping the class ossimMapProjection */
%include "ossim/projection/ossimMapProjection.h"

#endif
