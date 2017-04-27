%module pyossim

%{
#include <ossim/imaging/ossimImageCombiner.h>
#include <ossim/imaging/ossimMaskFilter.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

%include "ossim/base/ossimConstants.h"

/**
 * <pre>
 * class ossimMaskFilter
 *
 *    Requires 2 inputs.  The first input is assumed to be the image input
 *    and the second input is assumed to be the mask data.  It will only
 *    use one band of the mask and multi band masks are not supported.
 *
 *    the number of bands, min pix, max pix, null pix ...etc are mapped
 *    to the first input.
 *
 * Keywords:
 *      mask_type:
 *
 * keywords description
 *      mask_type  This keyword can have the following values:
 *                 select, invert, or weighted.
 *                  - select will use the input data and every where
 *                    the mask is greater than 0 it will copy the input to the output.
 *                  - invert will use the input data and every where the mask is 0 it
 *                    will copy the input to the output else it will place a null in
 *                    the output.
 *                  - weighted will normalize the mask between 0 and 1 and then multiply
 *                    the input by that normalized value and copy to the output.
 *
 * example of keywords:
 *
 *      mask_type: select
 *
 * </pre>
 */
%include "ossim/imaging/ossimMaskFilter.h"

#endif
