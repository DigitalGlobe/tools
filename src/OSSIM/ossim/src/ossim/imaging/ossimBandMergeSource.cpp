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
// $Id: ossimBandMergeSource.cpp 23664 2015-12-14 14:17:27Z dburken $
#include <ossim/imaging/ossimBandMergeSource.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimIrect.h>

RTTI_DEF1(ossimBandMergeSource, "ossimBandMergeSource", ossimImageCombiner)

ossimBandMergeSource::ossimBandMergeSource()
   :ossimImageCombiner(),
    theNumberOfOutputBands(0),
    theTile(NULL)
{
}

ossimBandMergeSource::ossimBandMergeSource(ossimConnectableObject::ConnectableObjectList& inputSources)
   :ossimImageCombiner(inputSources),
    theNumberOfOutputBands(0),
    theTile(NULL)
{
   initialize();
}

ossimBandMergeSource::~ossimBandMergeSource()
{
   theNumberOfOutputBands = 0;
}

ossimRefPtr<ossimImageData> ossimBandMergeSource::getTile(const ossimIrect& tileRect,
                                                          ossim_uint32 resLevel)
{
   ossim_uint32 layerIdx = 0;
   if( ( getNumberOfInputs() == 1) || !isSourceEnabled() )
   {
      return getNextTile(layerIdx, 0, tileRect, resLevel);
   }

   // test if initialized
   if(!theTile.get())
   {
      allocate();
   }
   
   if(!theTile.get())
   {
      return getNextTile(layerIdx, 0, tileRect, resLevel);
   }
   
   long w     = tileRect.width();
   long h     = tileRect.height();
   long tileW = theTile->getWidth();
   long tileH = theTile->getHeight();
   if ((w != tileW) || (h != tileH))
   {
      theTile->setWidth(w);
      theTile->setHeight(h);
      if((w*h)!=(tileW*tileH))
      {
         theTile->initialize();
      }
      else
      {
         theTile->makeBlank();
      }
   }
   else
   {
      theTile->makeBlank();
   }

   theTile->setOrigin(tileRect.ul());
   getTile(theTile.get(), resLevel);

   return theTile;
}

bool ossimBandMergeSource::getTile(ossimImageData* tile, ossim_uint32 resLevel)
{
   if (!tile)
      return false;

   ossim_uint32 layerIdx = 0;
   if( ( getNumberOfInputs() == 1) || !isSourceEnabled() )
   {
      return getNextTile(layerIdx, 0, tile, resLevel);
   }

   tile->makeBlank();
   ossim_uint32 currentBand = 0;
   ossim_uint32 maxBands = tile->getNumberOfBands();
   ossim_uint32 inputIdx = 0;
   ossimRefPtr<ossimImageData> currentTile;
   for(inputIdx = 0; inputIdx < getNumberOfInputs(); ++inputIdx)
   {
      ossimImageSource* input = PTR_CAST(ossimImageSource, getInput(inputIdx));
      ossim_uint32 maxInputBands = 1;

      if(input)
      {
         currentTile = input->getTile(tile->getImageRectangle(), resLevel);
         if(currentTile.valid())
         {
            maxInputBands = currentTile->getNumberOfBands();
            //std::cout << "MAX INPUT BANDS === " << maxInputBands << "\n";
            if (maxInputBands == 0)
               maxInputBands = 1;
         }
      }
      else
      {
         currentTile = 0;
      }

      if(currentTile.valid()&&(currentTile->getBuf()))
      {
         for(ossim_uint32 band = 0; (band < maxInputBands) && (currentBand < maxBands); ++band)
         {
            //std::cout << "Actual Band, BAND == " << currentBand << ", " << band << "\n";
            // clear the band with the actual NULL
            tile->fill(currentBand, tile->getNullPix(band));

            if(currentTile.valid())
            {
               if((currentTile->getDataObjectStatus() != OSSIM_NULL) &&
                  (currentTile->getDataObjectStatus() != OSSIM_EMPTY))
               {
                  memmove(tile->getBuf(currentBand),
                          currentTile->getBuf(band),
                          currentTile->getSizePerBandInBytes());
               }
            }
            ++currentBand;
         }
      }
   }
   tile->validate();
   return true;
}

double ossimBandMergeSource::getNullPixelValue(ossim_uint32 band)const
{
   ossim_uint32 currentBandCount = 0;
   ossim_uint32 idx              = 0;

   ossim_uint32 maxBands = getNumberOfOutputBands();
   if(!maxBands) return 0.0;
   
   while((currentBandCount < maxBands)&&
	 (idx < getNumberOfInputs()))
   {
      ossimImageSource* temp = PTR_CAST(ossimImageSource, getInput(idx));
      if(temp)
      {
	ossim_uint32 previousCount = currentBandCount;
	currentBandCount += temp->getNumberOfOutputBands();

         if(band < currentBandCount)
         {
            return temp->getNullPixelValue(band - previousCount);
         }
      }
      ++idx;
   }
   
   return ossim::nan();
}

double ossimBandMergeSource::getMinPixelValue(ossim_uint32 band)const
{
   ossim_uint32 currentBandCount = 0;
   ossim_uint32 idx              = 0;

   ossim_uint32 maxBands = getNumberOfOutputBands();
   if(!maxBands) return 0.0;
   
   while((currentBandCount < maxBands)&&
	 (idx < getNumberOfInputs()))
   {
      ossimImageSource* temp = PTR_CAST(ossimImageSource, getInput(idx));
      if(temp)
      {
	ossim_uint32 previousCount = currentBandCount;
	currentBandCount += temp->getNumberOfOutputBands();

         if(band < currentBandCount)
         {
            return temp->getMinPixelValue(band - previousCount); 
         }
      }
      ++idx;
   }
   
   return 0.0;
}

double ossimBandMergeSource::getMaxPixelValue(ossim_uint32 band)const
{
   ossim_uint32 currentBandCount = 0;
   ossim_uint32 idx              = 0;

   ossim_uint32 maxBands = getNumberOfOutputBands();
   if(!maxBands) return 0.0;
   
   while((currentBandCount < maxBands)&&
	 (idx < getNumberOfInputs()))
   {
      ossimImageSource* temp = PTR_CAST(ossimImageSource, getInput(idx));
      if(temp)
      {
	ossim_uint32 previousCount = currentBandCount;
	currentBandCount += temp->getNumberOfOutputBands();

         if(band < currentBandCount)
         {
            return temp->getMaxPixelValue(band - previousCount);
         }
      }
      ++idx;
   }
   
   return ossim::nan();
}

void ossimBandMergeSource::initialize()
{
   ossimImageCombiner::initialize();

   if(theTile.get())
   {
      theTile = NULL;
   }
   
   theNumberOfOutputBands = computeNumberOfInputBands();
}

void ossimBandMergeSource::allocate()
{
   if(theNumberOfOutputBands)
   {
      theTile = ossimImageDataFactory::instance()->create(this,
                                                          this);
      theTile->initialize();
   }
}

ossim_uint32 ossimBandMergeSource::computeNumberOfInputBands()const
{
   ossim_uint32 result = 0;
   ossim_uint32 size   = getNumberOfInputs();
   for(ossim_uint32 index = 0; index < size; ++index)
   {
      ossimImageSource* temp = PTR_CAST(ossimImageSource, getInput(index));
      if(temp)
      {
         if(temp->getNumberOfOutputBands() == 0)
         {
            ++result;
         }
         else
         {
            result += temp->getNumberOfOutputBands();
         }
      }
   }
   return result;
}

ossim_uint32 ossimBandMergeSource::getNumberOfOutputBands() const
{
   if(!theNumberOfOutputBands)
   {
      return computeNumberOfInputBands();
   }
   
   return theNumberOfOutputBands;
}

