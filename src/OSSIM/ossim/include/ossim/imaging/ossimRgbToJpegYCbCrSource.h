//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimRgbToJpegYCbCrSource.h 23664 2015-12-14 14:17:27Z dburken $
#ifndef ossimRgbToJpegYCbCrSource_HEADER
#define ossimRgbToJpegYCbCrSource_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>

class ossimRgbToJpegYCbCrSource : public ossimImageSourceFilter
{
public:
   
   ossimRgbToJpegYCbCrSource();
   ossimRgbToJpegYCbCrSource(ossimImageSource* inputSource);
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);
       
protected:
   virtual ~ossimRgbToJpegYCbCrSource();
   ossimRefPtr<ossimImageData> theBlankTile;

TYPE_DATA
};

#endif

