/*-----------------------------------------------------------------------------
 * Filename        : ossimAdjustableParameterInfo.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimProperty
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <iostream>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimAdjustableParameterInfo.h>

%}

/* Handling assignment operator */
%rename(__lshift__) operator<<;

/* Wrapping the class ossimAdjustableParameterInfo */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimAdjustableParameterInfo.h"
