%module pyossim

%{
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimConvolutionFilter1D.h>
#include <ossim/imaging/ossimImageGaussianFilter.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

/**
 * class for symmetric Gaussian filtering
 * implemented as two separable horizontal/vertical gaussian filters
 *
 * PROPERTIES:
 * -GaussStd is the standard deviation of the gaussian
 *  (filter widths are equal to 2*floor(2.5*GaussStd+0.5)+1)
 * -strictNoData selects the behaviour for NODATA pixels
 *   true  : any NODATA pixels in the convolution will Nullify the center pixel
 *   false : center pixel will be NODATA only if it was NODATA before 
 *     other NODATA pixels are processed as zero in the convolution calculation
 */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimImageGaussianFilter.h"

#endif
