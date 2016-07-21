//*******************************************************************
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimQtDataManagerListBox.cpp 15766 2009-10-20 12:37:09Z gpotts $
// #include <qdragobject.h>

#include <Qt3Support/Q3TextDrag>
#include <ossimQt/ossimQtDataManagerListBox.h>
#include <ossimQt/ossimDataManager.h>
#include <ossimQt/ossimQtApplicationUtility.h>
#include <ossimQt/ossimQtEvent.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>

ossimQtDataManagerListBox::ossimQtDataManagerListBox(QWidget* parent, const char* name, Qt::WFlags f)
:Q3ListBox(parent, name, f),
theDataManager(0)
{
   setSelectionMode((Q3ListBox::SelectionMode)(Q3ListBox::Extended));
   
   // lets grab the data manager from the main application
   // by sending a message
   //
   ossimQtGetDataManagerEvent tempEvt;
   ossimQtApplicationUtility::sendEventToRoot(this, &tempEvt);
   theDataManager = tempEvt.getDataManager();
   setDataManager(theDataManager);
}


ossimQtDataManagerListBox::ossimQtDataManagerListBox(ossimConnectableObject::ConnectableObjectList& layers,
                                                     QWidget* parent,
                                                     const char* name,
                                                     Qt::WFlags f)
:Q3ListBox(parent, name, f),
theDataManager(0)
{
   setSelectionMode((Q3ListBox::SelectionMode)(Q3ListBox::Extended));
   
   // lets grab the data manager from the main application
   // by sending a message
   //
   ossimQtGetDataManagerEvent tempEvt;
   ossimQtApplicationUtility::sendEventToRoot(this, &tempEvt);
   theDataManager = tempEvt.getDataManager();
   addObjects(layers);
   theDragStartFlag = false;
}

void ossimQtDataManagerListBox::setObjects(ossimConnectableObject::ConnectableObjectList& objList)
{
   clear();
   addObjects(objList);
}


void ossimQtDataManagerListBox::setDataManager(ossimDataManager* dataManager)
{
   clear();
   theDataManager = dataManager;
   if(theDataManager)
   {
      ossimConnectableObject* obj = theDataManager->getFirstObject();
      while(obj)
      {
         addObject(obj);
         obj = theDataManager->getNextObject();
      }
   }
}

void ossimQtDataManagerListBox::contentsMousePressEvent(QMouseEvent* e)
{
   theDragStartFlag = true;
   Q3ListBox::contentsMousePressEvent(e);
}

void ossimQtDataManagerListBox::contentsMouseMoveEvent(QMouseEvent* e)
{
   Q3ListBox::contentsMouseMoveEvent(e);
   if((theDragStartFlag)&&(e->state() == Qt::LeftButton))
   {
      startDrag();
      theDragStartFlag = false;
   }
}

void ossimQtDataManagerListBox::addObjects(ossimConnectableObject::ConnectableObjectList& objList)
{
   int idx = 0;
   for(idx = 0; idx < (int)objList.size(); ++idx)
   {
      addObject(objList[idx].get());
   }
}

void ossimQtDataManagerListBox::addObject(ossimConnectableObject* obj)
{
   if(obj)
   {
      QString item = QString::number((int)obj->getId().getId()) + ":";
      item = item + theDataManager->getDescription(obj).c_str();
      insertItem(item);
   }
}

void ossimQtDataManagerListBox::getSelectedObjects(ossimConnectableObject::ConnectableObjectList& objList)
{
   Q3ListBoxItem* currentItem = 0;
   if(!theDataManager)
   {
      return;
   }
   
   currentItem = item(0);
   while(currentItem)
   {
      if(currentItem->isSelected())
      {
         QString text = currentItem->text();
         text.truncate(text.find(':'));
         ossimId id(text.toInt());
         ossimConnectableObject* tempObj = theDataManager->getObject(id);
         if(tempObj)
         {
            objList.push_back(tempObj);
         }
      }
      currentItem = currentItem->next();
   }
}

ossimConnectableObject::ConnectableObjectList ossimQtDataManagerListBox::getSelectedObjects()
{
   ossimConnectableObject::ConnectableObjectList result;
   
   getSelectedObjects(result);
   
   return result;
}

void ossimQtDataManagerListBox::getSelectedObjectsAsIds(std::vector<ossimId>& idList)
{
   Q3ListBoxItem* currentItem = 0;
   
   currentItem = item(0);
   while(currentItem)
   {
      if(currentItem->isSelected())
      {
         QString text = currentItem->text();
         text.truncate(text.find(':'));
         ossimId id(text.toInt());
         idList.push_back(id);
      }
      currentItem = currentItem->next();
   }
}

std::vector<ossimId> ossimQtDataManagerListBox::getSelectedObjectsAsIds()
{
   std::vector<ossimId> result;
   
   getSelectedObjectsAsIds(result);
   
   return result;
}

void ossimQtDataManagerListBox::deleteSelectedObjects()
{
   ossimConnectableObject::ConnectableObjectList objList = getSelectedObjects();
   
   theDataManager->deleteObjects(objList);
   
   setDataManager(theDataManager);
}

void ossimQtDataManagerListBox::duplicateSelectedObjects()
{
   int idx = 0;
   ossimConnectableObject::ConnectableObjectList objList = getSelectedObjects();
   std::vector<ossimId> result;
   for(idx = 0; idx < (int)objList.size(); ++ idx)
   {
      result.push_back(theDataManager->duplicate(objList[idx].get()));
   }
   
   setDataManager(theDataManager);
   
   for(idx = 0; idx < (int)result.size(); ++ idx)
   {
      QString item = QString::number((int)result[idx].getId()) + ":";
      Q3ListBoxItem* listboxItem = findItem(item);
      if(listboxItem)
      {
         setSelected(listboxItem, true);
      }
   }
}

void ossimQtDataManagerListBox::mosaicSelectedObjects()
{
   ossimConnectableObject::ConnectableObjectList objList = getSelectedObjects();
   
   if(objList.size() == 0)
   {
      return;
   }
   ossimConnectableObject* obj = theDataManager->createStandardMosaic(objList,
                                                                      "ossimImageMosaic");
   if(obj)
   {
      QString item = QString::number(obj->getId().getId()) + ":" + obj->getDescription().c_str();
      insertItem(item);
      ossimQtDisplayChainEvent* evt = new ossimQtDisplayChainEvent(obj->getId().getId());
      ossimQtApplicationUtility::postEventToRoot(this, evt);
   }
}

void ossimQtDataManagerListBox::blendSelectedObjects()
{
   ossimConnectableObject::ConnectableObjectList objList = getSelectedObjects();
   if(objList.size() == 0)
   {
      return;
   }
   ossimConnectableObject* obj = theDataManager->createStandardMosaic(objList,
                                                                      "ossimBlendMosaic");
   if(obj)
   {
      QString item = QString::number(obj->getId().getId()) + ":" + obj->getDescription().c_str();
      insertItem(item);
      ossimQtDisplayChainEvent* evt = new ossimQtDisplayChainEvent(obj->getId().getId());
      ossimQtApplicationUtility::postEventToRoot(this, evt);
   }
}

void ossimQtDataManagerListBox::featherSelectedObjects()
{
   ossimConnectableObject::ConnectableObjectList objList = getSelectedObjects();
   if(objList.size() == 0)
   {
      return;
   }
   ossimConnectableObject* obj = theDataManager->createStandardMosaic(objList,
                                                                      "ossimFeatherMosaic");
   if(obj)
   {
      QString item = QString::number(obj->getId().getId()) + ":" + obj->getDescription().c_str();
      insertItem(item);
      ossimQtDisplayChainEvent* evt = new ossimQtDisplayChainEvent(obj->getId().getId());
      ossimQtApplicationUtility::postEventToRoot(this, evt);
   }
}

void ossimQtDataManagerListBox::startDrag()
{
   ossimConnectableObject::ConnectableObjectList objList;
   getSelectedObjects(objList);
   
   if(objList.size()>0)
   {
      Q3TextDrag* dragObj = new Q3TextDrag(this);
      ossimKeywordlist kwl;
      int idx = 0;
      kwl.add(ossimKeywordNames::TYPE_KW,
              "ossimDataManagerObjectList",
              true);
      
      for(idx = 0; idx < (int)objList.size();++idx)
      {
         ossimString prefix = "object" + ossimString::toString(idx) + ".";
         kwl.add(prefix.c_str(),
                 ossimKeywordNames::ID_KW,
                 objList[idx]->getId().getId(),
                 true);
      }
      
      dragObj->setText(kwl.toString().c_str());
      dragObj->drag();
   }
}
