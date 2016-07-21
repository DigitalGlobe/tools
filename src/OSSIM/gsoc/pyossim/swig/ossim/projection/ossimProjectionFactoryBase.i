/*-----------------------------------------------------------------------------
Filename        : ossimProjectionFactoryBase.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimProjectionFactoryBase
-----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimProjectionFactoryBase.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

%include "ossim/base/ossimConstants.h"

/*Wrapping the class ossimProjectionFactoryBase */
%include "ossim/projection/ossimProjectionFactoryBase.h"

#endif
