/*-----------------------------------------------------------------------------
 * Filename        : ossimTrace.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimTrace
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimTrace.h>
%}

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimTrace.h"
