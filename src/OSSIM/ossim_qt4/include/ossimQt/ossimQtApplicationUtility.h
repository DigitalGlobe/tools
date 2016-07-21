//Added by qt3to4:
#include <QtCore/QEvent>
//*******************************************************************
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimQtApplicationUtility.h 12108 2007-12-04 17:42:05Z gpotts $
#ifndef ossimQtApplicationUtility_HEADER
#define ossimQtApplicationUtility_HEADER

class QObject;
class QEvent;
class ossimQtApplicationUtility
{
public:
   static QObject* getRoot(QObject* obj);
   static void postEventToRoot(QObject* obj, 
                               QEvent* event);
   static void sendEventToRoot(QObject* obj, 
                               QEvent* event);
};

#endif
