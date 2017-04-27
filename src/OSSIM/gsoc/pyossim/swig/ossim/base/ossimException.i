/*-----------------------------------------------------------------------------
 * Filename        : ossimException.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimProperty
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <exception>
#include <string>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimException.h>

%}

/* Wrapping class */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimException.h"
