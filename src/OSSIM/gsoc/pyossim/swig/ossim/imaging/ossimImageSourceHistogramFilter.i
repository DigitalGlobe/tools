/*-----------------------------------------------------------------------------
Filename        : ossimImageSourceHistogramFilter.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimImageSourceHistogramFilter
-----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimImageSourceHistogramFilter.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimMultiResLevelHistogram.h>

%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Include the required header files */
%import "ossim/base/ossimConstants.h";

/* Handling the reserved function print */
%rename(ossimImageSourceHistogramFilter_print) ossimImageSourceHistogramFilter::print;


/* Wrapping the class ossimImageSourceHistogramFilter */
%include "ossim/imaging/ossimImageSourceHistogramFilter.h"

#endif                                        
