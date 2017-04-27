/*-----------------------------------------------------------------------------
 * Filename        : ossimImageMosaic.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimImageMosaic 
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <vector>

#include <ossim/imaging/ossimImageCombiner.h>
#include <ossim/imaging/ossimImageData.h>

%}

/* Include the header file containing the declarations to be wrapped */
%import "ossim/base/ossimConstants.h"

%include "ossim/imaging/ossimImageCombiner.h"
%include "ossim/imaging/ossimImageMosaic.h"
