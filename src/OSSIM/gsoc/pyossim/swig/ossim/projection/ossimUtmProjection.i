/*-----------------------------------------------------------------------------
 * Filename        : ossimUtmProjection.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimMapProjection
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimUtmProjection.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

%rename(__cmp__) ossimUtmProjection::operator==;

/* Handling the reserved function print */
%rename(ossimUtmProjection_print) ossimUtmProjection::print;

%include "ossim/base/ossimConstants.h"

/* Wrapping the class */
%include "ossim/projection/ossimUtmProjection.h"

#endif
