%module pyossim

%{
#include <iostream>

#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/imaging/ossimRgbImage.h>
#include <ossim/base/ossimString.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

%rename(ossimAnnotationObject_print) print;

%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimAnnotationObject.h"

#endif
