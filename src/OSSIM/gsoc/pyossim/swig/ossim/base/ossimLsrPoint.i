/*-----------------------------------------------------------------------------
 * Filename        : ossimLsrPoint.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimLsrPoint
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/base/ossimLsrSpace.h>
#include <ossim/base/ossimLsrPoint.h>
#include <ossim/base/ossimLsrVector.h>
#include <ossim/base/ossimColumnVector3d.h>
#include <ossim/base/ossimNotify.h>

%}

%rename(non_const_lsrPoint_x) ossimLsrPoint::x();
%rename(non_const_lsrPoint_y) ossimLsrPoint::y();
%rename(non_const_lsrPoint_z) ossimLsrPoint::z();
%rename(const_lsrPoint_data) ossimLsrPoint::data() const;
%rename(const_lsrPoint_lsrSpace) ossimLsrPoint::lsrSpace() const;

%rename(non_const_lsrVector_x) ossimLsrVector::x();
%rename(non_const_lsrVector_y) ossimLsrVector::y();
%rename(non_const_lsrVector_z) ossimLsrVector::z();
%rename(const_lsrVector_data) ossimLsrVector::data() const;
%rename(const_lsrVector_lsrSpace) ossimLsrVector::lsrSpace() const;

/* Handling operators */
%rename(__set__) ossimLsrPoint::operator=;
%rename(__add__) ossimLsrPoint::operator+;
%rename(__sub__) ossimLsrPoint::operator-;
%rename(__cmp__) ossimLsrPoint::operator==;
%rename(__ne__) ossimLsrPoint::operator!=;
%rename(ossimLsrPoint_ossimEcefPoint) ossimLsrPoint::operator ossimEcefPoint;
%rename(__lshift__) operator<<;
%rename(ossimLsrPoint_print) print;


%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimLsrPoint.h"
