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
// $Id: ossimQtPropertyColorItem.h 12134 2007-12-06 21:35:36Z dburken $
#ifndef ossimQtPropertyColorItem_HEADER
#define ossimQtPropertyColorItem_HEADER
#include <QtCore/QPointer>
#include <Qt3Support/Q3HBox>
#include <QtGui/QPushButton>
//Added by qt3to4:
#include <Qt3Support/Q3Frame>
#include "ossimQtPropertyItem.h"

class ossimQtPropertyColorItem : public ossimQtPropertyItem
{
   Q_OBJECT
public:
   ossimQtPropertyColorItem(ossimQtPropertyListView *propList,
                            ossimQtPropertyItem *after,
                            ossimQtPropertyItem *parent,
                            ossimRefPtr<ossimProperty> oProp);
   virtual ~ossimQtPropertyColorItem();
   
   virtual void createChildren();
   virtual void initChildren();
   virtual void showEditor();
   virtual void hideEditor();
   virtual bool hasSubItems() const;
   virtual void childValueChanged( ossimQtPropertyItem *child );
   virtual bool hasCustomContents() const;
   virtual void drawCustomContents( QPainter *p, const QRect &r );
   virtual void resetProperty(bool notify=true);

protected slots:
   void getColor();

protected:
   QPointer<Q3HBox>       theBox;
   QPointer<Q3Frame>      theColorPreview;
   QPointer<QPushButton> theButton;
};

#endif
