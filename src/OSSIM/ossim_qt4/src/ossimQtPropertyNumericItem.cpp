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
// $Id: ossimQtPropertyNumericItem.cpp 12141 2007-12-07 18:05:13Z gpotts $
#include <QtGui/QLineEdit>
#include <QtGui/QValidator>
#include <QtCore/QVariant>
#include <ossim/base/ossimNumericProperty.h>
#include <ossimQt/ossimQtPropertyNumericItem.h>
#include <ossimQt/ossimQtPropertyListView.h>

ossimQtPropertyNumericItem::ossimQtPropertyNumericItem(ossimQtPropertyListView *propList,
                                                       ossimQtPropertyItem *after,
                                                       ossimQtPropertyItem *parent,
                                                       ossimRefPtr<ossimProperty> oProp)
   :ossimQtPropertyItem(propList, after, parent, oProp)
{
   theLineEditor = (QLineEdit*)NULL;
   if(getOssimProperty().valid())
   {
      setText( 1, getOssimProperty()->valueToString().c_str());
   }
   else
   {
      setText( 1, "");
   }
}

ossimQtPropertyNumericItem::~ossimQtPropertyNumericItem()
{
   if(theLineEditor)
   {
      delete theLineEditor;
      theLineEditor = NULL;
   }
}

void ossimQtPropertyNumericItem::showEditor()
{
   if(!getOssimProperty()) return;
   
    ossimQtPropertyItem::showEditor();
    if ( !theLineEditor || theLineEditor->text().length() == 0 )
    {
       lineEditor()->blockSignals( TRUE );
       lineEditor()->setText( getOssimProperty()->valueToString().c_str() );
       lineEditor()->blockSignals( FALSE );
    }

    placeEditor( lineEditor() );
    if ( !lineEditor()->isVisible() || !lineEditor()->hasFocus() )
    {
       lineEditor()->show();
       lineEditor()->setFocus();
    }
}

void ossimQtPropertyNumericItem::hideEditor()
{
    ossimQtPropertyItem::hideEditor();
    
    lineEditor()->hide();
}

void ossimQtPropertyNumericItem::setValue(const QVariant& value)
{
   if(!getOssimProperty()) return;
   
   ossimQtPropertyItem::setValue(value);
   getOssimProperty()->setValue(value.toString().ascii());
   setText(1, value.toString());
   lineEditor()->setText(value.toString());
}

QLineEdit *ossimQtPropertyNumericItem::lineEditor()
{
    if ( theLineEditor )
       return theLineEditor;

    theLineEditor = new QLineEdit( theListView->viewport());
    theLineEditor->hide();

    ossimNumericProperty* numericProperty = PTR_CAST(ossimNumericProperty,
                                                     getOssimProperty().get());
    if(numericProperty)
    {
       QValidator* validator = NULL;
       switch(numericProperty->getNumericType())
       {
       case ossimNumericProperty::ossimNumericPropertyType_INT:
       case ossimNumericProperty::ossimNumericPropertyType_UINT:
       {
          if(numericProperty->hasConstraints())
          {
             validator = new QIntValidator((int)numericProperty->getMinValue(),
                                           (int)numericProperty->getMaxValue(),
                                           theLineEditor);
          }
          else
          {
             validator = new QIntValidator(theLineEditor);
          }
          break;
       }
       case ossimNumericProperty::ossimNumericPropertyType_FLOAT32:
       {
          if(numericProperty->hasConstraints())
          {
             validator = new QDoubleValidator(numericProperty->getMinValue(),
                                              numericProperty->getMaxValue(),
                                              8,
                                              theLineEditor);
          }
          else
          {
             validator = new QDoubleValidator(theLineEditor);
          }
          break;
       }
       case ossimNumericProperty::ossimNumericPropertyType_FLOAT64:
       {
          if(numericProperty->hasConstraints())
          {
             validator = new QDoubleValidator(numericProperty->getMinValue(),
                                              numericProperty->getMaxValue(),
                                              15,
                                              theLineEditor);
          }
          else
          {
             validator = new QDoubleValidator(theLineEditor);
          }
          break;
       }
       }
       if(validator)
       {
          theLineEditor->setValidator(validator);
       }
    }
    
    connect(theLineEditor, SIGNAL( returnPressed()),
            this, SLOT( setValue() ) );
    
    return theLineEditor;
}

void ossimQtPropertyNumericItem::setValue()
{
   if(theLineEditor->text() == getOssimProperty()->valueToString().c_str())
   {
      return;
   }
   
   if(getOssimProperty().valid())
   {
      getOssimProperty()->setValue(theLineEditor->text().ascii());

      setText( 1, theLineEditor->text().ascii());
      notifyValueChange();
   }
}

void ossimQtPropertyNumericItem::resetProperty(bool notify)
{
   ossimQtPropertyItem::resetProperty(notify);
   setText( 1, getOssimProperty()->valueToString().c_str());
   if(theLineEditor)
   {
      theLineEditor->setText(getOssimProperty()->valueToString().c_str());
   }
}
