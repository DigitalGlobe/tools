/*-----------------------------------------------------------------------------
 * Filename        : ossimElevSourceFactory.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimElevSourceFactory
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/elevation/ossimElevSourceFactory.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

%include "ossim/base/ossimConstants.h"

/* Wrapping class ossimElevSourceFactory */
%include "ossim/elevation/ossimElevSourceFactory.h"

#endif
