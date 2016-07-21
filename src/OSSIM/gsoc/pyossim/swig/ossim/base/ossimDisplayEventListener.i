/*-----------------------------------------------------------------------------
 * Filename        : ossimDisplayEventListener.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimProperty
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/base/ossimListener.h>
#include <ossim/base/ossimDisplayEventListener.h>

%}

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimDisplayEventListener.h"

