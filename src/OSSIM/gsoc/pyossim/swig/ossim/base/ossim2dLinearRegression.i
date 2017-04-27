/*-----------------------------------------------------------------------------
 * Filename        : ossim2dLinearRegression.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimProperty
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
        
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossim2dLinearRegression.h>
#include <vector>
#include <iostream>

%}

/* Handling assignment operator */
%rename(__lshift__) operator<<;

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossim2dLinearRegression.h"
