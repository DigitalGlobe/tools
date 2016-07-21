%module pyossim

%{
#include <ossim/imaging/ossimGeoAnnotationObject.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimGeoPolygon.h>
#include <ossim/imaging/ossimAnnotationMultiPolyObject.h>
#include <ossim/imaging/ossimGeoAnnotationMultiPolyObject.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

%rename(ossimGeoAnnotationMultiPolyObject_print) print;

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimGeoAnnotationMultiPolyObject.h"

#endif
