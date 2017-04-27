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
// Description: SPOT False Natural Colour Genertion Algorithm
//
//*************************************************************************
// $Id: ossimSPOTNatural.cpp 16169 2009-12-22 15:41:54Z dburken $

#include <vector>

#include <ossim/base/ossimRefPtr.h>
#include "ossimSPOTNatural.h"
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
* SPOT and ASTER False Natural Colour Algorithm, output is in Byte
* R = Band 2
* G = (3*Band 1 + Band 3)/4
* B = (3*Band 1 - band 3)/4
* Band 1 = Green
* Band 2 = Red
* Band 3 = NIR
**************************************************************************/

RTTI_DEF1(ossimSPOTNatural, "ossimSPOTNatural", ossimImageSourceFilter)

ossimSPOTNatural::ossimSPOTNatural(ossimObject* owner)
   :ossimImageSourceFilter(owner),
    theTile(NULL),
    theC1(1.0/3.0),
    theC2(1.0/3.0),
    theC3(1.0/3.0)
{
}

ossimSPOTNatural::ossimSPOTNatural(ossimImageSource* inputSource,
                                           double c1,
                                           double c2,
                                           double c3)
   : ossimImageSourceFilter(NULL, inputSource),
     theTile(NULL),
     theC1(c1),
     theC2(c2),
     theC3(c3)
{
}

ossimSPOTNatural::ossimSPOTNatural(ossimObject* owner,
                                           ossimImageSource* inputSource,
                                           double c1,
                                           double c2,
                                           double c3)
   : ossimImageSourceFilter(owner, inputSource),
     theTile(NULL),
     theC1(c1),
     theC2(c2),
     theC3(c3)
{
}

ossimSPOTNatural::~ossimSPOTNatural()
{
}

ossimRefPtr<ossimImageData> ossimSPOTNatural::getTile(const ossimIrect& tileRect,
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
   if(!data.valid() || !data->getBuf())
   {
      return data;
   }
	
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
   
   switch(data->getScalarType())
   {
      case OSSIM_UCHAR:
      {
         runNatColour(static_cast<ossim_uint8>(0), data, theTile);
         break;
      }
      case OSSIM_FLOAT: 
      case OSSIM_NORMALIZED_FLOAT:
      {
         runNatColour(static_cast<float>(0), data, theTile);
         break;
      }
      case OSSIM_USHORT16:
      case OSSIM_USHORT11:
      {
         runNatColour(static_cast<ossim_uint16>(0), data, theTile);
         break;
      }
      case OSSIM_SSHORT16:
      {
         runNatColour(static_cast<ossim_sint16>(0), data, theTile);
         break;
      }
      case OSSIM_DOUBLE:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         runNatColour(static_cast<double>(0), data, theTile);
         break;
      }
      default:
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimSPOTNatural::getTile WARNING:\n"
            << "Scalar type = " << theTile->getScalarType()
            << " Not supported by ossimSPOTNatural" << endl;
         break;
      }
   }
   theTile->validate();
   
   return theTile;
}

void ossimSPOTNatural::initialize()
{
   if(theInputConnection)
   {
      theTile = NULL;
      /*
      theTile = new ossimU8ImageData(this,
                                     3,
                                     theInputConnection->getTileWidth(),
                                     theInputConnection->getTileHeight());  
      theTile->initialize();
      */
   }
}

void ossimSPOTNatural::allocate()
{   
   if(theInputConnection)
   {
      ossimImageDataFactory* idf = ossimImageDataFactory::instance();
      
      theTile = idf->create(this,
                            this);
      
      theTile->initialize();
   }
}

ossim_uint32 ossimSPOTNatural::getNumberOfOutputBands() const
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getNumberOfOutputBands();
   }
   return 3;
}

bool ossimSPOTNatural::saveState(ossimKeywordlist& kwl,
                                     const char* prefix)const
{
   ossimImageSourceFilter::saveState(kwl, prefix);

   kwl.add(prefix,
           "c1",
           theC1,
           true);
   kwl.add(prefix,
           "c2",
           theC2,
           true);
   kwl.add(prefix,
           "c3",
           theC3,
           true);
   
   return true;
}

bool ossimSPOTNatural::loadState(const ossimKeywordlist& kwl,
                                     const char* prefix)
{
   ossimImageSourceFilter::loadState(kwl, prefix);

   const char* lookup = kwl.find(prefix, "c1");
   if(lookup)
   {
      theC1 = ossimString(lookup).toDouble();
   }
   lookup = kwl.find(prefix, "c2");
   if(lookup)
   {
      theC2 = ossimString(lookup).toDouble();
   }
   lookup = kwl.find(prefix, "c3");
   if(lookup)
   {
      theC3 = ossimString(lookup).toDouble();
   }
   return true;
}

template<class T> void ossimSPOTNatural::runNatColour(T,
   ossimRefPtr<ossimImageData> inputData,
   ossimRefPtr<ossimImageData> outputData)
{   
   std::vector<T*> bandSrc(inputData->getNumberOfBands());
   std::vector<T*> bandDest(3);
   
   if(inputData->getNumberOfBands() == 1)
   {
      bandSrc[0]  = static_cast<T*>(inputData->getBuf(0));
      bandSrc[1]  = static_cast<T*>(inputData->getBuf(0));
      bandSrc[2]  = static_cast<T*>(inputData->getBuf(0));
   }
   else if(inputData->getNumberOfBands() == 2)
   {
      bandSrc[0]  = static_cast<T*>(inputData->getBuf(0));
      bandSrc[1]  = static_cast<T*>(inputData->getBuf(1));
      bandSrc[2]  = static_cast<T*>(inputData->getBuf(1));      
   }
   else if(inputData->getNumberOfBands() == 3)
   {
      bandSrc[0]  = static_cast<T*>(inputData->getBuf(0));
      bandSrc[1]  = static_cast<T*>(inputData->getBuf(1));
      bandSrc[2]  = static_cast<T*>(inputData->getBuf(2));      
   }
   
   bandDest[0] = static_cast<T*>(outputData->getBuf(0));
   bandDest[1] = static_cast<T*>(outputData->getBuf(1));
   bandDest[2] = static_cast<T*>(outputData->getBuf(2));
   
   long offset;
	
   long upperBound = outputData->getWidth()*outputData->getHeight();
   
   for(offset = 0; offset < upperBound; ++offset)
   {
      T value1,value2,value3;
      
      value1 = static_cast<T>(bandSrc[1][offset]);
      value2 = static_cast<T>(0.75*(double)bandSrc[0][offset]
      					+0.25*(double)bandSrc[2][offset]);
      value3 = static_cast<T>(0.75*(double)bandSrc[0][offset]
      					-0.25*(double)bandSrc[2][offset]);
      /*
      value1 = value1<255?value1:255;
      value1 = value1>0?value1:0;
      
      value2 = value2<255?value2:255;
      value2 = value2>0?value2:0;
      
      value3 = value3<255?value3:255;
      value3 = value3>0?value3:0;
		*/
		
      bandDest[0][offset] = value1;
      bandDest[1][offset] = value2;
      bandDest[2][offset] = value3;
   }

   outputData->validate();
}
