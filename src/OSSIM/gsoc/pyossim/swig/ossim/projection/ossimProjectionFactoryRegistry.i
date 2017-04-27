/*-----------------------------------------------------------------------------
Filename        : ossimProjectionFactoryRegistry.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
-----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimObjectFactory.h>
#include <ossim/base/ossimFactoryListInterface.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimProjectionFactoryBase.h>
%}

%template(ossimFactoryListInterface_ossimProjectionFactoryBase_ossimProjection) ossimFactoryListInterface< ossimProjectionFactoryBase,ossimProjection >;

/* Handling assignment operator */
%rename(__set__) ossimProjectionFactoryRegistry::operator=;

%include "ossim/base/ossimConstants.h"

/* Wrapping class ossimProjectionFactoryRegistry */
%include "ossim/projection/ossimProjectionFactoryRegistry.h"
