/*-----------------------------------------------------------------------------
Filename        : ossimMetadataFileWriter.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimMetadataFileWriter
-----------------------------------------------------------------------------*/

%module pyossim

%{
        
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimConnectableObject.h>
#include <ossim/base/ossimProcessInterface.h>
#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimObjectEvents.h>
#include <ossim/base/ossimProcessProgressEvent.h>

#include <ossim/imaging/ossimMetadataFileWriter.h>

%}

#ifndef TYPE_DATA
#define TYPE_DATA


/* Include the required header files */
%import "ossim/base/ossimConstants.h"

/* Wrapping class ossimMetadataFileWriter */
%include "ossim/imaging/ossimMetadataFileWriter.h"

#endif
