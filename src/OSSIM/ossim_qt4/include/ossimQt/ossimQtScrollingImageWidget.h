//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimQtScrollingImageWidget.h 12108 2007-12-04 17:42:05Z gpotts $
#ifndef ossimQtScrollingImageWidget_HEADER
#define ossimQtScrollingImageWidget_HEADER

#include <queue>

#include <QtGui/QMouseEvent>
#include <Qt3Support/Q3ScrollView>

#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimPropertyEvent.h>
#include <ossim/base/ossimConnectableObject.h>
#include <ossim/imaging/ossimFixedTileCache.h>
#include "ossimQtStaticTileImageCache.h"
#include "ossimQtExport.h"

class QScrollBar;
class QResizeEvent;
class QPaintEvent;
class ossimImageChain;
class ossimImageData;
class ossimScalarRemapper;
class ossimCacheTileSource;
class ossimNativeBandSelectorTileSource;
class ossimImageSource;
class ossimImageSourceSequencer;
class ossimQtMouseEvent;
class ossimAnnotationSource;

class OSSIMQT_DLL ossimQtScrollingImageWidgetReqQueueItem
{
public:
    ossimQtScrollingImageWidgetReqQueueItem(ossim_int32 priority=0,
					    ossim_int32 id=-1)
    :thePriority(priority),
    theId(id)
    {
    }
    bool operator <(const ossimQtScrollingImageWidgetReqQueueItem& rhs)const
    {
	return  (thePriority < rhs.thePriority);
    }
    bool operator >(const ossimQtScrollingImageWidgetReqQueueItem& rhs)const
    {
	return  (thePriority > rhs.thePriority);
    }
    bool operator ==(const ossimQtScrollingImageWidgetReqQueueItem& rhs)const
    {
	return  (thePriority == rhs.thePriority);
    }
    
    ossim_int32 thePriority;
    ossim_int32 theId;
};

class OSSIMQT_DLL ossimQtScrollingImageWidget : public Q3ScrollView, 
                                    public ossimConnectableObject,
                                    public ossimConnectableObjectListener
{
    Q_OBJECT
  
public:
    ossimQtScrollingImageWidget(QWidget* parent, QString name);
    virtual ~ossimQtScrollingImageWidget ();

   void repaint();
   void panTo(const ossimIpt& pt, bool fullRepaintFlag=false);
   virtual void refresh(const ossimIpt& pt, bool repaintFlag=true);
   virtual void refresh(bool repaintFlag=true)
      {
         ossimIpt newCenter;
         newCenter.makeNan();
         refresh(newCenter, repaintFlag);
      }

   /** Repaints graphics onto the widget. */
   void refreshGraphics();
   
   virtual bool canConnectMyInputTo(ossim_int32 myInputIndex,
                                    const ossimConnectableObject* object)const;
//  virtual void propertyEvent(ossimPropertyEvent& event);

   /**
    * @brief Get the scene bounding rectangle.
    *
    * This is the fitted bounding rectangle of the input connection(s) NOT
    * stretched to tile boundaries.
    *
    * @return The scene bounding rectangle.
    */
  ossimIrect getSceneBoundingRect()const;

  /**
   * @brief Gets the bounding rectangle.
   * 
   * This is the scene rectectangle stretched to tile boundaries; hence,
   * could have boundary nulls depending upon the scene.
   *
   * @return The bounding rectangle.
   */
  ossimIrect getBoundingRect()const;

  /*!
   * Note:  the viewport rect in QT has origin of 0,0
   *        since the input may not necesarily be
   *        at 0,0 then this shifts the viewport rect
   *        correctly to a non 0,0 origin.  Basically I look at
   *        the bounding rect and see if it's upper left is at 0,0
   *        
   *        Use getViewportRect() if you don't want the
   *        shift to be incorporated
   */
  ossimIrect getAbsoluteViewportRect();
  ossimIrect getViewportRect();
	
  ossimIrect getVisibleRect();
  ossimIrect getAbsoluteVisibleRect(); 
  void getAbsoluteViewportRect(ossimIrect& result) const;
  void getViewportRect(ossimIrect& result) const;
  
  virtual void flushCache(bool repaintFlag);
  virtual bool isProcessingTiles()const;
   
  virtual void viewportResizeEvent(QResizeEvent* event);
  virtual void timerEvent(QTimerEvent* event);
  ossimIpt getShiftToViewport()const
  {
    return theShiftToZeroZero;
  }
  virtual void setTrackingFlag(bool flag);
  virtual bool getTrackingFlag()const
  {
    return theCursorTrackingFlag;
  }
  virtual void trackCursor(const ossimIpt& location);
  virtual void getRgb(const ossimIpt& location,
		      ossim_uint8& r,
		      ossim_uint8& g,
		      ossim_uint8& b);
  void getRaw(const ossimIpt& location,
              std::vector<ossim_float64>& values);

  virtual void disconnectInputEvent(ossimConnectionEvent& event);

  /**
   * This flag controls whether or not random caching is enabled.
   * 
   * @return true if random queing is enabled; else, false.
   */
  virtual bool getRandomPriorityQueueEnabledFlag() const;

  /**
   * This flag controls whether or not random caching is enabled.
   * 
   * @param flag Sets theRandomPriorityQueueEnabledFlag.  If true the priority
   * queue will be enabled for random mode.
   */
  virtual void setRandomPriorityQueueEnabledFlag(bool flag);

   /** Gets widget()->contentsRect() as an ossimIrect. */
   void contentsRect(ossimIrect& rect) const;

protected:
    ossimIrect                  theBoundingRect;
    ossimIrect                  theSceneBoundingRect;
    ossimIpt                    theShiftToZeroZero;
    ossimRefPtr<ossimImageData> theBlankTile;
    ossimRefPtr<ossimImageChain> theRgbChain;
    ossimRefPtr<ossimCacheTileSource> theRawPixelCache;
    ossimIpt                    theMousePosition;
    QImage                      theImageBuffer;
    ossimIrect                  theImageBufferRect;
    int                         theTimerId;
    ossimIpt                    theTileSize;
    ossim_int32                 theCurrentPriority;
    bool                        theCursorTrackingFlag;
//    bool                        theCursorDrawnFlag;
    ossimIpt                    theCursorLocation;
    mutable QImage              theRgbQueryBuffer;
    ossimIrect                  theRgbQueryBufferRect;
    ossimQtStaticTileImageCache theBackingStoreCache;
   
    std::map<ossim_int32, ossimIrect>     theReqMap;
    std::priority_queue<ossimQtScrollingImageWidgetReqQueueItem> theReqQueue;
    bool theRandomPriorityQueueEnabledFlag;
    
   
   virtual void setInputInformation();
   virtual void setInputInformation(const ossimIpt& pt);
   virtual void fillImage(ossimRefPtr<ossimImageData>& data,
                          QImage* tempImage);
   virtual void drawContents(QPainter* p, int clipx, int clipy, int clipw, int cliph);

   void drawCursor();
   virtual void drawCursor(QPainter* p);
   virtual void eraseCursor();
   virtual void eraseCursor(QPainter* p);

   virtual bool event ( QEvent * e );

   virtual void contentsMousePressEvent ( QMouseEvent * e );
   virtual void contentsMouseReleaseEvent ( QMouseEvent * e );
   virtual void contentsMouseDoubleClickEvent ( QMouseEvent * e );
   virtual void contentsMouseMoveEvent ( QMouseEvent * e );

   virtual void convertRequest(const ossimIrect& reqRect);
   virtual ossimIrect computeBoundingRect()const;
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                               ossim_uint32 resLevel=0);


protected slots:
   virtual void viewportMoving(int x, int y);

signals:
   void scrollingImageWidgetMouseEvent(ossimQtMouseEvent* evt);

   void paintYourGraphics(QPainter* p,
                          int clipx,
                          int clipy,
                          int clipw,
                          int cliph);

};

#endif
