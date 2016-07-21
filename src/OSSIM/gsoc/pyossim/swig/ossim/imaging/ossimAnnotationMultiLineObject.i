%module pyossim

%{

#include <ossim/imaging/ossimAnnotationObject.h>
#include <ossim/imaging/ossimAnnotationMultiLineObject.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimPolyLine.h>

%}
        
#ifndef TYPE_DATA
#define TYPE_DATA

%rename(ossimAnnotationMultiLineObject_print) print;

%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimAnnotationMultiLineObject.h"

#endif
