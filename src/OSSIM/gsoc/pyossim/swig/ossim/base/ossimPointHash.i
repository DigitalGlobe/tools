/*-----------------------------------------------------------------------------
 * Filename        : ossimPointHash.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimPointHash
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimFpt.h>
%}

%include "ossim/base/ossimPointHash.h"
