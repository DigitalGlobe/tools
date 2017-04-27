%module pyossim

%{
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimPixelFlipper.h>
#include <ossim/base/ossimPolygon.h>
#include <OpenThreads/ReentrantMutex>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

%rename(ossimPixelFlipper_print) print;

%include "ossim/base/ossimConstants.h"

/**
 *  Class to scan pixels and flip target dn value to a replacement dn
 *  value.
 *
 *  This was written to fix problems with null pixels, i.e. an image has a
 *  digital number(dn) of 255 for null and it is desired to use a dn of 0 as
 *  a null.  This can really be used to flip any pixel value to another.
 *
 *  @note This filter currently works on the input tile directly and does
 *  not copy the data.
 *
 *  @see theReplacementMode data member documentation for more info.
 */
%include "ossim/imaging/ossimPixelFlipper.h"

#endif
