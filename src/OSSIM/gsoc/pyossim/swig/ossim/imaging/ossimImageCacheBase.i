%module pyossim

%{
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageCacheBase.h>
#include <ossim/support_data/ossimRpfFrameEntry.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimImageCacheBase.h"

#endif
