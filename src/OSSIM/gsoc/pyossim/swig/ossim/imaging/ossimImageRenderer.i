/*

*/

%module pyossim

%{
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/projection/ossimImageViewTransform.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimViewInterface.h>
#include <ossim/base/ossimRationalNumber.h>
%}


%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimViewInterface.h"
%include "ossim/imaging/ossimImageRenderer.h"
