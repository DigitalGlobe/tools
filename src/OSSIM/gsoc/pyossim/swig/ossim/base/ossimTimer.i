/*-----------------------------------------------------------------------------
 * Filename        : ossimTimer.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimTimer
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimTimer.h>
%}

/** Timer class is used for measuring elapsed time or time between two points. */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimTimer.h"
