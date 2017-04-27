%module pyossim

%{
#include <ossim/imaging/ossimGeoAnnotationObject.h>
#include <ossim/imaging/ossimGeoAnnotationBitmap.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

%rename(ossimAnnotationFontObject_print) print;

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimGeoAnnotationBitmap.h"

#endif
