/*

*/

%module pyossim
%feature("notabstract") ossimSingleImageChain;
%{
#include <vector>
#include <ossim/base/ossimConstants.h>
#include <ossim/imaging/ossimBandSelector.h>
#include <ossim/imaging/ossimCacheTileSource.h>
#include <ossim/imaging/ossimHistogramRemapper.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageRenderer.h>
#include <ossim/imaging/ossimScalarRemapper.h>

%}


%include "ossim/imaging/ossimSingleImageChain.h"
