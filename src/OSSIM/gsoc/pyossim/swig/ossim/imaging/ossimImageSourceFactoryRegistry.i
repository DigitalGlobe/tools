/*-----------------------------------------------------------------------------
Filename        : ossimImageSourceFactoryRegistry.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimImageSourceFactoryRegistry
-----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/imaging/ossimImageSourceFactoryBase.h>
#include <ossim/imaging/ossimImageSourceFactoryRegistry.h>

%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Include the header file  */
%import "ossim/base/ossimConstants.h";

/* Handling ossimImageSourceFactoryRegistry Assignment operator */
%rename(__setattr__) ossimImageSourceFactoryRegistry::operator=;


/* Wrapping class ossimImageSourceFactoryRegistry */
%include "ossim/imaging/ossimImageSourceFactoryRegistry.h"

#endif 
