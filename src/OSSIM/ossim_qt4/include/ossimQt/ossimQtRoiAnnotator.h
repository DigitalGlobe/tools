//----------------------------------------------------------------------------
// Copyright (C) 2004 Intelligence Data Systems, Inc. All rights reserved.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Region Of Interest (ROI) annotator object base class.
//
// $Id: ossimQtRoiAnnotator.h 9495 2006-09-01 22:49:22Z dburken $
//----------------------------------------------------------------------------
#ifndef ossimQtRoiAnnotator_HEADER
#define ossimQtRoiAnnotator_HEADER

#include <vector>
#include "ossimQtAnnotatorBase.h"
#include <ossim/base/ossimIpt.h>

class ossimQtScrollingImageWidget;
class QPainter;
class ossimQtMouseEvent;

/**
 * Base class for drawing a region of interest on the scrolling image widget.
 */
class ossimQtRoiAnnotator : public ossimQtAnnotatorBase
{
   Q_OBJECT
public:

   /** default constructor */
   ossimQtRoiAnnotator();

   /** destructor */
   virtual ~ossimQtRoiAnnotator();

   /**
    * @param pts The points to draw.
    *
    * @note Points should be in QT space.
    * 0,0 = upper left, x positive right, y positive down.
    */
   virtual void setPoints(const std::vector<ossimIpt>& pts);

  /**
    * @param pts The points to draw.
    * 
    * @note Assumes points passed in are in ossim image space and adds
    * theShiftToZeroZero to convert to QT space.
    */
   virtual void setShiftPoints(const std::vector<ossimIpt>& pts);

   /** Clears thePoints. */
   void clear();
   
   /**
    * @param pts Vector of ossimIpts to initialize.
    * 
    * @note Points are in QT space.
    * 0,0 = upper left, x positive right, y positive down.
    */
   virtual void getPoints(std::vector<ossimIpt>& pts) const;

   /**
    * @param pts Vector of ossimIpts to initialize.
    *
    * @note Converts stored points to ossim image space by subtracting
    * theShiftToZeroZero to the stored points.
    */
   virtual void getShiftPoints(std::vector<ossimIpt>& pts) const;

   /**
    * Overrides ossimQtAnnotatorBase to connect updateRegionOfInterest
    * to scrollingImageWidgetMouseEvent.
    * 
    * @param widget The widget to paint to.
    * 
    * @note This will connect the widgets scrollingImageWidgetMouseEvent
    * signal to slot updateRegionOfInterest.
    */
   virtual void setImageWidget(ossimQtScrollingImageWidget* widget);

public slots:
   /**
    * Slot to hook up to the widget's scrollingImageWidgetMouseEvent.
    */
   void updateRegionOfInterest(ossimQtMouseEvent* evt);

protected:

   /**
    * Pure virtual.  Derived class must implement to be concrete.
    * Called by slot updateRegionOfInterest when a moouse event occurs.
    */
   virtual void updateYourRegionOfInterest(ossimQtMouseEvent* evt) = 0;

   /**
    * Points which drawing to widget will be based on.
    * 
    * @note These points are stored non shifted in QT widget space.
    * 0,0 = upper left, x positive right, y positive down.
    */
   std::vector<ossimIpt> thePoints;
   
private:
   /** Hidden from use copy constructor. */
   ossimQtRoiAnnotator( const ossimQtRoiAnnotator&);

   /** Hidden from use assignment operator. */
   ossimQtRoiAnnotator& operator=( const ossimQtRoiAnnotator&);
};

#endif /* #ifndef ossimQtRoiAnnotator_HEADER */

