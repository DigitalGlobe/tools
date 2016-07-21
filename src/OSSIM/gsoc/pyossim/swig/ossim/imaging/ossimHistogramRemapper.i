/*-----------------------------------------------------------------------------
Filename        : ossimHistogramRemapper.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimHistogramRemapper 
-----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/imaging/ossimTableRemapper.h>
#include <ossim/imaging/ossimHistogramRemapper.h>
#include <ossim/base/ossimMultiResLevelHistogram.h>

%}


#ifndef TYPE_DATA
#define TYPE_DATA


/* Include the required header files */
%import "ossim/base/ossimConstants.h";

/* Handling ossimHistogramRemapper assignment operator */
%rename(__set__) ossimHistogramRemapper::operator=;
%rename(__lshift__) operator<<;

/* Handling the reserved function print */
%rename(ossimHistogramRemapper_print) ossimHistogramRemapper::print;

/* Handling the vector<> */
%include "std_vector.i"
namespace std
{
        %template(vectorossimFloat64) vector<ossim_float64>;
        %template(vectorossimUInt32) vector<ossim_uint32>;
};


/* Wrapping the class ossimHistogramRemapper */
%include "ossim/imaging/ossimHistogramRemapper.h"

#endif                               
