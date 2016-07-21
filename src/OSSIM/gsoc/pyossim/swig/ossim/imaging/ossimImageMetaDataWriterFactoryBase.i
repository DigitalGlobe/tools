/*-----------------------------------------------------------------------------
Filename        : ossimImageMetaDataWriterFactoryBase.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimImageMetaDataWriterFactoryBase 
-----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimObjectFactory.h>
#include <ossim/imaging/ossimMetadataFileWriter.h>
#include <ossim/imaging/ossimImageMetaDataWriterFactoryBase.h>

%}

/* Handling unclean Macro parsing */
#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling ossimImageMetaDataWriterFactoryBase Assignment operator */
%rename(__set__) ossimImageMetaDataWriterFactoryBase::operator=;

/* Wrapping class ossimImageMetaDataWriterFactoryBase */
%include "ossim/imaging/ossimImageMetaDataWriterFactoryBase.h"

#endif
