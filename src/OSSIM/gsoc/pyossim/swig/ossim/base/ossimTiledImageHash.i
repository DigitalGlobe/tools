/*-----------------------------------------------------------------------------
 * Filename        : ossimTiledImageHash.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimTiledImageHash
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimPointHash.h>
#include <ossim/base/ossimTiledImageHash.h>
%}        

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimTiledImageHash.h"
