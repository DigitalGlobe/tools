//*******************************************************************
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimQtDisplayUtility.cpp 12141 2007-12-07 18:05:13Z gpotts $

  // #include <iostream>


#include <ossimQt/ossimQtDisplayUtility.h>
#include <QtGui/QApplication>
#include <ossimQt/ossimQtEvent.h>
#include <ossim/base/ossimDisplayListEvent.h>
#include <ossim/base/ossimConnectableObject.h>
#include <ossim/base/ossimPropertyEvent.h>

void ossimQtDisplayUtility::flushAllOutputs(ossimConnectableObject* obj, bool doDisplaysFlag)
{
   if(!obj)
   {
      return;
   }

   ossimPropertyEvent evt(obj);
   obj->fireEvent(evt);
   obj->propagateEventToOutputs(evt);

   if(doDisplaysFlag)
   {
      findAndFlushAllConnectedDisplays(obj);
   }
}

void ossimQtDisplayUtility::refreshAllOutputs(ossimConnectableObject* obj, bool doDisplaysFlag)
{
   if(!obj) return;
   ossimPropertyEvent evt(obj);
   obj->fireEvent(evt);
   obj->propagateEventToOutputs(evt);
   
   if(doDisplaysFlag)
   {
      findAndRefreshAllConnectedDisplays(obj);
   }
}

void ossimQtDisplayUtility::findAndRefreshAllConnectedDisplays(
   ossimConnectableObject* obj)
{
   if(!obj) return;

   std::map<QObject*, QObject*> displayList;
   
   findAllDisplays(displayList, obj);
   refreshAllDisplays(displayList);
}

void ossimQtDisplayUtility::findAndFlushAllConnectedDisplays(
   ossimConnectableObject* obj)
{
   if(!obj)
   {
      return;
   }
   std::map<QObject*, QObject*> displayList;
   
   findAllDisplays(displayList, obj);

   flushAllDisplays(displayList);
}

void ossimQtDisplayUtility::findAllDisplays(
   std::map<QObject*, QObject*>& displayList,
   ossimConnectableObject* obj)
{
  if(!obj)
  {
     return;
  }
  ossimDisplayListEvent displayListEvent(obj);

  ossimConnectableObject* root = getRoot(obj);
  root->propagateEventToOutputs(displayListEvent);

  QObject* display = 0;
  display = (QObject*)displayListEvent.firstDisplay();
  if(display)
  {
     do
     {
        if(displayList.find(display) == displayList.end())
        {
           displayList.insert(std::make_pair((QObject*)display,
                                             (QObject*)display));
        }
     }while((display = (QObject*)displayListEvent.nextDisplay())!=0);
  }
}

void ossimQtDisplayUtility::refreshAllDisplays(std::map<QObject*,QObject*>& displayList)
{
  std::map<QObject*, QObject*>::iterator iter = displayList.begin();
  while(iter != displayList.end())
    {
      ossimQtRefreshDisplayEvent* evt = new ossimQtRefreshDisplayEvent;
      QApplication::postEvent((*iter).first, evt);
      ++iter;
    }
}

void ossimQtDisplayUtility::flushAllDisplays(
   std::map<QObject*,QObject*>& displayList)
{
   std::map<QObject*, QObject*>::iterator iter = displayList.begin();
   while(iter != displayList.end())
   {
      ossimQtFlushDisplayEvent* evt = new ossimQtFlushDisplayEvent;
      QApplication::postEvent((*iter).first, evt);
      ++iter;
   }
}

ossimConnectableObject* ossimQtDisplayUtility::getRoot(ossimConnectableObject* obj)
{
   ossimConnectableObject* root = obj;
   if(!root) return root;
   while(PTR_CAST(ossimConnectableObject, root->getOwner()))
   {
      root=PTR_CAST(ossimConnectableObject, root->getOwner());
   }

   return root;
}
