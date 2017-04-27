/*-----------------------------------------------------------------------------
 * Filename        : ossimBonneProjection.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimMapProjection
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimBonneProjection.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling ossimHistogramRemapper assignment operator */
%rename(__cmp__) ossimBonneProjection::operator==;

%include "ossim/base/ossimConstants.h"
%include "ossim/projection/ossimBonneProjection.h"

#endif
