//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimQtImageWindowControllerEventFilter.cpp 23664 2015-12-14 14:17:27Z dburken $
#include <QtGui/QApplication>
#include <QtCore/QEvent>
//Added by qt3to4:
#include <QtGui/QKeyEvent>
#include <QtGui/QCloseEvent>
#include <ossimQt/ossimQtImageWindowControllerEventFilter.h>
#include <ossimQt/ossimQtImageWindowController.h>
#include <ossimQt/ossimQtApplicationUtility.h>
#include <ossimQt/ossimQtSwipeScrollingImageWidget.h>
#include <ossimQt/ossimQtImageWindow.h>

void ossimQtImageControllerPanEventFilter::scrollingImageWidgetMouseEvent(
   ossimQtMouseEvent* event)
{
   if (!theController) return;
   
   if(event->getQtEventType() == QEvent::MouseButtonRelease)
   {
      ossimIpt pt = event->getShiftPoint();
      theController->panTo(pt);
   }
   
   return;
}

void ossimQtImageControllerZoomEventFilter::scrollingImageWidgetMouseEvent(
   ossimQtMouseEvent* event)
{
   if (!theController) return;
   
   switch(event->getQtEventType())
   {
      case QEvent::MouseButtonRelease:
      {
         if(!thePanToOverrideFlag)
         {
            ossimIrect regionOfInterest = theAnnotator.getShiftedRoiRect();

            theController->zoomTo(regionOfInterest.ul(),
                                  regionOfInterest.lr());
         }
         else
         {
	    theController->panTo(event->getShiftPoint());
         }

         // Get rid of the box that was drawn.
         theAnnotator.clear();
         
         break;
      }
      default:
      {
         break;
      }
   }
}

bool ossimQtImageControllerZoomEventFilter::eventFilter(QObject* /*obj*/,
                                                        QEvent* event)
{
   if(!theController) return false;
   
   if(!theController->getImageWidget()) return false;
   
   switch(event->type())
   {
      case QEvent::KeyPress:
      {
         QKeyEvent* keyEvent = (QKeyEvent*)event;
         if(keyEvent->stateAfter() & Qt::ShiftModifier)
         {
	    theController->setZoomModeOut();
         }
         if(keyEvent->stateAfter() & Qt::ControlModifier)
         {
            thePanToOverrideFlag = true;
         }
         break;
      }
      case QEvent::KeyRelease:
      {
         QKeyEvent* keyEvent = (QKeyEvent*)event;
         if(keyEvent->state() & Qt::ShiftModifier)
         {
            theController->setZoomModeIn();
         }
         if(keyEvent->state() & Qt::ControlModifier)
         {
            thePanToOverrideFlag = false;
         }
         break;
      }
      default:
      {
         break;
      }
   }
   
   return false;
}

ossimQtImageControllerOssimEventCapture::~ossimQtImageControllerOssimEventCapture()
{
   removeListener((ossimConnectableObjectListener*)this);
   removeListener((ossimDisplayEventListener*)this);
   theController = NULL;
   ossimConnectableObject::disconnect();

   //---
   // Avoid a double delete from reference count going to zero and ossimRefPtr destroying
   // us and destructor call in ossimQtImageWindowController::~ossimQtImageWindowController.
   // Very messy! (drb)
   //---
   ossimReferenced::unref_nodelete();
}

void ossimQtImageControllerOssimEventCapture::connectInputEvent(ossimConnectionEvent& /*event*/)
{
//   if(event.getNewObject())
//     {
//       event.getNewObject()->removeListener((ossimConnectableObjectListener*)this);
//       event.getNewObject()->addListener((ossimConnectableObjectListener*)this);
//     }
}

void ossimQtImageControllerOssimEventCapture::disconnectInputEvent(ossimConnectionEvent& /*event*/)
{
  
//   if(event.getOldObject())
//     {
//       event.getOldObject()->removeListener((ossimConnectableObjectListener*)this);
//     }
   
   if(!theController||!theController->getImageWindow())
   {
      return;
   }
   if(!isConnected()) // Seems like reverse login here...
   {
      removeListener((ossimConnectableObjectListener*)this);
      removeListener((ossimDisplayEventListener*)this);
      
      if(theController->getAutoDeleteIfNoInputFlag())
      {
         QCloseEvent* closeEvent = new QCloseEvent;
         QApplication::postEvent(theController->getImageWindow(), closeEvent);
         theController = NULL;
      }
   }
   else
   {
      ossimConnectableObjectListener::disableListener();
      theController->initializePointers();
      theController->initializeMenus();
      QApplication::postEvent(theController->getImageWindow(),
                              new ossimQtRefreshDisplayEvent);
      ossimConnectableObjectListener::enableListener();
   }
}

void ossimQtImageControllerOssimEventCapture::objectDestructingEvent(
   ossimObjectDestructingEvent& event)
{
   if(event.getObject())
   {
      ossimConnectableObject* obj = PTR_CAST(ossimConnectableObject, event.getObject());
      if(obj)
      {
         obj->removeListener((ossimConnectableObjectListener*)this);
      }
   }
   if(!theController||!theController->getImageWindow())
   {
      return;
   }
   if(getInput(1))
   {
      if(event.getObject() == getInput(1))
      {
         ossimConnectableObjectListener::disableListener();
         theController->removeInput(PTR_CAST(ossimConnectableObject,event.getObject()));
         ossimConnectableObjectListener::enableListener();
      }
   }
}

void ossimQtImageControllerOssimEventCapture::displayListEvent(
   ossimDisplayListEvent& event)
{
   if(theController)
   {
      event.addDisplay(theController->getImageWindow());
   }
}

ossimString ossimQtImageControllerOssimEventCapture::getClassName() const
{
   return "ossimQtImageControllerOssimEventCapture";
}

ossimQtImageControllerZoomEventFilter::ossimQtImageControllerZoomEventFilter(
   ossimQtImageWindowController* controller)
   : theController(controller),
     thePanToOverrideFlag(false),
     theAnnotator()
{
   if (theController)
   {
      theAnnotator.setImageWidget(theController->getImageWidget());
   }
}

ossimQtImageControllerPanEventFilter::ossimQtImageControllerPanEventFilter(
   ossimQtImageWindowController* controller)
   : theController(controller)
{
}

ossimQtImageControllerOssimEventCapture::ossimQtImageControllerOssimEventCapture(
   ossimQtImageWindowController* imageWindow)
   :ossimConnectableObject(NULL, 1, 0, false, false),
    theController(imageWindow)
{
   //---
   // Was getting double delete so we manually increment the reference count
   // as a workaround...
   //---
   ossimReferenced::ref();
   addListener((ossimConnectableObjectListener*)this);
   addListener((ossimDisplayEventListener*)this);
}

bool ossimQtImageControllerOssimEventCapture::canConnectMyInputTo(
   ossim_int32 /*myInputIndex*/, const ossimConnectableObject* /* object */)const
{
   return true;
}
void ossimQtImageControllerOssimEventCapture::propertyEvent(
   ossimPropertyEvent& /* event */)
{
}

void ossimQtImageControllerZoomEventFilter::enableAnnotator()
{
   theAnnotator.enablePaint();
}

void ossimQtImageControllerZoomEventFilter::disableAnnotator()
{
   theAnnotator.disablePaint();
}

