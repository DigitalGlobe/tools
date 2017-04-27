/*-----------------------------------------------------------------------------
 * Filename        : ossimHistogramEqualization.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimHistogramEqualization
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/imaging/ossimImageSourceHistogramFilter.h>
#include <ossim/imaging/ossimHistogramEqualization.h>

#include <vector>

%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling the vector<ossim_uint32> */
%include "std_vector.i"
namespace std
{
        %template(vectorossimUInt32) vector<ossim_uint32>;
};

/* Handling the reserved function print */
%rename(ossimHistogramEqualization_print) ossimHistogramEqualization::print;


/* Wrapping the class ossimHistogramEqualization */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimHistogramEqualization.h"

#endif
