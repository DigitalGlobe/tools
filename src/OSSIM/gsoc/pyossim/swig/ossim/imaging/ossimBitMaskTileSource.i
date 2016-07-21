%module pyossim

%{
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimBitMaskWriter.h>
#include <ossim/imaging/ossimBitMaskTileSource.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimBitMaskTileSource.h"

#endif

