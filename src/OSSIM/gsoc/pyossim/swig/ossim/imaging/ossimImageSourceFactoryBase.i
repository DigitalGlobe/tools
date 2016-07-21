/*-----------------------------------------------------------------------------
Filename        : ossimImageSourceFactoryBase.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimImageSourceFactoryBase
-----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/base/ossimObjectFactory.h>
#include <ossim/imaging/ossimImageSourceFactoryBase.h>
#include <ossim/imaging/ossimImageSource.h>

%}

/* Include the header files */
%import "ossim/base/ossimConstants.h"

/* Handling ossimImageSourceFactoryBase Assignment operator */
%rename(__set__) ossimImageSourceFactoryBase::operator=;

/* Wrapping class ossimImageSourceFactoryBase */
%include "ossim/imaging/ossimImageSourceFactoryBase.h"
