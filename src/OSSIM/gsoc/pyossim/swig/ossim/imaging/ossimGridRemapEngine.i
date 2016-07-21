%module pyossim

%{
#include <vector>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/imaging/ossimGridRemapEngine.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

/* Wrapping the class */
%include "ossim/imaging/ossimGridRemapEngine.h"

#endif
