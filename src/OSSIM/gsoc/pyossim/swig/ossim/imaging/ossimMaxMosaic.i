%module pyossim

%{
#include <vector>
#include <ossim/imaging/ossimImageCombiner.h>
#include <ossim/imaging/ossimMaxMosaic.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

%include "ossim/base/ossimConstants.h"

/**
 * An image mosaic is a simple combiner that will
 * just do a simple mosaic.  It just checks NULL pix values until it finds a
 * pixel that is not empty and copies it out to the output.  The list will
 * have same size tiles and have the same number of bands.
 */
%include "ossim/imaging/ossimMaxMosaic.h"

#endif
