/*

*/

%module pyossim

%{
#include <vector>

#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/imaging/ossimImageData.h>
%}       

%rename(const_ossimRgbImage_getImageData) ossimRgbImage::getImageData() const;

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimRgbImage.h"
