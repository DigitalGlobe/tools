%module pyossim

%{
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimOutputSource.h>
#include <ossim/imaging/ossimPixelFlipper.h>
#include <ossim/imaging/ossimBitMaskWriter.h>
#include <vector>
%}

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimBitMaskWriter.h"
