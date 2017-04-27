/*-----------------------------------------------------------------------------
 * Filename        : ossimSpaceObliqueMercatorProjection.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimMapProjection
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimSpaceObliqueMercatorProjection.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

%rename(__cmp__) ossimSpaceObliqueMercatorProjection::operator==;

%include "ossim/base/ossimConstants.h"

/*!
 * This class implments the formulas for the ellipsoid and
 * circular orbit.  All formulas were directly taken from
 * proj 4.  Proj 4 implementation is from the following
 * manual reference:
 *
 * Map Projections - A Working Manual
 * by John Snyder
 * Space Oblique Mercator p. 221.
 *
 * 
 */
%include "ossim/projection/ossimSpaceObliqueMercatorProjection.h"

#endif
