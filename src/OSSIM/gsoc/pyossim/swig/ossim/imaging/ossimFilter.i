%module pyossim

%{
#include <math.h>
#include <ossim/matrix/newmat.h>
#include <ossim/base/ossimRtti.h>
#include <ossim/imaging/ossimFilter.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

%include "ossim/base/ossimConstants.h"

%include "ossim/imaging/ossimFilter.h"

#endif
