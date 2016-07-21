%module pyossim

%{
#include <ossim/imaging/ossimGridRemapEngine.h>
#include <ossim/imaging/ossimRgbGridRemapEngine.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

%include "ossim/imaging/ossimRgbGridRemapEngine.h"

#endif
