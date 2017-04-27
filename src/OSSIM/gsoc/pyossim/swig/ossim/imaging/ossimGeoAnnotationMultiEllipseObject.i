%module pyossim

%{
#include <ossim/imaging/ossimGeoAnnotationObject.h>
#include <ossim/imaging/ossimGeoAnnotationMultiEllipseObject.h>
#include <vector>
%}

%rename(ossimGeoAnnotationMultiEllipseObject_print) print;

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimGeoAnnotationObject.h"
%include "ossim/imaging/ossimGeoAnnotationMultiEllipseObject.h"
