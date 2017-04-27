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
// $Id: ossimQtStaticTileImageCache.h 12121 2007-12-06 13:14:41Z gpotts $
#ifndef ossimQtStaticTileImageCache_HEADER
#define ossimQtStaticTileImageCache_HEADER
#include <QtGui/QImage>
#include <vector>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimIrect.h>

class ossimQtStaticTileImageCache
{
public:
   ossimQtStaticTileImageCache(const ossimIpt& tileSize=ossimIpt(0,0));
   
   QImage& getCache();
   const QImage& getCache()const;
    ossimIrect getCacheRect()const;

   /*!
    * Will invalidate the entire cache and set the image cache to blank
    */
   void flush();

   /*!
    * Will determine the til that is associated with the pt
    * and will invalidate that tile and blank out that portion of the image
    */
   void flush(const ossimIpt& pt);

   /*!
    * Will invalidate all tiles that fall within the defined rect
    */
   void flush(const ossimIrect& rect);

   /*!
    * This will do a non destructive resize or translate
    * of the cache.
    */
   void setRect(const ossimIrect& newRect);

   /*!
    * Will return true or false if the tile rectangle underneath
    * the passed in point is valid.
    */
   bool isValid(const ossimIpt& pt)const;

   /*!
    * Will return true or false if the tile rectangle underneath
    * the passed in point is valid.
    */
   ossim_int32 getTileIndex(const ossimIpt& origin)const;
   ossim_int32 getTileIndex(ossim_int32 x,
                            ossim_int32 y)const;
   /*!
    * will return the tile rectangle at the point.
    */
   ossimIrect getTileRect(const ossimIpt& pt)const;

   /*!
    * Given the passed in point it will resize QImage and copy the tile.
    * Note the image object will have its offset X, Y set to be the upper left
    * coordinate of the tile being copied.
    */
   bool getTile(const ossimIpt& pt,
                   QImage& image)const;

   ossimIpt getTileOrigin(const ossimIpt& pt)const;
   /*!
    * Will use the offset method of the passed in image and
    * copy a sub rect of the cache out.  Note:  This will not resize
    * the image.  It keeps it the same size as the passed in image.
    */
   void getSubImage(QImage& image)const;
   
   /*!
    * sets the new tile size.  The current implementation
    * will invalidate the entire cache.
    */
   void setTileSize(const ossimIpt& tileSize);

   /*!
    * Returns the current tile size used in the cache.
    */
   const ossimIpt& getTileSize()const;

   /*!
    * The passed in image can cover several tiles but this image object
    * must be on a tile boundary in order for it to succeed.
    */
   bool addTile(QImage& image);

   /*!
    * Will use the clip rect to paint the tiles.  If one is not defined it will
    * check to see if there is a viewport rect and paint that rect.
    */
   void paintTiles(QPainter* p);

  static ossim_int32 computeTileId(const ossimIpt& origin,
				  const ossimIrect& tileBounderyRect,
				  const ossimIpt&   tileSize);
protected:
   QImage theCache;
   ossimIpt theTileSize;
   std::vector<bool> theValidTileArray;
   ossimIpt          theNumberOfTiles;

   ossim_int32 getTileIndex(const ossimIrect& rect,
                            const ossimIpt& numberOfTiles,
                            const ossimIpt& origin)const;
   
   ossim_int32 getTileIndex(const ossimIrect& rect,
                            const ossimIpt& numberOfTiles,
                            ossim_int32 x,
                            ossim_int32 y)const;


};

#endif
