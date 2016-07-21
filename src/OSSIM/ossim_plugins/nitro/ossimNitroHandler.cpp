#include "ossimNitroHandler.h"
#include <ossim/imaging/ossimTiffTileSource.h>
#include <ossim/base/ossimBooleanProperty.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimNBandLutDataObject.h>
#include <ossim/imaging/ossimAnnotationPolyObject.h>
#include <ossim/imaging/ossimAnnotationMultiPolyLineObject.h>
#include <ossim/imaging/ossimAnnotationFontObject.h>
#include <ossim/imaging/ossimAnnotationEllipseObject.h>

#include <ossim/base/ossimPolyLine.h>

RTTI_DEF1(ossimNitroHandler, "ossimNitroHandler", ossimImageHandler)
static const ossimTrace traceDebug("ossimNitroHandler:debug");

ossimNitroHandler::ossimNitroHandler()
:theReader(0),
theRecord(0),
theIoHandle(0),
theNumberOfImages(0),
theCurrentImageReader(0),
theCurrentImageSegment(0),
theNumberOfBits(0),
theNumberOfInputBands(0),
theNumberOfRows(0),
theNumberOfColumns(0),
theSubImageSize(0),
theCurrentImageEntry(0),
theScalarType(OSSIM_SCALAR_UNKNOWN),
theReadBuffer(0),
theSubImage(0),
theIsVqCompressedFlag(false),
theEnableAnnotationsFlag(false),
theAnnotationImage(new ossimRgbImage())
{
   nitf_Error e;
   theSubImage = nitf_SubWindow_construct(&e);
   
}

ossimNitroHandler::~ossimNitroHandler()
{
   if(theSubImage)
   {
      nitf_SubWindow_destruct(&theSubImage);
      theSubImage = 0;
   }
   close();
}

void ossimNitroHandler::close()
{
   theAnnotationBounds.makeNan();
   theAnnotations.clear();
   if(theCurrentImageReader) 
   {
      nitf_ImageReader_destruct(&theCurrentImageReader);
      theCurrentImageReader = 0;
   }
   if(theRecord)
   {
      nitf_Record_destruct(&theRecord);
      theRecord = 0;
   }
   if(theReader)
   {
      nitf_Reader_destruct(&theReader);
      theReader = 0;
   }
   if(theIoHandle)
   {
      nitf_IOHandle_close(theIoHandle);
   }
   if(theReadBuffer)
   {
      ossim_uint32 i=0;
      for (i = 0; i < theNumberOfInputBands; i++)
         NITF_FREE(theReadBuffer[i]);
      NITF_FREE(theReadBuffer);
   }
   theReadBuffer = 0;
   theCurrentImageSegment = 0;
   theNumberOfImages = 0;
   theNumberOfBits = 0;
   theNumberOfInputBands = 0;
   theNumberOfRows = 0;
   theNumberOfColumns = 0;
   theSubImageSize = 0;
   theCurrentImageEntry = 0;
   theIsVqCompressedFlag = false;
}

bool ossimNitroHandler::open()
{
   close();
   nitf_Error e;
   theReader = nitf_Reader_construct(&e);
   if(!theReader)
   {
      close(); // clean up
      return false;
   }
   theCurrentImageRect.makeNan();
   theIoHandle = nitf_IOHandle_create(getFilename().c_str(),
                                      NITF_ACCESS_READONLY,
                                      NITF_OPEN_EXISTING, &e);
   if (NITF_INVALID_HANDLE(theIoHandle))
   {
      close();
      return false;
   }
   theRecord = nitf_Reader_read(theReader, theIoHandle, &e);
   if(!theRecord)
   {
      close();
      return false;
   }
   
   int numImages;
   if (!nitf_Field_get
       (theRecord->header->numImages, &numImages, NITF_CONV_INT,
        NITF_INT32_SZ, &e))
   {
      theNumberOfImages = 0;
   }
   else
   {
      theNumberOfImages = numImages;
   }
   
   int numSegments;
   /**************************************************************************/
   /* GRAPHICS                                                               */
   /**************************************************************************/
   /* get the # of images from the field */
   if (!nitf_Field_get(theRecord->header->numGraphics,
                       &numSegments, NITF_CONV_INT, NITF_INT32_SZ, &e))
   {
      theNumberOfGraphicSegments = 0;
   }
   else
   {
      theNumberOfGraphicSegments = numSegments;
   }
   /**************************************************************************/
   /* TEXT                                                               */
   /**************************************************************************/
   /* get the # of images from the field */
   if (!nitf_Field_get(theRecord->header->numTexts,
                       &numSegments, NITF_CONV_INT, NITF_INT32_SZ, &e))
   {
      theNumberOfTextSegments = 0;
   }
   else
   {
      theNumberOfTextSegments = numSegments;
   }
   
   /**************************************************************************/
   /* TEXT                                                               */
   /**************************************************************************/
   /* get the # of images from the field */
   if (!nitf_Field_get(theRecord->header->numLabels,
                       &numSegments, NITF_CONV_INT, NITF_INT32_SZ, &e))
   {
      theNumberOfLabelSegments = 0;
   }
   else
   {
      theNumberOfLabelSegments = numSegments;
   }
   
   
   if(theNumberOfImages > 0)
   {
      if(!setCurrentEntry(0)) 
      {
         return false;
      }
   }
   loadAnnotations();
   if(theAnnotations.size())
   {
      theMaxAnnotationRlevel = 0;
      ossim_uint32 maxDimension = ossim::max(theAnnotationBounds.width(), theAnnotationBounds.height());
      while(maxDimension > 128)
      {
         maxDimension <<=1;
         ++theMaxAnnotationRlevel;
      }
      ossim_uint32 idx = 0;
      for(idx=0; idx < theMaxAnnotationRlevel; ++idx)
      {
         getAnnotationListForRlevel(idx);
      }
   }
   if(theNumberOfInputBands == 0)
   {
      if(theAnnotations.size() > 0)
      {
         theNumberOfInputBands = 3;
         theNumberOfOutputBands = theNumberOfInputBands;
      }
   }
   initializeScalarType();
   
   std::cout << "IMAGES === " << theNumberOfImages << std::endl
   << "LABELS === " << theNumberOfLabelSegments << std::endl
   << "GRAPHICS = " << theNumberOfGraphicSegments << std::endl;
   nitf_Uint32 theNumberOfTextSegments;
   nitf_Uint32 theNumberOfLabelSegments;
   
   bool result = ((theNumberOfImages>0)&&
                  (theAnnotations.size()>0));
   if(result)
   {
      completeOpen();
   }
   return result;
}

ossimRefPtr<ossimImageData> ossimNitroHandler::getTile(const ossimIrect& tileRect,
                                                       ossim_uint32 resLevel)
{
   if(!isSourceEnabled() || !isOpen() || !isValidRLevel(resLevel))
   {
      return ossimRefPtr<ossimImageData>();
   }
   if(!theTile.valid())
   {
      initializeOutputTile();
   }
   if(!theTile.valid()) return theTile;
   theTile->setImageRectangle(tileRect);
   theTile->makeBlank();
   if(resLevel > 0)
   {
      
      if (theOverview)
      {
         if (theOverview->hasR0() || resLevel)
         {
            // Subtract any offsets since the overview doesn't know about them.
            ossimIrect rr_rect = tileRect - getSubImageOffset(resLevel);
            
            //---
            // If there is a sub image offset then we will not be requesting
            // the same rectangle as was requested from us.  For this reason,
            // we'll do a setOrigin, rather than just returning the getTile
            // result.
            //---
            ossimRefPtr<ossimImageData> tileData = theOverview->getTile(rr_rect,
                                                                        resLevel);
            if(tileData.valid())
            {
               tileData->setImageRectangle(tileRect);
               
               if(getOutputScalarType() == OSSIM_USHORT11)
               {
                  //---
                  // Temp fix:
                  // The overview handler could return a tile of OSSIM_UINT16 if
                  // the max sample value was not set to 2047.  
                  //---
                  tileData->setScalarType(OSSIM_USHORT11);
               }
            }
            if((tileData->getDataObjectStatus() != OSSIM_NULL)&&
               (tileData->getDataObjectStatus() != OSSIM_EMPTY))
            {
               theTile->loadTile(tileData.get());
               theTile->validate();
            }
         }
      }
   }
   else
   {
      int padded=0;
      nitf_Error e;
      
      if(resLevel == 0)
      {
         theTile->setImageRectangle(tileRect);
         theTile->makeBlank();
         if(tileRect.intersects(theCurrentImageRect))
         {
            if(theNumberOfImages > 0)
            {
               ossimIrect clipRect = theCurrentImageRect.clipToRect(tileRect);
               
               theSubImage->startCol = clipRect.ul().x;
               theSubImage->startRow = clipRect.ul().y;
               theSubImage->numRows  = clipRect.height();
               theSubImage->numCols  = clipRect.width();
               
               int bufSize = clipRect.width()*clipRect.height()*ossim::scalarSizeInBytes(theScalarType);
               if(theSubImageSize!=(bufSize))
               {
                  ossim_uint32 i=0;
                  if(theReadBuffer)
                  {
                     for (i = 0; i < theNumberOfInputBands; i++)
                        NITF_FREE(theReadBuffer[i]);
                     NITF_FREE(theReadBuffer);
                  }
                  theReadBuffer = (nitf_Uint8 **) NITF_MALLOC(theNumberOfInputBands * sizeof(nitf_Uint8*));
                  for(i = 0; i < theNumberOfInputBands; ++i)
                  {
                     theReadBuffer[i] = (nitf_Uint8 *) NITF_MALLOC(bufSize);
                  }
                  theSubImageSize = bufSize;
               }
               if (nitf_ImageReader_read
                   (theCurrentImageReader, theSubImage, theReadBuffer, &padded, &e))
               {
                  // now load per band
                  //
                  if(!theLut.valid())
                  {
                     ossim_uint32 idx = 0;
                     for(idx = 0; idx < theNumberOfInputBands; ++idx)
                     {
                        theTile->loadBand(theReadBuffer[idx],
                                          clipRect,
                                          idx);
                     }
                  }
                  else
                  {
                     theUncompressedClipTile->setImageRectangle(clipRect);
                     lutUncompress(theUncompressedClipTile, theReadBuffer[0]);
                     theTile->loadTile(theUncompressedClipTile.get());
                  }
                  theTile->validate();
               }
               else
               {
                  nitf_Error_print(&e, stderr, "image read failed");
                  
               }
            }
         }
      }
   }
   
   // now merge the annotations into the image tile
   //
   if(theAnnotations.size()&&theEnableAnnotationsFlag)
   {
      ossim_uint32 numberOfOutputBands = theTile->getNumberOfBands();
      ossim_uint8* srcBandPtr[3];
      theRgbAnnotationImageData->setImageRectangle(tileRect);
      theRgbAnnotationImageData->makeBlank();
      
      
      //      drawAnnotations(theTile, resLevel);
      drawAnnotations(theRgbAnnotationImageData, resLevel);
      
      if((theRgbAnnotationImageData->getDataObjectStatus()!=OSSIM_EMPTY)&&
         (theRgbAnnotationImageData->getDataObjectStatus()!=OSSIM_NULL))
      {
         
         // now merge annotations to destination tile
         srcBandPtr[0] = (ossim_uint8*)theRgbAnnotationImageData->getBuf(0);
         srcBandPtr[1] = (ossim_uint8*)theRgbAnnotationImageData->getBuf(1);
         srcBandPtr[2] = (ossim_uint8*)theRgbAnnotationImageData->getBuf(2);
         
         switch(theTile->getScalarType())
         {
            case OSSIM_UINT8:
            {
               ossim_uint8* bandBuf = 0;
               ossim_uint8 minPix = 0.0;
               ossim_uint8 maxPix = 0.0;
               ossim_uint8 nullPix = 0.0;
               ossim_uint8 deltaPix = 254;
               ossim_uint8 color = 0;
               ossim_uint32 bands = ossim::min((ossim_uint32)3, (ossim_uint32)numberOfOutputBands);
               ossim_uint32 bandIdx = 0;
               ossim_uint32 offsetBounds = theTile->getWidth()*theTile->getHeight();
               ossim_uint32 offset = 0;
               for(bandIdx = 0; bandIdx < bands; ++bandIdx)
               {
                  bandBuf = (ossim_uint8*)theTile->getBuf(bandIdx);
                  minPix  = ossim::defaultMin(theTile->getScalarType());
                  maxPix  = ossim::defaultMax(theTile->getScalarType());
                  nullPix = (ossim_uint8)theTile->getNullPix(bandIdx);
                  deltaPix = maxPix-minPix;
                  for(offset = 0; offset < offsetBounds; ++offset)
                  {
                     
                     if((srcBandPtr[0][offset] != 0)&&
                        (srcBandPtr[1][offset] != 0)&&
                        (srcBandPtr[2][offset] != 0))
                     {
                        if(bands == 1)
                        {
                           color = static_cast<ossim_uint8>(((float)srcBandPtr[0][offset] + 
                                                             (float)srcBandPtr[1][offset] +
                                                             (float)srcBandPtr[2][offset])/3.0);
                        }
                        else
                        {
                           color = static_cast<ossim_uint8>(minPix + deltaPix*((srcBandPtr[bandIdx][offset])/255.0));
                        }
                        *bandBuf =  color;
                     }
                     ++bandBuf;
                  }
               }
               break;
            }
            case OSSIM_USHORT11:
            case OSSIM_UINT16:
            {
               ossim_uint16* bandBuf = 0;
               ossim_uint16 minPix = 0.0;
               ossim_uint16 maxPix = 0.0;
               ossim_uint16 nullPix = 0.0;
               ossim_uint16 deltaPix = 254;
               ossim_uint16 color = 0;
               ossim_uint32 bands = ossim::min((ossim_uint32)3, (ossim_uint32)numberOfOutputBands);
               ossim_uint32 bandIdx = 0;
               ossim_uint32 offsetBounds = theTile->getWidth()*theTile->getHeight();
               ossim_uint32 offset = 0;
               for(bandIdx = 0; bandIdx < bands; ++bandIdx)
               {
                  bandBuf = (ossim_uint16*)theTile->getBuf(bandIdx);
                  minPix  = ossim::defaultMin(theTile->getScalarType());
                  maxPix  = ossim::defaultMax(theTile->getScalarType());
                  nullPix = (ossim_uint16)theTile->getNullPix(bandIdx);
                  deltaPix = maxPix-minPix;
                  for(offset = 0; offset < offsetBounds; ++offset)
                  {
                     if((srcBandPtr[0][offset] != 0)&&
                        (srcBandPtr[1][offset] != 0)&&
                        (srcBandPtr[2][offset] != 0))
                     {
                        if(bands == 1)
                        {
                           color = static_cast<ossim_uint16>(((float)srcBandPtr[0][offset] + 
                                                              (float)srcBandPtr[1][offset] +
                                                              (float)srcBandPtr[2][offset])/3.0);
                        }
                        else
                        {
                           color = static_cast<ossim_uint16>(minPix + deltaPix*((srcBandPtr[bandIdx][offset])/255.0));
                        }
                        *bandBuf =  color;
                     }
                     ++bandBuf;
                  }
               }
               break;
            }
            default:
            {
               break;
            }
         }
         
         theTile->validate();
      }
   }
   return theTile;
}

void ossimNitroHandler::drawAnnotations(ossimRefPtr<ossimImageData> result, ossim_uint32 resLevel)
{
   if((result->getScalarType() == OSSIM_UINT8)&&
      ((result->getNumberOfBands() == 1)||
       (result->getNumberOfBands() == 3)))
   {
      ossimIrect tempRect = result->getImageRectangle();
      result->setImageRectangle(tempRect);
      theAnnotationImage->setCurrentImageData(result);
      
      std::vector<ossimRefPtr<ossimAnnotationObject> >* currentList = getAnnotationListForRlevel(resLevel);
      
      if(currentList)
      {
         ossim_uint32 idx = 0;
         for(idx=0; idx < currentList->size(); ++idx)
         {
            (*currentList)[idx]->draw(*theAnnotationImage);
         }
         result->setImageRectangle(tempRect);
         result->validate();
      }
   }
   
}


ossim_uint32 ossimNitroHandler::getNumberOfInputBands() const
{
   return theNumberOfInputBands;
}

ossim_uint32 ossimNitroHandler::getNumberOfOutputBands() const
{
   return theNumberOfOutputBands;
}

ossim_uint32 ossimNitroHandler::getNumberOfLines(ossim_uint32 reduced_res_level) const
{
   if (reduced_res_level == 0)
   {
      return  theNumberOfRows;
   }
   else if (theOverview)
   {
      return theOverview->getNumberOfLines(reduced_res_level);
   }
   return 0;
}

ossim_uint32 ossimNitroHandler::getNumberOfSamples(ossim_uint32 reduced_res_level) const
{
   if (reduced_res_level == 0)
   {
      return  theNumberOfColumns;
   }
   else if (theOverview)
   {
      return theOverview->getNumberOfSamples(reduced_res_level);
   }
   return 0;
}

bool ossimNitroHandler::saveState(ossimKeywordlist& kwl,
                                  const char* prefix)const
{
}

bool ossimNitroHandler::loadState(const ossimKeywordlist& kwl,
                                  const char* prefix)
{
   return ossimImageHandler::loadState(kwl, prefix);
}

ossimIrect ossimNitroHandler::getImageRectangle(ossim_uint32 resLevel) const
{
   ossimIrect rect;
   rect.makeNan();
   rect = theCurrentImageRect;
   if(!theAnnotationBounds.hasNans())
   {
      if(rect.hasNans())
      {
         if(resLevel == 0)
         {
            rect = theAnnotationBounds;
         }
         else
         {
            ossim_uint32 idx = resLevel-1;
            if(idx < theRlevelAnnotationBounds.size())
            {
               rect = theRlevelAnnotationBounds[idx];
            }
         }
      }
      else
      {
         if(resLevel == 0)
         {
            rect = rect.combine(theAnnotationBounds);
         }
         else
         {
            ossim_uint32 idx = resLevel-1;
            if(idx < theRlevelAnnotationBounds.size())
            {
               rect = rect.combine(theRlevelAnnotationBounds[idx]);
            }
         }
      }
   }
   return rect;
}


bool ossimNitroHandler::getImageGeometry(ossimKeywordlist& kwl,
                                         const char* prefix)
{
   return ossimImageHandler::getImageGeometry(kwl, prefix);
}

ossimScalarType ossimNitroHandler::getOutputScalarType() const
{
   return theScalarType;
}

ossim_uint32 ossimNitroHandler::getImageTileWidth() const
{
   return theNumberOfPixelsPerHorizBlock;
}

ossim_uint32 ossimNitroHandler::getImageTileHeight() const
{
   return theNumberOfPixelsPerVertBlock;
}

ossim_uint32 ossimNitroHandler::getCurrentEntry()const
{
   return theCurrentImageEntry;
}

bool ossimNitroHandler::setCurrentEntry(ossim_uint32 entryIdx)
{
   bool result = false;
   theTile = 0;
   if(isOpen())
   {
      nitf_Error e;
      theCurrentImageRect.makeNan();
      
      if(entryIdx < theNumberOfImages)
      {
         if(theCurrentImageReader) 
         {
            nitf_ImageReader_destruct(&theCurrentImageReader);
            theCurrentImageReader = 0;
         }
         if(theReadBuffer)
         {
            ossim_uint32 i=0;
            for (i = 0; i < theNumberOfInputBands; i++)
               NITF_FREE(theReadBuffer[i]);
            NITF_FREE(theReadBuffer);
         }
         theReadBuffer = 0;
         theCurrentImageReader = nitf_Reader_newImageReader(theReader, entryIdx, &e);
         theCurrentImageIterator = nitf_List_at(theRecord->images, entryIdx);
         result = (theCurrentImageReader != 0);
         theCurrentImageEntry = entryIdx;
         theCurrentImageSegment = (nitf_ImageSegment *) nitf_ListIterator_get(&theCurrentImageIterator);
         size_t subimageSize;
         nitf_Uint32 xBands;
         nitf_Error e;
         nitf_Field_get(theCurrentImageSegment->subheader->numBitsPerPixel, &theNumberOfBits,
                        NITF_CONV_UINT, NITF_INT32_SZ, &e);
         nitf_Field_get(theCurrentImageSegment->subheader->numImageBands, &theNumberOfInputBands,
                        NITF_CONV_UINT, NITF_INT32_SZ, &e);
         nitf_Field_get(theCurrentImageSegment->subheader->numMultispectralImageBands,
                        &xBands,
                        NITF_CONV_UINT, NITF_INT32_SZ, &e);
         theNumberOfInputBands += xBands;
         nitf_Field_get(theCurrentImageSegment->subheader->numRows, &theNumberOfRows, 
                        NITF_CONV_UINT, NITF_INT32_SZ, &e);
         nitf_Field_get(theCurrentImageSegment->subheader->numCols, &theNumberOfColumns, 
                        NITF_CONV_UINT, NITF_INT32_SZ, &e);
         nitf_Field_get(theCurrentImageSegment->subheader->numBlocksPerRow, &theNumberOfBlocksPerRow, 
                        NITF_CONV_UINT, NITF_INT32_SZ, &e);
         nitf_Field_get(theCurrentImageSegment->subheader->numBlocksPerCol, &theNumberOfBlocksPerColumn, 
                        NITF_CONV_UINT, NITF_INT32_SZ, &e);
         nitf_Field_get(theCurrentImageSegment->subheader->numPixelsPerHorizBlock, &theNumberOfPixelsPerHorizBlock, 
                        NITF_CONV_UINT, NITF_INT32_SZ, &e);
         nitf_Field_get(theCurrentImageSegment->subheader->numPixelsPerVertBlock, &theNumberOfPixelsPerVertBlock, 
                        NITF_CONV_UINT, NITF_INT32_SZ, &e);
         theImageRepresentation = ossimString(theCurrentImageSegment->subheader->imageRepresentation->raw,
                                              theCurrentImageSegment->subheader->imageRepresentation->raw + theCurrentImageSegment->subheader->imageRepresentation->length);
         theCurrentImageRect = ossimIrect(0,0,theNumberOfRows-1, theNumberOfColumns-1);
         theImageRepresentation = theImageRepresentation.upcase();
         theSubImageSize = 0;
         setCompressionFlags();
         populateLut();
         theNumberOfOutputBands = theNumberOfInputBands;
         if(theIsVqCompressedFlag&&(theNumberOfInputBands==1))
         {
            theNumberOfOutputBands = 3;
         }
         else if(theLut.valid())
         {
            theNumberOfOutputBands = theLut->getNumberOfBands();
         }
         theBandList.resize(theNumberOfInputBands);
         int idx = 0;
         theReadBuffer = 0;
         theSubImage->bandList=&theBandList.front();
         theSubImage->numBands=theBandList.size();
         initializeScalarType();
      }
   }
   if(!result)
   {
      if((theAnnotations.size()>0)&&
         (entryIdx == 0))
      {
         result = true;
         theNumberOfOutputBands = 3;
         theNumberOfInputBands = 1;
         theScalarType=OSSIM_UINT8;
      }
   }
   
   return result;
}

void ossimNitroHandler::getEntryList(std::vector<ossim_uint32>& entryList)const
{
   ossim_uint32 idx = 0;
   for(idx = 0; idx < theNumberOfImages; ++idx)
   {
      entryList.push_back(idx);
   }
   if(theNumberOfImages == 0)
   {
      if(theAnnotations.size())
      {
         entryList.push_back(0);
      }
   }
}

void ossimNitroHandler::getEntryNames(std::vector<ossimString>& entryStringList) const
{
   ossim_uint32 idx = 0;
   for(idx = 0; idx < theNumberOfImages; ++idx)
   {
      entryStringList.push_back(ossimString::toString(idx));
   }
   if(theNumberOfImages == 0)
   {
      if(theAnnotations.size())
      {
         entryStringList.push_back("0");
      }
   }
}

bool ossimNitroHandler::isOpen()const
{
   return (theRecord!=0);
}

double ossimNitroHandler::getNullPixelValue(ossim_uint32 band)const
{
   double result = ossimImageHandler::getNullPixelValue(band);
   
   //   if(thePackedBitsFlag)
   //   {
   //      if((result < 0) ||
   //         (result > getMaxPixelValue(band)))
   //      {
   //         result = 0.0;
   //      }
   //   }
   
   
   return result;
}

double ossimNitroHandler::getMinPixelValue(ossim_uint32 band)const
{
   double result = ossimImageHandler::getMinPixelValue(band);
   
   //   if(thePackedBitsFlag)
   //   {
   //      if(result < 1.0) result = 1.0;
   //   }
   
   return result;
}

double ossimNitroHandler::getMaxPixelValue(ossim_uint32 band)const
{
   double result = ossimImageHandler::getMaxPixelValue(band);
   
#if 0
   if(thePackedBitsFlag)
   {
      double test = 1<<(hdr->getBitsPerPixelPerBand());
      
      if(result > test) result = test;
   }
   else
#endif
      switch (theNumberOfBits)
   {
      case 11:
      {
         if (result > 2047.0)
         {
            result = 2047.0;
         }
         break;
      }
      case 12:
      {
         if (result > 4095.0)
         {
            result = 4095.0;
         }
         break;
      }
      default:
      {
         if(theNumberOfBits < 16)
         {
            result = (2>>theNumberOfBits) - 1;
         }
      }
         break;
   }
   
   return result;
}

void ossimNitroHandler::setProperty(ossimRefPtr<ossimProperty> property)
{
   ossimString name = property->getName();
   if(name == "enable_annotations_flag")
   {
      theEnableAnnotationsFlag = property->valueToString().toBool();
   }
   else
   {
      ossimImageHandler::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimNitroHandler::getProperty(const ossimString& name)const
{
   ossimRefPtr<ossimProperty> result;
   if(name == "enable_annotations_flag")
   {
      ossimBooleanProperty* prop = new ossimBooleanProperty(name,
                                                            theEnableAnnotationsFlag);
      prop->setFullRefreshBit();
      
      result = prop;
   }
   else
   {
      result = ossimImageHandler::getProperty(name);
   }
   
   return result.get();
}

void ossimNitroHandler::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageHandler::getPropertyNames(propertyNames);
   propertyNames.push_back("enable_annotations_flag");
}

void ossimNitroHandler::initializeScalarType()
{
   theScalarType = OSSIM_SCALAR_UNKNOWN;
   if(theCurrentImageSegment)
   {
      nitf_Error e;
      nitf_Uint32 bitsPerPixel=0;
      nitf_Field_get(theCurrentImageSegment->subheader->actualBitsPerPixel, &bitsPerPixel, 
                     NITF_CONV_UINT, NITF_INT32_SZ, &e);
      if (bitsPerPixel < 1)
      {
         nitf_Field_get(theCurrentImageSegment->subheader->numBitsPerPixel, &bitsPerPixel, 
                        NITF_CONV_UINT, NITF_INT32_SZ, &e);
      }
      if(bitsPerPixel <=0) return;
      
      ossimString pixelValueType(theCurrentImageSegment->subheader->pixelValueType->raw, theCurrentImageSegment->subheader->pixelValueType->raw + theCurrentImageSegment->subheader->pixelValueType->length);
      pixelValueType = pixelValueType.upcase();
      
      switch (bitsPerPixel)
      {
         case 8:
         {
            theScalarType = OSSIM_UINT8;
            break;
         }
         case 11:
         {
            theScalarType = OSSIM_USHORT11;
            break;
         }
         case 12:
         {
            theScalarType = OSSIM_UINT16;
            break;
         }
         case 16:
         {
            if(pixelValueType == "SI")
            {
               theScalarType = OSSIM_SINT16;
            }
            else
            {
               theScalarType = OSSIM_UINT16;
            }
            break;
         }
         case 32:
         {
            if(pixelValueType == "R")
            {
               theScalarType = OSSIM_FLOAT32;
            }
            
            break;
         }
         case 64:
         {
            if(pixelValueType == "R")
            {
               theScalarType = OSSIM_FLOAT64;
            }
            
            break;
         }
         default:
         {
            if(bitsPerPixel <=8)
            {
               theScalarType = OSSIM_UINT8;
            }
            else if(bitsPerPixel <=16)
            {
               if(pixelValueType == "SI")
               {
                  theScalarType = OSSIM_SINT16;
               }
               else
               {
                  theScalarType = OSSIM_UINT16;
               }
            }
            break;
         }
      }
   }
   
   if(theScalarType == OSSIM_SCALAR_UNKNOWN)
   {
      if(theAnnotations.size() > 0)
      {
         theScalarType = OSSIM_UINT8;
      }
   }
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
      << "ossimNitroHandler::initializeScalarType DEBUG:"
      << "\nScalar type:  "
      << (ossimScalarTypeLut::instance()->getEntryString(theScalarType))
      << endl;
   }
}

void ossimNitroHandler::initializeOutputTile()
{
   ossimImageDataFactory* idf = ossimImageDataFactory::instance();
   theTile = idf->create(this, this);
   theTile->initialize();
   theUncompressedClipTile = (ossimImageData*)theTile->dup();
   theUncompressedClipTile->initialize();
   theRgbAnnotationImageData = 0;
   
   theRgbAnnotationImageData = new ossimImageData(0,
                                                  OSSIM_UINT8,
                                                  3,
                                                  64,
                                                  64);
   theRgbAnnotationImageData->initialize();
}

bool ossimNitroHandler::setCompressionFlags()
{
   ossimString compressionCode(theCurrentImageSegment->subheader->imageCompression->raw, 
                               theCurrentImageSegment->subheader->imageCompression->raw+theCurrentImageSegment->subheader->imageCompression->length);
   compressionCode = compressionCode.upcase();
   theIsVqCompressedFlag = ((compressionCode == "C4")||
                            (compressionCode == "M4"));
}

void ossimNitroHandler::populateLut()
{
   theLut = 0 ;
   if(theCurrentImageSegment&&theCurrentImageSegment->subheader->bandInfo[0])
   {
      nitf_Error e;
      int numLuts, numEntries;
      int outputBands = 0;
      nitf_Field_get(theCurrentImageSegment->subheader->bandInfo[0]->numLUTs, &numLuts,
                     NITF_CONV_UINT, NITF_INT32_SZ, &e);
      if(numLuts > 0)
      {
         nitf_Field_get(theCurrentImageSegment->subheader->bandInfo[0]->bandEntriesPerLUT, &numEntries,
                        NITF_CONV_UINT, NITF_INT32_SZ, &e);
         
         outputBands = theCurrentImageSegment->subheader->bandInfo[0]->lut->tables;
         if(numLuts = 1)
         {
            theLut = new ossimNBandLutDataObject(numEntries, outputBands);
            ossim_uint32 idx = 0;
            ossim_uint32 bandIdx = 0;
            ossim_uint32 offset = 0;
            for(idx = 0; idx < numEntries; ++idx)
            {
               for(bandIdx = 0; bandIdx < outputBands; ++bandIdx)
               {
                  (*theLut)[idx][bandIdx] = theCurrentImageSegment->subheader->bandInfo[0]->lut->table[offset+bandIdx];
                  //               (*theLut)[idx][1] = theCurrentImageSegment->subheader->bandInfo[0]->lut->table[offset+1];
                  //               (*theLut)[idx][2] = theCurrentImageSegment->subheader->bandInfo[0]->lut->table[offset+2];
                  
                  //               offset+=3;
               }
               offset+=outputBands;
            }
         }
      }
   }
}

void ossimNitroHandler::lutUncompress(ossimRefPtr<ossimImageData> destination, nitf_Uint8* source)
{
   if(!theLut.valid()) return;
   if((!destination->getBuf())||
      (destination->getScalarType()!=OSSIM_UINT8))
   {
      return;
   }
   
   if(destination->getNumberOfBands()!=theLut->getNumberOfBands())
   {
      return;
   }
   std::vector<ossim_uint8*> bandList(destination->getNumberOfBands());
   ossim_uint8** tempRows = &bandList.front();
   ossim_uint32 idx = 0;
   ossim_uint32 bands = bandList.size();
   for(idx = 0; idx <bands; ++idx)
   {
      tempRows[idx] = (ossim_uint8*)(destination->getBuf(idx));
   }   
   ossim_uint8* srcPtr = source;
   ossim_uint32 compressionYidx   = 0;
   ossim_uint32 compressionXidx   = 0;
   ossim_uint32 uncompressIdx     = 0;
   ossim_uint32 h = destination->getHeight();
   ossim_uint32 w = destination->getWidth();
   
   for(compressionYidx = 0; compressionYidx < h; ++compressionYidx)
   {
      for(compressionXidx = 0; compressionXidx < w; ++compressionXidx)
      {
         for(idx = 0; idx < bands;++idx)
         {
            tempRows[idx][uncompressIdx] = (*theLut)[*srcPtr][idx];
         }
         ++srcPtr;
         ++uncompressIdx;
      }
   }
}

void printCGM(cgm_Metafile *mf, nitf_Error *error)
{
   nitf_ListIterator iter;
   nitf_ListIterator end;
   int i;
   
   printf("metafile:\n");
   printf("    name        : %s\n", mf->name);
   printf("    version     : %d\n", mf->version);
   printf("    description : %s\n", mf->description);
   printf("    fonts       : { ");
   
   iter = nitf_List_begin(mf->fontList);
   end = nitf_List_end(mf->fontList);
   while(nitf_ListIterator_notEqualTo(&iter, &end))
   {
      printf("%s, ", (char*)nitf_ListIterator_get(&iter));
      nitf_ListIterator_increment(&iter);
   }
   printf("}\n");
   
   printf("    picture     : \n");
   printf("        name               : %s\n", mf->picture->name);
   printf("        colorSelectionMode : %d\n", mf->picture->colorSelectionMode);
   printf("        edgeWidthSpec      : %d\n", mf->picture->edgeWidthSpec);
   printf("        lineWidthSpec      : %d\n", mf->picture->lineWidthSpec);
   printf("        vdcExtent          : \n");
   printf("            x1 : %d\n", mf->picture->vdcExtent->x1);
   printf("            y1 : %d\n", mf->picture->vdcExtent->y1);
   printf("            x2 : %d\n", mf->picture->vdcExtent->x2);
   printf("            y2 : %d\n", mf->picture->vdcExtent->y2);
   printf("        body               :\n");
   printf("            transparency : %d\n", mf->picture->body->transparency);
   
   iter = nitf_List_begin(mf->picture->body->elements);
   end = nitf_List_end(mf->picture->body->elements);
   while(nitf_ListIterator_notEqualTo(&iter, &end))
   {
      cgm_Element_print((cgm_Element*)nitf_ListIterator_get(&iter));
      nitf_ListIterator_increment(&iter);
   }
}

void ossimNitroHandler::addElement(cgm_Element* element)
{   
   //   cgm_Element_print(element);
   nitf_ListIterator iter;
   nitf_ListIterator end;
   ossimDrect rect;
   switch(element->type)
   {
      case CGM_TEXT_ELEMENT:
      {
         ossimDpt scale(1.0,1.0); 
         ossimDpt shear(0.0,0.0); 
         ossimIpt size(18, 18);
         ossim_uint8 r,g,b;
         double rotation = 0.0;
         ossimRefPtr<ossimAnnotationFontObject> fontObject;
         cgm_TextElement* el = (cgm_TextElement*)element->data;
         if(el->attributes)
         {
            if(el->attributes->textColor)
            {
               r = el->attributes->textColor->r;
               g = el->attributes->textColor->g;
               b = el->attributes->textColor->b;
            }
            if(el->attributes->characterHeight > 0)
            {
               size.x = el->attributes->characterHeight;
               size.y = size.x;
            }
         }
         
         fontObject = new ossimAnnotationFontObject(ossimIpt(el->text->x, el->text->y),
                                                    ossimString(el->text->str),
                                                    size,
                                                    rotation,
                                                    scale,
                                                    shear,
                                                    r,g,b);
         fontObject->getBoundingRect(rect);
         theAnnotations.push_back(fontObject.get());
         if(!rect.hasNans())
         {
            addAnnotationBounds(rect);
         }
         break;
      }
      case CGM_POLYGON_ELEMENT:
      {
         ossimRefPtr<ossimAnnotationPolyObject> polyObject = new ossimAnnotationPolyObject;
         cgm_PolygonElement* el = (cgm_PolygonElement*)element->data;
         if(el->attributes)
         {
            if(el->attributes->edgeColor)
            {
               polyObject->setColor(el->attributes->edgeColor->r,
                                    el->attributes->edgeColor->g,
                                    el->attributes->edgeColor->b);
            }
            if(el->attributes->edgeWidth>0)
            {
               polyObject->setThickness(el->attributes->edgeWidth);
            }
         }
         iter = nitf_List_begin(el->vertices);
         end = nitf_List_end(el->vertices);
         while(nitf_ListIterator_notEqualTo(&iter, &end))
         {
            cgm_Vertex* vert = (cgm_Vertex*)nitf_ListIterator_get(&iter);
            polyObject->addPoint(ossimDpt(vert->x, vert->y));
            nitf_ListIterator_increment(&iter);
         }
         polyObject->computeBoundingRect();
         theAnnotations.push_back(polyObject.get());
         polyObject->getBoundingRect(rect);
         if(!rect.hasNans())
         {
            addAnnotationBounds(rect);
         }
         break;
      }
      case CGM_POLYLINE_ELEMENT:
      {
         ossimPolyLine polyLine;
         ossimRefPtr<ossimAnnotationMultiPolyLineObject> polyLineObject = new ossimAnnotationMultiPolyLineObject();
         cgm_PolyLineElement* el = (cgm_PolyLineElement*)element->data;
         if(el->attributes)
         {
            if(el->attributes->lineColor)
            {
               polyLineObject->setColor(el->attributes->lineColor->r,
                                        el->attributes->lineColor->g,
                                        el->attributes->lineColor->b);
            }
            if(el->attributes->lineWidth > 0)
            {
               polyLineObject->setThickness(el->attributes->lineWidth);
            }
         }
         iter = nitf_List_begin(el->vertices);
         end = nitf_List_end(el->vertices);
         while(nitf_ListIterator_notEqualTo(&iter, &end))
         {
            cgm_Vertex* vert = (cgm_Vertex*)nitf_ListIterator_get(&iter);
            polyLine.addPoint(ossimDpt(vert->x, vert->y));
            nitf_ListIterator_increment(&iter);
         }
         polyLineObject->addPolyLine(polyLine);
         polyLineObject->computeBoundingRect();
         theAnnotations.push_back(polyLineObject.get());
         polyLineObject->getBoundingRect(rect);
         if(!rect.hasNans())
         {
            addAnnotationBounds(rect);
         }
         break;
      }
      case CGM_POLYSET_ELEMENT:
      {
         break;
      }
      case CGM_ELLIPSE_ELEMENT:
      {
         // we still need to fix for rotation.  For now no rotation will be added
         //
         cgm_EllipseElement* el = (cgm_EllipseElement*)element->data;
         double azimuthInDegrees = 0.0;
         int lineWidth = 1;
         ossimDpt center(el->centerX, el->centerY);
         ossimDpt pt1(el->end1X, el->end1Y);
         ossimDpt pt2(el->end2X, el->end2Y);
         ossimDpt widthHeight((pt2-center).length(),
                              (pt1-center).length());
         ossim_uint8 lineColorR=255, lineColorG=255, lineColorB=255;
         if(el->attributes)
         {
            lineWidth = el->attributes->edgeWidth;
            if(lineWidth < 0 ) lineWidth = 1;
            if(el->attributes->edgeColor)
            {
               lineColorR = el->attributes->edgeColor->r;
               lineColorG = el->attributes->edgeColor->g;
               lineColorB = el->attributes->edgeColor->b;
            }
         }
         ossimRefPtr<ossimAnnotationEllipseObject> ellipseObject = new ossimAnnotationEllipseObject(center, widthHeight);
         ellipseObject->setColor(lineColorR, lineColorG, lineColorB);
         ellipseObject->computeBoundingRect();
         //ellipseObject->setThickness(lineWidth);
         theAnnotations.push_back(ellipseObject.get());
         ellipseObject->getBoundingRect(rect);
         if(!rect.hasNans())
         {
            addAnnotationBounds(rect);
         }
         break;
      }
      case CGM_ELLIPTICAL_ARC_CENTER_ELEMENT:
      {
         break;
      }
      case CGM_ELLIPTICAL_ARC_CENTER_CLOSE_ELEMENT:
      {
         break;
      }
      case CGM_RECTANGLE_ELEMENT:
      {
         ossimRefPtr<ossimAnnotationPolyObject> polyObject = new ossimAnnotationPolyObject;
         int lineWidth = 1;
         polyObject->setThickness(lineWidth);
         cgm_RectangleElement* el = (cgm_RectangleElement*)element->data;
         if(el->attributes)
         {
            lineWidth = el->attributes->edgeWidth;
            if(lineWidth < 0 ) lineWidth = 1;
            if(el->attributes->edgeColor)
            {
               polyObject->setColor(el->attributes->edgeColor->r,
                                    el->attributes->edgeColor->g,
                                    el->attributes->edgeColor->b);
            }
         }
         ossimIrect tempRectangle(ossimIpt(el->rectangle->x1, el->rectangle->y1),
                                  ossimIpt(el->rectangle->x2, el->rectangle->y2));
         polyObject->addPoint(ossimDpt(tempRectangle.ul()));
         polyObject->addPoint(ossimDpt(tempRectangle.ur()));
         polyObject->addPoint(ossimDpt(tempRectangle.lr()));
         polyObject->addPoint(ossimDpt(tempRectangle.ll()));
         
         polyObject->computeBoundingRect();
         theAnnotations.push_back(polyObject.get());
         polyObject->getBoundingRect(rect);
         if(!rect.hasNans())
         {
            addAnnotationBounds(rect);
         }
         break;
      }
      case CGM_CIRCLE_ELEMENT:
      {
         cgm_CircleElement* el = (cgm_CircleElement*)element->data;
         ossimDpt center(el->centerX, el->centerY);
         ossimDpt widthHeight(el->radius, el->radius);
         ossimRefPtr<ossimAnnotationEllipseObject> ellipseObject = new ossimAnnotationEllipseObject(center, widthHeight);
         
         if(el->attributes)
         {
            if(el->attributes->edgeColor)
            {
               ellipseObject->setColor(el->attributes->edgeColor->r,
                                       el->attributes->edgeColor->g,
                                       el->attributes->edgeColor->b);
            }
         }
         ellipseObject->computeBoundingRect();
         theAnnotations.push_back(ellipseObject.get());
         ellipseObject->getBoundingRect(rect);
         if(!rect.hasNans())
         {
            addAnnotationBounds(rect);
         }
         
         break;
      }
      case CGM_CIRCULAR_ARC_CENTER_ELEMENT:
      {
         break;
      }
      case CGM_CIRCULAR_ARC_CENTER_CLOSE_ELEMENT:
      {
         break;
      }
   }
}

void ossimNitroHandler::loadAnnotations()
{
   nitf_ListIterator iter;
   nitf_ListIterator end;
   ossim_uint32 num = 0;
   nitf_Error e;
   
   if (theRecord->graphics)
   {
      iter = nitf_List_begin(theRecord->graphics);
      end = nitf_List_end(theRecord->graphics);
      
      num = 0;
      while(nitf_ListIterator_notEqualTo(&iter, &end))
      {
         nitf_SegmentReader *segmentReader = NULL;
         nitf_GraphicSegment *segment =
         (nitf_GraphicSegment *) nitf_ListIterator_get(&iter);
         
         segmentReader = nitf_Reader_newGraphicReader(theReader, num, &e);
         
         // size_t bytes;
         // bytes = (size_t)(segment->end - segment->offset);
         cgm_MetafileReader* metafileReader = cgm_MetafileReader_construct(&e);
         /** put yourself at the beginning of the data segment **/
         nitf_IOHandle_seek(segmentReader->inputHandle, segmentReader->baseOffset, NITF_SEEK_SET, &e);
         cgm_Metafile* metaFile = cgm_MetafileReader_read(metafileReader,
                                                          segmentReader->inputHandle, 
                                                          &e);
         cgm_Picture* picture = 0;
         if(metaFile)
         {
            nitf_ListIterator elementIter;
            nitf_ListIterator elementIterEnd;
            elementIter = nitf_List_begin(metaFile->picture->body->elements);
            elementIterEnd = nitf_List_end(metaFile->picture->body->elements);
            while(nitf_ListIterator_notEqualTo(&elementIter, &elementIterEnd))
            {
               //cgm_Element_print((cgm_Element*)nitf_ListIterator_get(&iter));
               addElement((cgm_Element*)nitf_ListIterator_get(&elementIter));
               nitf_ListIterator_increment(&elementIter);
            }
            //            printCGM(metaFile, &e);
            cgm_Metafile_destruct(&metaFile);
         }
         cgm_MetafileReader_destruct(&metafileReader);
         nitf_SegmentReader_destruct(&segmentReader);
         nitf_ListIterator_increment(&iter);
         ++num;
      }
   }
   if(theRecord->labels)
   {
      ossimDpt scale(1.0,1.0); 
      ossimDpt shear(0.0,0.0); 
      double rotation = 0.0;
      int x,y;
      int cellHeight, cellWidth;
      ossim_uint8 r,g,b;
      iter = nitf_List_begin(theRecord->labels);
      end = nitf_List_end(theRecord->labels);
      
      num = 0;
      while(nitf_ListIterator_notEqualTo(&iter, &end))
      {
         ossimDrect rect;
         nitf_LabelSegment *segment =
         (nitf_LabelSegment *) nitf_ListIterator_get(&iter);
         
         r = (ossim_uint8)segment->subheader->textColor->raw[0];
         g = (ossim_uint8)segment->subheader->textColor->raw[1];
         b = (ossim_uint8)segment->subheader->textColor->raw[2];
         nitf_Field_get(segment->subheader->locationColumn, &x, 
                        NITF_CONV_INT, NITF_INT32_SZ, &e);
         nitf_Field_get(segment->subheader->locationRow, &y, 
                        NITF_CONV_INT, NITF_INT32_SZ, &e);
         nitf_Field_get(segment->subheader->cellHeight, &cellHeight, 
                        NITF_CONV_INT, NITF_INT32_SZ, &e);
         if(cellHeight <=0)
         {
            cellHeight = 18;
         }
         ossimRefPtr<ossimAnnotationFontObject> fontObject = new ossimAnnotationFontObject(ossimIpt(x, y),
                                                                                           segment->label,
                                                                                           ossimIpt(cellHeight, cellHeight),
                                                                                           rotation,
                                                                                           scale,
                                                                                           shear,
                                                                                           r,g,b);
         fontObject->getBoundingRect(rect);
         theAnnotations.push_back(fontObject.get());
         if(!rect.hasNans())
         {
            addAnnotationBounds(rect);
         }
         nitf_ListIterator_increment(&iter);
      }
   }
}

void ossimNitroHandler::addAnnotationBounds(const ossimDrect& rect)
{
   if(rect.hasNans()) return;
   if(theAnnotationBounds.hasNans())
   {
      theAnnotationBounds = rect;
   }
   else
   {
      theAnnotationBounds = theAnnotationBounds.combine(ossimIrect(rect));
   }
}

ossim_uint32 ossimNitroHandler::getNumberOfDecimationLevels()const
{
   if(theNumberOfImages > 0)
   {
      return ossimImageHandler::getNumberOfDecimationLevels();
   }
   
   if(theMaxAnnotationRlevel > 0)
   {
      return theMaxAnnotationRlevel;
   }
   
   return 0;
}

std::vector<ossimRefPtr<ossimAnnotationObject> >* ossimNitroHandler::getAnnotationListForRlevel(ossim_uint32 rlevel)
{
   if(rlevel == 0)
   {
      
      return &theAnnotations;
   }
   else
   {
      ossim_uint32 annotationIndex = rlevel-1;
      
      if(rlevel <= theMaxAnnotationRlevel)
      {
         if(annotationIndex < theRlevelAnnotations.size())
         {
            return &(theRlevelAnnotations[annotationIndex]);
         }
         else // scale the annotations to reach the desired rlevel
         {
            ossimIrect rlevelBounds;
            ossim_uint32 idx = 0;
            ossim_uint32 currentRlevel = theRlevelAnnotations.size();
            for(;currentRlevel<rlevel;++currentRlevel)
            {
               rlevelBounds.makeNan();
               theRlevelAnnotations.push_back(std::vector<ossimRefPtr<ossimAnnotationObject> >());
               std::vector<ossimRefPtr<ossimAnnotationObject> >& destinationList = theRlevelAnnotations[theRlevelAnnotations.size() - 1];
               {
                  double scale = 1.0/(double)(1<<(currentRlevel+1));
                  ossim_uint32 annotationToScaleIndex = 0;
                  for(annotationToScaleIndex=0; annotationToScaleIndex<theAnnotations.size();++annotationToScaleIndex)
                  {
                     ossimRefPtr<ossimAnnotationObject> obj =  (ossimAnnotationObject*)theAnnotations[annotationToScaleIndex]->dup();
                     obj->applyScale(ossimDpt(scale,scale));
                     if(rlevelBounds.hasNans())
                     {
                        rlevelBounds = obj->getBoundingRect();
                     }
                     else
                     {
                        rlevelBounds = rlevelBounds.combine(obj->getBoundingRect());
                     }
                     
                     destinationList.push_back(obj.get());
                  }
               }
               theRlevelAnnotationBounds.push_back(rlevelBounds);
            }
            return &(theRlevelAnnotations[rlevel-1]);
         }
      }
   }
   
   return (std::vector<ossimRefPtr<ossimAnnotationObject> >*)0;
}

