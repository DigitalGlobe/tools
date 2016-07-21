%module pyossim

%{
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimRectangleCutFilter.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimRectangleCutFilter.h"

#endif /* #ifndef ossimRectangleCutFilter_HEADER */
