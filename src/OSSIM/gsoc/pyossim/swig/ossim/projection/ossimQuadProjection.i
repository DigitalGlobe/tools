/*-----------------------------------------------------------------------------
 * Filename        : ossimQuadProjection.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimMapProjection
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimQuadProjection.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimDblGrid.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

%rename(__cmp__) ossimQuadProjection::operator==;

%include "ossim/base/ossimConstants.h"

/* Wrapping the class */
%include "ossim/projection/ossimQuadProjection.h"

#endif
