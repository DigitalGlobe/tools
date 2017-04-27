/*-----------------------------------------------------------------------------
 * Filename        : ossimViewListener.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimViewListener
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimViewListener.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Wrapping class ossimViewListener */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimViewListener.h"

#endif
