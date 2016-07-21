%module pyossim

%{
#include <ossim/imaging/ossimAnnotationObject.h>
#include <ossim/imaging/ossimAnnotationEllipseObject.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

%rename(ossimAnnotationEllipseObject_print) print;

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimAnnotationEllipseObject.h"
   
#endif
