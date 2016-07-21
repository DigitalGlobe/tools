%module pyossim

%{
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimOutputSource.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/imaging/ossimImageSourceSequencer.h>
%}

%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimImageWriter.h"
