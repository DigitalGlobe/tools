%module pyossim

%{
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimWatermarkFilter.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

%rename(ossimWatermarkFilter_print) print;

%include "ossim/base/ossimConstants.h"


/**
 * class ossimWatermarkFilter
 * Applies an image or watermark to image.
 * Positioning is based on mode.
 * Density is base on alpha weight.
 */
%include "ossim/imaging/ossimWatermarkFilter.h"

#endif
