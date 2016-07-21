/*-----------------------------------------------------------------------------
 * Filename        : ossimSrtmFactory.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimSrtmFactory
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/elevation/ossimSrtmFactory.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

%include "ossim/base/ossimConstants.h"

/* Wrapping the class ossimSrtmFactory */
%include "ossim/elevation/ossimSrtmFactory.h"

#endif
