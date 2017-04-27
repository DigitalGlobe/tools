// Copyright (C) 2006 Apogee Imaging International
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Tisham Dhar (tisham@apogee.com.au)
// Description: Converts complex data is ossim format to absolute values
//
//*************************************************************************
// $Id: ossimComplexAbs.cpp 10108 2006-12-14 16:15:30Z gpotts $
#include <ossim/base/ossimRefPtr.h>
#include "ossimComplexAbs.h"
#include <ossim/imaging/ossimU8ImageData.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimImageSourceFactoryBase.h>
#include <ossim/imaging/ossimImageSourceFactoryRegistry.h>
#include <ossim/base/ossimRefPtr.h>

/*************************************************************************
* Absolute value = sqrt(real^2 + imag^2)
* The number of input bands has to be even
* The number of output bands 1/2 the input bands
* output data type == double/float64 (input data type can be double,float or int)
**************************************************************************/

RTTI_DEF1(ossimComplexAbs, "ossimComplexAbs", ossimImageSourceFilter)

ossimComplexAbs::ossimComplexAbs(ossimObject* owner)
   :ossimImageSourceFilter(owner),
    theTile(NULL),
	 theOutputBandCount(1)
{
}

ossimComplexAbs::ossimComplexAbs(ossimImageSource* inputSource,
                                           unsigned int outputbandcount)
   : ossimImageSourceFilter(NULL, inputSource),
     theTile(NULL),
     theOutputBandCount(outputbandcount)     
{
}

ossimComplexAbs::ossimComplexAbs(ossimObject* owner,
                                           ossimImageSource* inputSource,
                                           unsigned int outputbandcount)
   : ossimImageSourceFilter(owner, inputSource),
     theTile(NULL),
     theOutputBandCount(outputbandcount)
{
}

ossimComplexAbs::~ossimComplexAbs()
{
}

ossimRefPtr<ossimImageData> ossimComplexAbs::getTile(const ossimIrect& tileRect,
                                                                ossim_uint32 resLevel)
{
   if(!theInputConnection)
   {
      return theTile;
   }

   if(!isSourceEnabled())
   {
      return theInputConnection->getTile(tileRect, resLevel);
   }
   ossimRefPtr<ossimImageData> data = theInputConnection->getTile(tileRect,
                                                                  resLevel);
	
   // First time through or after an initialize()...
   if (!theTile.valid())
   {
      allocate();
      if (!theTile.valid()) // Should never happen!
      {
         return data;
      }
   }
	
   theTile->setImageRectangle(tileRect);
   theTile->makeBlank();
   if(!data.valid() || !data->getBuf())
   {
      return theTile.get();
   }
   
   switch(data->getScalarType())
   {
      case OSSIM_UCHAR:
      {
         runAbsCalc(static_cast<ossim_uint8>(0), data, theTile);
         break;
      }
      case OSSIM_FLOAT: 
      case OSSIM_NORMALIZED_FLOAT:
      {
         runAbsCalc(static_cast<float>(0), data, theTile);
         break;
      }
      case OSSIM_USHORT16:
      case OSSIM_USHORT11:
      {
         runAbsCalc(static_cast<ossim_uint16>(0), data, theTile);
         break;
      }
      case OSSIM_SSHORT16:
      {
         runAbsCalc(static_cast<ossim_sint16>(0), data, theTile);
         break;
      }
      case OSSIM_DOUBLE:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         runAbsCalc(static_cast<double>(0), data, theTile);
         break;
      }
      default:
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimComplexAbs::getTile WARNING:\n"
            << "Scalar type = " << theTile->getScalarType()
            << " Not supported by ossimComplexAbs" << endl;
         break;
      }
   }
   theTile->validate();
   
   return theTile;
}

void ossimComplexAbs::initialize()
{
   if(theInputConnection)
   {
      theInputBandCount  = theInputConnection->getNumberOfOutputBands();
      theOutputBandCount = theInputBandCount/2;
      theTile = 0;
   }
}

void ossimComplexAbs::allocate()
{   
   if(theInputConnection)
   {
      theTile = ossimImageDataFactory::instance()->create(this,
                                                          this);
      
      theTile->initialize();
   }
}

ossim_uint32 ossimComplexAbs::getNumberOfOutputBands() const
{
   if(!isSourceEnabled()||((theInputBandCount%2) != 0))
   {
      return ossimImageSourceFilter::getNumberOfOutputBands();
   }
   return theOutputBandCount;
}

bool ossimComplexAbs::isSourceEnabled()const
{
   if(ossimSource::isSourceEnabled())
   {
      if((theInputBandCount%2) != 0)
      {
         return false;
      }
   }
   return ossimSource::isSourceEnabled();
}

bool ossimComplexAbs::saveState(ossimKeywordlist& kwl,
                                     const char* prefix)const
{
   ossimImageSourceFilter::saveState(kwl, prefix);

   kwl.add(prefix,
           "bandcount",
           theOutputBandCount,
           true);
   
   return true;
}

bool ossimComplexAbs::loadState(const ossimKeywordlist& kwl,
                                     const char* prefix)
{
   ossimImageSourceFilter::loadState(kwl, prefix);

   const char* lookup = kwl.find(prefix, "bandcount");
   if(lookup)
   {
      theOutputBandCount = ossimString(lookup).toUInt32();
   }
   return true;
}

template<class T> void ossimComplexAbs::runAbsCalc(T,
                                                   ossimRefPtr<ossimImageData> inputData,
                                                   ossimRefPtr<ossimImageData> outputData)
{
   T* bandSrc[2];
   T* bandDest;
   
   unsigned int inputBandCount = inputData->getNumberOfBands();
   unsigned int n = 0;

   if(inputData->getDataObjectStatus() == OSSIM_EMPTY) return;
   if(!inputData->getBuf()) return;
   if(inputData->getDataObjectStatus() == OSSIM_PARTIAL)
   {
      for(unsigned int m =0 ; m < inputBandCount ; m+=2 )
      {
         T np1 = (T)inputData->getNullPix(m);
         T np2 = (T)inputData->getNullPix(m+1);
         bandSrc[0]  = static_cast<T*>(inputData->getBuf(m));
         bandSrc[1]  = static_cast<T*>(inputData->getBuf(m+1));
         
         bandDest = static_cast<T*>(outputData->getBuf(n));
         
         long offset;
         
         long upperBound = outputData->getWidth()*outputData->getHeight();
         
         for(offset = 0; offset < upperBound; ++offset)
         {
            if((bandSrc[0][offset] != np1) &&
               (bandSrc[1][offset] != np2))
            {
               bandDest[offset] = static_cast<T>(sqrt((double)
                                                      (bandSrc[0][offset]*bandSrc[0][offset]) +
                                                      (double)(bandSrc[1][offset]*bandSrc[1][offset])));
            }
         }
         
         n++;
      }
   }
   else
   {
      for(unsigned int m =0 ; m < inputBandCount ; m+=2 )
      {
         bandSrc[0]  = static_cast<T*>(inputData->getBuf(m));
         bandSrc[1]  = static_cast<T*>(inputData->getBuf(m+1));
         
         bandDest = static_cast<T*>(outputData->getBuf(n));
         
         long offset;
         
         long upperBound = outputData->getWidth()*outputData->getHeight();
         
         for(offset = 0; offset < upperBound; ++offset)
         {
            bandDest[offset] = static_cast<T>(sqrt((double)
                                                   (bandSrc[0][offset]*bandSrc[0][offset]) +
                                                   (double)(bandSrc[1][offset]*bandSrc[1][offset])));
         }
         
         n++;
      }
   }
   
   
   outputData->validate();
}
