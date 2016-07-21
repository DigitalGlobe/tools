//----------------------------------------------------------------------------
// Copyright (C) 2004 Intelligence Data Systems, Inc. All rights reserved.
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Region Of Interest (ROI) annotator object base class.
//
// $Id: ossimQtAnnotatorBase.cpp 12141 2007-12-07 18:05:13Z gpotts $
//----------------------------------------------------------------------------

#include <QtGui/QPainter>
#include <ossimQt/ossimQtAnnotatorBase.h>
#include <ossimQt/ossimQtScrollingImageWidget.h>

ossimQtAnnotatorBase::ossimQtAnnotatorBase()
   : QObject(),
     theImageWidget(NULL),
     theEnableFlag(true)
{
}

ossimQtAnnotatorBase::~ossimQtAnnotatorBase()
{
}

void ossimQtAnnotatorBase::setImageWidget(ossimQtScrollingImageWidget* widget)
{
   if (theImageWidget)
   {
      disconnect(theImageWidget, 0, 0, 0);
   }
   theImageWidget = widget;

   // Connect the widget's paintYourGraphics signal to our pain slot.
   connect(widget,
           SIGNAL(paintYourGraphics(QPainter*, int, int, int, int)),
           this,
           SLOT(paint(QPainter*, int, int, int, int)));
}

void ossimQtAnnotatorBase::paint(QPainter* p,
                                 int clipx,
                                 int clipy,
                                 int clipw,
                                 int cliph)
{
   if (theEnableFlag)
   {
      // Pass it on to the derived class.
      paintAnnotation(p, clipx, clipy, clipw, cliph);
   }
}

// Private to disallow copy constructor, assignment operator use...
ossimQtAnnotatorBase::ossimQtAnnotatorBase(const ossimQtAnnotatorBase&)
   :
      QObject(),
      theImageWidget(NULL),
      theEnableFlag(true)
{
}

ossimQtAnnotatorBase& ossimQtAnnotatorBase::operator=(const ossimQtAnnotatorBase&)
{
   return *this;
}

void ossimQtAnnotatorBase::enablePaint()
{
   theEnableFlag = true;
}

void ossimQtAnnotatorBase::disablePaint()
{
   theEnableFlag = false;
}
