/*-----------------------------------------------------------------------------
 * Filename        : ossimImageViewTransformFactory.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimImageViewTransformFactory
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimImageViewTransformFactory.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling the assignment operator */
%rename(__set__) ossimImageViewTransformFactory::operator=;

/* Wrapping the class ossimImageViewTransformFactory */
%include "ossim/projection/ossimImageViewTransformFactory.h"

#endif
