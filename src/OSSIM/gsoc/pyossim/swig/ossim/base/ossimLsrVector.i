/*-----------------------------------------------------------------------------
 * Filename        : ossimLsrVector.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimLsrVector
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <iosfwd>

#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimLsrPoint.h>
#include <ossim/base/ossimLsrSpace.h>
#include <ossim/base/ossimLsrVector.h>
#include <ossim/base/ossimEcefVector.h>
#include <ossim/base/ossimColumnVector3d.h>

%}

/* Handling operators */
%rename(__set__) ossimLsrVector::operator=;
%rename(__cmp__) ossimLsrVector::operator==;
%rename(__ne__) ossimLsrVector::operator!=;
%rename(__add__) ossimLsrVector::operator+;
%rename(__sub__) ossimLsrVector::operator-;
%rename(__mul__) ossimLsrVector::operator*;
%rename(__div__) ossimLsrVector::operator/;
%rename(__lshift__) operator<<;
%rename(ossimLsrVector_print) print;
%rename(ossimLsrVector_ossimEcefVector) operator ossimEcefVector;

/* Wrapping class ossimLsrVector */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimLsrVector.h"
