/*

*/

%module pyossim

%{
#include <ossim/imaging/ossimImageCombiner.h>
#include <ossim/imaging/ossimBandMergeSource.h>
%}



#ifndef TYPE_DATA   
#define TYPE_DATA
#endif
/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimBandMergeSource.h"



