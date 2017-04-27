/*-----------------------------------------------------------------------------
 * Filename        : ossimTraceManager.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimTraceManager
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimTraceManager.h>
%}

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimTraceManager.h"
