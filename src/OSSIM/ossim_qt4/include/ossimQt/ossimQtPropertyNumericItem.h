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
// $Id: ossimQtPropertyNumericItem.h 12109 2007-12-04 18:09:45Z gpotts $
#ifndef ossimQtPropertyNumericItem_HEADER
#define ossimQtPropertyNumericItem_HEADER
#include <QtCore/QPointer>
#include <Qt3Support/Q3HBox>
#include "ossimQtPropertyItem.h"

class ossimQtPropertyNumericItem : public ossimQtPropertyItem
{
   Q_OBJECT
public:
   ossimQtPropertyNumericItem(ossimQtPropertyListView *propList,
                           ossimQtPropertyItem *after,
                           ossimQtPropertyItem *parent,
                           ossimRefPtr<ossimProperty> oProp);
   virtual ~ossimQtPropertyNumericItem();
   
   virtual void showEditor();
   virtual void hideEditor();
   virtual void setValue(const QVariant& value);
   
protected slots:
   void setValue();   
   virtual void resetProperty(bool notify=true);

protected:
   QLineEdit *lineEditor();
   QPointer<QLineEdit> theLineEditor;
};
                                

#endif
