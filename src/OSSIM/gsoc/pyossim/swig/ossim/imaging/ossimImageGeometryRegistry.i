/*-----------------------------------------------------------------------------
 * Filename        : ossimImageGeometryRegistry.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimImageGeometryRegistry
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/base/ossimConstants.h>
#include <ossim/imaging/ossimImageGeometryFactoryBase.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/base/ossimFactoryListInterface.h>
#include <ossim/imaging/ossimImageGeometryRegistry.h>

%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Include the required header files */
%import "ossim/base/ossimConstants.h";

/* Handling the vector<ossimDpt> */
%include "std_vector.i"
namespace std
{
//        %template(ossimImageGeometryRegistry_ossimFactoryListInterface) 
//                ossimFactoryListInterface<ossimImageGeometryFactoryBase,
//                ossimImageGeometry>;
        %template(vectorossimString) vector<ossimString>;
};

/* Handling ossimImageMetaDataWriterFactory Assignment operator */
%rename(__set__) ossimImageGeometryRegistry::operator=;

/* Wrapping the class ossimImageGeometryRegistry */
%include "ossim/imaging/ossimImageGeometryRegistry.h"
 
#endif
