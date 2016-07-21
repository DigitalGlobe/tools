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
// $Id: ossimJpegYCbCrToRgbSource.h 23664 2015-12-14 14:17:27Z dburken $
#ifndef ossimJpegYCbCrToRgbSource_HEADER
#define ossimJpegYCbCrToRgbSource_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>

class ossimJpegYCbCrToRgbSource : public ossimImageSourceFilter
{
public:
   ossimJpegYCbCrToRgbSource();
   ossimJpegYCbCrToRgbSource(ossimImageSource* inputSource);
   
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& origin,
                                               ossim_uint32 resLevel=0);

   virtual void initialize();
       
protected:
   virtual ~ossimJpegYCbCrToRgbSource();

   virtual void allocate();
   
   ossimRefPtr<ossimImageData> theBlankTile;

TYPE_DATA
};

#endif

