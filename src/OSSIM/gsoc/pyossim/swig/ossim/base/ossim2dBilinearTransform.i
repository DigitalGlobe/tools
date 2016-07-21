/*-----------------------------------------------------------------------------
 * Filename        : ossim2dTo2dTransform.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossim2dTo2dTransform
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/base/ossim2dBilinearTransform.h>
#include <ossim/base/ossim2dTo2dTransform.h>
#include <algorithm>

%}

/* Handling assignment operator */
%rename(__set__) ossim2dBilinearTransform::operator=;
%rename(ossim2dBilinearTransform_print) print;

/* Wrapping class ossim2dBilinearTransform */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossim2dBilinearTransform.h"
