/*-----------------------------------------------------------------------------
Filename        : ossimImageHandler.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimImageHandler 
-----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/imaging/ossimImageSource.h>
#include <ossim/imaging/ossimImageMetaData.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimNBandLutDataObject.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/imaging/ossimFilterResampler.h>

#include <vector>

%}

#ifndef TYPE_DATA
#define TYPE_DATA
#endif


%include "ossim/base/ossimConstants.h"

%include "ossim/imaging/ossimImageHandler.h"


