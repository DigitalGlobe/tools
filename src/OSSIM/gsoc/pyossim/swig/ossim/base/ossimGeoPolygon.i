/*-----------------------------------------------------------------------------
 * Filename        : ossimGeoPolygon.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimLsrPoint
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <vector>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimGeoPolygon.h>
%}

%rename(const_getitem) ossimGeoPolygon::operator [](int) const;

%rename(__set__) ossimGeoPolygon::operator=;
%rename(__getitem__) ossimGeoPolygon::operator[];
%rename(__lshift__) operator<<;

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimGeoPolygon.h" 
