/*-----------------------------------------------------------------------------
 * Filename        : ossim2dTo2dTransformRegistry.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossim2dTo2dTransformRegistry
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimObjectFactory.h>
#include <ossim/base/ossimRtti.h>
#include <ossim/base/ossim2dTo2dTransformFactoryBase.h>
#include <ossim/base/ossim2dTo2dTransform.h>
#include <vector>
#include <ossim/base/ossimFactoryListInterface.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling operators */
%rename(__set__) ossim2dTo2dTransformRegistry::operator=;

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimFactoryListInterface.h"

%template(ossimFactoryListInterface_ossim2dTo2dTransformFactoryBase_ossim2dTo2dTransform) ossimFactoryListInterface< ossim2dTo2dTransformFactoryBase,ossim2dTo2dTransform >;

/* Wrapping class ossim2dTo2dTransformRegistry */
%include "ossim/base/ossim2dTo2dTransformRegistry.h"

#endif
