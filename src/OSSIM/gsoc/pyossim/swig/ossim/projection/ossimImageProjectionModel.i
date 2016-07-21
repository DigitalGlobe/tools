/*-----------------------------------------------------------------------------
 * Filename        : ossimImageProjectionModel.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimImageProjectionModel 
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimImageProjectionModel.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

%include "ossim/base/ossimConstants.h"

/* Wrapping the class ossimImageProjectionModel */
%include "ossim/projection/ossimImageProjectionModel.h"

#endif
