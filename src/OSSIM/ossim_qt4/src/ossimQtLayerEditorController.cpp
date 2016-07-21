//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimQtLayerEditorController.cpp 12141 2007-12-07 18:05:13Z gpotts $

#include <Qt3Support/Q3ListBoxItem>
#include <QtGui/QListWidget>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <ossimQt/ossimQtLayerEditorController.h>
#include <ossimQt/ossimQtLayerEditor.h>

const char* ossimQtLayerEditorController::EMPTY_SLOT_STRING = "<EMPTY SLOT>";

ossimQtLayerEditorController::ossimQtLayerEditorController(ossimQtLayerEditor* layerEditor)
{
   theEditor = layerEditor;
   theMaxNumberOfInputs = -1;
}

const char* ossimQtLayerEditorController::getEmptySlotString()const
{
   return ossimQtLayerEditorController::EMPTY_SLOT_STRING;
}

void ossimQtLayerEditorController::setMaxNumberOfInputs(int maxNumber)
{
   theMaxNumberOfInputs = maxNumber;
}

void ossimQtLayerEditorController::upButtonClicked()
{
    bool canMoveUpFlag = true;
    emit movingSelectedLayersUp(canMoveUpFlag);
    
    if(canMoveUpFlag)
    {
	Q3ListBoxItem* currentItem = theEditor->theCurrentInputLayersListBox->item(0);
	if(currentItem->isSelected()) return;
	while(currentItem)
	{
	    Q3ListBoxItem* nextItem = currentItem->next();
	    if(currentItem->isSelected())
	    {
		swapInputListBoxItems(currentItem, currentItem->prev());
	    }
	    currentItem = nextItem;
	}
	emit inputLayersChanged();
    }
}

void ossimQtLayerEditorController::downButtonClicked()
{
    bool canMoveDownFlag = true;
    if(theEditor->theCurrentInputLayersListBox->count() < 1) return;
    
    movingSelectedLayersDown(canMoveDownFlag); 
    
    if(canMoveDownFlag)
    {
	Q3ListBoxItem* currentItem = theEditor->theCurrentInputLayersListBox->item(theEditor->theCurrentInputLayersListBox->count()-1);
	if(currentItem->isSelected()) return;
	while(currentItem)
	{
	    Q3ListBoxItem* nextItem = currentItem->prev();
	    if(currentItem->isSelected())
	    {
		swapInputListBoxItems(currentItem, currentItem->next());
	    }
	    currentItem = nextItem;
	}
	
	emit inputLayersChanged();
    }
}

void ossimQtLayerEditorController::swapInputListBoxItems(Q3ListBoxItem* item1, 
                                                         Q3ListBoxItem* item2)
{
    if(item1&&item2)
    {
      bool item1Selected = item1->isSelected();
      bool item2Selected = item2->isSelected();
      int i1 = theEditor->theCurrentInputLayersListBox->index(item1);
      int i2 = theEditor->theCurrentInputLayersListBox->index(item2);
      QString i1Text = item1->text();
      
      theEditor->theCurrentInputLayersListBox->changeItem(item2->text(), i1);
      theEditor->theCurrentInputLayersListBox->changeItem(i1Text, i2 );
      if(item1Selected)
      {
         theEditor->theCurrentInputLayersListBox->setSelected(i1, item2Selected);
         theEditor->theCurrentInputLayersListBox->setSelected(i2, item1Selected);
      }
    }
}

void ossimQtLayerEditorController::addLayerButtonClicked()
{
    bool canAddAvailableToInputs = true;

    if(theMaxNumberOfInputs < 1)
    {
       emit addingSelectedLayersToInput(canAddAvailableToInputs);
       if(canAddAvailableToInputs)
       {
          Q3ListBoxItem* currentItem = theEditor->theAvailableLayersListBox->item(0);
          while(currentItem)
          {
             if(currentItem->isSelected())
             {
		Q3ListBoxItem* tempItem = currentItem->next();
		QString newItem        = currentItem->text();
		theEditor->theAvailableLayersListBox->removeItem(theEditor->theAvailableLayersListBox->index(currentItem));
		theEditor->theCurrentInputLayersListBox->insertItem(newItem);
		currentItem = tempItem;
             }
             else
             {
		currentItem = currentItem->next();
             }
          }
          QStringList stringList;
          
          emit inputLayersChanged();	
       }
       else
       {
       }
    }
    else
    {
       int emptySlots = numberOfEmptySlots();
       int slotIdx = -1;
       if(emptySlots>0)
       {
          Q3ListBoxItem* currentItem = theEditor->theAvailableLayersListBox->item(0);
          while(currentItem&&(emptySlots > 0))
          {
             Q3ListBoxItem* tempItem = currentItem->next();
             
             if(currentItem->isSelected())
             {
             
                QString newItem        = currentItem->text();
                
                slotIdx = insertIntoEmptySlot(slotIdx,
                                              newItem);
                if(slotIdx < 0)
                {
                   break;
                }
                theEditor->theAvailableLayersListBox->removeItem(theEditor->theAvailableLayersListBox->index(currentItem));
                --emptySlots;
             }
             currentItem = tempItem;
          }
          emit inputLayersChanged();	
       }
    }
}

int ossimQtLayerEditorController::insertIntoEmptySlot(int idx,
                                                      const QString& value)
{
   int tempIdx = idx;
   if(tempIdx < 0)
   {
      tempIdx = 0;
   }

   if(tempIdx > (int)theEditor->theCurrentInputLayersListBox->count()) return -1;

   Q3ListBoxItem* currentItem = theEditor->theCurrentInputLayersListBox->item(tempIdx);

   while(currentItem)
   {
      Q3ListBoxItem* tempItem = currentItem->next();
      if(currentItem->text() == ossimQtLayerEditorController::EMPTY_SLOT_STRING)
      {
         theEditor->theCurrentInputLayersListBox->changeItem(value, tempIdx);
         
         return tempIdx;
      }
      currentItem = tempItem;
      
      ++tempIdx;
   }

   return tempIdx;
}

int ossimQtLayerEditorController::numberOfEmptySlots()const
{
   Q3ListBoxItem* currentItem = theEditor->theCurrentInputLayersListBox->item(0);
   int result = 0;
   while(currentItem)
   {
      if(currentItem->text() == EMPTY_SLOT_STRING)
      {
         ++result;
      }
      currentItem = currentItem->next();
   }

   return result;
}


void ossimQtLayerEditorController::removeLayerButtonClicked()
{
    bool canRemoveLayersFromInput = true;
    

    if(theMaxNumberOfInputs < 0)
    {
       emit removingSelectedLayersFromInput(canRemoveLayersFromInput);
       if(canRemoveLayersFromInput)
       {
          Q3ListBoxItem* currentItem = theEditor->theCurrentInputLayersListBox->item(0);
          while(currentItem)
          {
             if(currentItem->isSelected())
             {
		Q3ListBoxItem* tempItem = currentItem->next();
		QString newItem        = currentItem->text();
		theEditor->theCurrentInputLayersListBox->removeItem(theEditor->theCurrentInputLayersListBox->index(currentItem));
		theEditor->theAvailableLayersListBox->insertItem(newItem);
		currentItem = tempItem;
             }
             else
             {
		currentItem = currentItem->next();
             }
          }
          emit inputLayersChanged();	
       }
    }
    else
    {
       Q3ListBoxItem* currentItem = theEditor->theCurrentInputLayersListBox->item(0);
       bool layerRemovedFlag = false;
       while(currentItem)
       {
          Q3ListBoxItem* tempItem = currentItem->next();
          if(currentItem->isSelected())
          {
             if(currentItem->text() != EMPTY_SLOT_STRING)
             {
                layerRemovedFlag = true;
                int idx = theEditor->theCurrentInputLayersListBox->index(currentItem);
                QString newItem        = currentItem->text();
                theEditor->theAvailableLayersListBox->insertItem(newItem);
                theEditor->theCurrentInputLayersListBox->changeItem(EMPTY_SLOT_STRING, idx);
             }
          }
          currentItem = tempItem;
       }
       if(layerRemovedFlag)
       {
          emit inputLayersChanged();
       }
    }
}
