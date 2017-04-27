/*-----------------------------------------------------------------------------
 * Filename        : ossim2dTo2dTransform.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossim2dTo2dTransform
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/base/ossim2dTo2dTransform.h>
#include <ossim/base/ossim2dTo2dIdentityTransform.h>

%}

/* Handling operators */
%rename(__set__) ossim2dTo2dIdentityTransform::operator=;

/**
 * This is the identity transform and just passes the input to the output.
 */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossim2dTo2dIdentityTransform.h"
