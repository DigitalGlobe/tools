/*-----------------------------------------------------------------------------
 * Filename        : ossimTileHash.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimTileHash
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimPointHash.h>
#include <ossim/base/ossimTileHash.h>
%}

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimTileHash.h"
