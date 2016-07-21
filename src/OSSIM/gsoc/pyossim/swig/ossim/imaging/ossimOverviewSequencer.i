%module pyossim

%{
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimMultiBandHistogram.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimFilterResampler.h>
#include <ossim/imaging/ossimBitMaskWriter.h>
#include <ossim/imaging/ossimMaskFilter.h>
#include <ossim/imaging/ossimOverviewSequencer.h>
#include <string>
#include <vector>
%}

%include "ossim/base/ossimConstants.h"

/**
 * @class Sequencer for building overview files.
 */
%include "ossim/imaging/ossimOverviewSequencer.h"
