/*-----------------------------------------------------------------------------
 * Filename        : ossimEcefPoint.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimLsrRay
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <iosfwd>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimColumnVector3d.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimEcefVector.h>
%}

%rename(ossimEcefPoint_x) ossimEcefPoint::x();
%rename(ossimEcefPoint_y) ossimEcefPoint::y();
%rename(ossimEcefPoint_z) ossimEcefPoint::z();
%rename(const_ossimEcefPoint_getitem) ossimEcefPoint::operator [](int) const;
%rename(const_ossimEcefPoint_data) ossimEcefPoint::data() const;

/* Handling operators */
%rename(__add__) ossimEcefPoint::operator+;
%rename(__sub__) ossimEcefPoint::operator-;
%rename(__set__) ossimEcefPoint::operator=;
%rename(__cmp__) ossimEcefPoint::operator==;
%rename(__ne__) ossimEcefPoint::operator!=;
%rename(__lshift__) operator<<;
%rename(ossimEcefPoint_print) print;
%rename(__getitem__) ossimEcefPoint::operator[];

/* Wrapping class */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimEcefPoint.h"
