/*-----------------------------------------------------------------------------
 * Filename        : ossimTransCylEquAreaProjection.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimMapProjection
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimTransCylEquAreaProjection.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

%rename(__cmp__) ossimTransCylEquAreaProjection::operator==;

%include "ossim/base/ossimConstants.h"

/* Wrapping the class ossimMapProjection */
%include "ossim/projection/ossimTransCylEquAreaProjection.h"

#endif
