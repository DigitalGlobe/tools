//----------------------------------------------------------------------------
// Copyright (C) 2004 Intelligence Data Systems, Inc. All rights reserved.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Annotator object base class.
//
// $Id: ossimQtAnnotatorBase.h 12108 2007-12-04 17:42:05Z gpotts $
//----------------------------------------------------------------------------
#ifndef ossimQtAnnotatorBase_HEADER
#define ossimQtAnnotatorBase_HEADER

#include <QtCore/QObject>

class ossimQtScrollingImageWidget;
class QPainter;
class ossimQtMouseEvent;

/**
 * Base class for drawing a region of interest on the scrolling image widget.
 */
class ossimQtAnnotatorBase : public QObject
{
   Q_OBJECT

public:

   /** default constructor */
   ossimQtAnnotatorBase();

   /** destructor */
   virtual ~ossimQtAnnotatorBase();

   /**
    * @param widget The widget to paint to.
    * 
    * @note This will connect the widgets paintYourGraphics
    * signal to slot paint.
    */
   virtual void setImageWidget(ossimQtScrollingImageWidget* widget);

   /** Set theEnableFlag to true enabling the paint method. */
   void enablePaint();
   
   /** Set theEnableFlag to false short circuiting the paint method. */
   void disablePaint();   
   
public slots:
   /**
    * Slot to hook up to the widget's paintYourGraphics signal.
    */
   void paint(QPainter* p,
              int clipx,
              int clipy,
              int clipw,
              int cliph);
   
protected:
   /**
    * Pure virtual.  Derived class must implement to be concrete.
    * Called by slot paint.
    */
   virtual void paintAnnotation(QPainter* p,
                                int clipx,
                                int clipy,
                                int clipw,
                                int cliph) = 0;

   /** The widget to paint to. */
   ossimQtScrollingImageWidget* theImageWidget;

private:
   /** Hidden from use copy constructor. */
   ossimQtAnnotatorBase( const ossimQtAnnotatorBase&);

   /** Hidden from use assignment operator. */
   ossimQtAnnotatorBase& operator=( const ossimQtAnnotatorBase&);

   /**
    * Enables/disables call to paintAnnotation method.
    * Defaulted to true in constructor.
    */
   bool theEnableFlag;
};

#endif /* #ifndef #ifndef ossimQtAnnotatorBase_HEADER */

