/*-----------------------------------------------------------------------------
 * Filename        : ossimPolynomProjection.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimMapProjection
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimPolynomProjection.h>
#include <ossim/projection/ossimOptimizableProjection.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimPolynom.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling assignment operator */
%rename(__cmp__) ossimPolynomProjection::operator==;

/* Handling the reserved function print */
%rename(ossimPolynomProjection_print) ossimPolynomProjection::print;

%include "ossim/base/ossimConstants.h"

/**
 * class for ground to image polynomial projection: (x,y,z)|->(x,y)
 * polynom order is configurable
 */
/* Wrapping the class ossimMapProjection */
%include "ossim/projection/ossimPolynomProjection.h"

#endif
