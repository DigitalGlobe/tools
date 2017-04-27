%module pyossim

%{
#include <ossim/imaging/ossimMetadataFileWriter.h>
#include <ossim/imaging/ossimWorldFileWriter.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimWorldFileWriter.h"

#endif
