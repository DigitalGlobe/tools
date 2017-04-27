%module pyossim

%{
#include <vector>
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimBandSelector.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimBandSelector.h"

#endif
