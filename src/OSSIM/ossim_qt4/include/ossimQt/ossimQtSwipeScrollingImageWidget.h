//*******************************************************************
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimQtSwipeScrollingImageWidget.h 12013 2007-11-07 20:16:04Z gpotts $
#ifndef ossimQtSwipeScrollingImageWidget_HEADER
#define ossimQtSwipeScrollingImageWidget_HEADER
#include "ossimQtScrollingImageWidget.h"
class ossimQtMouseEvent;

class ossimQtSwipeScrollingImageWidget : public ossimQtScrollingImageWidget
{
   Q_OBJECT
   Q_ENUMS(ossimQtSwipeType)
   Q_PROPERTY( ossimQtSwipeType swipeType READ swipeType WRITE setSwipeType )
public:
   enum ossimQtSwipeType
      {
         ossimQtSwipeType_NONE = 0,
         ossimQtSwipeType_HORIZONTAL,
         ossimQtSwipeType_VERTICAL,
         ossimQtSwipeType_BOX,
         ossimQtSwipeType_CIRCLE
      };
   
   ossimQtSwipeScrollingImageWidget(QWidget* parent,
                                    QString  name);
   virtual ~ossimQtSwipeScrollingImageWidget();
   ossimQtSwipeType swipeType()const
      {
         return theSwipeType;
      }
  virtual void trackCursor(const ossimIpt& location);
   void setSwipeType(ossimQtSwipeType type);
   virtual void refresh(const ossimIpt& pt, bool repaintFlag=true);
   virtual void refresh(bool repaintFlag=true);
   virtual void flushCache(bool repaintFlag);
   virtual void disconnectInputEvent(ossimConnectionEvent& event);
  virtual void viewportResizeEvent(QResizeEvent* event);
   virtual void contentsMousePressEvent ( QMouseEvent * e );
   virtual void contentsMouseReleaseEvent ( QMouseEvent * e );
   virtual void contentsMouseDoubleClickEvent ( QMouseEvent * e );
   virtual void contentsMouseMoveEvent ( QMouseEvent * e );

protected:
   ossimRefPtr<ossimImageChain> theBottomLayerRgbChain;
   ossimQtStaticTileImageCache  theBottomLayerBackingStore;
   ossimQtSwipeType  theSwipeType;
   QImage    theTopLayer;
   QImage    theBottomLayer;
   QImage    theSwipeBuffer;
   ossimIpt  theCurrentMousePosition;
   ossimIpt  theCenterMousePosition;
   int       theLastRadius;
   
   virtual void setInputInformation();
   virtual void setInputInformation(const ossimIpt& pt);
   virtual void convertRequest(const ossimIrect& reqRect);
   virtual void drawContents(QPainter* p, int clipx, int clipy, int clipw, int cliph);

   virtual void eraseCursor(QPainter* p);

   virtual void performHorizontalSwipe(QPainter* p);//const ossimIpt& mousePoint);//ossimQtMouseEvent* evt);
   virtual void performVerticalSwipe(QPainter* p);//const ossimIpt& mousePoint);
   virtual void performBoxSwipe(QPainter* p);//const ossimIpt& mousePoint);
   virtual void performCircleSwipe(QPainter* p);
   virtual void swipeWidget(QPainter* p);	
protected slots:
   virtual void viewportMoving(int x, int y);
  
public slots:
   void swipeWidgetMouseEvent(ossimQtMouseEvent* evt);

};

#endif
