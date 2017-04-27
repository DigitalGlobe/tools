%module pyossim

%{
#include <vector>
#include <ossim/base/ossimGpt.h>
#include <ossim/imaging/ossimGeoAnnotationObject.h>
#include <ossim/imaging/ossimAnnotationPolyObject.h>
#include <ossim/imaging/ossimGeoAnnotationPolyObject.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

%rename(ossimAnnotationFontObject_print) print;

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimGeoAnnotationPolyObject.h"

#endif
