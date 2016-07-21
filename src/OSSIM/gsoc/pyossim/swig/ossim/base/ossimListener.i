/*-----------------------------------------------------------------------------
 * Filename        : ossimListener.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimListener
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimObject.h>
%}

/* Wrapping class ossimListener */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimListener.h"
