/*-----------------------------------------------------------------------------
 * Filename        : ossimLsrRay.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimLsrRay
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/base/ossimLsrPoint.h>
#include <ossim/base/ossimLsrRay.h>
#include <ossim/base/ossimLsrVector.h>
#include <ossim/base/ossimEcefRay.h>
#include <ossim/base/ossimNotifyContext.h>

%}

/* Handling operators */
%rename(__set__) ossimLsrRay::operator=;
%rename(__cmp__) ossimLsrRay::operator==;
%rename(__ne__) ossimLsrRay::operator!=;
%rename(__lshift__) operator<<;
%rename(ossimLsrRay_print) print;
%rename(ossimLsrRay_ossimEcefRay) operator ossimEcefRay;


/* Wrapping class ossimLsrRay */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimLsrRay.h"
