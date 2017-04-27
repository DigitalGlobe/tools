/*-----------------------------------------------------------------------------
 * Filename        : ossimElevationDatabaseFactory.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimElevationDatabaseFactory
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/elevation/ossimElevationDatabaseFactory.h>
%}

%include "ossim/base/ossimConstants.h"

%include "ossim/elevation/ossimElevationDatabaseFactory.h"
