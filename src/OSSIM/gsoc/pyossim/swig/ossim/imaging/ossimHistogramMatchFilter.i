/*-----------------------------------------------------------------------------
 * Filename        : ossimHistogramMatchFilter.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimHistogramMatchFilter
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimHistogramEqualization.h>
#include <ossim/imaging/ossimHistogramMatchFilter.h>

%}

#ifndef TYPE_DATA
#define TYPE_DATA

%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimHistogramMatchFilter.h"

#endif
