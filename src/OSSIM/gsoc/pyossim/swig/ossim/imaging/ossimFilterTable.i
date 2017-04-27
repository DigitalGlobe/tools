%module pyossim

%{
#include <cmath>
#include <ossim/base/ossimConstants.h>
#include <ossim/imaging/ossimFilterTable.h>
%}

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimFilterTable.h"
