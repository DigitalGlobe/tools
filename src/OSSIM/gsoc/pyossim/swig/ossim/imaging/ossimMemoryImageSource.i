/*
*/

%module pyossim

%{
#include <ossim/base/ossimConstants.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/imaging/ossimMemoryImageSource.h>
%}


%include "ossim/base/ossimConstants.h"


%include "ossim/imaging/ossimMemoryImageSource.h"
