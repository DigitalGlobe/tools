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
// $Id: ossimQtPropertyFontItem.h 9505 2006-09-02 20:36:55Z dburken $
#ifndef ossimQtPropertyFontItem_HEADER
#define ossimQtPropertyFontItem_HEADER
#include "ossimQtPropertyItem.h"

class ossimQtPropertyFontItem : public ossimQtPropertyItem
{
   Q_OBJECT
public:
   ossimQtPropertyFontItem(ossimQtPropertyListView *propList,
                           ossimQtPropertyItem *after,
                           ossimQtPropertyItem *parent,
                           ossimRefPtr<ossimProperty> oProp);
   virtual ~ossimQtPropertyFontItem();
   virtual void createChildren();
   virtual void initChildren();
   virtual void showEditor();
   virtual void hideEditor();
   virtual bool hasSubItems() const;
   virtual void childValueChanged( ossimQtPropertyItem *child );


public slots:
   virtual void resetProperty(bool notify=true);


};

#endif
