/*-----------------------------------------------------------------------------
 * Filename        : ossimViewInterface.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimViewInterface
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimViewInterface.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Wrapping class ossimViewInterface */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimViewInterface.h"

#endif
