/*-----------------------------------------------------------------------------
 * Filename        : ossimObjectFactoryRegistry.i 
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimObjectFactoryRegistry
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimObjectFactoryRegistry.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling assignment operator */
%rename(__set__) ossimObjectFactoryRegistry::operator=;

/* Wrapping the class ossimObjectFactoryRegistry */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimObjectFactoryRegistry.h"

#endif
