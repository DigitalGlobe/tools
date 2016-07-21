%module pyossim

%{
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/base/ossimPolygon.h>
#include <ossim/imaging/ossimImageDataHelper.h>
#include <ossim/imaging/ossimPolyCutter.h>
#include <vector>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimPolyCutter.h"

#endif
