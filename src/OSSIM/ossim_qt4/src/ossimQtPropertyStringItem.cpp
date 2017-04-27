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
// $Id: ossimQtPropertyStringItem.cpp 12141 2007-12-07 18:05:13Z gpotts $
#include <ossimQt/ossimQtPropertyStringItem.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossimQt/ossimQtPropertyListView.h>
#include <QtCore/QStringList>

ossimQtPropertyStringItem::ossimQtPropertyStringItem(ossimQtPropertyListView *propList,
                                                     ossimQtPropertyItem *after,
                                                     ossimQtPropertyItem *parent,
                                                     ossimRefPtr<ossimProperty> oProp)
   :ossimQtPropertyItem(propList, after, parent, oProp),
    theSelectionList(0),
    theLineEditor(0)
{
   if(getOssimProperty().valid())
   {
      setText( 1, getOssimProperty()->valueToString().c_str());
   }
   else
   {
      setText( 1, "");
   }
}

ossimQtPropertyStringItem::~ossimQtPropertyStringItem()
{
   if(theSelectionList)
   {
      delete theSelectionList;
      theSelectionList = 0;
   }

   if(theLineEditor)
   {
      delete theLineEditor;
      theLineEditor = 0;
   }
}

void ossimQtPropertyStringItem::showEditor()
{
   if(!getOssimProperty()) return;
   
   ossimQtPropertyItem::showEditor();
   ossimStringProperty* stringProp = PTR_CAST(ossimStringProperty,
                                              getOssimProperty().get());
   if(stringProp)
   {
      if(stringProp->hasConstraints())
      {
         placeEditor(comboBox());
         comboBox()->blockSignals(TRUE);
         comboBox()->setCurrentText(getOssimProperty()->valueToString().c_str());
         comboBox()->blockSignals(FALSE);
         
         comboBox()->show();
         comboBox()->setFocus();
      }
      else
      {
         placeEditor(lineEditor());
         lineEditor()->blockSignals(TRUE);
         lineEditor()->setText(getOssimProperty()->valueToString().c_str());
         lineEditor()->blockSignals(FALSE);
         lineEditor()->show();
         lineEditor()->setFocus();
      }
   }
}

void ossimQtPropertyStringItem::hideEditor()
{
   ossimQtPropertyItem::hideEditor();
   ossimStringProperty* stringProp = PTR_CAST(ossimStringProperty,
                                              getOssimProperty().get());
   if(stringProp)
   {
      if(stringProp->hasConstraints())
      {
         comboBox()->hide();
      }
      else
      {
         lineEditor()->hide();
      }
   }
   
}
   
QLineEdit* ossimQtPropertyStringItem::lineEditor()
{
   if(theLineEditor)
   {
      return theLineEditor;
   }

   theLineEditor = new QLineEdit( theListView->viewport() );
   theLineEditor->hide();
   connect( theLineEditor, SIGNAL( returnPressed() ),
            this, SLOT( returnPressed() ) );
   return theLineEditor;
}

QComboBox* ossimQtPropertyStringItem::comboBox()
{
   if(theSelectionList)
   {
      return theSelectionList;
   }
   if(getOssimProperty().valid())
   {
      ossimStringProperty* stringProp = PTR_CAST(ossimStringProperty,
                                                 getOssimProperty().get());
      if(stringProp)
      {
         
         const std::vector<ossimString>& constraintList = stringProp->getConstraints();
         
         QStringList strList;
         int idx = 0;
         for(idx = 0; idx < (int)constraintList.size();++idx)
         {
            strList.push_back(constraintList[idx].c_str());
         }
         theSelectionList = new QComboBox(theListView);
         theSelectionList->insertStringList(strList);
	 theSelectionList->setEditable(stringProp->isEditable());
         theSelectionList->setEnabled(!stringProp->getReadOnlyFlag());
	 connect(theSelectionList,
		 SIGNAL(activated(const QString& )),
		 this,
		 SLOT(activated(const QString&)));
      }
   }

   return theSelectionList;
}

void ossimQtPropertyStringItem::returnPressed()
{
   setValue();
}

void ossimQtPropertyStringItem::setValue()
{
  QString value;
  if(theSelectionList)
    {
      value = theSelectionList->currentText();

    }
  else if(theLineEditor)
    {
      value = theLineEditor->text();
    }
   if(getOssimProperty().valid())
   {
      getOssimProperty()->setValue(value.ascii());

      setText(1, value);
      notifyValueChange();
   }
}

void ossimQtPropertyStringItem::resetProperty(bool notify)
{
   ossimQtPropertyItem::resetProperty(notify);
   setText( 1, getOssimProperty()->valueToString().c_str());

   if(theLineEditor)
   {
       lineEditor()->blockSignals(TRUE);
       lineEditor()->setText(getOssimProperty()->valueToString().c_str());
       lineEditor()->blockSignals(FALSE);
   }
   else if(theSelectionList)
   {
      comboBox()->blockSignals(TRUE);
      comboBox()->setCurrentText(getOssimProperty()->valueToString().c_str());
      comboBox()->blockSignals(FALSE);
   }
}

void ossimQtPropertyStringItem::activated(const QString& value)
{
   if(getOssimProperty().valid())
   {
      setText(1, value);
      getOssimProperty()->setValue(value.ascii());
      notifyValueChange();
   }
}
