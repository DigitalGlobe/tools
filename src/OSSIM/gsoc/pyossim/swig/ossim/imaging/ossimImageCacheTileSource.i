%module pyossim

%{
#include <ossim/imaging/ossimImageCacheBase.h>
#include <ossim/imaging/ossimImageCacheTileSource.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimImageCacheTileSource.h"

#endif
