//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimQtScrollingImageWidget.cpp 15766 2009-10-20 12:37:09Z gpotts $
#include <ossimQt/ossimQtScrollingImageWidget.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimScalarRemapper.h>
#include <ossim/imaging/ossimCacheTileSource.h>
#include <ossim/imaging/ossimImageSourceSequencer.h>
#include <ossim/imaging/ossimU8ImageData.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimObjectDestructingEvent.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossimQt/ossimQtEvent.h>

#include <algorithm>
#include <QtCore/QDateTime>
#include <QtGui/QPainter>
#include <Qt3Support/Q3Painter>
#include <QtCore/QEvent>
#include <QtGui/QScrollBar>
#include <QtGui/QCursor>
#include <iostream>


static const int maxProcessingTime = 50;



ossimQtScrollingImageWidget::~ossimQtScrollingImageWidget ()
{
   removeListener((ossimConnectableObjectListener*)this);
   theRgbChain->disconnect();
   theRgbChain = 0;
    if(theTimerId != -1)
    {
       killTimer(theTimerId);
       theTimerId = -1;
    }
   ossimConnectableObject::disconnect();
   ossimReferenced::unref_nodelete();
}

ossimQtScrollingImageWidget::ossimQtScrollingImageWidget(QWidget* parent,
							 QString name)
   :
      Q3ScrollView(parent, name),
      theRandomPriorityQueueEnabledFlag(true)
{
   ossimReferenced::ref();
   
//     theBackingStoreRect.makeNan();
    theRgbChain              = new ossimImageChain;
    theRawPixelCache     = new ossimCacheTileSource;
    ossimScalarRemapper* eightBitRemapper  = new ossimScalarRemapper;
    
    theRgbChain->addChild(theRawPixelCache.get());
    theRgbChain->addChild(eightBitRemapper);
    theRgbChain->initialize();

    if(viewport())
      {
         viewport()->setMouseTracking(true);
      }
    setMouseTracking(true);
    theImageBufferRect.makeNan();
    theTimerId = -1;
    ossim::defaultTileSize(theTileSize);
    theImageBuffer.create(theTileSize.x, theTileSize.y, 32);
    theRgbQueryBuffer.create(theTileSize.x, theTileSize.y, 32);
    theRgbQueryBuffer.fill(0);
    theRgbQueryBufferRect.makeNan();
    theCurrentPriority = 0;
    viewport()->setEraseColor(QColor(0,0,0));
    setVScrollBarMode(Q3ScrollView::AlwaysOn);
    setHScrollBarMode(Q3ScrollView::AlwaysOn);
    viewport()->setCursor(Qt::CrossCursor);
    theBlankTile = new ossimU8ImageData(NULL,
                                        1,
                                        theTileSize.x,
                                        theTileSize.y);
    
    theBlankTile->initialize();
    theCursorTrackingFlag = true;
    //theCursorDrawnFlag = false;
    theCursorLocation.makeNan();
    theBackingStoreCache.setTileSize(theTileSize);
    addListener((ossimConnectableObjectListener*)this);
    connect(this, SIGNAL(contentsMoving(int,int)),
            this, SLOT(viewportMoving(int,int)));
   
}


ossimIrect ossimQtScrollingImageWidget::getBoundingRect()const
{
   return theBoundingRect;
}

ossimIrect ossimQtScrollingImageWidget::getSceneBoundingRect()const
{
   return theSceneBoundingRect;
}

ossimIrect ossimQtScrollingImageWidget::getAbsoluteViewportRect()
{
	ossimIrect result;

	getAbsoluteViewportRect(result);

	return result;
#if 0	
  // cout << "ossimQtScrollingImageWidget::getAbsoluteViewportRect() entered..."
  //      << endl;
    QSize framesize = viewport()->frameSize();
    ossimIrect result(contentsX(),
		    contentsY(),
		    contentsX() + (framesize.width() - 1),
		    contentsY() + (framesize.height() -1));

    if(theBoundingRect.hasNans())
    {
	result.makeNan();
    }
    else
    {
	result = result - theShiftToZeroZero;
    }
    
    return result;
#endif
}

ossimIrect ossimQtScrollingImageWidget::getViewportRect()
{
//   cout << "ossimQtScrollingImageWidget::getViewportRect() entered..."
//        << endl;
	ossimIrect result;
	getViewportRect(result);
#if 0   
    QSize framesize = viewport()->frameSize();
    ossimIrect result(contentsX(),
		      contentsY(),
		      contentsX() + (framesize.width() - 1),
		      contentsY() + (framesize.height() -1));
#endif
    if(theBoundingRect.hasNans())
    {
	result.makeNan();
    }
    
    return result;
}

void ossimQtScrollingImageWidget::getAbsoluteViewportRect(ossimIrect& result) const
{
   getViewportRect(result);

   if(result.hasNans() == false)
   {
      result = result - theShiftToZeroZero;
   }
	
 //  cout << "ossimQtScrollingImageWidget::getAbsoluteViewportRect:\n"
 //       << result << endl;
}

ossimIrect ossimQtScrollingImageWidget::getVisibleRect()
{
	return getAbsoluteVisibleRect() + theShiftToZeroZero;
}

ossimIrect ossimQtScrollingImageWidget::getAbsoluteVisibleRect()
{
	int cx = contentsX(), cy = contentsY(), vw = visibleWidth(), vh=visibleHeight();
	return ossimIrect(cx, cy, cx+(vw-1), cy+(vh-1));
}

void ossimQtScrollingImageWidget::getViewportRect(ossimIrect& result) const
{

   if(theBoundingRect.hasNans() == false)
   {
      contentsRect(result);
   }
   else
   {
      result.makeNan();
   }
   
  // cout << "ossimQtScrollingImageWidget::getViewportRect:\n"
  //      << result << endl;
}

void ossimQtScrollingImageWidget::contentsRect(ossimIrect& rect) const
{
#if 1	
	int x1=contentsX(), y1=contentsY(), x2=x1+(contentsWidth()-1), y2=y1+(contentsHeight()-1);
	int vx1=x1, vy1=y1, vx2=x2, vy2=y2;
	contentsToViewport(x1, y1, vx1, vy1);
	contentsToViewport(x2, y2, vx2, vy2);
	rect = ossimIrect(vx1, vy1, vx2, vy2);
#endif
#if 0
   if (viewport())
   {
      // QRect frameRect = viewport()->contentsRect();
      QRect frameRect = viewport()->geometry();      
      rect = ossimIrect(frameRect.left(), frameRect.top(),
                        frameRect.right(), frameRect.bottom());
   }
#endif
//   cout << "ossimQtScrollingImageWidget::contentsRect:\n"
//        << rect << endl;
}

bool ossimQtScrollingImageWidget::isProcessingTiles()const
{
  return (theTimerId >= 0);
}

bool ossimQtScrollingImageWidget::canConnectMyInputTo(ossim_int32 /*myInputIndex */,
						      const ossimConnectableObject* object)const
{
  return PTR_CAST(ossimImageSource, object);
}


void ossimQtScrollingImageWidget::repaint()
{
  //eraseCursor();
   repaintContents(false);
}

void ossimQtScrollingImageWidget::flushCache(bool repaintFlag)
{
//  eraseCursor();
  theBackingStoreCache.flush();
  if(repaintFlag)
  {
     repaint();
  }
}

void ossimQtScrollingImageWidget::panTo(const ossimIpt& pt, bool fullRepaintFlag)
{
//  eraseCursor();
  ossimIpt local = pt + theShiftToZeroZero;

  center(local.x, local.y);
  if(fullRepaintFlag)
  {
     repaintContents(false);
  }
}

void ossimQtScrollingImageWidget::refresh(const ossimIpt& pt, bool repaintFlag)
{
 //  eraseCursor();
   flushCache(false);

   theRgbChain->connectMyInputTo(0, getInput());
   theRgbChain->initialize();
   if(theTimerId >= 0)
   {
      killTimer(theTimerId);
      theTimerId = -1;
   }
//   eraseCursor();
   theReqMap.clear();
   while(!theReqQueue.empty())
   {
      theReqQueue.pop();
   }
   setInputInformation(pt);
   
//    ossimIrect vrect = getViewportRect();
   
//    bool testFlag = ((theBoundingRect.width() < vrect.width()) ||
//                     (theBoundingRect.height() < vrect.height()));
   
//    if(testFlag)
//    {
   // cout << "getCache().fill(0)" << endl;
   theBackingStoreCache.getCache().fill(0);
//   }
   
   //   viewport()->erase();
   if(repaintFlag)
   {
      repaintContents(false);
   }
}

void ossimQtScrollingImageWidget::refreshGraphics()
{
   repaintContents(false);
}

void ossimQtScrollingImageWidget::drawContents(QPainter* p, int clipx, int clipy, int clipw, int cliph)
{
   ossimIrect clipRect(clipx, clipy, clipx + (clipw - 1), clipy + (cliph - 1));
   ossimIrect clipTileRect = clipRect;
   clipTileRect.stretchToTileBoundary(theTileSize);
   ossimIpt ul = clipTileRect.ul();
   ossimIpt lr = clipTileRect.lr();
   ossimIpt shiftedOrigin = ul - theShiftToZeroZero;
   unsigned long totalNumberOfTiles = (clipTileRect.width()/theTileSize.x)*
      (clipTileRect.height()/theTileSize.y);
   ossim_int32 deltaPriority = totalNumberOfTiles;
   
   ossimIrect cacheRect =theBackingStoreCache.getCacheRect(); 
   
   p->drawImage(clipx,
		clipy,
		theBackingStoreCache.getCache(),
		clipx - cacheRect.ul().x,
		clipy - cacheRect.ul().y,
		clipw,
		cliph);
   
   for(int y = ul.y; y < lr.y; y+=theTileSize.y)
   {
      for(int x = ul.x; x < lr.x; x += theTileSize.x)
      {
         ossimIpt origin(x, y);
         shiftedOrigin = origin - theShiftToZeroZero;
         ossim_int32 idx = theBackingStoreCache.getTileIndex(origin);
         
         if( idx >= 0)
         {
            ossimIrect tileRect = ossimIrect(shiftedOrigin.x,
                                             shiftedOrigin.y,
                                             shiftedOrigin.x + (theTileSize.x-1),
                                             shiftedOrigin.y + (theTileSize.y-1));
            
            if(!theBackingStoreCache.isValid(origin))
            {
               ossim_int32 id = theBackingStoreCache.computeTileId(shiftedOrigin,
                                                                   theBoundingRect,
                                                                   theTileSize);
               if(id < 0)
               {
                  theImageBuffer.fill(0);
                  p->drawImage(origin.x, origin.y, theImageBuffer);
               }
               else if(theReqMap.find(id) == theReqMap.end())
               {
                  ossim_int32 tempPriority = theCurrentPriority;
                  if ( theRandomPriorityQueueEnabledFlag )
                  {
                     double t = rand()/(double)RAND_MAX;
                     tempPriority += (ossim_int32)(deltaPriority*t);
                  }
                  else
                  {
                     // Temporary fix to make request go top down.
                     tempPriority = -tempPriority;
                  }
                  theReqQueue.push(ossimQtScrollingImageWidgetReqQueueItem(tempPriority,
                                                                           id));
                  theReqMap.insert(make_pair(id, tileRect));
                  ++theCurrentPriority;
               }
               else
               {
                  theReqMap[id] = tileRect;
               }
            }
         }
         else
         {
            theImageBuffer.fill(0);
            p->drawImage(origin.x, origin.y, theImageBuffer);
         }
      }
   }
   
   // Signal sent to any slots that would like to paint something here.
   emit paintYourGraphics(p, clipx, clipy, clipw, cliph);
    
   if(((theReqMap.size() > 0)) && (theTimerId < 0))
   {
      theTimerId = startTimer(10);
   }
	if(theCursorTrackingFlag)
	{
//		eraseCursor(p);
		drawCursor(p);
	}
}

void ossimQtScrollingImageWidget::fillImage(ossimRefPtr<ossimImageData>& data,
					    QImage* tempImage)
{
   if(data.valid() && data->getBuf() && !tempImage->isNull())
   {
	ossim_uint8* buf[3];
	int numberOfBands = data->getNumberOfBands();
	int aWidth        = tempImage->size().width();
	int aHeight       = tempImage->size().height();
	int maxPixels     = aWidth*aHeight;
	int offset;
	
	if(numberOfBands >= 3)
	{
	    buf[0] = static_cast<ossim_uint8*>(data->getBuf(0));
	    buf[1] = static_cast<ossim_uint8*>(data->getBuf(1));
	    buf[2] = static_cast<ossim_uint8*>(data->getBuf(2));
	}
	else 
	{
	    buf[0] = static_cast<ossim_uint8*>(data->getBuf(0));
	    buf[1] = static_cast<ossim_uint8*>(data->getBuf(0));
	    buf[2] = static_cast<ossim_uint8*>(data->getBuf(0));
	}
	QRgb *bits = (QRgb*)tempImage->bits();
	if(tempImage->bits())
	{
	  for(offset = 0; offset < maxPixels;++offset)
	    {
	      *bits = qRgb(*buf[0], *buf[1], *buf[2]);
	      ++buf[0];++buf[1];++buf[2];
	      ++bits;
	    }
	}
	else
	{
	    tempImage->fill(0);
	}
    }
    else
    {
	tempImage->fill(0);
    }
}

void ossimQtScrollingImageWidget::setInputInformation()
{
   ossimIpt pt;
   pt.makeNan();
   
   setInputInformation(pt);
}

void ossimQtScrollingImageWidget::setInputInformation(const ossimIpt& pt)
{
   theBackingStoreCache.flush();
    ossimIrect cacheRect;
    cacheRect.makeNan();
    theBoundingRect = computeBoundingRect();
    theSceneBoundingRect = theBoundingRect;
    if(!theBoundingRect.hasNans())
    {
       theBoundingRect.stretchToTileBoundary(theTileSize);
    }

    theShiftToZeroZero = ossimIpt(0,0) - theBoundingRect.ul();

    ossimIpt aCenter = theBoundingRect.midPoint() + theShiftToZeroZero;
    if(!theBoundingRect.hasNans())
    {
       resizeContents(theBoundingRect.width(), theBoundingRect.height());
       if(pt.hasNans())
       {
          center(aCenter.x, aCenter.y);
       }
       else
       {
          ossimIpt newPoint = pt + theShiftToZeroZero;
          center(newPoint.x, newPoint.y);
       }
    }    
    else	
    {
	resizeContents(0, 0);
    }
}

void ossimQtScrollingImageWidget::timerEvent(QTimerEvent* /* evt */)
{
  QTime start = QTime::currentTime();
//  eraseCursor();
  viewport()->setCursor(Qt::WaitCursor);

    while(theReqMap.size() > 0)
    {
	ossimQtScrollingImageWidgetReqQueueItem item = theReqQueue.top();
	std::map<ossim_int32, ossimIrect>::iterator currentIter = theReqMap.find(item.theId);
	theReqQueue.pop();
	
	ossimIrect currentTileRect = (*currentIter).second;
	
	theReqMap.erase(currentIter);
	convertRequest(currentTileRect);
	QTime end = QTime::currentTime();
	if(start.msecsTo(end) >= maxProcessingTime)
	{
	    return;
	}
    }
    if(theReqMap.size() == 0)
    {
	while(!theReqQueue.empty())
	{
	    theReqQueue.pop();
	}
    }
    killTimer(theTimerId);
    theTimerId = -1;
    viewport()->setCursor(Qt::CrossCursor);
}


void ossimQtScrollingImageWidget::convertRequest(const ossimIrect& reqRect)
{
    if(theRgbChain->getInput())
    {
        QImage tempImage(theTileSize.x,
                         theTileSize.y,
                         32);
        tempImage.fill(0);
        ossimIrect shiftedCacheRect = theBackingStoreCache.getCacheRect() - theShiftToZeroZero;
	ossimIrect tempRect = getAbsoluteViewportRect();
	
	tempImage.setOffset(QPoint(theShiftToZeroZero.x + reqRect.ul().x,
				   theShiftToZeroZero.y + reqRect.ul().y));

	if(reqRect.intersects(shiftedCacheRect))
	{
           if(reqRect.intersects(theBoundingRect))
           {
              ossimRefPtr<ossimImageData> data = getTile(reqRect);
              
              if(data.valid() && data->getBuf() &&
                 (data->getDataObjectStatus()!=OSSIM_EMPTY))
              {
                 fillImage(data, &tempImage);
              }
           }
	}
	else
        {
           return;
        }
	theBackingStoreCache.addTile(tempImage);
        ossimIrect viewClip = reqRect.clipToRect(tempRect);
        viewClip  = viewClip + theShiftToZeroZero;
        int xLoc  = viewClip.ul().x;
        int yLoc  = viewClip.ul().y;
        int xSize = theTileSize.x;
        int ySize = theTileSize.y;
//        eraseCursor();
        repaintContents(xLoc, yLoc, xSize, ySize, false);
    }
}


void ossimQtScrollingImageWidget::drawCursor()
{
   if((!theCursorLocation.hasNans())&&
      (!isProcessingTiles()))
   {
     Q3Painter painter;
	 //(viewport());
     if(painter.begin(viewport()))
	 {
		drawCursor(&painter);
//		theCursorDrawnFlag = true;
	 }
	 painter.end();
   }
}

void ossimQtScrollingImageWidget::drawCursor(QPainter* painter)
{
   if(//(!theCursorDrawnFlag)&&
      (!theCursorLocation.hasNans())&&
      (!isProcessingTiles()))
   {
     bool hasClipping = painter->hasClipping();
     painter->setClipping(false);
     painter->setPen(QColor(255, 255, 255));
     ossimIrect rect = getViewportRect();
//	 std::cout << "VIEWPORT RECT = " << rect << std::endl;
     ossimIpt ul = rect.ul();
     ossimIpt lr = rect.lr();
     int left   = rect.ul().x;
     int right  = rect.lr().x;
     int top    = rect.ul().y;
     int bottom = rect.lr().y;
     
     int shiftX = ul.x;
     int shiftY = ul.y;
     int vx1, vy1,vx2, vy2;
     if((theCursorLocation.y >= ul.y)&&
	(theCursorLocation.y <= lr.y))
       {
		   vx1 = left-shiftX;
		   vx2 = right-shiftX;
		   vy1 = theCursorLocation.y-shiftY;
		   vy2 = vy1;
#if 0
		   contentsToViewport(left-shiftX, theCursorLocation.y-shiftY, vx1, vy1);
		   contentsToViewport(right-shiftX, theCursorLocation.y-shiftY, vx2, vy2);
#endif
	 // draw horizontal
	 //
	 painter->drawLine(vx1, vy1,
			  vx2, vy2);
       }
     
     if((theCursorLocation.x >= ul.x)&&
	(theCursorLocation.x <= lr.x))
       {
		   vx1 = theCursorLocation.x-shiftX;
		   vx2 = vx1;
		   vy1 = top-shiftY;
		   vy2 = bottom-shiftY;
#if 0
		   contentsToViewport(theCursorLocation.x-shiftX, top-shiftY, vx1, vy1);
		   contentsToViewport(theCursorLocation.x-shiftX, bottom-shiftY, vx2, vy2);
#endif
	       painter->drawLine(vx1, vy1, vx2, vy2);
       }
     painter->setClipping(hasClipping);
   }
}

void ossimQtScrollingImageWidget::eraseCursor()
{
  if(//theCursorDrawnFlag&&
     (!theCursorLocation.hasNans()))
    {
      Q3Painter painter;//(viewport());
	  if(painter.begin(viewport()))
	  {
		eraseCursor(&painter);
	  }
	  else
	  {
	  }
	  painter.end();
    }
}

void ossimQtScrollingImageWidget::eraseCursor(QPainter* painter)
{
  if(//theCursorDrawnFlag&&
     (!theCursorLocation.hasNans()))
  {
     QImage& cacheImage = theBackingStoreCache.getCache();
     ossimIrect cacheRect = theBackingStoreCache.getCacheRect();
     ossimIrect viewportRect = getViewportRect();
      ossimIpt ul = viewportRect.ul();
      ossimIpt lr = viewportRect.lr();
     
      if((theCursorLocation.y >= ul.y)&&
         (theCursorLocation.y <= lr.y))
      {
	 // erase horizontal line
         
         painter->drawImage(0,
                            theCursorLocation.y-ul.y,
                            cacheImage,
                            viewportRect.ul().x - cacheRect.ul().x,
                            theCursorLocation.y - cacheRect.ul().y,
                            (int)viewportRect.width(),
                            (int)1);
      }
      
      if((theCursorLocation.x >= ul.x)&&
         (theCursorLocation.x <= lr.x))
      {
	 // erase vertical line

         painter->drawImage(theCursorLocation.x-ul.x,
                             0,
                            cacheImage,
                            theCursorLocation.x - cacheRect.ul().x,
                            viewportRect.ul().y - cacheRect.ul().y,
                            (int)1,
                            (int)viewportRect.height());
      }
      //theCursorDrawnFlag = false;
   }
}


void ossimQtScrollingImageWidget::setTrackingFlag(bool flag)
{
   if(flag != theCursorTrackingFlag)
   {
		repaint();
//      if(theCursorTrackingFlag&&theCursorDrawnFlag)
//      {
//         eraseCursor();
//      }
   }
   theCursorTrackingFlag = flag;
}

void ossimQtScrollingImageWidget::trackCursor(const ossimIpt& location)
{
   if(theCursorTrackingFlag)
   {
      theCursorLocation = location + theShiftToZeroZero;
	   repaint();
      //eraseCursor();
      
//	  std::cout << "<"<<theCursorLocation.x <<","<<theCursorLocation.y << std::endl;
      //drawCursor();
   }
}


void ossimQtScrollingImageWidget::getRgb(const ossimIpt& location,
					 ossim_uint8& r,
					 ossim_uint8& g,
					 ossim_uint8& b)
{
  r = 0;
  g = 0;
  b = 0;

  ossimIrect cacheRect = theBackingStoreCache.getCacheRect();

  cacheRect = cacheRect - theShiftToZeroZero;

  if(cacheRect.pointWithin(location))
  {
     ossimIpt delta = (location - cacheRect.ul());
     QColor pixel(theBackingStoreCache.getCache().pixel(delta.x, delta.y));
     r = (ossim_uint8)pixel.red();
     g = (ossim_uint8)pixel.green();
     b = (ossim_uint8)pixel.blue();
  }
}


void ossimQtScrollingImageWidget::getRaw(const ossimIpt& location,
                                         std::vector<ossim_float64>& values)
{
  ossimIrect cacheRect = theBackingStoreCache.getCacheRect();

  cacheRect = cacheRect - theShiftToZeroZero;

  values.clear();
  if(cacheRect.pointWithin(location))
  {
     ossimIrect requestRect(location.x,
                            location.y,
                            location.x,
                            location.y);
     
     requestRect.stretchToTileBoundary(theTileSize);
     
     ossimRefPtr<ossimImageData> data = theRawPixelCache->getTile(requestRect);
     
     if(data.valid()&&
        data->getBuf())
     {
        values.resize(data->getNumberOfBands());
        ossim_uint32 ux, uy;
        ossim_uint32 bands = data->getNumberOfBands();
        ux = data->getImageRectangle().ul().x;
        uy = data->getImageRectangle().ul().y;
        
        ossim_uint32 offset = ((location.y-uy)*data->getWidth() +
                               (location.x-ux));
        
        ossim_uint32 bidx = 0;
        for(bidx = 0; bidx < bands; ++bidx)
        {
           values[bidx] = data->getPix(offset, bidx);
        }
     }
  }
}

bool ossimQtScrollingImageWidget::event ( QEvent * e )
{
//   switch(e->type())
//     {
//     case QEvent::Enter:
//       {
// 	eraseCursor();
// 	theCursorTrackingFlag = false;
// 	break;
//       }
//     case QEvent::Leave:
//       {
// 	theRgbQueryBufferRect.makeNan();
// 	theRgbQueryBuffer.fill(0);
// 	theCursorTrackingFlag = true;
// 	drawCursor();
// 	break;
//       }
//     default:
//       {
// 	break;
//       }
//     }

   return Q3ScrollView::event(e);
}

ossimIrect ossimQtScrollingImageWidget::computeBoundingRect()const
{
   ossimIrect result;

   result.makeNan();
   if(getInput())
   {
      for(int i = 0; i < (int)getNumberOfInputs(); ++i)
      {
         const ossimImageSource* it = PTR_CAST(ossimImageSource, getInput(i));
         if(it)
         {
            if(result.hasNans())
            {
               result = it->getBoundingRect();
            }
            else
            {
               ossimIrect tempRect = it->getBoundingRect();
               if(!tempRect.hasNans())
               {
                  result =  result.combine(tempRect);
               }
            }
         }
      }
   }
   
   return result;
}

ossimRefPtr<ossimImageData> ossimQtScrollingImageWidget::getTile(
   const ossimIrect& rect, ossim_uint32 resLevel)
{
   return theRgbChain->getTile(rect, resLevel);
}

void ossimQtScrollingImageWidget::contentsMousePressEvent(QMouseEvent * e )
{
//   eraseCursor();

   ossimQtMouseEvent evt(theShiftToZeroZero, e);
   
   emit scrollingImageWidgetMouseEvent(&evt);
}

void ossimQtScrollingImageWidget::contentsMouseReleaseEvent(QMouseEvent * e )
{
//  eraseCursor();

   ossimQtMouseEvent evt(theShiftToZeroZero, e);
   
   emit scrollingImageWidgetMouseEvent(&evt);
}

void ossimQtScrollingImageWidget::contentsMouseDoubleClickEvent(QMouseEvent * e )
{
//  eraseCursor();
  theCursorTrackingFlag = false;
  ossimQtMouseEvent evt(theShiftToZeroZero, e);
  emit scrollingImageWidgetMouseEvent(&evt);
}

void ossimQtScrollingImageWidget::contentsMouseMoveEvent(QMouseEvent * e )
{
//   eraseCursor();

   ossimQtMouseEvent evt(theShiftToZeroZero, e);

   emit scrollingImageWidgetMouseEvent(&evt);
}

void ossimQtScrollingImageWidget::disconnectInputEvent(ossimConnectionEvent& /*event*/)
{
   theRgbChain->disconnect();
}

void ossimQtScrollingImageWidget::viewportMoving(int x, int y)
{
   QSize framesize = viewport()->frameSize();

   theBackingStoreCache.setRect(ossimIrect(x, 
                                           y, 
                                           x + framesize.width()-1, 
                                           y + framesize.height()-1));
}

void ossimQtScrollingImageWidget::viewportResizeEvent(QResizeEvent* event)
{
   int newW = event->size().width();
   int newH = event->size().height();
   if(newW < 1)
   {
      newW = 1;
   }
   if(newH < 1)
   {
      newH = 1;
   }
   
   ossimIrect newBounds(contentsX()-(2*theTileSize.x),
                        contentsY()-(2*theTileSize.y),
                        contentsX() + (newW+4*theTileSize.x)-1,
                        contentsY() + (newH+4*theTileSize.y)-1);
   
   theBackingStoreCache.setRect(newBounds);
//   eraseCursor();
   Q3ScrollView::viewportResizeEvent(event);
}

bool ossimQtScrollingImageWidget::getRandomPriorityQueueEnabledFlag() const
{
   return theRandomPriorityQueueEnabledFlag;
}

void ossimQtScrollingImageWidget::setRandomPriorityQueueEnabledFlag(bool flag)
{
   theRandomPriorityQueueEnabledFlag = flag;
}
