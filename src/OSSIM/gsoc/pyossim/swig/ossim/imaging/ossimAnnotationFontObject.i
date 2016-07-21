%module pyossim

%{
#include <ossim/imaging/ossimAnnotationObject.h>
#include <ossim/imaging/ossimAnnotationFontObject.h>
#include <ossim/base/ossimFontInformation.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/font/ossimFont.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

%rename(ossimAnnotationFontObject_print) print;

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimAnnotationFontObject.h"

#endif
