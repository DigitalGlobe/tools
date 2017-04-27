/*-----------------------------------------------------------------------------
 * Filename        : ossim2dTo2dShiftTransformi
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossim2dTo2dShiftTransform
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/base/ossim2dTo2dTransform.h>
#include <ossim/base/ossim2dTo2dShiftTransform.h>

%}

/* Handling operators */
%rename(__set__) ossim2dTo2dShiftTransform::operator=;
%rename(ossim2dTo2dShiftTransform_print) print;

/* Wrapping class ossim2dTo2dShiftTransform */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossim2dTo2dShiftTransform.h"

