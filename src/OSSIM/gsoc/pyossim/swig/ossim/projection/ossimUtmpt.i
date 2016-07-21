/*-----------------------------------------------------------------------------
 * Filename        : ossimUtmpt.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimMapProjection
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimConstants.h>
#include <ossim/projection/ossimUtmpt.h>
%}

%include "ossim/base/ossimConstants.h"

%include "ossim/projection/ossimUtmpt.h"
