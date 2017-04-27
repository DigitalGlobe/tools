/*-----------------------------------------------------------------------------
Filename        : ossimObject.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimObject
-----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimDataObject.h>
%}
%include "ossim/base/ossimRectilinearDataObject.h"

