/*-----------------------------------------------------------------------------
 * Filename        : ossimTempFilename.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimTempFilename
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimTempFilename.h>
%}

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimTempFilename.h"
