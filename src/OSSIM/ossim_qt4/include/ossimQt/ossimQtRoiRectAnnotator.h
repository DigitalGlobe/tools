//----------------------------------------------------------------------------
// Copyright (C) 2004 Intelligence Data Systems, Inc. All rights reserved.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Region Of Interest (ROI) rectangle annotator object.
//
// $Id: ossimQtRoiRectAnnotator.h 12108 2007-12-04 17:42:05Z gpotts $
//----------------------------------------------------------------------------
#ifndef ossimQtRoiRectAnnotator_HEADER
#define ossimQtRoiRectAnnotator_HEADER

#include <QtGui/QColor>
#include "ossimQtRoiAnnotator.h"
#include <ossim/base/ossimListenerManager.h>

class ossimIrect;
class ossimQtMouseEvent;

class ossimQtRoiRectAnnotator : public ossimQtRoiAnnotator,
                                public ossimListenerManager
{
   Q_OBJECT

public:
   
   ossimQtRoiRectAnnotator();
   virtual ~ossimQtRoiRectAnnotator();

   /**
    * Set the roi rectangle.
    * 
    * @param rect Rectangle to set.
    *
    * @note This method takes a rectangle and converts it back to QImage space
    * by adding theImageWidget->getShiftToViewport().
    * 
    * */
   void setShiftedRoiRect(const ossimIrect& rect);

   /**
    * Set the roi rectangle.
    * 
    * @param rect Rectangle to set.
    *
    * @note This method takes a rectangle that's in native QImage space.
    * 
    * */
   void setRoiRect(const ossimIrect& rect);

   /**
    * @return the current roi rectangle.
    *
    * @note This method takes a rectangle and converts it back to QImage space
    * by subtracting theImageWidget->getShiftToViewport().
    */
   ossimIrect getShiftedRoiRect() const;

   /**
    * @return the current roi rectangle.
    *
    * @note This method returns a rectangle that's in QImage space.
    */
   ossimIrect getRoiRect() const;

protected:
   /**
    * Called by base class slot updateRegionOfInterest when a moouse event
    * occurs.
    */
   virtual void updateYourRegionOfInterest(ossimQtMouseEvent* evt);   

   /**
    * Called by base class slot paint.
    */
   virtual void paintAnnotation(QPainter* p,
                                int clipx,
                                int clipy,
                                int clipw,
                                int cliph);
   
private:
   /** Hidden from use copy constructor. */
   ossimQtRoiRectAnnotator( const ossimQtRoiRectAnnotator&);

   /** Hidden from use assignment operator. */
   ossimQtRoiRectAnnotator& operator=( const ossimQtRoiRectAnnotator&);

   bool                      theRoiLeftPressedFlag;
   bool                      theRoiMiddlePresedFlag;
   ossimIpt                  theRoiPressStart;
   QColor                    thePenColor;
};

#endif /* #ifndef ossimQtRoiRectAnnotator_HEADER */

