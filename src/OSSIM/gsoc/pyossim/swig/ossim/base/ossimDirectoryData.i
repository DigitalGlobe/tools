/*-----------------------------------------------------------------------------
 * Filename        : ossimDirectoryData.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimProperty
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <dirent.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimDirectoryData.h>

%}

/* Wrapping class */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimDirectoryData.h"
