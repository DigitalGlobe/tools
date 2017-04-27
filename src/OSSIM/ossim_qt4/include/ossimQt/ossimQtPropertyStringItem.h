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
// $Id: ossimQtPropertyStringItem.h 12109 2007-12-04 18:09:45Z gpotts $
#ifndef ossimQtPropertyStringItem_HEADER
#define ossimQtPropertyStringItem_HEADER
#include <QtCore/QPointer>
#include <Qt3Support/Q3HBox>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include "ossimQtPropertyItem.h"

class ossimQtPropertyStringItem : public ossimQtPropertyItem
{
   Q_OBJECT
public:
   ossimQtPropertyStringItem(ossimQtPropertyListView *propList,
                             ossimQtPropertyItem *after,
                             ossimQtPropertyItem *parent,
                             ossimRefPtr<ossimProperty> oProp);
   virtual ~ossimQtPropertyStringItem();
   virtual void showEditor();
   virtual void hideEditor();
   
protected:
   QComboBox* theSelectionList;
   QLineEdit* theLineEditor;

   QLineEdit* lineEditor();
   QComboBox* comboBox();
   
public slots:
   virtual void returnPressed();
   virtual void setValue();
   virtual void activated(const QString& value);
   virtual void resetProperty(bool notify = true);

};

#endif
