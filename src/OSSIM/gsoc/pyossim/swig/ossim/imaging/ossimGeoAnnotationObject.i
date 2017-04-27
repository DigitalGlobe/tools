%module pyossim

%{
#include <ossim/imaging/ossimAnnotationObject.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/imaging/ossimImageGeometry.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

%rename(ossimGeoAnnotationObject_print) print;

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimGeoAnnotationObject.h"

#endif
