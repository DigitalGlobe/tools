%module pyossim

%{
#include <ossim/base/ossimSource.h>
#include <ossim/imaging/ossimImageStatisticsSource.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimImageStatisticsSource.h"

#endif
