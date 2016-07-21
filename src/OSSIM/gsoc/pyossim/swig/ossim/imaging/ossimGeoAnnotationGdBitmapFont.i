%module pyossim

%{
#include <ossim/imaging/ossimGeoAnnotationObject.h>
#include <ossim/imaging/ossimGeoAnnotationGdBitmapFont.h>
#include <ossim/imaging/ossimGdFont.h>
#include <ossim/imaging/ossimGdFontExterns.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

%rename(ossimGeoAnnotationGdBitmapFont_print) print;

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimGeoAnnotationGdBitmapFont.h"

#endif
