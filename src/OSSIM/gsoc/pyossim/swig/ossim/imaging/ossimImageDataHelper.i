/*

*/

%module pyossim

%{
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageDataHelper.h>
#include <ossim/base/ossimPolyArea2d.h>
#include <ossim/base/ossimRgbVector.h>
%}        

/* Wrapping the class */
%include "ossim/imaging/ossimImageDataHelper.h"
