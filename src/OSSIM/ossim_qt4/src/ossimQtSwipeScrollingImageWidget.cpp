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
// $Id: ossimQtSwipeScrollingImageWidget.cpp 15766 2009-10-20 12:37:09Z gpotts $
#include <QtGui/QPainter>
#include <QtGui/QPixmap>
#include <QtGui/QImage>
#include <ossimQt/ossimQtSwipeScrollingImageWidget.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimScalarRemapper.h>
#include <ossim/imaging/ossimCacheTileSource.h>
#include <ossim/imaging/ossimImageSourceSequencer.h>
#include <ossimQt/ossimQtEvent.h>
#include <QtGui/QMessageBox>

ossimQtSwipeScrollingImageWidget::ossimQtSwipeScrollingImageWidget(QWidget* parent,
                                                                   QString  name)
   :ossimQtScrollingImageWidget(parent, name),
    theSwipeType(ossimQtSwipeType_NONE)
{
   connect(this, SIGNAL(scrollingImageWidgetMouseEvent(ossimQtMouseEvent*)),
           this, SLOT(swipeWidgetMouseEvent(ossimQtMouseEvent*)));
   theBottomLayerRgbChain = new ossimImageChain;
   theBottomLayerRgbChain->addChild(new ossimScalarRemapper);
   theBottomLayerBackingStore.setTileSize(theTileSize);
   theLastRadius = -1;
   theCurrentMousePosition.makeNan();
   theCenterMousePosition.makeNan();
}

ossimQtSwipeScrollingImageWidget::~ossimQtSwipeScrollingImageWidget ()
{
   theBottomLayerRgbChain->disconnect();
   theTopLayer.reset();
   theBottomLayer.reset();
}


void ossimQtSwipeScrollingImageWidget::setSwipeType(ossimQtSwipeType type)
{
   theSwipeType  = type;
   theLastRadius = -1;
   theCurrentMousePosition.makeNan();
   theCenterMousePosition.makeNan();
}

void ossimQtSwipeScrollingImageWidget::setInputInformation()
{
   theBottomLayerRgbChain->disconnect();
   
   theBottomLayerBackingStore.flush();
   theBottomLayerRgbChain->connectMyInputTo(0, getInput(1));
   theBottomLayerRgbChain->initialize();
   ossimQtScrollingImageWidget::setInputInformation();
   theLastRadius = -1;
   theCurrentMousePosition.makeNan();
   theCenterMousePosition.makeNan();
}

void ossimQtSwipeScrollingImageWidget::setInputInformation(const ossimIpt& pt)
{
   theBottomLayerRgbChain->disconnect();
   theBottomLayerBackingStore.flush();
   theTopLayer.reset();
   theBottomLayer.reset();
   theBottomLayerRgbChain->connectMyInputTo(0, getInput(1));
   theBottomLayerRgbChain->initialize();
   ossimQtScrollingImageWidget::setInputInformation(pt);
   theLastRadius = -1;
   theCurrentMousePosition.makeNan();
   theCenterMousePosition.makeNan();
}

void ossimQtSwipeScrollingImageWidget::flushCache(bool repaintFlag)
{
   theBottomLayerBackingStore.flush();
   theTopLayer.reset();
   theBottomLayer.reset();
   ossimQtScrollingImageWidget::flushCache(repaintFlag);
   theLastRadius = -1;
   theCurrentMousePosition.makeNan();
   theCenterMousePosition.makeNan();
}

void ossimQtSwipeScrollingImageWidget::refresh(const ossimIpt& pt,
                                               bool repaintFlag)
{
   theBottomLayerBackingStore.flush();
   theTopLayer.reset();
   theBottomLayer.reset();
   theLastRadius = -1;
   theCurrentMousePosition.makeNan();
   theCenterMousePosition.makeNan();
   
   ossimQtScrollingImageWidget::refresh(pt, repaintFlag); 
}

void ossimQtSwipeScrollingImageWidget::refresh(bool repaintFlag)
{
   theBottomLayerBackingStore.flush();
   theTopLayer.reset();
   theBottomLayer.reset();
   theLastRadius = -1;
   theCurrentMousePosition.makeNan();
   theCenterMousePosition.makeNan();
   
   ossimQtScrollingImageWidget::refresh(repaintFlag); 
}


void ossimQtSwipeScrollingImageWidget::convertRequest(const ossimIrect& reqRect)
{
   if((theBottomLayerRgbChain->getInput())&&
      (theSwipeType != ossimQtSwipeType_NONE))
   {
      ossimRefPtr<ossimImageData> data = theBottomLayerRgbChain->getTile(reqRect);
      
      QImage tempImage(theTileSize.x,
		       theTileSize.y,
		       32);
      if(data.valid() && data->getBuf())
        {
	  fillImage(data, &tempImage);
        }
        tempImage.setOffset(QPoint(theShiftToZeroZero.x + reqRect.ul().x,
                                   theShiftToZeroZero.y + reqRect.ul().y));
	
        theBottomLayerBackingStore.addTile(tempImage);
   }
   if(!theTopLayer.isNull())
    {
       theTopLayer.reset();
       theBottomLayer.reset();
    }
    ossimQtScrollingImageWidget::convertRequest(reqRect);
}

void ossimQtSwipeScrollingImageWidget::drawContents(QPainter* p, int clipx, int clipy, int clipw, int cliph)
{
   ossimQtScrollingImageWidget::drawContents(p,
                                             clipx,
                                             clipy,
                                             clipw,
                                             cliph);

   if(getInput(0)&&getInput(1))//&&!isProcessingTiles())
   {
     ossimIrect vRect = theBackingStoreCache.getCacheRect();
      swipeWidget(p);
   }
}


void ossimQtSwipeScrollingImageWidget::trackCursor(const ossimIpt& location)
{
	ossimQtScrollingImageWidget::trackCursor(location);
}

void ossimQtSwipeScrollingImageWidget::eraseCursor(QPainter* p)
{
#if 1
   if(!getInput(0)||!getInput(1))
   {
      ossimQtScrollingImageWidget::eraseCursor(p);
      return;
   }
   
   if( //theCursorDrawnFlag && 
	   (!theCurrentMousePosition.hasNans()) )
   {
      QImage& topCacheImage       = theBackingStoreCache.getCache();
      QImage& bottomCacheImage    = theBottomLayerBackingStore.getCache();
      ossimIrect topCacheRect     = theBackingStoreCache.getCacheRect();
      ossimIrect bottomCacheRect  = theBottomLayerBackingStore.getCacheRect();
      
      ossimIpt mousePosition = theCursorLocation;
      ossimIpt swipePosition = theCurrentMousePosition + theShiftToZeroZero;
      ossimIrect viewportRect = getViewportRect();
      ossimIpt ul = viewportRect.ul();
      ossimIpt lr = viewportRect.lr();
      
      // erase horizontal line
      
      switch(theSwipeType)
      {
         case ossimQtSwipeType_HORIZONTAL:
         {
            // erase vertical line
            if(mousePosition.x < swipePosition.x)
            {
               p->drawImage(mousePosition.x - ul.x,
                            0,
                            topCacheImage,
                            mousePosition.x     - topCacheRect.ul().x,
                            viewportRect.ul().y - topCacheRect.ul().y,
                            1,
                            viewportRect.height());
            }
            else
            {
               p->drawImage(mousePosition.x - ul.x,
                            0,
                            bottomCacheImage,
                            mousePosition.x     - bottomCacheRect.ul().x,
                            viewportRect.ul().y - bottomCacheRect.ul().y,
                            1,
                            viewportRect.height());
            }
            
            // erase horizontal
            
            if((mousePosition.y >= ul.y)&&
               (mousePosition.y <= lr.y))
            {
               p->drawImage(0,
                            mousePosition.y-ul.y,
                            topCacheImage,
                            ul.x - topCacheRect.ul().x,
                            mousePosition.y - topCacheRect.ul().y,
                            swipePosition.x - ul.x,
                            (int)1);
               
               p->drawImage(swipePosition.x - ul.x,
                            mousePosition.y - ul.y,
                            bottomCacheImage,
                            swipePosition.x - bottomCacheRect.ul().x,
                            mousePosition.y - bottomCacheRect.ul().y,
                            (int)swipePosition.x - (int)lr.x,
                            (int)1);
            }
            
            break;
         }
         case ossimQtSwipeType_VERTICAL:
         {
            // erase vertical line
            if((mousePosition.x >= ul.x)&&
               (mousePosition.x <= lr.x))
            {
               
               p->drawImage(mousePosition.x-ul.x,
                            0,
                            topCacheImage,
                            mousePosition.x - topCacheRect.ul().x,
                            viewportRect.ul().y - topCacheRect.ul().y,
                            (int)1,
                            swipePosition.y - ul.y);
               
               p->drawImage(mousePosition.x - ul.x,
                            swipePosition.y - ul.y,
                            bottomCacheImage,
                            mousePosition.x - bottomCacheRect.ul().x,
                            swipePosition.y - bottomCacheRect.ul().y,
                            (int)1,
                            (int)swipePosition.y - (int)lr.y);
            }
            
            // erase horizontal
            if(mousePosition.y < swipePosition.y)
            {
               // use top layer to erase mouse  
               p->drawImage(0,
                            mousePosition.y-ul.y,
                            topCacheImage,
                            ul.x - topCacheRect.ul().x,
                            mousePosition.y - topCacheRect.ul().y,
                            (int)viewportRect.width(),
                            (int)1);
            }
            else
            {
               // use bottom layer to erase
               p->drawImage(0,
                            mousePosition.y-ul.y,
                            bottomCacheImage,
                            ul.x - bottomCacheRect.ul().x,
                            mousePosition.y - bottomCacheRect.ul().y,
                            (int)viewportRect.width(),
                            (int)1);
               
            }
            break;
         }

         default:
            break;

      } // End of "switch(theSwipeType)"
      
//      theCursorDrawnFlag = false;
   }
#endif
}

void ossimQtSwipeScrollingImageWidget::swipeWidget(QPainter* p)//const ossimIpt& mousePoint)
{
   switch(theSwipeType)
   {
   case ossimQtSwipeType_HORIZONTAL:
   {
      performHorizontalSwipe(p);
      break;
   }
   case ossimQtSwipeType_VERTICAL:
   {
      performVerticalSwipe(p);
      break;
   }
   case ossimQtSwipeType_BOX:
   {
      performBoxSwipe(p);
      break;
   }
   case ossimQtSwipeType_CIRCLE:
   {
     performCircleSwipe(p);
      break;
   }
   default:
   {
      break;
   }
   }
}

void ossimQtSwipeScrollingImageWidget::swipeWidgetMouseEvent(ossimQtMouseEvent* evt)
{
   if((theSwipeType == ossimQtSwipeType_NONE)||
      (!getInput(1)))
   {
      return;
   }
  ossimIpt       mousePoint = evt->getPoint();
  theCurrentMousePosition = mousePoint;
  if(theCenterMousePosition.hasNans())
  {
	theCenterMousePosition = theCurrentMousePosition;
  }
}
void ossimQtSwipeScrollingImageWidget::contentsMousePressEvent(QMouseEvent * e )
{
	theCenterMousePosition.makeNan();
	ossimQtScrollingImageWidget::contentsMousePressEvent(e);
}

void ossimQtSwipeScrollingImageWidget::contentsMouseReleaseEvent(QMouseEvent * e )
{
	ossimQtScrollingImageWidget::contentsMouseReleaseEvent(e);
}

void ossimQtSwipeScrollingImageWidget::contentsMouseDoubleClickEvent(QMouseEvent * e )
{
	ossimQtScrollingImageWidget::contentsMouseDoubleClickEvent(e);
}

void ossimQtSwipeScrollingImageWidget::contentsMouseMoveEvent ( QMouseEvent * e )
{
	ossimQtScrollingImageWidget::contentsMouseMoveEvent(e);
}

void ossimQtSwipeScrollingImageWidget::performHorizontalSwipe(QPainter* p)
{
   if(theCurrentMousePosition.hasNans()) return;
   ossimIrect vRect = theBackingStoreCache.getCacheRect();
   int leftWidth = theCurrentMousePosition.x - vRect.ul().x;
   if(leftWidth < 0) leftWidth = 0;
   if(leftWidth > (int)vRect.width()) leftWidth = vRect.width();
   int rightWidth = vRect.width() - leftWidth;
   int offsetx=vRect.ul().x, offsety=vRect.ul().y;
      
   p->drawImage(offsetx+leftWidth, offsety,
                theBottomLayerBackingStore.getCache(),
                leftWidth, 0, rightWidth);
}

void ossimQtSwipeScrollingImageWidget::performVerticalSwipe(QPainter* p)//const ossimIpt& mousePoint)//ossimQtMouseEvent* evt)
{
   if(theCurrentMousePosition.hasNans()) return;
    ossimIrect vRect = theBackingStoreCache.getCacheRect();

   int offsetx=vRect.ul().x, offsety=vRect.ul().y;
   int topHeight = theCurrentMousePosition.y - vRect.ul().y;
   if(topHeight < 0) topHeight = 0;
   if(topHeight > (int)vRect.height()) topHeight = vRect.height();
   int bottomHeight = vRect.height() - topHeight;
   
   p->drawImage(offsetx, offsety + topHeight,
                theBottomLayerBackingStore.getCache(),
                0, topHeight, -1, bottomHeight);
}

void ossimQtSwipeScrollingImageWidget::performBoxSwipe(QPainter* p)//const ossimIpt& mousePoint)//ossimQtMouseEvent* evt)
{
   if(theCenterMousePosition.hasNans()) return;
   ossimIrect vRect = theBackingStoreCache.getCacheRect();
   //int offsetx=vRect.ul().x, offsety=vRect.ul().y;
   ossimIpt deltaPt = theCurrentMousePosition-theCenterMousePosition;
   deltaPt.x = std::abs(deltaPt.x);
   deltaPt.y = std::abs(deltaPt.y);
   ossimIrect subRectToDraw(theCenterMousePosition.x - deltaPt.x,
	                        theCenterMousePosition.y - deltaPt.y,
                            theCenterMousePosition.x + deltaPt.x,
	                        theCenterMousePosition.y + deltaPt.y);
   ossimIrect clipRect = subRectToDraw.clipToRect(vRect);
   ossimIpt originDelta = clipRect.ul() - vRect.ul();
	p->drawImage(clipRect.ul().x, clipRect.ul().y,
				 theBottomLayerBackingStore.getCache(),
				 originDelta.x,
				 originDelta.y,
				 clipRect.width(),
				 clipRect.height());
}

void ossimQtSwipeScrollingImageWidget::performCircleSwipe(QPainter* /*p*/)
{
   ossimIrect vRect = theBackingStoreCache.getCacheRect();
}

void ossimQtSwipeScrollingImageWidget::disconnectInputEvent(ossimConnectionEvent& event)
{
   ossimQtScrollingImageWidget::disconnectInputEvent(event);
   theRgbChain->disconnect();
}

void ossimQtSwipeScrollingImageWidget::viewportMoving(int x, int y)
{
  ossimQtScrollingImageWidget::viewportMoving(x, y);

  QSize framesize = viewport()->frameSize();

  theBottomLayerBackingStore.setRect(ossimIrect(x, 
						y, 
						x + framesize.width()-1, 
						y + framesize.height()-1));
}

void ossimQtSwipeScrollingImageWidget::viewportResizeEvent(QResizeEvent* event)
{
  ossimQtScrollingImageWidget::viewportResizeEvent(event);
  
  theBottomLayerBackingStore.setRect(theBackingStoreCache.getCacheRect());
}
