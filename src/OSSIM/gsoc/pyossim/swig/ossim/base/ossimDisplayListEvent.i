/*-----------------------------------------------------------------------------
 * Filename        : ossimDisplayListEvent.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimProperty
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/base/ossimEvent.h>
#include <ossim/base/ossimDisplayListEvent.h>
#include <map>

%}

/* Wrapping class */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimDisplayListEvent.h"

