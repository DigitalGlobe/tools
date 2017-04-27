//*******************************************************************
//
// See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimQtApplicationUtility.cpp 12141 2007-12-07 18:05:13Z gpotts $
#include <QtCore/QObject>
#include <QtCore/QEvent>
#include <QtGui/QApplication>
#include <ossimQt/ossimQtApplicationUtility.h>

QObject* ossimQtApplicationUtility::getRoot(QObject* obj)
{
   if(!obj) return NULL;
   QObject* current = obj;
   while(current->parent())
   {
      current = current->parent();
   }
   
   return current;
}

void ossimQtApplicationUtility::postEventToRoot(QObject* obj, 
                                                QEvent* event)
{
   QObject* tempRoot = getRoot(obj);
   QApplication::postEvent(tempRoot, event);
}

void ossimQtApplicationUtility::sendEventToRoot(QObject* obj, 
                                                QEvent* event)
{
   QObject* tempRoot = getRoot(obj);
   QApplication::sendEvent(tempRoot, event);
}
