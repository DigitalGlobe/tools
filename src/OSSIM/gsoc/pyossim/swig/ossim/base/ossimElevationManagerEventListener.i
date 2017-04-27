/*-----------------------------------------------------------------------------
Filename        : ossimElevationManagerEventListener.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimObject
-----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimListener.h>
#include <ossim/base/ossimElevationManagerEvent.h>
#include <ossim/base/ossimElevationManagerEventListener.h>
%}

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimElevationManagerEventListener.h"

