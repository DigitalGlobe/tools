//----------------------------------------------------------------------------
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: OSSIM Portable Network Graphics (PNG) reader (tile source).
//
//----------------------------------------------------------------------------
// $Id: ossimLibRawReader.cpp 16420 2010-01-27 15:41:52Z dburken $

#include <cstdlib>
#include <cstddef> /* for NULL */
#include <cmath>   /* for pow */

#include "ossimLibRawReader.h"
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>


RTTI_DEF1(ossimLibRawReader, "ossimLibRawReader", ossimImageHandler)

#ifdef OSSIM_ID_ENABLED
   static const char OSSIM_ID[] = "$Id: ossimLibRawReader.cpp 16420 2010-01-27 15:41:52Z dburken $";
#endif
   
static ossimTrace traceDebug("ossimLibRawReader:degug");  

ossimLibRawReader::ossimLibRawReader()
   :
      ossimImageHandler(),
      theRawProcessor(0)
{
}

ossimLibRawReader::~ossimLibRawReader()
{
   if (isOpen())
   {
      close();
   }
}

void ossimLibRawReader::destroy()
{
   if(theRawProcessor)
   {
      delete theRawProcessor;
      theRawProcessor = 0;
   }
   theTile = 0;
   theMemoryTile = 0;
}

ossimRefPtr<ossimImageData> ossimLibRawReader::getTile(
                                                         const  ossimIrect& tile_rect,
                                                         ossim_uint32 resLevel)
{
   if(!theTile.valid()||!theMemoryTile.valid())
   {
      cacheImage();
   }
   if (theTile.valid())
   {
      // Image rectangle must be set prior to calling getTile.
      theTile->setImageRectangle(tile_rect);
      
      if ( getTile( theTile.get(), resLevel ) == false )
      {
         if (theTile->getDataObjectStatus() != OSSIM_NULL)
         {
            theTile->makeBlank();
         }
      }
   }
   
   return theTile;
}

bool ossimLibRawReader::getTile(ossimImageData* result,
                                  ossim_uint32 resLevel)
{
   bool status = false;
   status = getOverviewTile(resLevel, result);
   
   ossimIrect rect = result->getImageRectangle();
   if (!status) // Did not get an overview tile.
   {
      if(!theTile.valid()||!theMemoryTile.valid())
      {
         cacheImage();
      }
      if (theTile.valid())
      {
         // Image rectangle must be set prior to calling getTile.
         theTile->setImageRectangle(rect);
         
         theTile->makeBlank();
         
         ossimIrect memRect = theMemoryTile->getImageRectangle();
         
         if(memRect.intersects(rect))
         {
            ossimIrect clampRect = theMemoryTile->getImageRectangle().clipToRect(rect);
            
            theTile->loadTile(theMemoryTile->getBuf(),
                              memRect,
                              OSSIM_BSQ);
            
            theTile->validate();
            status = true;
         }
      }
   }
   return status;
}



ossimIrect
ossimLibRawReader::getImageRectangle(ossim_uint32 reduced_res_level) const
{
   return ossimIrect(0,
                     0,
                     getNumberOfSamples(reduced_res_level) - 1,
                     getNumberOfLines(reduced_res_level)   - 1);
}

bool ossimLibRawReader::saveState(ossimKeywordlist& kwl,
                               const char* prefix) const
{
   return ossimImageHandler::saveState(kwl, prefix);
}

bool ossimLibRawReader::loadState(const ossimKeywordlist& kwl,
                               const char* prefix)
{
   if (ossimImageHandler::loadState(kwl, prefix))
   {
      return open();
   }

   return false;
}

bool ossimLibRawReader::open()
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   int ret = 0;
   static const char MODULE[] = "ossimLibRawReader::open";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimLibRawReader::open entered..."
         << "File:  " << theImageFile.c_str()
         << std::endl;
   }

   // Start with a clean slate.
   if (isOpen())
   {
      close();
   }
   
   // Check for empty filename.
   if (theImageFile.empty())
   {
      return false;
   }
   theRawProcessor = new LibRaw();
   if( (ret = theRawProcessor->open_file(theImageFile.c_str()) != LIBRAW_SUCCESS))
   {
      destroy();
      return false;
   }
   
   ossimImageHandler::completeOpen();


   return true;
}

ossim_uint32 ossimLibRawReader::getNumberOfLines(ossim_uint32 reduced_res_level) const
{
   cacheImage();
   if (reduced_res_level == 0)
   {
      if(theMemoryTile.valid())
      {
         return theMemoryTile->getHeight();
      }
   }
   else if ( theOverview.valid() )
   {
      return theOverview->getNumberOfLines(reduced_res_level);
   }

   return 0;
}

ossim_uint32 ossimLibRawReader::getNumberOfSamples(ossim_uint32 reduced_res_level) const
{
   cacheImage();
   if (reduced_res_level == 0)
   {
      if(theMemoryTile.valid())
      {
         return theMemoryTile->getWidth();
      }
   }
   else if ( theOverview.valid() )
   {
      return theOverview->getNumberOfSamples(reduced_res_level);
   }

   return 0;
}

ossim_uint32 ossimLibRawReader::getImageTileWidth() const
{
   return 0;
}

ossim_uint32 ossimLibRawReader::getImageTileHeight() const
{
   return 0;
}

ossimString ossimLibRawReader::getShortName()const
{
   return ossimString("ossim_libraw_reader");
}
   
ossimString ossimLibRawReader::getLongName()const
{
   return ossimString("ossim libraw reader");
}

ossimString  ossimLibRawReader::getClassName()const
{
   return ossimString("ossimLibRawReader");
}

ossim_uint32 ossimLibRawReader::getNumberOfInputBands() const
{
   ossim_uint32 result = 0;
   cacheImage();
   if(theMemoryTile.valid())
   {
      result = theMemoryTile->getNumberOfBands();
   }
   return result;
}

ossim_uint32 ossimLibRawReader::getNumberOfOutputBands()const
{
   return getNumberOfInputBands();
}

ossimScalarType ossimLibRawReader::getOutputScalarType() const
{
   cacheImage();
   if(theMemoryTile.valid())
   {
      return theMemoryTile->getScalarType();
   }
   return ossimImageHandler::getOutputScalarType();
}

bool ossimLibRawReader::isOpen()const
{
   return ((theRawProcessor != 0)||
           (theMemoryTile.valid()));
}

double ossimLibRawReader::getMaxPixelValue(ossim_uint32 band)const
{
   cacheImage();
   if(theMemoryTile.valid())
   {
      return theMemoryTile->getMaxPix(band);
   }
   return ossimImageHandler::getMaxPixelValue(band);
}

void ossimLibRawReader::close()
{
   destroy();
   ossimImageHandler::close();
}

bool ossimLibRawReader::cacheImage()const
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   
   bool result = false;
   if(theMemoryTile.valid()) return true;
   if(!theRawProcessor) return result;
   int ret = 0;
   if( (ret = theRawProcessor->unpack() ) == LIBRAW_SUCCESS)
   {
      theRawProcessor->imgdata.params.output_bps = 16;
      theRawProcessor->imgdata.params.output_color = 1;
      theRawProcessor->imgdata.params.no_auto_bright = 1;
      
      
      ret = theRawProcessor->dcraw_process();
      if(LIBRAW_SUCCESS ==ret)
      {
         libraw_processed_image_t *image = theRawProcessor->dcraw_make_mem_image(&ret);
         if(image)
         {
            theMemoryTile = 0;
            theTile = 0;
            ossimScalarType scalarType = OSSIM_UINT16;
            if(image->bits == 8)
            {
               scalarType = OSSIM_UINT8;
            }
            theMemoryTile = new ossimImageData(0, scalarType, image->colors, image->width, image->height);
            theTile       = new ossimImageData(0, scalarType, image->colors, 8,8);
            theTile->initialize();
            theMemoryTile->initialize();
            theMemoryTile->loadTile(image->data, 
                                    theMemoryTile->getImageRectangle(), 
                                    OSSIM_BIP);
            result = true;
            free(image);
            image = 0;
         }
      }
   }
   delete theRawProcessor;
   theRawProcessor = 0;
   
   return result;
}


