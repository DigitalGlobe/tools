//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
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
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimQtStaticTileImageCache.cpp 12141 2007-12-07 18:05:13Z gpotts $
#include <ossimQt/ossimQtStaticTileImageCache.h>

ossimQtStaticTileImageCache::ossimQtStaticTileImageCache(const ossimIpt& tileSize)
   :theTileSize(tileSize)
{
   if(theTileSize.x <=0)
   {
      theTileSize.x = 64;
   }
   if(theTileSize.y <=0)
   {
      theTileSize.y = 64;
   }
   theCache.create(theTileSize.x,
                   theTileSize.y,
                   32);
   theValidTileArray.resize(1);
   theNumberOfTiles.x = 1;
   theNumberOfTiles.y = 1;
   theValidTileArray[0] = false;
}

QImage& ossimQtStaticTileImageCache::getCache()
{
   return theCache;
}

const QImage& ossimQtStaticTileImageCache::getCache()const
{
   return theCache;
}

void ossimQtStaticTileImageCache::flush()
{
   std::fill(theValidTileArray.begin(),
             theValidTileArray.end(),
             false);
//   theCache.fill(0);
}

void ossimQtStaticTileImageCache::flush(const ossimIpt& pt)
{
   flush(ossimIrect(pt.x,
                    pt.y,
                    pt.x,
                    pt.y));
}


void ossimQtStaticTileImageCache::flush(const ossimIrect& rect)
{
   if(rect == getCacheRect())
   {
      std::fill(theValidTileArray.begin(),
                theValidTileArray.end(),
                false);
//      theCache.fill(0);
   }
   else
   {
      int x = 0;
      int y = 0;
      int upperX = 0;
      int upperY = 0;
      ossimIrect tempRect = rect;
      tempRect.stretchToTileBoundary(theTileSize);

      upperX = tempRect.lr().x;
      upperY = tempRect.lr().y;
      for(y = tempRect.ul().y; y < upperY; y+=theTileSize.y)
      {
         for(x = tempRect.ul().x; x < upperX; x+=theTileSize.x)
         {
            ossim_int32 idx = getTileIndex(x, y);
            if(idx >= 0)
            {
               theValidTileArray[idx] = false; 
            }
         }
      }
   }
}

ossimIrect ossimQtStaticTileImageCache::getCacheRect()const
{
   QPoint pt = theCache.offset();
   
   return ossimIrect( pt.x(),
                      pt.y(),
                      pt.x() + theCache.width() - 1,
                      pt.y() + theCache.height() - 1);
}

void ossimQtStaticTileImageCache::setRect(const ossimIrect& newRect)
{
   ossimIrect tempRect = newRect;

   tempRect.stretchToTileBoundary(theTileSize);

   ossimIrect currentRect = getCacheRect();

   if(currentRect != tempRect)
   {
      if(!currentRect.intersects(tempRect))
      {
         if((currentRect.width()  != tempRect.width()) ||
            (currentRect.height() != tempRect.height()))
         {
            theCache.create(tempRect.width(),
                            tempRect.height(),
                            32);
         }
         theCache.fill(0);
         theCache.setOffset(QPoint(tempRect.ul().x,
                                   tempRect.ul().y));
         currentRect = getCacheRect();
         theNumberOfTiles.x = currentRect.width()/theTileSize.x;
         theNumberOfTiles.y = currentRect.height()/theTileSize.y;
         theValidTileArray.resize(theNumberOfTiles.x*theNumberOfTiles.y);
         std::fill(theValidTileArray.begin(),
                   theValidTileArray.end(),
                   false);
      }
      else
      {
         ossimIrect intersectionRect = currentRect.clipToRect(tempRect);
         ossimIpt offset = tempRect.ul() - currentRect.ul();
         
         ossimIpt oldNumberOfTiles = theNumberOfTiles;
         std::vector<bool> oldValidTileArray = theValidTileArray;
         ossimIrect  oldRect = currentRect;
         
         theCache = theCache.copy(offset.x,
                                  offset.y,
                                  tempRect.width(),
                                  tempRect.height());
         
         theCache.setOffset(QPoint(tempRect.ul().x,
                                   tempRect.ul().y));
         
         currentRect = getCacheRect();
         theNumberOfTiles.x = currentRect.width()/theTileSize.x;
         theNumberOfTiles.y = currentRect.height()/theTileSize.y;

         
         theValidTileArray.resize(theNumberOfTiles.x*theNumberOfTiles.y);

         std::fill(theValidTileArray.begin(),
                   theValidTileArray.end(),
                   false);
         
         int x = 0;
         int y = 0;
         int urX = intersectionRect.ur().x;
         int lrY = intersectionRect.lr().y;
         
         for(x = intersectionRect.ul().x; x <= urX; x+=theTileSize.x)
         {
            for(y = intersectionRect.ul().y; y <= lrY; y+=theTileSize.y)
            {
               ossim_int32 idx    = getTileIndex(x, y);
               ossim_int32 oldIdx = getTileIndex(oldRect,
                                                 oldNumberOfTiles,
                                                 x,
                                                 y);
               if(idx > -1)
               {
                  if(oldIdx > -1)
                  {
                     theValidTileArray[idx] = oldValidTileArray[oldIdx];
                  }
               }
            }
         }
      }
   }
}

bool ossimQtStaticTileImageCache::isValid(const ossimIpt& pt)const
{
   ossim_int32 idx = getTileIndex(pt);

   if(idx >= 0)
   {
      return theValidTileArray[idx];
   }

   return false;
}

ossim_int32 ossimQtStaticTileImageCache::getTileIndex(const ossimIpt& origin)const
{
   return getTileIndex(origin. x,
                       origin.y);
}

ossim_int32 ossimQtStaticTileImageCache::getTileIndex(ossim_int32 x,
                                                      ossim_int32 y)const
{
   ossimIpt ul = getCacheRect().ul();
   ossimIpt delta = ossimIpt(x,y) - ul;

   if((delta.x < 0) ||
      (delta.y < 0) ||
      (delta.x >= theCache.width())||
      (delta.y >= theCache.height()))
   {
      return -1;
   }
   delta.x /= theTileSize.x;
   delta.y /= theTileSize.y;

   return delta.y*theNumberOfTiles.x + delta.x;
}

bool ossimQtStaticTileImageCache::getTile(const ossimIpt& pt,
                                      QImage& image)const
{
  bool result = false;
   ossimIpt tileOrigin = getTileOrigin(pt);
   ossimIrect cacheRect = getCacheRect();

   if((image.width() != theTileSize.x)||
      (image.height() != theTileSize.y))
   {
      image.create(theTileSize.x,
                   theTileSize.y,
                   32);
   }
   if(cacheRect.pointWithin(tileOrigin))
   {
      ossimIpt delta(tileOrigin.x - cacheRect.ul().x,
                     tileOrigin.y - cacheRect.ul().y);

      if((delta.x >= 0)&&(delta.y >= 0))
      {
         image = theCache.copy(tileOrigin.x - cacheRect.ul().x,
                               tileOrigin.y - cacheRect.ul().y,
                               theTileSize.x,
                               theTileSize.y);
	 ossim_int32 idx = getTileIndex(pt);
	 if(idx >=0)
	   {
	     result = theValidTileArray[idx];
	   }
      }
      else
      {
         image.fill(0);
      }
   }
   else
   {
      image.fill(0);
   }

   return result;
}

ossimIpt ossimQtStaticTileImageCache::getTileOrigin(const ossimIpt& pt)const
{
   ossimIpt tempPt;

   if(pt.x < 0)
   {
      tempPt.x = pt.x - (theTileSize.x-1);
   }
   else
   {
      tempPt.x = pt.x + (theTileSize.x-1);
   }
   if(pt.y < 0)
   {
      tempPt.y = pt.y - (theTileSize.y-1);
   }
   else
   {
      tempPt.y = pt.y + (theTileSize.y-1);
   }

   return ossimIpt((tempPt.x/theTileSize.x)*theTileSize.x,
                   (tempPt.y/theTileSize.y)*theTileSize.y);
}

void ossimQtStaticTileImageCache::getSubImage(QImage& image)const
{
   QPoint ulSubImage(image.offset().x(),
                     image.offset().y());

   QPoint ulCache(theCache.offset().x(),
                  theCache.offset().y());

   image = theCache.copy(ulSubImage.x() - ulCache.x(),
                         ulSubImage.y() - ulCache.y(),
                         image.width(),
                         image.height());
}

void ossimQtStaticTileImageCache::setTileSize(const ossimIpt& tileSize)
{
   flush();
   theTileSize = tileSize;
   ossimIrect currentRect = getCacheRect();
   theNumberOfTiles.x = currentRect.width()/theTileSize.x;
   theNumberOfTiles.y = currentRect.height()/theTileSize.y;
}

const ossimIpt& ossimQtStaticTileImageCache::getTileSize()const
{
   return theTileSize;
}

bool ossimQtStaticTileImageCache::addTile(QImage& image)
{
   bool result = false;
   
   if(((image.offset().x()%theTileSize.x)==0)&&
      ((image.offset().y()%theTileSize.y)==0)&&
      (image.width() == theTileSize.x)&&
      (image.height() == theTileSize.y))
   {
      ossimIrect tileRect(image.offset().x(),
                          image.offset().y(),
                          image.offset().x() + (image.width()-1),
                          image.offset().y() + (image.height()-1));
      ossimIrect cacheRect = getCacheRect();

      if(tileRect.completely_within(cacheRect))
      {
         QRgb* tilePtr = (QRgb*)image.bits();
         ossimIpt offset = tileRect.ul() - cacheRect.ul();
         int x = 0;
         int y = 0;
         for(y = 0; y < theTileSize.y; ++y)
         {
            QRgb* cachePtr = ((QRgb*)theCache.scanLine(y+offset.y))+offset.x;
            for(x = 0; x < theTileSize.x; ++x)
            {
               *cachePtr = *tilePtr;
               ++cachePtr;
               ++tilePtr;
            }
         }
         ossim_int32 idx = getTileIndex(tileRect.ul());

         theValidTileArray[idx] = true;
         
         result = true;

      }
   }

   return result;
}

void ossimQtStaticTileImageCache::paintTiles(QPainter* /*p*/)
{
   
}

ossim_int32 ossimQtStaticTileImageCache::getTileIndex(const ossimIrect& rect,
                                                      const ossimIpt& numberOfTiles,
                                                      const ossimIpt& origin)const
{
   return getTileIndex(rect,
                       numberOfTiles,
                       origin.x,
                       origin.y);
}

ossim_int32 ossimQtStaticTileImageCache::getTileIndex(const ossimIrect& rect,
                                                      const ossimIpt& numberOfTiles,
                                                      ossim_int32 x,
                                                      ossim_int32 y)const
{
   ossimIpt ul = rect.ul();
   ossimIpt delta = ossimIpt(x,y) - ul;

   if((delta.x < 0) ||
      (delta.y < 0) ||
      (delta.x >= (int)rect.width())||
      (delta.y >= (int)rect.height()))
   {
      return -1;
   }
   delta.x /= theTileSize.x;
   delta.y /= theTileSize.y;

   return delta.y*numberOfTiles.x + delta.x;
}

ossim_int32 ossimQtStaticTileImageCache::computeTileId(const ossimIpt& origin,
						       const ossimIrect& tileBounderyRect,
						       const ossimIpt&   tileSize)
{
  ossim_uint32 numberOfTilesX = tileBounderyRect.width()/tileSize.x;
  
  ossimIpt ul = tileBounderyRect.ul();
  ossimIpt delta = origin - ul;
  
  if((delta.x < 0) ||
     (delta.y < 0) ||
     (delta.x >= (int)tileBounderyRect.width())||
     (delta.y >= (int)tileBounderyRect.height()))
    {
      return -1;
    }
  delta.x /= tileSize.x;
  delta.y /= tileSize.y;
  
  return delta.y*numberOfTilesX + delta.x;
}

