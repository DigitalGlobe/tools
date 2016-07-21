%module pyossim

%{
#include <ossim/imaging/ossimAnnotationObject.h>
#include <ossim/imaging/ossimAnnotationMultiEllipseObject.h>
#include <vector>
%}

%rename (ossimAnnotationMultiEllipseObject_print) print;
%rename(ossimAnnotationMultiEllipseObject_getitem_int) ossimAnnotationMultiEllipseObject::operator [](int) const;
%rename (__getitem__) ossimAnnotationMultiEllipseObject::operator[];

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimAnnotationObject.h"
%include "ossim/imaging/ossimAnnotationMultiEllipseObject.h"
