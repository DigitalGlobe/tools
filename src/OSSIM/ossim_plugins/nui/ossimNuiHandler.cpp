#include "ossimNuiHandler.h"
#include <ossim/base/ossimTrace.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimFixedTileCache.h>
#include <OpenThreads/Mutex>
#include <OpenThreads/SCopedLock>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>

ossimNuiData::ossimNuiData()
{
   nuif              = NULL;
   geo               = NULL;
   tile_width        = 0;
   tile_height       = 0;
   tile_size         = 0;
   image_width       = 0;
   image_height      = 0;
   x_pixel_size      = 0;
   y_pixel_size      = 0;
   channels          = 0;
   bytes_per_channel = 0;
   compression_ratio = 0.0;
   total_levels      = 0;
   top_left_x        = 0.0;
   top_left_y        = 0.0;
   bot_right_x       = 0.0;
   bot_right_y       = 0.0;
   tile_buffer       = 0;
   decode            = 0;
   codestream_size   = 0;
   codestream_buffer = 0;
   codec             = NUIF_CODEC_NONE;
}

ossimNuiData::~ossimNuiData()
{
//    assert(nuif == NULL);
//    assert(geo == NULL);
}


static ossimTrace traceDebug("ossimNuiHandler:debug");

RTTI_DEF1(ossimNuiHandler, "ossimNuiHandler", ossimImageHandler);

ossimNuiHandler::ossimNuiHandler()
{
}

ossimNuiHandler::~ossimNuiHandler()
{
   close();
}

void ossimNuiHandler::close()
{
   clearCache();
   if (theNui.decode)
   {
      if (theNui.codestream_buffer)
      {
         NUI_SUPPORT_free(theNui.codestream_buffer);
         theNui.codestream_buffer = 0;
      }
      NUI_J2K2_DECODE_destroy(theNui.decode);
      theNui.decode = 0;
   }
   if (theNui.nuif) 
   {
      NUIF_close_file(theNui.nuif);
      NUIF_destroy(theNui.nuif);
   }
   theNui.codec = NUIF_CODEC_NONE;
   theNui.nuif = 0;
   theNui.geo = 0;
   theNui.tile_width = 0;
   theNui.tile_height = 0;
   theNui.tile_size = 0;
   theNui.image_width = 0;
   theNui.image_height = 0;
   theNui.decode = 0;
   if(theNui.tile_buffer)
   {
      NUI_SUPPORT_free(theNui.tile_buffer);
      theNui.tile_buffer = 0;
      theNui.tile_buffer_rect.makeNan();
   }
}

bool ossimNuiHandler::open()
{
   static OpenThreads::Mutex openMutex;
   
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(openMutex);
   
   int ret_val = 0;
   
#ifdef NUI_PLATFORM_UNIX_MACOS
   //This is a step necessary for GCC on Mac OSX which through
   //optimizations removes nuiSDK_error and won't link.
   (void*)nuiSDK_error_string();
#endif
   
   if(isOpen())
   {
      close();
   }
   
   theNui.nuif = NUIF_open_file((NUI_CHAR*)theImageFile.c_str(), NUIF_MODE_READONLY);
   if (theNui.nuif == NULL) 
   {
//       std::cout << "ossimNuiHandler:open could not open " << theImageFile.c_str()
// 	            << " (" << NUIF_error_string() << ")" << std::endl;
      return false;
   }
//    std::cout << "KJ: ossimNuiHandler:open NUI file " << theImageFile.c_str()  << " succeeded." << std::endl;
   theNui.tile_buffer = 0;
   theNui.geo = NUIF_GEO_get_object(theNui.nuif);
   if (theNui.geo == NULL)
   {
//       std::cout << "KJ: Unable to get GEO header from the NUI file.  Sorry." << std::endl;
//       close();
//       return false;   
   }
   
   NUIF_get_image_parameters(theNui.nuif,
      &theNui.image_width, 
	  &theNui.image_height,
	  &theNui.channels,
	  &theNui.bits_per_channel,
	  &theNui.type,
	  &theNui.layout,
	  &theNui.codec,
	  &theNui.compression_ratio);
   theNui.bytes_per_channel = NUIF_get_total_bytes_per_channel(theNui.nuif);

   switch (theNui.bits_per_channel)
   {
   case NUIF_BITS_8:
   case NUIF_BITS_16:
   case NUIF_BITS_32:
   case NUIF_BITS_FLOAT:
   case NUIF_BITS_DOUBLE:
      break;

   default:
   {
      ossimNotify(ossimNotifyLevel_WARN) << "ossimNuiHandler::open WARNING: Type either unsupported or not sure and need to verify. Sorry." << std::endl;
      close();
      return false;
   }
   };
   
   NUIF_get_tile_size(theNui.nuif, &theNui.tile_width, &theNui.tile_height);
   theNui.tile_size = theNui.tile_width * theNui.tile_height 
                    * theNui.channels * theNui.bytes_per_channel;
					
   NUIF_GEO_get_basic_parameters(theNui.geo, NULL, NULL,
	                            &theNui.x_pixel_size, &theNui.y_pixel_size);
   theNui.total_levels = NUIF_get_total_pyramid_levels(theNui.nuif);
   
   theNui.top_left_x = theNui.top_left_y = theNui.bot_right_x = theNui.bot_right_y = 0;
   ret_val = NUIF_GEO_get_projected_bounding_box(theNui.nuif, 
                 &theNui.top_left_x,  &theNui.top_left_y,
				 &theNui.bot_right_x, &theNui.bot_right_y);
   if (ret_val < 0)
   {
      ossimNotify(ossimNotifyLevel_WARN) << "ossimNuiHandler::open WARNING: Unable to get projected bounding box from the NUI file." << std::endl;
   }
   if (theNui.codec == NUIF_CODEC_J2K) 
   {
      theNui.decode = NUI_J2K2_DECODE_create(NUI_J2K2_SDK_KAKADU);
      if (theNui.decode == NULL)
      {
         close();
         return false;
      }
      
      //Create the codestream buffer.  
      theNui.codestream_size = (NUI_UINT32) 
         NUIF_LEVEL_get_tile_data_size_in_bytes(
            NUIF_LEVEL_get_first(theNui.nuif));
      theNui.codestream_buffer =  (NUI_UCHAR  *)NUI_SUPPORT_malloc(theNui.codestream_size, 0);
      if (theNui.codestream_buffer == NULL)
      {
         close();
         return false;
      }
      NUI_J2K2_DECODE_set_raw_image_mem_size(theNui.decode,
                                             theNui.tile_size); 
   }
   else if(theNui.codec != NUIF_CODEC_NONE)
   {
      ossimNotify(ossimNotifyLevel_WARN) << "ossimNuiHandler::open WARNGIN: The NUI plugin to OSSIM does not handle the CODEC yet." << std::endl;
      close();
      return false;
   }
   std::vector<NUI_CHAR> coordName(1024*32);
   std::vector<NUI_CHAR> coordUnits(1024*32);
   NUIF_GEO_get_coordinate_system_name_and_units(theNui.nuif,
                                                 &coordName.front(),
                                                 coordName.size(),
                                                 &coordUnits.front(),
                                                 coordUnits.size());
   ossimString units(&coordUnits.front());

   theNui.coord_unit_string = "unknown";
   theNui.coord_unit = OSSIM_UNIT_UNKNOWN;
   if(units.size())
   {
      units = units.downcase();
      if(units == ossimString("u.u. foot"))
      {
         theNui.coord_unit_string = "us_survey_feet";
         theNui.coord_unit = OSSIM_US_SURVEY_FEET;
      }
      else if(units == ossimString("us survey foot"))
      {
         theNui.coord_unit_string = "us_survey_feet";
         theNui.coord_unit = OSSIM_US_SURVEY_FEET;
      }
      else if(units == ossimString("degree"))
      {
         theNui.coord_unit_string = "degrees";
         theNui.coord_unit = OSSIM_DEGREES;
      }
      else if(units == ossimString("meter"))
      {
         theNui.coord_unit_string = "meters";
         theNui.coord_unit = OSSIM_METERS;
      }
      else if(units == ossimString("metre"))
      {
         theNui.coord_unit_string = "meters";
         theNui.coord_unit = OSSIM_METERS;
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN) << "ossimNuiHandler::open WARNING: need a translation for projection cooord units == " << std::endl;
      }
   }
   std::vector<NUI_CHAR> wktString(32*1024);
   theNui.wkt_string.resize(32*1024); // resize to 32k
   theNui.wkt_string = "";
   if(NUIF_GEO_get_coordinate_system_as_OpenGIS_WKT(theNui.nuif,
                                                    NUI_OPENGIS_WKT_STANDARD,
                                                    &wktString.front(),
                                                    wktString.size())==0)
   {
      theNui.wkt_string = &wktString.front();
   }

   theNui.channelMins.clear();
   theNui.channelMaxs.clear();
   NUIF_CHANNEL* channel = NUIF_CHANNEL_get_first(theNui.nuif);
   NUI_DOUBLE minValue;
   NUI_DOUBLE maxValue;

   while(channel)
   {
      
      NUIF_CHANNEL_get_range(channel,
                             &minValue,
                             &maxValue);
      
      theNui.channelMins.push_back(minValue);
      theNui.channelMaxs.push_back(maxValue);
      channel = NUIF_CHANNEL_get_next(channel);
   }
   return true;
}

bool ossimNuiHandler::isOpen()const
{
   if (theNui.nuif != NULL)
      return true;
   
   return false;
}

ossim_uint32 ossimNuiHandler::getNumberOfLines(ossim_uint32 resLevel)const
{
   if(isOpen())
   {
      // return the number of lines for a given resolution level(Height)
      // note,  in OSSIM library it expects the level 0 to typically be the full res image and then
      // the image get's smaller
      
      if (resLevel < theNui.total_levels)
      {
         NUIF_LEVEL *level = NUIF_LEVEL_get_by_id(theNui.nuif, resLevel);
//         assert(level != NULL);
         if (level != NULL) 
         {
            NUI_UINT32   height;
            NUIF_LEVEL_get_size(level, NULL, &height, NULL, NULL, NULL, NULL);
            return height;
         }
      }
   }
   
   return 0;
}

ossim_uint32 ossimNuiHandler::getNumberOfSamples(ossim_uint32 resLevel)const
{
   if(isOpen())
   {
      // return the number of samples for a given resolution level (Width)
      // note,  in OSSIM library it expects the level 0 to typically be the full res image and then
      // the image get's smaller

	  if (resLevel < theNui.total_levels)
	  {
	     NUIF_LEVEL *level = NUIF_LEVEL_get_by_id(theNui.nuif, resLevel);
// 		 assert(level != NULL);
		 if (level != NULL) 
		 {
			NUI_UINT32   width;
			NUIF_LEVEL_get_size(level, &width, NULL, NULL, NULL, NULL, NULL);
			return width;
		 }
	  }
   }
   return 0;
}

ossim_uint32 ossimNuiHandler::getImageTileWidth()const
{
   if(isOpen())
   {
      // if this image is tiled it will return the tiled dimensions.  This is used to create
      // a nui tile buffer.  Typical nui files should be blocked or tiled with a width and height
	  return theNui.tile_width;
   }
   return 0;
}

ossim_uint32 ossimNuiHandler::getImageTileHeight()const
{
   if(isOpen())
   {
      // if this image is tiled it will return the tiled dimensions.  This is used to create
      // a nui tile buffer.  Typical nui files should be blocked or tiled with a width and height
	  return theNui.tile_height;
   }
   
   return 0;
}

ossim_uint32 ossimNuiHandler::getNumberOfInputBands()const
{
   if (isOpen())
   {
//       std::cout << "KJ: Garrett, please make sure Input and Output bands = number of channels for NUI." << std::endl;
	  // return the number of bands for this handler.
      return theNui.channels;
   }
   return 0;
}

ossim_uint32 ossimNuiHandler::getNumberOfOutputBands()const
{
   if(isOpen())
   {
      // return the number of bands for this handler.
//       std::cout << "KJ: Garrett, please make sure Input and Output bands = number of channels for NUI." << std::endl;
      return theNui.channels;
   }
   return 0;
}

ossimScalarType ossimNuiHandler::getOutputScalarType() const
{
//    std::cout << "Need to implement ossimNuiHandler::getOutputScalarType()" << std::endl;
   // need to return the correct scalar type of the pixel data.  These are found in ossim/base/ossimConstants.h
   //
   //
   
   if (isOpen())
   {
      switch (theNui.bits_per_channel)
      {
         case NUIF_BITS_8:
         {
            return OSSIM_UINT8;
         }
         case NUIF_BITS_16:
	 {
            return OSSIM_SINT16;
         }
         case NUIF_BITS_32:
	 {
            return OSSIM_SINT32;
         }
         case NUIF_BITS_FLOAT:
	 {
            return OSSIM_FLOAT32;
         }
         case NUIF_BITS_DOUBLE:
	 {
            return OSSIM_FLOAT64;
         }
         default:
	 {
            return OSSIM_SCALAR_UNKNOWN;
         }
      }
   }
   
   return OSSIM_SCALAR_UNKNOWN;
}

ossim_uint32 ossimNuiHandler::getNumberOfDecimationLevels()const
{
   if(isOpen())
   {
      // this value returned should include the full res set.  So at the minimum it
      // should be 1
	  return theNui.total_levels;
   }
   
   return 0;
}


void ossimNuiHandler::initialize()
{
   theTile          = 0;
   theNui.tile_buffer_rect.makeNan();
   clearCache();
//    theTileCache.clear();
}

void ossimNuiHandler::allocateTile()
{
   if(!isOpen()) return;
   clearCache();
//    theTileCache.clear();
   if(theNui.tile_width&&theNui.tile_height)
   {
      theTile          = ossimImageDataFactory::instance()->create(this, this);
      if(theTile.valid())
      {
         theNuiTileBuffer = (ossimImageData*)theTile->dup();
         
         theNuiTileBuffer->setWidthHeight(theNui.tile_width,
                                          theNui.tile_height);
         theTile->initialize();
         theNuiTileBuffer->initialize();
         if(theNui.tile_buffer)
         {
            NUI_SUPPORT_free(theNui.tile_buffer);
            theNui.tile_buffer = 0;
         }
         theNui.tile_buffer = (NUI_UCHAR*)NUI_SUPPORT_malloc(theNui.tile_size,
                                                             0);
         theTileCache.resize(theNui.total_levels);
         ossim_uint32 idx = 0;

         for(idx = 0; idx < theTileCache.size(); ++idx)
         {
            theTileCache[idx] = ossimAppFixedTileCache::instance()->newTileCache();
//              = new ossimFixedTileCache;
            ossimAppFixedTileCache::instance()->setRect(theTileCache[idx],
                                                        getBoundingRect(idx));
            ossimAppFixedTileCache::instance()->setTileSize(theTileCache[idx],
                                                            ossimIpt(theNui.tile_width,
                                                                     theNui.tile_height));
            
//             theTileCache[idx]->setRect(getBoundingRect(idx),
//                                        ossimIpt(theNui.tile_width,
//                                                 theNui.tile_height));
//             theTileCache[idx]->setMaxCacheSize(theNui.tile_size*4);
         }
      }
   }
}

ossimRefPtr<ossimImageGeometry> ossimNuiHandler::getInternalImageGeometry() const
{
   ossimRefPtr<ossimImageGeometry> geom = new ossimImageGeometry();
   
   if( isOpen() )
   {
      if((theNui.wkt_string!="")&&(theNui.coord_unit!=OSSIM_UNIT_UNKNOWN))
      {
         ossimKeywordlist kwl;
         kwl.add(ossimKeywordNames::TYPE_KW,
                 theNui.wkt_string,
                 true);
         ossimDpt tiePoint(theNui.top_left_x,
                           theNui.top_left_y);
         kwl.add(ossimKeywordNames::TIE_POINT_XY_KW,
                 tiePoint.toString(),
                 true);
         kwl.add(ossimKeywordNames::TIE_POINT_UNITS_KW,
                 theNui.coord_unit_string,
                 true);
         kwl.add(ossimKeywordNames::PIXEL_SCALE_XY_KW,
                 ossimDpt( theNui.x_pixel_size,
                           theNui.y_pixel_size).toString(),
                 true);
         kwl.add(ossimKeywordNames::PIXEL_SCALE_UNITS_KW,
                 theNui.coord_unit_string,
                 true);
         geom->setProjection(ossimProjectionFactoryRegistry::instance()->createProjection(kwl));
      }
   }
   
   return geom;
}

ossimRefPtr<ossimImageData> ossimNuiHandler::getTile(const ossimIrect& rect,
                                                           ossim_uint32 resLevel)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) <<  "ossimNuiHandler::getTile: entered........" << std::endl;
   }
   if(!isOpen())
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) <<  "ossimNuiHandler::getTile: leaving 1........" << std::endl;
      }
      return 0;
   }
   if(!theTile.valid()||!theNuiTileBuffer.valid())
   {
      allocateTile();
   }
   
   if(theTile.valid())
   {
      theTile->setImageRectangle(rect);
   }
   else
   {
      return 0;
   }
   if(!theNuiTileBuffer.valid())
   {
      return theTile;
   }
   if(resLevel >= theNui.total_levels)
   {
      return 0;
   }
   theTile->makeBlank();
   ossimInterleaveType interleaveType;
   switch(theNui.layout)
   {
      case NUIF_DATA_LAYOUT_PACKED:
      {
         interleaveType = OSSIM_BIP;
         break;
      }
      case NUIF_DATA_LAYOUT_PLANAR:
      {
         interleaveType = OSSIM_BSQ;
         break;
      }
      default:
      {
         return theTile;
      }
   }

   ossimIrect boundingRect = getBoundingRect(resLevel);
   if(!boundingRect.intersects(rect)) return 0;
   ossimIrect tempRect = boundingRect.clipToRect(rect);
   tempRect.stretchToTileBoundary(ossimIpt(theNui.tile_width,
                                           theNui.tile_height));
   ossim_uint32 x, y;
   ossim_uint32 h = tempRect.height();
   ossim_uint32 w = tempRect.width();
   ossimIpt origin;
   ossim_uint32 row, col;
   ossimIpt ul = tempRect.ul();
   ossimRefPtr<ossimImageData> data = 0;
   
   for(y = 0; y < h; y+=theNui.tile_height)
   {
      for(x = 0; x < w; x+=theNui.tile_width)
      {
         data = 0;
         origin = ul+ossimIpt(x, y);
         if(resLevel < theTileCache.size())
         {
            data = ossimAppFixedTileCache::instance()->getTile(theTileCache[resLevel], origin);
         }
         if(data.valid())
         {
            if(data->getBuf() &&
               (data->getDataObjectStatus()!=OSSIM_EMPTY))
            {
               theTile->loadTile(data->getBuf(),
                                 data->getImageRectangle(),
                                 OSSIM_BSQ);
            }
         }
         else
         {
            bool decoded = true;
            row = origin.y/theNui.tile_height;
            col = origin.x/theNui.tile_width;
            if(NUIF_read_packed_tile_from_level(theNui.nuif,
                                                resLevel,
                                                row,
                                                col,
                                                ((theNui.decode)?theNui.codestream_buffer:
                                                 theNui.tile_buffer)) == 0)
            {
               if(theNui.decode)
               {
                  if(NUI_J2K2_DECODE_set_code_stream(theNui.decode,
                                                     theNui.codestream_buffer, 
                                                     theNui.codestream_size) < 0)
                  {
                     decoded = false;
                  }
               
                  if(NUI_J2K2_DECODE_decompress_code_stream(theNui.decode,
                                                            NULL, NULL, NULL, NULL, theNui.tile_buffer) < 0)
                  {
                     decoded = false;
                  }
               }
               if(decoded)
               {
                  theNui.tile_buffer_rect=ossimIrect(origin.x,
                                                     origin.y,
                                                     origin.x + (theNui.tile_width-1),
                                                     origin.y + (theNui.tile_height-1));
                  
                  if(resLevel < theTileCache.size())
                  {
                     ossimRefPtr<ossimImageData> dataToUse = theNuiTileBuffer;
                     dataToUse->setImageRectangle(theNui.tile_buffer_rect);
                     dataToUse->makeBlank();
                     dataToUse->loadTile(theNui.tile_buffer,
                                         theNui.tile_buffer_rect,
                                         interleaveType);
                     dataToUse->setDataObjectStatus(OSSIM_FULL);
                     ossimAppFixedTileCache::instance()->addTile(theTileCache[resLevel], dataToUse);                     
                  }
                  theTile->loadTile(theNui.tile_buffer,
                                    theNui.tile_buffer_rect,
                                    interleaveType);
               }
            }
         }
      }
   }
   theTile->validate();
   
   return theTile;
}

double ossimNuiHandler::getNullPixelValue(ossim_uint32 band)const
{
   return ossimImageHandler::getNullPixelValue(band);
}

double ossimNuiHandler::getMinPixelValue(ossim_uint32 band)const
{
   if(isOpen())
   {
      if(band < theNui.channelMins.size())
      {
			if(theNui.channelMins[band] != getNullPixelValue(band))
			{
				return theNui.channelMins[band];
			}
      }
   }
   return ossimImageHandler::getMinPixelValue(band);
}

double ossimNuiHandler::getMaxPixelValue(ossim_uint32 band)const
{
   if(isOpen())
   {
      if(band < theNui.channelMaxs.size())
      {
         return theNui.channelMaxs[band];
      }
   }
   return ossimImageHandler::getMaxPixelValue(band);
}

bool ossimNuiHandler::saveState(ossimKeywordlist& kwl,
                                      const char* prefix)const
{
   return ossimImageHandler::saveState(kwl, prefix);
}

bool ossimNuiHandler::loadState(const ossimKeywordlist& kwl,
                                      const char* prefix)
{
   ossimImageHandler::loadState(kwl, prefix);

   return open();
}

void ossimNuiHandler::clearCache()
{
   ossim_uint32 idx = 0;
   for(idx = 0; idx < theTileCache.size();++idx)
   {
      ossimAppFixedTileCache::instance()->deleteCache(theTileCache[idx]);
   }
   theTileCache.clear();
}
