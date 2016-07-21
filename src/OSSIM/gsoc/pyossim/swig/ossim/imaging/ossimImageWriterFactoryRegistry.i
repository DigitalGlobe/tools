/*-----------------------------------------------------------------------------
Filename        : ossimImageWriterFactoryRegistry.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimImageWriterFactoryRegistry 
-----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/base/ossimObjectFactory.h>
#include <ossim/imaging/ossimImageWriterFactoryBase.h>
#include <ossim/base/ossimFactoryListInterface.h>
#include <vector>
#include <iosfwd>
#include <ossim/base/ossimCommon.h>
#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA
#endif
%template(ossimFactoryListInterface_ossimImageWriterFactoryBase_ossimImageFileWriter) ossimFactoryListInterface< ossimImageWriterFactoryBase,ossimImageFileWriter >;

/* Include the required header files */
%include "ossim/base/ossimConstants.h"

/* Handling ossimImageWriterFactoryRegistry Assignment operator */
%rename(__set__) ossimImageWriterFactoryRegistry::operator=;


/* Wrapping class ossimImageWriterFactoryRegistry */
%include "ossim/imaging/ossimImageWriterFactoryRegistry.h"

