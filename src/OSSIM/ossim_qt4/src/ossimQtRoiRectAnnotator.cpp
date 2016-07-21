//----------------------------------------------------------------------------
// Copyright (C) 2004 Intelligence Data Systems, Inc. All rights reserved.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:   Region Of Interest (ROI) rectangle annotator object.
//
// $Id: ossimQtRoiRectAnnotator.cpp 23664 2015-12-14 14:17:27Z dburken $
//----------------------------------------------------------------------------

#include <QtGui/QPainter>
//Added by qt3to4:
#include <QtCore/QEvent>
#include <ossimQt/ossimQtRoiRectAnnotator.h>
#include <ossimQt/ossimQtEvent.h>
#include <ossimQt/ossimQtScrollingImageWidget.h>
#include <ossim/base/ossimROIEvent.h>
#include <ossim/base/ossimIrect.h>

ossimQtRoiRectAnnotator::ossimQtRoiRectAnnotator()
   : ossimQtRoiAnnotator(),
     ossimListenerManager(),
     theRoiLeftPressedFlag(false),
     theRoiMiddlePresedFlag(false),
     theRoiPressStart(0,0),
     thePenColor(Qt::white)
{
   thePoints.resize(2);
}

ossimQtRoiRectAnnotator::~ossimQtRoiRectAnnotator()
{
}

void ossimQtRoiRectAnnotator::setShiftedRoiRect(const ossimIrect& rect)
{
   if (!theImageWidget)
   {
      return;
   }
   
   // Shift the rectangle to native QImage space.
   ossimIrect r = rect;

   r.set_ul(r.ul() + theImageWidget->getShiftToViewport());
   r.set_lr(r.lr() + theImageWidget->getShiftToViewport());

   setRoiRect(r);
}

void ossimQtRoiRectAnnotator::setRoiRect(const ossimIrect& rect)
{
   if (thePoints.size() != 2)
   {
      thePoints.resize(2);
   }

   thePoints[0] = rect.ul();
   thePoints[1] = rect.lr();

   if (theImageWidget)
   {
      theImageWidget->refreshGraphics();
   }
}

ossimIrect ossimQtRoiRectAnnotator::getShiftedRoiRect() const
{
   ossimIrect r(0,0,0,0);
   if (thePoints.size() == 2 && theImageWidget)
   {
      r = getRoiRect();
      r.set_ul(r.ul() - theImageWidget->getShiftToViewport());
      r.set_lr(r.lr() - theImageWidget->getShiftToViewport());
   }
   return r;
}

ossimIrect ossimQtRoiRectAnnotator::getRoiRect() const
{
   if (thePoints.size() == 2 && theImageWidget)
   {
      // Sort the points.
      ossim_int32 ulx = (thePoints[0].x < thePoints[1].x) ?
         thePoints[0].x : thePoints[1].x;
      
      ossim_int32 uly = (thePoints[0].y < thePoints[1].y) ?
         thePoints[0].y : thePoints[1].y;
      
      ossim_int32 lrx = (thePoints[1].x > thePoints[0].x) ?
         thePoints[1].x : thePoints[0].x;
      
      ossim_int32 lry = (thePoints[1].y > thePoints[0].y) ?
         thePoints[1].y : thePoints[0].y;
      
      return ossimIrect(ulx, uly, lrx, lry);
   }
   
   return ossimIrect(0,0,0,0);
}

void ossimQtRoiRectAnnotator::updateYourRegionOfInterest(ossimQtMouseEvent* evt)
{
   if (!theImageWidget) return;

   if (thePoints.size() != 2)
   {
      thePoints.resize(2);
   }
   
   //---
   // On left click:
   // - start rectangle
   //
   // On left move:
   // - grow rectangle
   //
   // On left release:
   // - end rectangle
   //
   // On middle click:
   // - change rectangle color to green
   //
   // On middle move:
   // - shift existing rectangle by the movement.
   //
   // On middle release:
   // - change rectangle color back to white
   //---
   
   Qt::ButtonState  button = evt->getButton();
   QEvent::Type state  = evt->getQtEventType();
   
   // Most frequently called "moving" first...
   if (state == QEvent::MouseMove)
   {
      if (theRoiLeftPressedFlag)
      {
         if (thePoints[0] != theRoiPressStart)
         {
            thePoints[0] = theRoiPressStart;
         }
         thePoints[1] = evt->getPoint();
         theImageWidget->refreshGraphics();
      }
      else if (theRoiMiddlePresedFlag)
      {
         // move the existing rectangle.
         ossimIpt shift = (evt->getPoint() - theRoiPressStart);
         thePoints[0] += shift;
         thePoints[1] += shift;
         theRoiPressStart = evt->getPoint();
         theImageWidget->refreshGraphics();
      }
      return;
   }
   else if(state == QEvent::MouseButtonPress)
   {
      if (button == Qt::LeftButton)
      {
         theRoiPressStart = evt->getPoint();
         theRoiLeftPressedFlag = true;
         thePoints[0] = theRoiPressStart;
      }
      else if (button == Qt::MidButton)
      {
         // Change the color from white to green.
         thePenColor = Qt::green;
         theRoiPressStart = evt->getPoint();
         theRoiMiddlePresedFlag = true;
      }
      return;
   }
   else if (state == QEvent::MouseButtonRelease)
   {
      ossimROIEvent roiEvent;
      roiEvent.setEventType(ossimROIEvent::OSSIM_RECTANGLE_ROI);

      if (button == Qt::LeftButton)
      {
         // Check for some travel to avoid resizing roi on a simple click.
         ossimIpt pt = evt->getPoint();
         ossim_int32 x_delta = abs(theRoiPressStart.x - pt.x);
         ossim_int32 y_delta = abs(theRoiPressStart.y - pt.y);
         if (x_delta > 4 || y_delta > 4)
         {
            thePoints[1] = evt->getPoint();
            theImageWidget->refreshGraphics();
         }
         else
         {
            thePoints[1] = thePoints[0];
         }
         theRoiLeftPressedFlag = false;
      }
      else if (button == Qt::MidButton)
      {
         // Change the color from green to white.
         thePenColor = Qt::white;
         
         // move the existing rectangle.
         ossimIpt shift = (evt->getPoint() - theRoiPressStart);
         thePoints[0] += shift;
         thePoints[1] += shift;
         theRoiMiddlePresedFlag = false;
         theImageWidget->refreshGraphics();

         // Set the moving flag in the event.
         roiEvent.setMovingFlag(true);
      }
      
      ossimIrect r(thePoints[0], thePoints[1]);
      roiEvent.setRect(r);
      fireEvent(roiEvent);
      
      return;
   }
}

void ossimQtRoiRectAnnotator::paintAnnotation( QPainter* p,
                                               int clipx,
                                               int clipy,
                                               int clipw,
                                               int cliph )
{
   if ( !p || (thePoints.size() != 2) )
   {
      return;
   }

   ossimIrect clipRect(clipx, clipy, clipx + (clipw - 1), clipy + (cliph - 1));
   if (clipRect.intersects(getRoiRect()))
   {
      QRect r;
      r.setCoords(thePoints[0].x, thePoints[0].y,
                  thePoints[1].x, thePoints[1].y);
      p->setPen(thePenColor);
      p->drawRect(r);
   }
}

// Hidden from use.
ossimQtRoiRectAnnotator::ossimQtRoiRectAnnotator( const ossimQtRoiRectAnnotator&)
   : ossimQtRoiAnnotator(),
     ossimListenerManager()
{}

// Hidden from use.
ossimQtRoiRectAnnotator& ossimQtRoiRectAnnotator::operator=( const ossimQtRoiRectAnnotator&)
{
   return *this;
}

