/*-----------------------------------------------------------------------------
 * Filename        : ossimViewEvent.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimViewEvent
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimViewEvent.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Wraping class ossimViewEvent */
%include "ossim/base/ossimConstants.h" 
%include "ossim/base/ossimViewEvent.h" 

#endif
