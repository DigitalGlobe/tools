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
// $Id: ossimQtPropertyContainerItem.cpp 12141 2007-12-07 18:05:13Z gpotts $
#include <ossimQt/ossimQtPropertyContainerItem.h>
#include <ossim/base/ossimContainerProperty.h>
#include <ossimQt/ossimQtPropertyFactory.h>
#include <ossim/base/ossimNotifyContext.h>

ossimQtPropertyContainerItem::ossimQtPropertyContainerItem(ossimQtPropertyListView *propList,
                                                           ossimQtPropertyItem *after,
                                                           ossimQtPropertyItem *parent,
                                                           ossimRefPtr<ossimProperty> oProp)
   :ossimQtPropertyItem(propList, after, parent, oProp)
{
   
}

ossimQtPropertyContainerItem::~ossimQtPropertyContainerItem()
{
}

void ossimQtPropertyContainerItem::createChildren()
{
   if(childCount() != 0)
   {
      return;
   }
   ossimContainerProperty* containerProp = PTR_CAST(ossimContainerProperty,
                                                    getOssimProperty().get());

   ossimQtPropertyItem* previous = this;
   
   if(containerProp)
   {
      int n = (int)containerProp->getNumberOfProperties();
      for(int idx = 0; idx < n; ++idx)
      {
         ossimQtPropertyItem* tempItem = ossimQtPropertyFactory::instance()->createProperty(theListView,
                                                                                            previous,
                                                                                            this,
                                                                                            containerProp->getProperty(idx));
         
         if(tempItem)
         {
            addChild(tempItem);
            previous = tempItem;
         }
         else
         {
            ossimNotify(ossimNotifyLevel_WARN) << "WARNING ossimQtPropertyContainerItem::createChildren(): RETURNED NULL" << std::endl;
            
         }
      }
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN) << "WARING ossimQtPropertyContainerItem::createChildren(): NOT A CONTAINER" << std::endl;
   }
}

bool ossimQtPropertyContainerItem::hasSubItems() const
{
   return true;
}

void ossimQtPropertyContainerItem::showEditor()
{
   ossimQtPropertyItem::showEditor();
   placeEditor(0);
}

void ossimQtPropertyContainerItem::hideEditor()
{
   ossimQtPropertyItem::hideEditor();
}

void ossimQtPropertyContainerItem::resetProperty(bool notify)
{
   if(childCount() > 0)
   {
      // since we are sharing the property with the children we will not call the base class
      // resetProperty.  This would cause a core dump.
      //
      int idx = 0;
      
      for(idx = 0; idx < childCount(); ++idx)
      {
         child(idx)->resetProperty(false);
      }

      setChanged(false);
   }
   else
   {
      ossimQtPropertyItem::resetProperty(notify);
   }
}
