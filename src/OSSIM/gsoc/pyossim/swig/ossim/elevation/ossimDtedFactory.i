/*-----------------------------------------------------------------------------
 * Filename        : ossimDtedFactory.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimDtedFactory
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/elevation/ossimElevSourceFactory.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Wrapping class ossimDtedFactory */
%include "ossim/base/ossimConstants.h"
%include "ossim/elevation/ossimElevSourceFactory.h"
%include "ossim/elevation/ossimDtedFactory.h"

#endif
