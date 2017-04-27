/*-----------------------------------------------------------------------------
 * Filename        : ossimImageViewAffineTransform.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimImageViewAffineTransform 
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimImageViewAffineTransform.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

%include "ossim/base/ossimConstants.h"

/* Wrapping the class ossimImageViewAffineTransform */
%include "ossim/projection/ossimImageViewAffineTransform.h"

#endif
