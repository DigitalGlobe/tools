/*-----------------------------------------------------------------------------
 * Filename        : ossimTiffProjectionFactory.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimTiffProjectionFactory
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimProjectionFactoryBase.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

%include "ossim/projection/ossimProjectionFactoryBase.h"

/* Wrapping the class */
%include "ossim/projection/ossimTiffProjectionFactory.h"

#endif
