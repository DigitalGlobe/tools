%module pyossim

%{
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimMaskedImageHandler.h>
#include <ossim/imaging/ossimMaskFilter.h>
#include <ossim/base/ossimIrect.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimMaskedImageHandler.h"

#endif
