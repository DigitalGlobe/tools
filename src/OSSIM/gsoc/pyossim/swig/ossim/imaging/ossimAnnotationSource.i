%module pyossim

%{
#include <vector>
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/imaging/ossimRgbImage.h>
#include <ossim/imaging/ossimAnnotationObject.h>
#include <ossim/imaging/ossimAnnotationSource.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

%rename(ossimAnnotationSource_print) print;

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimAnnotationSource.h"

#endif
