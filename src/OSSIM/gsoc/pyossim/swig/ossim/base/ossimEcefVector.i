/*-----------------------------------------------------------------------------
 * Filename        : ossimEcefVector.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimLsrRay
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <cmath>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimColumnVector3d.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimString.h>
%}

%rename(ossimEcefVector_x) ossimEcefVector::x();
%rename(ossimEcefVector_y) ossimEcefVector::y();
%rename(ossimEcefVector_z) ossimEcefVector::z();
%rename(const_ossimEcefVector_getitem) ossimEcefVector::operator [](int) const;
%rename(const_ossimEcefVactor_data) ossimEcefVector::data() const;

/* Handling operators */
%rename(__sub__) ossimEcefVector::operator-;
%rename(__add__) ossimEcefVector::operator+;
%rename(__mul__) ossimEcefVector::operator*;
%rename(__div__) ossimEcefVector::operator/;
%rename(__set__) ossimEcefVector::operator=;
%rename(__cmp__) ossimEcefVector::operator==;
%rename(__ne__) ossimEcefVector::operator!=;
%rename(__getitem__) ossimEcefVector::operator[];
%rename(__lshift__) operator<<;
%rename(ossimEcefVector_print) print;


/* Wrapping class */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimEcefVector.h"
