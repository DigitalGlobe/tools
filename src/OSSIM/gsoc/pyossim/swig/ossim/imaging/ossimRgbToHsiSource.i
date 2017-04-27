%module pyossim

%{
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimRgbToHsiSource.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

%include "ossim/imaging/ossimRgbToHsiSource.h"

#endif
