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
// $Id: ossimQtLayerEditorController.h 12133 2007-12-06 21:27:02Z dburken $
#ifndef ossimQtLayerEditorController_HEADER
#define ossimQtLayerEditorController_HEADER
#include <QtCore/QObject>
#include <QtCore/QStringList>

class ossimQtLayerEditor;
class Q3ListBoxItem;

class ossimQtLayerEditorController : public QObject
{
   Q_OBJECT
public:
   ossimQtLayerEditorController(ossimQtLayerEditor* layerEditor);

   static const char* EMPTY_SLOT_STRING;
   const char* getEmptySlotString()const;
   void setMaxNumberOfInputs(int maxNumber);
public slots:
   virtual void upButtonClicked();
   virtual void downButtonClicked();
   virtual void addLayerButtonClicked();
   virtual void removeLayerButtonClicked();

protected:
   ossimQtLayerEditor* theEditor;
   int                 theMaxNumberOfInputs;
   void swapInputListBoxItems(Q3ListBoxItem* item1, 
                              Q3ListBoxItem* item2);
   int numberOfEmptySlots()const;
   int insertIntoEmptySlot(int idx,
                           const QString& value);

signals:
   void movingSelectedLayersUp(bool& canMoveUpFlag);
   void movingSelectedLayersDown(bool& canMoveDownFlag);
   void removingSelectedLayersFromInput(bool& canRemoveSelected);
   void addingSelectedLayersToInput(bool& canAddSelected);
   void inputLayersChanged();
   void getInputLayers(QStringList& stringList);
};

#endif
