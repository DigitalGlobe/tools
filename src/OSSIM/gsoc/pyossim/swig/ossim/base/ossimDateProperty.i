/*-----------------------------------------------------------------------------
 * Filename        : ossimDateProperty.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimProperty
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/base/ossimProperty.h>
#include <ossim/base/ossimDate.h>
#include <ossim/base/ossimDateProperty.h>

%}

/* Wrapping class */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimDateProperty.h"

