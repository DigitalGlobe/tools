/*-----------------------------------------------------------------------------
 * Filename        : ossimDirectoryData.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimProperty
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimDirectory.h>

%}

/* Wrapping class */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimDirectory.h"
