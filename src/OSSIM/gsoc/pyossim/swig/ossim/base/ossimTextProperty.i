/*-----------------------------------------------------------------------------
 * Filename        : ossimTextProperty.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimTextProperty
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimProperty.h>
#include <ossim/base/ossimTextProperty.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Wrapping class ossimTextProperty */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimTextProperty.h" 

#endif
