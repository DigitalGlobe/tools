%module pyossim

%{
#include <ossim/imaging/ossimAnnotationObject.h>
#include <ossim/imaging/ossimAnnotationPolyObject.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimPolygon.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

%rename(ossimAnnotationPolyObject_print) print;

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimAnnotationPolyObject.h"

#endif
