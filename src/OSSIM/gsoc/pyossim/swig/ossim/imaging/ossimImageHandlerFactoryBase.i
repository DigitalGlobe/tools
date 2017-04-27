/*-----------------------------------------------------------------------------
 * Filename        : ossimImageHandlerFactoryBase.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimImageHandlerFactoryBase
 * -----------------------------------------------------------------------------*/
%module pyossim

%{
#include <ossim/imaging/ossimImageHandlerFactoryBase.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

%rename(__getitem__) ossimImageHandlerFactoryBase::operator[];

/* Importing ossim constants */
%import "ossim/base/ossimConstants.h"

/* Wrapping the class ossimImageHandlerFactoryBase */
%include "ossim/imaging/ossimImageHandlerFactoryBase.h"
   
#endif
