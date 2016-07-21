//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimQtPropertyBooleanItem.cpp 23664 2015-12-14 14:17:27Z dburken $
#include <QtCore/QVariant>
#include <ossimQt/ossimQtPropertyBooleanItem.h>
#include <ossimQt/ossimQtPropertyListView.h>
#include <ossim/base/ossimProperty.h>
#include <ossim/base/ossimPropertyInterface.h>

ossimQtPropertyBooleanItem::ossimQtPropertyBooleanItem(ossimQtPropertyListView *propList,
                                                       ossimQtPropertyItem *after,
                                                       ossimQtPropertyItem *parent,
                                                       ossimRefPtr<ossimProperty> oProp)
   :ossimQtPropertyItem(propList, after, parent, oProp)
{
   theCheckBox = 0;
   if(getOssimProperty().valid())
   {
      if(getOssimProperty()->valueToString().toBool())
      {
         setText( 1, "True");
      }
      else
      {
         setText( 1, "False");
      }
   }
   else
   {
      setText( 1, "");
   }
}

ossimQtPropertyBooleanItem::~ossimQtPropertyBooleanItem()
{
   if(theCheckBox)
   {
      delete theCheckBox;
      theCheckBox = 0;
   }
}

void ossimQtPropertyBooleanItem::toggle()
{
    bool b = value().toBool();
    setValue( QVariant( !b, 0 ) );
    setValue();
}

void ossimQtPropertyBooleanItem::showEditor()
{
   ossimQtPropertyItem::showEditor();
   if(!theCheckBox)
   {
      checkBox()->blockSignals( TRUE );
      checkBox()->setChecked(getOssimProperty()->valueToString().toBool());
      checkBox()->blockSignals( FALSE );
   }
   placeEditor(checkBox());
   if ( !checkBox()->isVisible()  || !checkBox()->hasFocus() )
   {
      checkBox()->show();
      checkBox()->setFocus();
   }
}

void ossimQtPropertyBooleanItem::hideEditor()
{
   ossimQtPropertyItem::hideEditor();
   checkBox()->hide();
}


void ossimQtPropertyBooleanItem::setValue( const QVariant &v )
{
   if ( ( !hasSubItems() || !isOpen() )
        && value() == v )
   {
      return;
   }
   
   if ( theCheckBox )
   {
      checkBox()->blockSignals( TRUE );
      checkBox()->setChecked(v.toBool());
      checkBox()->blockSignals( FALSE );
   }
   if(v.toBool())
   {
      setText( 1,  "True");
   }
   else
   {
      setText( 1,  "False");
   }
   ossimQtPropertyItem::setValue( v );
  
}

void ossimQtPropertyBooleanItem::setValue()
{
   if ( !theCheckBox )
   {
      return;
   }
   if(theCheckBox->isChecked())
   {
      setText( 1, "True" );
   }
   else
   {
      setText( 1, "False" );
   }
   ossimQtPropertyItem::setValue( QVariant( theCheckBox->isChecked(), 0 ) );
   notifyValueChange();
}

QCheckBox* ossimQtPropertyBooleanItem::checkBox()
{
   if(theCheckBox)
   {
      return theCheckBox;
   }
   
   theCheckBox = new QCheckBox( theListView->viewport() );
   connect( theCheckBox, SIGNAL( clicked() ),
            this, SLOT( setValue() ) );
   theCheckBox->installEventFilter( theListView );

   return theCheckBox;
}

void ossimQtPropertyBooleanItem::resetProperty(bool notify)
{
   ossimQtPropertyItem::resetProperty(notify);

   if(getOssimProperty().valid())
   {
      bool value = getOssimProperty()->valueToString().toBool();
      
      if(value)
      {
         setText( 1, "True" );
      }
      else
      {
         setText( 1, "False" );
      }
      if(theCheckBox)
      {
         theCheckBox->setChecked(value);
      }
   }

}

void ossimQtPropertyBooleanItem::updateGUI()
{
   if (theMaker && getOssimProperty().valid())
   {
      // Get the current text in the GUI.
      bool guiValue = value().toBool();

      // Get a new "ossimProperty" from the maker.
      ossimString name = getOssimProperty()->getName();
      theProperty = theMaker->getProperty(name);
      
      if(getOssimProperty().valid())
      {
         // Get the current property value.
         bool propertyValue = getOssimProperty()->valueToString().toBool();
      
         if (guiValue != propertyValue)
         {
            // GUI diffs from current property so update the GUI.
            if (propertyValue)
            { 
               setText( 1, "True");
            }
            else
            {
               setText( 1, "False");
            }
            
            if(checkBox())
            {
               
               checkBox()->blockSignals( TRUE );
               checkBox()->setChecked(propertyValue);
               checkBox()->blockSignals( FALSE );
            }
         }
      }
   }
}

