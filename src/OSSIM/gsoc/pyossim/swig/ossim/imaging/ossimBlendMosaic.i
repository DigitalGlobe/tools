%module pyossim

%{
#include <ossim/imaging/ossimImageMosaic.h>
#include <ossim/imaging/ossimBlendMosaic.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimBlendMosaic.h"

#endif
