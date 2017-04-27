/*

*/

%module pyossim

%{
#include <ossim/imaging/ossimImageFileWriter.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/projection/ossimMapProjectionInfo.h>
#include <ossim/base/ossimNBandLutDataObject.h>
#include <ossim/imaging/ossimNBandToIndexFilter.h>
#include <ossim/imaging/ossimTiffWriter.h>
%}

%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimTiffWriter.h"
