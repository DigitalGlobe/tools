%module pyossim

%{
#include <ossim/base/ossimGpt.h>
#include <ossim/imaging/ossimGeoAnnotationObject.h>
#include <ossim/imaging/ossimGeoAnnotationLineObject.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

%rename(ossimGeoAnnotationLineObject_print) print;

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimGeoAnnotationLineObject.h"

#endif
