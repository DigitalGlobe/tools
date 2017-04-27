%module pyossim

%{
#include <ossim/imaging/ossimGridRemapEngine.h>
#include <ossim/imaging/ossimHsvGridRemapEngine.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimHsvGridRemapEngine.h"

#endif
