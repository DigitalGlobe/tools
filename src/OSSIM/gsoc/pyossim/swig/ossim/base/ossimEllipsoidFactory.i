/*-----------------------------------------------------------------------------
Filename        : ossimEllipsoidFactory.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimObject
-----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimEllipsoidFactory.h>
#include <map>
#include <string>
%}
        
/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimEllipsoidFactory.h"
