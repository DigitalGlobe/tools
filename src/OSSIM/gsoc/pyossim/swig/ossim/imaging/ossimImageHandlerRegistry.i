/*-----------------------------------------------------------------------------
 * Filename        : ossimImageHandlerRegistry.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimImageHandlerRegistry
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimObjectFactory.h>
#include <ossim/base/ossimRtti.h>
#include <ossim/imaging/ossimImageHandlerFactoryBase.h>
#include <ossim/base/ossimFactoryListInterface.h>
#include <vector>
%}
     
%rename(__set__) ossimImageHandlerRegistry::operator=;

%template(ossimFactoryListInterface_ossimImageHandlerFactoryBase_ossimImageHandler) ossimFactoryListInterface< ossimImageHandlerFactoryBase,ossimImageHandler >;

%include "ossim/base/ossimConstants.h"

/* Wrapping class ossimImageHandlerRegistry */
%include "ossim/imaging/ossimImageHandlerRegistry.h"
