%module pyossim

%{
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimHsiRemapper.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

%rename(ossimHsiRemapper_print) print;
%rename(__lshift__) operator<<;

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimHsiRemapper.h"

#endif
