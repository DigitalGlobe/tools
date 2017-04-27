//*******************************************************************
//
// License:  See LICENSE.txt file in the top level directory.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimQtDisplayUtility.h 11056 2007-05-26 18:38:19Z dburken $
#ifndef ossimQtDisplayUtility_HEADER
#define ossimQtDisplayUtility_HEADER
#include <map>
#include "ossimQtExport.h"

class ossimConnectableObject;
class QObject;

class OSSIMQT_DLL ossimQtDisplayUtility
{
public:

   /*!
    * will force a property event on the passed in object and pass to all
    * outputs.  It will then find and flush all display cache.
    */
  static void flushAllOutputs(ossimConnectableObject* obj, bool doDisplaysFlag=true);

   /*!
    * will force a property event on the passed in object and pass to all
    * outputs.  It will then find and do a complete refresh on all output
    * displays
    */
  static void refreshAllOutputs(ossimConnectableObject* obj, bool doDisplaysFlag=true);
   
  static void findAndRefreshAllConnectedDisplays(ossimConnectableObject* obj);
  static void findAndFlushAllConnectedDisplays(ossimConnectableObject* obj);
  static void refreshAllDisplays(std::map<QObject*,QObject*>& displayList);
  static void flushAllDisplays(std::map<QObject*,QObject*>& displayList);
  static void findAllDisplays(std::map<QObject*,QObject*>& displayList,
			      ossimConnectableObject* obj);
   static ossimConnectableObject* getRoot(ossimConnectableObject* obj);
};

#endif
