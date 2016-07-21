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
// $Id: ossimQtPropertyFactory.cpp 12141 2007-12-07 18:05:13Z gpotts $
#include <ossimQt/ossimQtPropertyTextItem.h>
#include <ossimQt/ossimQtPropertyBooleanItem.h>
#include <ossimQt/ossimQtPropertyContainerItem.h>
#include <ossimQt/ossimQtPropertyStringItem.h>
#include <ossimQt/ossimQtPropertyNumericItem.h>
#include <ossimQt/ossimQtPropertyColorItem.h>
#include <ossimQt/ossimQtPropertyFontItem.h>
#include <ossimQt/ossimQtPropertyFilenameItem.h>
#include <ossimQt/ossimQtPropertyDateItem.h>
#include <ossimQt/ossimQtPropertyMatrixItem.h>
#include <ossimQt/ossimQtPropertyStringListItem.h>
#include <ossimQt/ossimQtPropertyListView.h>
#include <ossimQt/ossimQtPropertyFactory.h>
#include <ossim/base/ossimTextProperty.h>
#include <ossim/base/ossimBooleanProperty.h>
#include <ossim/base/ossimContainerProperty.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimNumericProperty.h>
#include <ossim/base/ossimColorProperty.h>
#include <ossim/base/ossimFontProperty.h>
#include <ossim/base/ossimMatrixProperty.h>
#include <ossim/base/ossimStringListProperty.h>
#include <ossim/base/ossimFilenameProperty.h>
#include <ossim/base/ossimDateProperty.h>

ossimQtPropertyFactory* ossimQtPropertyFactory::theInstance = (ossimQtPropertyFactory*)0;

ossimQtPropertyFactory::ossimQtPropertyFactory()
{
   theInstance = this;
}

ossimQtPropertyFactory* ossimQtPropertyFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimQtPropertyFactory();
   }
   
   return theInstance;
}

ossimQtPropertyItem* ossimQtPropertyFactory::createProperty(ossimQtPropertyListView *propList,
                                                            ossimQtPropertyItem *after,
                                                            ossimQtPropertyItem *parent,
                                                            ossimRefPtr<ossimProperty> oProp)const
{
   ossimQtPropertyItem* result = 0;
   
   if(PTR_CAST(ossimTextProperty, oProp.get()))
   {
      result = new ossimQtPropertyTextItem(propList,
                                           after,
                                           parent,
                                           oProp);
   }
   else if(PTR_CAST(ossimBooleanProperty,
                    oProp.get()))
   {
      result = new ossimQtPropertyBooleanItem(propList,
                                              after,
                                              parent,
                                              oProp);
   }
   else if(PTR_CAST(ossimStringProperty, oProp.get()))
   {
      result = new ossimQtPropertyStringItem(propList,
                                             after,
                                             parent,
                                             oProp);
   }
   else if(PTR_CAST(ossimNumericProperty, oProp.get()))
   {
      result = new ossimQtPropertyNumericItem(propList,
                                              after,
                                              parent,
                                              oProp);
   }
   else if(PTR_CAST(ossimColorProperty, oProp.get()))
   {
      result = new ossimQtPropertyColorItem(propList,
                                            after,
                                            parent,
                                            oProp);
   }
   else if(PTR_CAST(ossimFontProperty, oProp.get()))
   {
      result = new ossimQtPropertyFontItem(propList,
                                           after,
                                           parent,
                                           oProp);
   }
   else if(PTR_CAST(ossimFilenameProperty, oProp.get()))
   {
      result = new ossimQtPropertyFilenameItem(propList,
                                               after,
                                               parent,
                                               oProp);
   }
   else if(PTR_CAST(ossimMatrixProperty, oProp.get()))
   {
      result = new ossimQtPropertyMatrixItem(propList,
                                             after,
                                             parent,
                                             oProp);
   }
   else if(PTR_CAST(ossimStringListProperty, oProp.get()))
   {
      result = new ossimQtPropertyStringListItem(propList,
                                                 after,
                                                 parent,
                                                 oProp);
   }
   else if(PTR_CAST(ossimDateProperty, oProp.get()))
   {
      result = new ossimQtPropertyDateItem(propList,
                                           after,
                                           parent,
                                           oProp);
   }
   else if(PTR_CAST(ossimContainerProperty,
                    oProp.get()))
   {
      result = new ossimQtPropertyContainerItem(propList,
                                                after,
                                                parent,
                                                oProp);
   }
   
   return result;
}
