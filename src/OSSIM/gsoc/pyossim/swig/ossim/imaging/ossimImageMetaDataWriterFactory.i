/*-----------------------------------------------------------------------------
Filename        : ossimImageMetaDataWriterFactory.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimImageMetaDataWriterFactory 
-----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimObjectFactory.h>
#include <ossim/imaging/ossimMetadataFileWriter.h>

#include <ossim/imaging/ossimImageMetaDataWriterFactory.h>
#include <ossim/imaging/ossimImageMetaDataWriterFactoryBase.h>

%}

/* Handling unclean Macro parsing */
#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling ossimImageMetaDataWriterFactory Assignment operator */
%rename(__set__) ossimImageMetaDataWriterFactory::operator=;

/* Include the header file containing the declarations to be wrapped */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimImageMetaDataWriterFactory.h"

#endif
