%module pyossim

%{
#include <ossim/imaging/ossimGeoAnnotationObject.h>
#include <ossim/imaging/ossimAnnotationMultiPolyObject.h>
#include <ossim/imaging/ossimAnnotationMultiPolyLineObject.h>
#include <ossim/imaging/ossimGeoAnnotationMultiPolyLineObject.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimPolyLine.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

%rename(ossimGeoAnnotationMultiPolyLineObject_print) print;

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimGeoAnnotationMultiPolyLineObject.h"

#endif
