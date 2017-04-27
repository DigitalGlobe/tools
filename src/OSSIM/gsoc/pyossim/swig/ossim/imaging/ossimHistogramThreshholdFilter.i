/*-----------------------------------------------------------------------------
 * Filename        : ossimHistogramThreshholdFilter.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimHistogramThreshholdFilter
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/imaging/ossimImageSourceHistogramFilter.h>
#include <ossim/imaging/ossimHistogramThreshholdFilter.h>

%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Include the required header files */
%import "ossim/base/ossimConstants.h";

/* Wrapping the class ossimHistogramThreshholdFilter */
%include "ossim/imaging/ossimHistogramThreshholdFilter.h"

#endif
