%module pyossim

%{
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimHsvToRgbSource.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimHsvToRgbSource.h"

#endif
