#include "ossimWorldWindHandler.h"
#include <ossim/base/ossimXmlNode.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <wms/wmsCurlMemoryStream.h>
#include <wms/wmsUrl.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <ossim/base/ossimKeywordNames.h>
#include "ossimSharedDiskCacheManager.h"
#include "ossimWorldWindJpegImage.h"
#include <ossim/base/ossimIoStream.h>
#include <ossim/base/ossimTrace.h>

//string relativePath = String.Format(@"{0}\{1:D4}\{1:D4}_{2:D4}.{3}",
//                      qt.Level, qt.Row, qt.Col, m_imageFileExtension);

static ossimTrace traceDebug("ossimWorldWindHandler:debug");

RTTI_DEF1(ossimWorldWindHandler, "ossimWorldWindHandler", ossimImageHandler);

ossimWorldWindHandler::ossimWorldWindHandler()
{
}

ossimWorldWindHandler::~ossimWorldWindHandler()
{
   close();
}

void ossimWorldWindHandler::close()
{
   if(theDiskCache.valid())
   {
      ossimFilename file = theDiskCache->rootDirectory();
      theDiskCache = 0;
      ossimSharedDiskCacheManager::instance()->releaseDiskCache(file);
   }
   theLayers.clear();
}

bool ossimWorldWindHandler::open()
{
   bool result = true;
   theCurrentIdx = 0;
   theXmlDocument = new ossimXmlDocument;
   
   if(theXmlDocument->openFile(getFilename()))
   {
      std::vector<ossimRefPtr<ossimXmlNode> > nodeList;
      theXmlDocument->findNodes("/LayerSet",
                                nodeList);

      
      if(nodeList.size())
      {
         ossimRefPtr<ossimXmlAttribute> attribute = nodeList[0]->findAttribute("redirect");
         if(attribute.get())
         {
            wmsCurlMemoryStream wmsStream(attribute->getValue());

            ossimNotify(ossimNotifyLevel_INFO) << "ossimWorldWindHandler::open() INFO: Trying to download remote image description from " << attribute->getValue() <<  std::endl;
            ossimFilename downloadXmlFilename = getFilename();
            downloadXmlFilename.setExtension("download");
            if(wmsStream.download(downloadXmlFilename))
            {
               theXmlDocument = new ossimXmlDocument;
                             
               if(!theXmlDocument->openFile(downloadXmlFilename))
               {
                  result = false;
                  theXmlDocument = 0;
               }
               else
               {
                  setFilename(downloadXmlFilename);
               }
            }
         }

         if(result)
         {
            if(!populateWorldWindStructure())
            {
               result = false;
            }
         }
      }
      
   }
   else
   {
      result = false;
   }

   if(result)
   {
      initialize();
   }
   
   return result;
}

bool ossimWorldWindHandler::isOpen()const
{
   return (theLayers.size() > 0);
}

ossim_uint32 ossimWorldWindHandler::getNumberOfLines(ossim_uint32 resLevel)const
{
   if(isOpen())
   {
      return theLayers[theCurrentIdx].theImageHeight>>resLevel;
   }
   
   return 0;
}

ossim_uint32 ossimWorldWindHandler::getNumberOfSamples(ossim_uint32 resLevel)const
{
   if(isOpen())
   {
      return theLayers[theCurrentIdx].theImageWidth>>resLevel;
   }
   return 0;
}

ossim_uint32 ossimWorldWindHandler::getImageTileWidth()const
{
   if(isOpen())
   {
      return theLayers[theCurrentIdx].theTileSize;
   }
   return 0;
}

ossim_uint32 ossimWorldWindHandler::getImageTileHeight()const
{
   if(isOpen())
   {
      return theLayers[theCurrentIdx].theTileSize;
   }
   return 0;
}

ossim_uint32 ossimWorldWindHandler::getNumberOfInputBands()const
{
   return 3;
}

ossim_uint32 ossimWorldWindHandler::getNumberOfOutputBands()const
{
   return 3;
}

ossimScalarType ossimWorldWindHandler::getOutputScalarType() const
{
   return OSSIM_UINT8;
}

void ossimWorldWindHandler::getEntryList(std::vector<ossim_uint32>& entryList) const
{
   ossim_uint32 idx = 0;

   for(idx = 0; idx < theLayers.size(); ++idx)
   {
      entryList.push_back(idx);
   }
}

ossim_uint32 ossimWorldWindHandler::getNumberOfEntries()const
{
   return theLayers.size();
}

ossim_uint32 ossimWorldWindHandler::getCurrentEntry()const
{
   return theCurrentIdx;
}

bool ossimWorldWindHandler::setCurrentEntry(ossim_uint32 entryIdx)
{
   if(entryIdx < theLayers.size())
   {
      theCurrentIdx = entryIdx;

      initialize();

      return true;
   }

   return false;
}

ossim_uint32 ossimWorldWindHandler::getNumberOfDecimationLevels()const
{
   if(isOpen())
   {
      return theLayers[theCurrentIdx].theLevels;
   }
   
   return 0;
}

void ossimWorldWindHandler::resetMemoryCache()
{
   ossim_uint32 idx = 0;
   if(theRLevelCacheId.size())
   {
      for(idx = 0; idx < theRLevelCacheId.size(); ++idx)
      {
         ossimAppFixedTileCache::instance()->deleteCache(theRLevelCacheId[idx]);
      }  
   }
   theRLevelCacheId.clear();
   ossim_uint32 rlevels = getNumberOfDecimationLevels();
   if(isOpen())
   {
      if(rlevels)
      {
         ossimIrect boundingRect;
         for(idx = 0; idx < rlevels; ++idx)
         {
            theRLevelCacheId.push_back(ossimAppFixedTileCache::instance()->newTileCache());
            boundingRect = getBoundingRect((rlevels-idx)-1);
            ossimIrect tileRect(0,
                                0,
                                (boundingRect.width()/theLayers[theCurrentIdx].theTileSize)-1,
                                (boundingRect.height()/theLayers[theCurrentIdx].theTileSize)-1);
            ossimAppFixedTileCache::instance()->setRect(theRLevelCacheId[idx],
                                                        tileRect);
            ossimAppFixedTileCache::instance()->setTileSize(theRLevelCacheId[idx], ossimIpt(1,1));
         }
      }
   }
}

void ossimWorldWindHandler::initialize()
{
   theResultTile = ossimImageDataFactory::instance()->create(this, this);
   theLocalTile   = ossimImageDataFactory::instance()->create(this, this);

   if(theResultTile.valid())
   {
      theResultTile->initialize();
	  // gave warnings in windows for unary minus on a unsigned value so put a 0 in front
      theResultTile->setOrigin(ossimIpt(0 - getNumberOfSamples(0),
                                        0 - getNumberOfLines(0)));
      theLocalTile->initialize();
      theLocalTile->setOrigin(ossimIpt(0-getNumberOfSamples(0),
                                       0-getNumberOfLines(0)));
   }
//    ossim_uint32 tileSizeInBytes = getImageTileWidth()*getImageTileHeight()*3;
//    if(theJpegDecodeBuffer.size() != tileSizeInBytes)
//    {
//       theJpegDecodeBuffer.resize(tileSizeInBytes);
//    }
   resetMemoryCache();
}

bool ossimWorldWindHandler::populateWorldWindStructure()
{
   theLayers.clear();
   
   std::vector<ossimRefPtr<ossimXmlNode> > childLayerSetList;
   // add layersets and child layersets for discovering QuadTileSets
   theXmlDocument->findNodes("/LayerSet/ChildLayerSet",
                             childLayerSetList);
   theXmlDocument->findNodes("/LayerSet",
                             childLayerSetList);
      
   if(childLayerSetList.size())
   {
      ossim_uint32 idx = 0;
      for(idx = 0; idx < childLayerSetList.size(); ++idx)
      {
         ossimRefPtr<ossimXmlAttribute> childLayerSetName = childLayerSetList[idx]->findAttribute("Name");
         std::vector<ossimRefPtr<ossimXmlNode> > quadTileSet;
         childLayerSetList[idx]->findChildNodes("QuadTileSet",
                                       quadTileSet);
         if(quadTileSet.size()&&childLayerSetName.valid())
         {
            ossim_uint32 quadTileIdx = 0;

            for(quadTileIdx = 0; quadTileIdx < quadTileSet.size(); ++quadTileIdx)
            {
               ossimWorldWindHandlerInfo info;
               ossimRefPtr<ossimXmlNode> name           = quadTileSet[quadTileIdx]->findFirstNode("Name");
               ossimRefPtr<ossimXmlNode> north          = quadTileSet[quadTileIdx]->findFirstNode("BoundingBox/North/Value");
               ossimRefPtr<ossimXmlNode> south          = quadTileSet[quadTileIdx]->findFirstNode("BoundingBox/South/Value");
               ossimRefPtr<ossimXmlNode> east           = quadTileSet[quadTileIdx]->findFirstNode("BoundingBox/East/Value");
               ossimRefPtr<ossimXmlNode> west           = quadTileSet[quadTileIdx]->findFirstNode("BoundingBox/West/Value");
               ossimRefPtr<ossimXmlNode> levels         = quadTileSet[quadTileIdx]->findFirstNode("ImageAccessor/NumberLevels");
               ossimRefPtr<ossimXmlNode> tileSizePixels = quadTileSet[quadTileIdx]->findFirstNode("ImageAccessor/TextureSizePixels");
               ossimRefPtr<ossimXmlNode> level0Gsd      = quadTileSet[quadTileIdx]->findFirstNode("ImageAccessor/LevelZeroTileSizeDegrees");
               ossimRefPtr<ossimXmlNode> extension      = quadTileSet[quadTileIdx]->findFirstNode("ImageAccessor/ImageFileExtension");
               
               if(name.valid()&&north.valid()&&south.valid()&&east.valid()&&west.valid()&&
                  levels.valid()&&tileSizePixels.valid()&&level0Gsd.valid())
               {
                  info.theQuadTileSetName   = name->getText();
                  info.theNorth  = north->getText().toDouble();
                  info.theSouth  = south->getText().toDouble();
                  info.theEast   = east->getText().toDouble();
                  info.theWest   = west->getText().toDouble();

                  info.theLevels = levels->getText().toUInt32();
                  info.theLevel0Gsd = level0Gsd->getText().toDouble();
                  info.theTileSize  = tileSizePixels->getText().toUInt32();


                  info.theNorth     = (std::ceil((info.theNorth+90.0)/info.theLevel0Gsd)*info.theLevel0Gsd)-90.0;
                  info.theSouth     = (std::floor((info.theSouth+90.0)/info.theLevel0Gsd)*info.theLevel0Gsd)-90.0;
                  info.theEast     = (std::floor((info.theEast+180.0)/info.theLevel0Gsd)*info.theLevel0Gsd)-180.0;
                  info.theWest     = (std::ceil((info.theWest+180.0)/info.theLevel0Gsd)*info.theLevel0Gsd)-180.0;

//                   std::cout << "n = " << info.theNorth << std::endl;
                  // set height and width
                  info.theImageHeight = (std::abs(info.theNorth-info.theSouth)/(info.theLevel0Gsd));
                  info.theImageHeight *= std::pow(2.0, (double)(info.theLevels-1));
                  info.theImageHeight *= info.theTileSize;

                  info.theImageWidth = (std::abs(info.theEast-info.theWest)/info.theLevel0Gsd);
                  info.theImageWidth *= std::pow(2.0, (double)(info.theLevels-1));
                  info.theImageWidth *= info.theTileSize;
                  info.theChildLayerName = childLayerSetName->getValue();
                  if(info.theLevels > 1)
                  {
                     info.theGsd    = (info.theNorth-info.theSouth)/info.theImageHeight;
                  }

                  

//                   ossim_float64 resX = (lon1-lon0)/info.theLevel0Gsd;
//                   ossim_float64 resY = (lat1-lat0)/info.theLevel0Gsd;

                  info.theExtension = "";
                  if(extension.valid())
                  {
                     info.theExtension = extension->getText();
                  }
                  ossimRefPtr<ossimXmlNode> node = quadTileSet[quadTileIdx]->findFirstNode("ImageAccessor/ImageTileService");
                  
                  if(node.valid())
                  {
                     if(node->getChildTextValue(info.theDatasetName, "DataSetName")&&
                        node->getChildTextValue(info.theUrl, "ServerUrl"))
                     {
                        info.theAccessorType   = ossimWorldWindHandlerInfo::WW_TILE_SERVER;
                        theLayers.push_back(info);
                     }
                  }
#if 1
                  else
                  {
                     node = quadTileSet[quadTileIdx]->findFirstNode("ImageAccessor/WMSAccessor");
                     if(node.valid())
                     {
                        info.theUrl = "";
                        info.theAccessorType   = ossimWorldWindHandlerInfo::WMS;
                        if(node->getChildTextValue(info.theDatasetName, "WMSLayerName")&&
                           node->getChildTextValue(info.theUrl, "ServerGetMapUrl"))
                        {
                           ossimString tempString;
                           info.theUrl += "?REQUEST=GetMap&LAYERS=" + info.theDatasetName;
                           if(node->getChildTextValue(tempString, "WMSLayerStyle"))
                           {
                              info.theUrl += ("&STYLES=" + tempString);
                           }
                           else
                           {
                              info.theUrl += "&STYLES=";
                           }
                           if(node->getChildTextValue(tempString, "UseTransparency"))
                           {
                              if(tempString.toBool())
                              {
                                 info.theUrl += "&TRANSPARENT=TRUE";
                              }
                              else
                              {
                                 info.theUrl += "&TRANSPARENT=FALSE";
                              }
                           }
                           if(node->getChildTextValue(tempString, "Version"))
                           {
                              info.theUrl += ("&VERSION=" + tempString);
                           }
                           else
                           {
                              info.theUrl += "&VERSION=1.0";
                           }
                           if(node->getChildTextValue(tempString, "ImageFormat"))
                           {
                              info.theUrl += ("&FORMAT=" + tempString);
                           }
                           else
                           {
                              info.theUrl += "&FORMAT=image/jpeg";
                           }
                           if(node->getChildTextValue(tempString, "ServiceName"))
                           {
                              info.theUrl += ("&SERVICE="+tempString);
                           }
                           info.theUrl += "&SRS=EPSG:4326";
                           theLayers.push_back(info);
                        }
                     }
                  }
#endif
               }
            }
         }
      }
   }

   if(theLayers.size())
   {
      theDiskCache =ossimSharedDiskCacheManager::instance()->getDiskCache(theImageFile.path());
   }
   
   if(!theDiskCache.valid())
   {
      theLayers.clear();
   }
   
   return (theLayers.size()>0);
}


bool ossimWorldWindHandler::getImageGeometry(ossimKeywordlist& kwl,
                                                  const char* prefix)
{
   if(isOpen())
   {

      ossimEquDistCylProjection equ;
      
      ossim_float64 halfGsd = theLayers[theCurrentIdx].theGsd*.5;
      equ.setDecimalDegreesPerPixel(ossimDpt(theLayers[theCurrentIdx].theGsd,
                                             theLayers[theCurrentIdx].theGsd));
      equ.setUlGpt(ossimGpt(theLayers[theCurrentIdx].theNorth - halfGsd,
                            theLayers[theCurrentIdx].theWest + halfGsd));
      
      equ.saveState(kwl);
      
      return true;
   }
   
   return false;
}

ossimRefPtr<ossimImageData> ossimWorldWindHandler::getTile(const ossimIrect& rect,
                                                           ossim_uint32 resLevel)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) <<  "ossimWorldWindHandler::getTile: entered........" << std::endl;
   }
   if(!isOpen())
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) <<  "ossimWorldWindHandler::getTile: leaving 1........" << std::endl;
      }
      return 0;
   }
   if(resLevel >= getNumberOfReducedResSets()) return 0;
   ossimIrect imageRect = getBoundingRect(resLevel);
   if(!rect.intersects(imageRect)) return 0;

   
   if(!theResultTile)
   {
      initialize();
      if(!theResultTile.valid())
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) <<  "ossimWorldWindHandler::getTile: leaving 2........" << std::endl;
         }
         return 0;
      }
   }
   
   ossim_uint32 tileSize = theLayers[theCurrentIdx].theTileSize;
   ossim_uint32 h = getNumberOfLines(resLevel);
   theResultTile->setImageRectangle(rect);

   theResultTile->makeBlank();
   ossimIrect clipRect = imageRect.clipToRect(rect);
   
   ossim_uint32 tileLeftX  = clipRect.ul().x/tileSize;
   ossim_uint32 tileRightX = clipRect.ur().x/tileSize;
   ossim_uint32 tileBottomY = ((imageRect.height() - clipRect.ll().y)/
                               tileSize);
   
   ossim_uint32 tileTopY = ((imageRect.height() - clipRect.ul().y)/
                            tileSize);
   

   ossim_uint32 wwLevel = (theLayers[theCurrentIdx].theLevels-1) - resLevel;
   ossim_uint32 leftTile = tileLeftX;


    ossim_uint32 xShift = (ossim_uint32)((theLayers[theCurrentIdx].theWest+180.0)/(theLayers[theCurrentIdx].theLevel0Gsd));
    ossim_uint32 yShift = (ossim_uint32)((theLayers[theCurrentIdx].theSouth+90.0)/(theLayers[theCurrentIdx].theLevel0Gsd));
    xShift<<=wwLevel;
    yShift<<=wwLevel;
   
   for(;tileBottomY <= tileTopY; ++tileBottomY)
   {
      for(leftTile=tileLeftX;leftTile <=tileRightX; ++leftTile)
      {
         ossimRefPtr<ossimImageData> tile = requestTile(wwLevel, leftTile+xShift, tileBottomY+yShift);

         if(tile.valid())
         {
            if(tile->getDataObjectStatus() != OSSIM_EMPTY)
            {
               tile->setOrigin(ossimIpt(tileSize*leftTile,
                                        (h - tileBottomY*tileSize) - tileSize));
               theResultTile->loadTile( convertToProperResult(tile.get()).get());
            }
         }
      }
   }
   theResultTile->validate();
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) <<  "ossimWorldWindHandler::getTile: leaving ........" << std::endl;
   }
   
   return theResultTile;
}

ossimRefPtr<ossimImageData> ossimWorldWindHandler::requestTile(ossim_uint32 level,
                                                               ossim_uint32 X,
                                                               ossim_uint32 Y)
{
   double gsd = theLayers[theCurrentIdx].theLevel0Gsd/std::pow(2.0, (double)level);
   ossimDpt lowerLeftLatLon(((X*gsd)-180),
                            ((Y*gsd)-90));
   ossimDpt upperRightLatLon((((X+1)*gsd)-180),
                             (((Y+1)*gsd)-90));
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) <<  "ossimWorldWindHandler::requestTile: <" << level << ", " << X << ", " << Y << "> entered........" << std::endl;
   }
   ossimRefPtr<ossimImageData> tempTile = requestMemoryCacheTile(level, X, Y);
   
   if(tempTile.valid())
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) <<  "ossimWorldWindHandler::requestTile: returning memory tile........" << std::endl;
      }
      return tempTile;
   }

   tempTile = requestDiskCacheTile(level, X, Y);
   if(tempTile.valid())
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) <<  "ossimWorldWindHandler::requestTile: returning disk tile........" << std::endl;
      }
      return tempTile;
   }
   ossimFilename relativeFile = buildFullRelativeFilename(level, Y, X);
   ossimFilename fullPath     = theDiskCache->fullPath(relativeFile);

   if(theDiskCache->isInCache(relativeFile))
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) <<  "ossimWorldWindHandler::requestTile: returning EMPTY tile........" << std::endl;
      }
      return 0;
   }
   ossimString completeUrl;
   if(theLayers[theCurrentIdx].theAccessorType == ossimWorldWindHandlerInfo::WW_TILE_SERVER)
   {
      completeUrl = (theLayers[theCurrentIdx].theUrl + "?T=" +
                     theLayers[theCurrentIdx].theDatasetName +
                     ossimString("&L=") +
                     ossimString::toString(level) + ossimString("&X=") +
                     ossimString::toString(X) + ossimString("&Y=") +
                     ossimString::toString(Y));
   }
   else if(theLayers[theCurrentIdx].theAccessorType == ossimWorldWindHandlerInfo::WMS)
   {
      completeUrl =  (theLayers[theCurrentIdx].theUrl +
                      "&BBOX=" +  ossimString::toString(lowerLeftLatLon.x)+ "," +
                      ossimString::toString(lowerLeftLatLon.y) + "," +
                      ossimString::toString(upperRightLatLon.x) + "," +
                      ossimString::toString(upperRightLatLon.y) +
                      "&BGCOLOR=0x000000" +
                      "&WIDTH=" + ossimString::toString(theLayers[theCurrentIdx].theTileSize) +
                      "&HEIGHT=" + ossimString::toString(theLayers[theCurrentIdx].theTileSize));

//       return 0;
   }
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "URL = " << completeUrl << std::endl;
   }
   
   if(!completeUrl.empty())
   {
      wmsUrl url(completeUrl);
      
      wmsCurlMemoryStream wmsStream(url.url());
      wmsStream.setMaxRedirects(1);
      wmsStream.setMaxNumberRetry(0);
      wmsStream.setFollowLocationFlag(1);
      wmsStream.setTimeout(5);
      fullPath.path().createDirectory();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) <<  "ossimWorldWindHandler::requestTile: downloading " << url.url() << std::endl;
      }
      if(wmsStream.download(fullPath))
      {
         std::ifstream in(fullPath.c_str(), ios::in|std::ios::binary);
         if(!in.fail())
         {
            bool ok = false;
            char buf[1024];
            in.read(buf, 1023);
            int c = in.gcount();
            if(c > 0)
            {
               buf[c] = '\0';
               ossimString tempTest(buf);

               if((!tempTest.contains("xml"))&&
                  (!tempTest.contains("Service Unavailable")))
               {
                  ok = true;
               }
               else
               {
                  if(traceDebug())
                  {
                     ossimNotify(ossimNotifyLevel_DEBUG) << "ERROR = " << buf << std::endl;
                  }
                  fullPath.remove();
                  theDiskCache->addFile(relativeFile);
               }
            }
            in.close();
            if(ok)
            {
               ossimRefPtr<ossimImageHandler> ih = ossimImageHandlerRegistry::instance()->open(fullPath);
               if(ih.valid())
               {
                  if(traceDebug())
                  {
                     ossimNotify(ossimNotifyLevel_DEBUG) <<  "ossimWorldWindHandler::requestTile: returning downloaded tile " << relativeFile << std::endl;
                  }
                  theDiskCache->addFile(relativeFile);
                  
                  return ih->getTile(ih->getBoundingRect());
               }
               else
               {
                  fullPath.remove();
                  theDiskCache->addFile(relativeFile);
               }
            }
            else
            {
               // let's still mark it for now
               theDiskCache->addFile(relativeFile);
            }
         }
         else
         {
            // let's mark it still in disk cache;
            fullPath.remove();
            theDiskCache->addFile(relativeFile);
         }
      }
      else
      {
      }
   }
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) <<  "ossimWorldWindHandler::requestTile: returning NULL tile " << std::endl;
   }
   
   return 0;
}


ossimRefPtr<ossimImageData> ossimWorldWindHandler::requestDiskCacheTile(ossim_uint32 level,
                                                                        ossim_uint32 X,
                                                                        ossim_uint32 Y)
{
   ossimRefPtr<ossimImageData> data;
   ossimFilename relativeFile = buildFullRelativeFilename(level, Y, X);
   ossimFilename fullPath     = theDiskCache->fullPath(relativeFile);
   if(fullPath.exists())
   {
      if(fullPath.fileSize() > 1)
      {
         ossimRefPtr<ossimImageHandler> ih = ossimImageHandlerRegistry::instance()->open(fullPath);
         
         if(ih.valid())
         {
            data = ih->getTile(ih->getBoundingRect());
            if(data.valid())
            {
               data->setOrigin(ossimIpt(X, Y));
               theDiskCache->accessFile(relativeFile);
               ossimAppFixedTileCache::instance()->addTile(theRLevelCacheId[level],
                                                           data);
//             if(data.valid())
//             {
//                ossim_uint8* bands[3];
//                bands[0] = (ossim_uint8*)data->getBuf(0);
//                bands[1] = (ossim_uint8*)data->getBuf(1);
//                bands[2] = (ossim_uint8*)data->getBuf(2);

//                ossim_uint32 maxSize = data->getWidth()*data->getHeight();
//                ossim_uint32 offset  = 0;

//                for(offset = 0; offset < maxSize; ++offset)
//                {
//                   if((bands[0][offset] == 255)&&
//                      (bands[1][offset] == 255)&&
//                      (bands[2][offset] == 255))
//                   {
//                      bands[0][offset] = bands[1][offset] = bands[2][offset] = 0;
//                   }
//                }
//                data->validate();
               
               if(data->getDataObjectStatus() == OSSIM_EMPTY)
               {
                  theDiskCache->removeFile(relativeFile);
                  theDiskCache->addFile(relativeFile);
                  data = 0;
               }
               return data;
            }
         }
      }
   }
   else if(theDiskCache->isInCache(relativeFile))
   {
      theDiskCache->accessFile(relativeFile);
   }

   return data;
}

ossimRefPtr<ossimImageData> ossimWorldWindHandler::requestMemoryCacheTile(ossim_uint32 level,
                                                                          ossim_uint32 X,
                                                                          ossim_uint32 Y)
{
   ossimFilename relativeFile = buildFullRelativeFilename(level, Y, X);
   ossimRefPtr<ossimImageData> data = ossimAppFixedTileCache::instance()->getTile(theRLevelCacheId[level],
                                                                                  ossimIpt(X,Y));
   return data;
}

ossimFilename ossimWorldWindHandler::buildTileFilename(ossim_uint32 level,
                                                   ossim_uint32 row,
                                                   ossim_uint32 col)const
{
   std::ostringstream out;

   out << level << "/"
       << std::setw(4) << std::setfill('0') << row << "/"
       << std::setw(4) << std::setfill('0') << row << "_"
       << std::setw(4) << std::setfill('0') << col;

   if(isOpen())
   {
      if(theLayers[theCurrentIdx].theExtension != "")
      {
         out << "." << theLayers[theCurrentIdx].theExtension;
      }
   }
   return ossimFilename(out.str().c_str());
}

ossimFilename ossimWorldWindHandler::buildFullRelativeFilename(ossim_uint32 level,
                                                               ossim_uint32 row,
                                                               ossim_uint32 col)const
{
   if(!isOpen()) return ossimFilename("");
   std::ostringstream out;

   ossimFilename tileFilename = buildTileFilename(level, row, col);

   out << theLayers[theCurrentIdx].theChildLayerName << "/"
       << theLayers[theCurrentIdx].theQuadTileSetName << "/"
       << tileFilename;

   return ossimFilename(out.str());
}

bool ossimWorldWindHandler::saveState(ossimKeywordlist& kwl,
                                      const char* prefix)const
{
   return ossimImageHandler::saveState(kwl, prefix);
}

bool ossimWorldWindHandler::loadState(const ossimKeywordlist& kwl,
                                      const char* prefix)
{
   ossimImageHandler::loadState(kwl, prefix);

   return open();
}


ossimRefPtr<ossimImageData> ossimWorldWindHandler::convertToProperResult(ossimRefPtr<ossimImageData> data)
{
   ossimRefPtr<ossimImageData> result = data.get();
   
   if(data->getNumberOfBands() != 3)
   {
      theLocalTile->setImageRectangle(data->getImageRectangle());
      ossim_uint8* srcBands[3];
      ossim_uint8* destBands[3];
      srcBands[0] = (ossim_uint8*)data->getBuf(0);
      srcBands[1] = (ossim_uint8*)data->getBuf(1);
      srcBands[2] = (ossim_uint8*)data->getBuf(2);
      destBands[0] = (ossim_uint8*)theLocalTile->getBuf(0);
      destBands[1] = (ossim_uint8*)theLocalTile->getBuf(1);
      destBands[2] = (ossim_uint8*)theLocalTile->getBuf(2);
      
      if(!srcBands[1]) srcBands[1] = srcBands[0];
      if(!srcBands[2]) srcBands[2] = srcBands[0];
      ossim_uint32 maxSize = data->getWidth()*data->getHeight();
      ossim_uint32 offset  = 0;
      
      for(offset = 0; offset < maxSize; ++offset)
      {
         destBands[0][offset] = srcBands[0][offset];
         destBands[1][offset] = srcBands[1][offset];
         destBands[2][offset] = srcBands[2][offset];
      }
      theLocalTile->validate();
      result = theLocalTile.get();
   }

   return result;
}
