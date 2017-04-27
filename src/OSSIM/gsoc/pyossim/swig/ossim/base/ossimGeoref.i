/*-----------------------------------------------------------------------------
 * Filename        : ossimGeoref.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimLsrPoint
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimGeoref.h>
%}

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimGeoref.h"
