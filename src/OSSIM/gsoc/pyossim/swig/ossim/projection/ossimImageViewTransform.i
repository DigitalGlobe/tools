/*-----------------------------------------------------------------------------
 * Filename        : ossimImageViewTransform.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimImageViewTransform
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <iostream>
#include <ossim/base/ossimViewInterface.h>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossim2dTo2dTransform.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA
#endif
/* Handling ossimImageViewTransform <<  operator */
%rename(__lshift__) operator <<;

/* Handling the reserved function print */
%rename(ossimImageViewTransform_print) ossimImageViewTransform::print;

%include "ossim/base/ossimConstants.h"

/* Wrapping class ossimImageViewTransform */
%include "ossim/projection/ossimImageViewTransform.h"


