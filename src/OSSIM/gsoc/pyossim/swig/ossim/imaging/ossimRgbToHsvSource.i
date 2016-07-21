%module pyossim

%{
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimRgbToHsvSource.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

%include "ossim/imaging/ossimRgbToHsvSource.h"

#endif
