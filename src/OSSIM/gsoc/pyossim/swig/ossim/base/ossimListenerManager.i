/*-----------------------------------------------------------------------------
 * Filename        : ossimListenerManager.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimListenerManager
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimConstants.h>
#include <list>
#include <ossim/base/ossimRtti.h>
%}

/* Wrapping class ossimListenerManager */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimListenerManager.h"
