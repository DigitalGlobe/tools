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
// $Id: ossimQtPropertyFilenameItem.h 12122 2007-12-06 13:27:09Z gpotts $
#ifndef ossimQtPropertyFilenameItem_HEADER
#define ossimQtPropertyFilenameItem_HEADER
#include <QtGui/QLineEdit>
#include <QtCore/QPointer>
#include <Qt3Support/Q3HBox>
#include "ossimQtPropertyItem.h"

class ossimQtPropertyFilenameItem : public ossimQtPropertyItem
{
   Q_OBJECT
public:
   ossimQtPropertyFilenameItem(ossimQtPropertyListView *propList,
                           ossimQtPropertyItem *after,
                           ossimQtPropertyItem *parent,
                           ossimRefPtr<ossimProperty> oProp);
   virtual ~ossimQtPropertyFilenameItem();
   
   virtual void showEditor();
   virtual void hideEditor();
   
protected slots:
   void setValue();   
   void getText();

public slots:
virtual void resetProperty(bool notify=true);

protected:
   
   QLineEdit*         theLineEditor;
   QPointer<Q3HBox> theBox;
   QPushButton *thePushButton;

   QLineEdit *lineEditor();
   
};

#endif
