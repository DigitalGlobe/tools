/*-----------------------------------------------------------------------------
Filename        : ossimImageMetaDataWriterRegistry.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimImageMetaDataRegistry 
-----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/base/ossimRefPtr.h>

#include <ossim/imaging/ossimImageMetaDataWriterFactoryBase.h>
#include <ossim/imaging/ossimMetadataFileWriter.h>

%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling the std::exception */
%include "exception.i"
%exception
{
        try
        {
                $action
        }
        catch(const std::exception& e)
        {
                SWIG_exception(SWIG_RuntimeError, e.what());
        }
}

/* Handling ossimImageMetaDataWriterRegistry Assignment operator */
%rename(__set__) ossimImageMetaDataWriterRegistry::operator=;

/* Include the header file containing the declarations to be wrapped */
%import "ossim/base/ossimConstants.h";

/* Wrapping the file */
%include "ossim/imaging/ossimImageMetaDataWriterRegistry.h"

#endif
