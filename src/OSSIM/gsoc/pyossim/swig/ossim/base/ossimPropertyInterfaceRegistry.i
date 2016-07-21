/*-----------------------------------------------------------------------------
 * Filename        : ossimPropertyInterfaceRegistry.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimPropertyInterfaceRegistry
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimPropertyInterfaceRegistry.h>
%}

/* Handling assignment operator */
%rename(__set__) ossimPropertyInterfaceRegistry::operator=;

/* Wrapping the class ossimPropertyInterfaceRegistry */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimPropertyInterfaceRegistry.h"

