%module pyossim

%{
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimMaskTileSource.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimMaskTileSource.h"

#endif

