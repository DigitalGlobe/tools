%module pyossim

%{
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimRgbToJpegYCbCrSource.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

%include "ossim/imaging/ossimRgbToJpegYCbCrSource.h"

#endif
