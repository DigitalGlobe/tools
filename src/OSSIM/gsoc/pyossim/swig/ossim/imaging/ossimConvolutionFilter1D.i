%module pyossim

%{
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimConvolutionFilter1D.h>
%}

/**
 * class for vertical or horizontal convolution
 *
 * PROPERTIES:
 *  -KernelSize   : length of linear kernel (>=1)
             NOTE - you need to exit the property editor for the kernel to be resized
 *  -Kernel       : Kernel vector (line matrix)
 *  -Horizontal   : boolean, otherwise vertical kernel
 *  -CenterOffset : center pixel position in the kernel, starting at 0
 *  -StrictNoData : controls NODATA use
 *      true  : any NODATA pixel in the convolution will make the center pixel NODATA
 *      false : if center is NODATA, then output center is NODATA,
 *              other NODATA pixels are processed as if they were zero in the convolution
 */
#ifndef TYPE_DATA   
#define TYPE_DATA

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimConvolutionFilter1D.h"

#endif
