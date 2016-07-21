/*-----------------------------------------------------------------------------
 * Filename        : ossim2dTo2dTransform.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossim2dTo2dTransform
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossim2dTo2dTransform.h>

%}

/* Handling operators */
%rename(__set__) ossim2dTo2dTransform::operator=;
%rename(ossim2dTo2dTransform_print) print;

/* Wrapping class ossim2dTo2dTransform */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossim2dTo2dTransform.h"

