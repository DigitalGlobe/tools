/*-----------------------------------------------------------------------------
 * Filename        : ossimLsrSpace.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimLsrSpace
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/base/ossimLsrSpace.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimEcefVector.h>
#include <ossim/base/ossimMatrix3x3.h>
#include <ossim/base/ossimNotifyContext.h>

%}

/* Handling operators */
%rename(__set__) ossimLsrSpace::operator=;
%rename(__cmp__) ossimLsrSpace::operator==;
%rename(__ne__) ossimLsrSpace::operator!=;
%rename(__lshift__) operator<<;
%rename(ossimLsrSpace_print) print;

/* Wrapping class ossimLsrSpace */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimLsrSpace.h"
