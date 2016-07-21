%module pyossim

%{
#include <ossim/imaging/ossimAnnotationObject.h>
#include <ossim/imaging/ossimAnnotationMultiPolyLineObject.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimPolyLine.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

%rename (ossimAnnotationMultiPolyLineObject_print) print;

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimAnnotationMultiPolyLineObject.h"

#endif
