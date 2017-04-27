%module pyossim

%{
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimGridRemapSource.h>
#include <vector>
#include <ossim/imaging/ossimGridRemapEngine.h>
#include <ossim/base/ossimFilename.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

/* Wrapping the class */
%include "ossim/imaging/ossimGridRemapSource.h"

#endif
