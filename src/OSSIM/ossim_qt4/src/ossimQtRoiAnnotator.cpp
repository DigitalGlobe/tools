//----------------------------------------------------------------------------
// Copyright (C) 2004 Intelligence Data Systems, Inc. All rights reserved.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:  Region Of Interest (ROI) annotator object base class.
//
// $Id: ossimQtRoiAnnotator.cpp 23664 2015-12-14 14:17:27Z dburken $
//----------------------------------------------------------------------------

#include <vector>


#include <ossimQt/ossimQtRoiAnnotator.h>
#include <ossimQt/ossimQtScrollingImageWidget.h>

ossimQtRoiAnnotator::ossimQtRoiAnnotator()
   : ossimQtAnnotatorBase(),
     thePoints(0)
{
}

ossimQtRoiAnnotator::~ossimQtRoiAnnotator()
{
}

void ossimQtRoiAnnotator::setImageWidget(ossimQtScrollingImageWidget* widget)
{
   //---
   // Call base class first.  This will do any disconnecting needed if the
   // widget was set previously.
   //---
   ossimQtAnnotatorBase::setImageWidget(widget);

   // Connect the widget's paintYourGraphics signal to our pain slot.
   connect(widget,
           SIGNAL(scrollingImageWidgetMouseEvent(ossimQtMouseEvent*)),
           this,
           SLOT(updateRegionOfInterest(ossimQtMouseEvent*)));
}

void ossimQtRoiAnnotator::setPoints(const std::vector<ossimIpt>& pts)
{
   thePoints = pts;
}

void ossimQtRoiAnnotator::setShiftPoints(const std::vector<ossimIpt>& pts)
{
   const ossim_uint32 SIZE = pts.size();
   if (thePoints.size() != SIZE)
   {
      thePoints.resize(SIZE);
   }

   if ( !theImageWidget || (SIZE == 0) )
   {
      return;
   }

   for (ossim_uint32 i = 0; i < SIZE; ++i)
   {
      thePoints[i] = pts[i] + theImageWidget->getShiftToViewport();
   }
}

void ossimQtRoiAnnotator::clear()
{
   thePoints.clear();
}

void ossimQtRoiAnnotator::getPoints(std::vector<ossimIpt>& pts) const
{
   pts = thePoints;
}

void ossimQtRoiAnnotator::getShiftPoints(std::vector<ossimIpt>& pts) const
{
   const ossim_uint32 SIZE = thePoints.size();
   if (pts.size() != SIZE)
   {
      pts.resize(SIZE);
   }
   
   if (!theImageWidget || SIZE == 0)
   {
      return;
   }

   for (ossim_uint32 i = 0; i < SIZE; ++i)
   {
      pts[i] = thePoints[i] - theImageWidget->getShiftToViewport();
   }
}

void ossimQtRoiAnnotator::updateRegionOfInterest(ossimQtMouseEvent* evt)
{
   // Call the derived class update.
   updateYourRegionOfInterest(evt);
}

// Private to disallow copy constructor, assignment operator use...
ossimQtRoiAnnotator::ossimQtRoiAnnotator(const ossimQtRoiAnnotator&)
   :
      ossimQtAnnotatorBase(),
      thePoints(0)
{
}

ossimQtRoiAnnotator& ossimQtRoiAnnotator::operator=(const ossimQtRoiAnnotator&)
{
   return *this;
}

