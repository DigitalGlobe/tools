%module pyossim

%{
#include <ossim/imaging/ossimGeoAnnotationObject.h>
#include <ossim/imaging/ossimAnnotationEllipseObject.h>
#include <ossim/imaging/ossimGeoAnnotationEllipseObject.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

%rename(ossimGeoAnnotationEllipseObject_print) print;

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimGeoAnnotationEllipseObject.h" 

#endif
