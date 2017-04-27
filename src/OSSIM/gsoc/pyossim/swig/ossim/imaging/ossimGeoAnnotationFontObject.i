%module pyossim

%{
#include <ossim/imaging/ossimGeoAnnotationObject.h>
#include <ossim/imaging/ossimAnnotationFontObject.h>
#include <ossim/imaging/ossimGeoAnnotationFontObject.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Wrapping the class */
%include "ossim/imaging/ossimGeoAnnotationFontObject.h" 

#endif
