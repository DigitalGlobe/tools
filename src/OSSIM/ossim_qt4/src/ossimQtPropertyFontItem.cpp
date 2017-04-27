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
// $Id: ossimQtPropertyFontItem.cpp 23343 2015-05-29 12:40:47Z dburken $


#include <ossimQt/ossimQtPropertyFontItem.h>
#include <ossim/base/ossimFontProperty.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimNumericProperty.h>
#include <ossim/base/ossimFontInformation.h>
#include <ossim/font/ossimFontFactoryRegistry.h>
#include <ossimQt/ossimQtPropertyFactory.h>
#include <QtCore/QVariant>
#include <map>

ossimQtPropertyFontItem::ossimQtPropertyFontItem(ossimQtPropertyListView *propList,
                                                 ossimQtPropertyItem *after,
                                                 ossimQtPropertyItem *parent,
                                                 ossimRefPtr<ossimProperty> oProp)
   :ossimQtPropertyItem(propList, after, parent, oProp)
{
   
}

ossimQtPropertyFontItem::~ossimQtPropertyFontItem()
{
   
}

void ossimQtPropertyFontItem::createChildren()
{
   if(childCount() > 0) return;

   ossimQtPropertyItem *i = this;
      
   ossimFontProperty* fontProperty = PTR_CAST(ossimFontProperty,
                                              getOssimProperty().get());

   if(fontProperty)
   {
      int idx = 0;
      std::vector<ossimFontInformation> fontInfoList;
      std::vector<ossimString> familyNameConstraints;
      std::vector<ossimString> styleConstraints;

      styleConstraints.push_back("normal");
      styleConstraints.push_back("bold");
      styleConstraints.push_back("italic");
      styleConstraints.push_back("bold italic");

      ossimFontFactoryRegistry::instance()->getFontInformation(fontInfoList);
      std::map<ossimString, char, ossimStringLtstr> testMap;
      for(idx = 0; idx < (int)fontInfoList.size(); ++idx)
      {
         if(testMap.find(fontInfoList[idx].theFamilyName) == testMap.end())
         {
            familyNameConstraints.push_back(fontInfoList[idx].theFamilyName);
            testMap.insert(make_pair(fontInfoList[idx].theFamilyName,
                                     '\0'));
         }
         }
         testMap.clear();

      if(fontInfoList.size())
      {
         ossimStringProperty* familyNameProperty = new ossimStringProperty("Family name",
                                                                           fontProperty->getFontInformation().theFamilyName,
                                                                           false,
                                                                           familyNameConstraints);
         
         
         i = ossimQtPropertyFactory::instance()->createProperty(theListView,
                                                                i,
                                                                this,
                                                                familyNameProperty);
         if(i)
         {
            addChild(i);
            
            ossimStringProperty* styleNameProperty =
               new ossimStringProperty("Style",
                                       fontProperty->getFontInformation().theStyleName,
                                       false,
                                       styleConstraints);
            
            ossimNumericProperty* pointSizeProperty =
               new ossimNumericProperty("Point size",
                                        ossimString::toString(fontProperty->getFontInformation().thePointSize.x));
            
            pointSizeProperty->setConstraints(8, 256);
            pointSizeProperty->setNumericType(ossimNumericProperty::ossimNumericPropertyType_INT);
            
            ossimNumericProperty* rotationProperty = new ossimNumericProperty("Rotation",
                                                                              ossimString::toString(fontProperty->getFontInformation().theRotation));
            rotationProperty->setConstraints(-180, 180);

            ossimNumericProperty* horizontalScaleProperty = new ossimNumericProperty("Horizontal scale",
                                                                                     ossimString::toString(fontProperty->getFontInformation().theScale.x));
            horizontalScaleProperty->setConstraints(-30, 30);
            horizontalScaleProperty->setNumericType(ossimNumericProperty::ossimNumericPropertyType_FLOAT32);
            
            ossimNumericProperty* verticalScaleProperty = new ossimNumericProperty("Vertical scale",
                                                                                   ossimString::toString(fontProperty->getFontInformation().theScale.y));
            verticalScaleProperty->setConstraints(-30, 30);
            verticalScaleProperty->setNumericType(ossimNumericProperty::ossimNumericPropertyType_FLOAT32);
            
            ossimNumericProperty* horizontalShearProperty = new ossimNumericProperty("Horizontal shear",
                                                                                     ossimString::toString(fontProperty->getFontInformation().theShear.x));
            horizontalShearProperty->setConstraints(-30, 30);
            horizontalShearProperty->setNumericType(ossimNumericProperty::ossimNumericPropertyType_FLOAT32);
            
            ossimNumericProperty* verticalShearProperty = new ossimNumericProperty("Vertical shear",
                                                                                   ossimString::toString(fontProperty->getFontInformation().theShear.y));
            verticalShearProperty->setConstraints(-30, 30);
            verticalShearProperty->setNumericType(ossimNumericProperty::ossimNumericPropertyType_FLOAT32);
            
            i = ossimQtPropertyFactory::instance()->createProperty(theListView,
                                                                   i,
                                                                   this,
                                                                   styleNameProperty);
            addChild(i);
            i = ossimQtPropertyFactory::instance()->createProperty(theListView,
                                                                   i,
                                                                   this,
                                                                   pointSizeProperty);
            addChild(i);
            i = ossimQtPropertyFactory::instance()->createProperty(theListView,
                                                                   i,
                                                                   this,
                                                                   rotationProperty);
            addChild(i);
            i = ossimQtPropertyFactory::instance()->createProperty(theListView,
                                                                   i,
                                                                   this,
                                                                   horizontalScaleProperty);
            addChild(i);
            i = ossimQtPropertyFactory::instance()->createProperty(theListView,
                                                                   i,
                                                                   this,
                                                                   verticalScaleProperty);
            addChild(i);
            i = ossimQtPropertyFactory::instance()->createProperty(theListView,
                                                                   i,
                                                                   this,
                                                                   horizontalShearProperty);
            addChild(i);
            i = ossimQtPropertyFactory::instance()->createProperty(theListView,
                                                                   i,
                                                                   this,
                                                                   verticalShearProperty);
            addChild(i);
            
         }
      }
   }
}

void ossimQtPropertyFontItem::initChildren()
{
   ossimQtPropertyItem *item = 0;
   ossimFontProperty* fontProperty = PTR_CAST(ossimFontProperty,
                                              getOssimProperty().get());
   if(fontProperty)
   {
      ossimFontInformation fontInfo = fontProperty->getFontInformation();
      for ( int i = 0; i < childCount(); ++i )
      {
         item = ossimQtPropertyItem::child( i );
         if(item->getOssimProperty().get())
         {
            if ( item->getOssimProperty()->getName() == "Family name")
            {
               item->setValue( QString(fontInfo.theFamilyName.c_str()) );
            }
            else if(item->getOssimProperty()->getName() == "Style")
            {
               item->setValue( QString(fontInfo.theStyleName.c_str()) );
            }
            else if(item->getOssimProperty()->getName() == "Point size")
            {
               item->setValue( QString(ossimString::toString(fontInfo.thePointSize.x).c_str()));
            }
            else if(item->getOssimProperty()->getName() == "Rotation")
            {
               item->setValue( QString(ossimString::toString(fontInfo.theRotation).c_str()));
            }
            else if(item->getOssimProperty()->getName() == "Horizontal scale")
            {
               item->setValue( QString(ossimString::toString(fontInfo.theScale.x).c_str()));
            }
            else if(item->getOssimProperty()->getName() == "Vertical scale")
            {
               item->setValue( QString(ossimString::toString(fontInfo.theScale.y).c_str()));
            }
            else if(item->getOssimProperty()->getName() == "Horizontal shear")
            {
               item->setValue( QString(ossimString::toString(fontInfo.theShear.x).c_str()));
            }
            else if(item->getOssimProperty()->getName() == "Vertical shear")
            {
               item->setValue( QString(ossimString::toString(fontInfo.theShear.y).c_str()));
            }
         }
      }
   }
}

void ossimQtPropertyFontItem::showEditor()
{
   ossimQtPropertyItem::showEditor();
   placeEditor(0);
}

void ossimQtPropertyFontItem::hideEditor()
{
    ossimQtPropertyItem::hideEditor();
}

bool ossimQtPropertyFontItem::hasSubItems() const
{
   return true;
}

void ossimQtPropertyFontItem::childValueChanged( ossimQtPropertyItem *child )
{
   ossimFontProperty* fontProperty = PTR_CAST(ossimFontProperty,
                                              getOssimProperty().get());
   
   if(!fontProperty) return;

   ossimFontInformation fontInfo = fontProperty->getFontInformation();
   
   if ( child->getOssimProperty()->getName() == "Family name")
   {
      fontInfo.theFamilyName = child->getOssimProperty()->valueToString();
   }
   else if(child->getOssimProperty()->getName() == "Style")
   {
      fontInfo.theStyleName = child->getOssimProperty()->valueToString();
   }
   else if(child->getOssimProperty()->getName() == "Point size")
   {
      double temp = child->getOssimProperty()->valueToString().toDouble();
      fontInfo.thePointSize = ossimDpt(temp, temp); 
   }
   else if(child->getOssimProperty()->getName() == "Rotation")
   {
      fontInfo.theRotation =  child->getOssimProperty()->valueToString().toDouble(); 
   }
   else if(child->getOssimProperty()->getName() == "Horizontal scale")
   {
      fontInfo.theScale.x = child->getOssimProperty()->valueToString().toDouble(); 
   }
   else if(child->getOssimProperty()->getName() == "Vertical scale")
   {
      fontInfo.theScale.y = child->getOssimProperty()->valueToString().toDouble(); 
   }
   else if(child->getOssimProperty()->getName() == "Horizontal shear")
   {
      fontInfo.theShear.x = child->getOssimProperty()->valueToString().toDouble(); 
   }
   else if(child->getOssimProperty()->getName() == "Vertical shear")
   {
      fontInfo.theShear.y = child->getOssimProperty()->valueToString().toDouble(); 
   }
   
   fontProperty->setFontInformation(fontInfo);
   
   notifyValueChange();
}

void ossimQtPropertyFontItem::resetProperty(bool notify)
{
   ossimQtPropertyItem::resetProperty(notify);

   if(isOpen())
   {
      for ( int i = 0; i < childCount(); ++i )
      {
         child(i)->resetProperty(false);
      }

      initChildren();
   }

   if(notify)
   {
      notifyValueChange();
   }

}
