%module pyossim

%{
#include <vector>
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimBandAverageFilter.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

%include "ossim/base/ossimConstants.h"

/**
 * This filter outputs a single band that is the weighted average of all the
 * input bands retrieved from the getTile.  This filter will work with any
 * input scalar type.  Note the OSSIMDLLEXPORT is used for the different
 * compilers that require importing and exporting symbols for shared library
 * support.
 */
%include "ossim/imaging/ossimBandAverageFilter.h"

#endif
