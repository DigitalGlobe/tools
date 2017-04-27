/*-----------------------------------------------------------------------------
 * Filename        : ossimPropertyInterfaceFactory.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimImageModel 
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimPropertyInterfaceFactory.h>
%}

/* Wrapping the class ossimPropertyInterfaceFactory */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimPropertyInterfaceFactory.h"

