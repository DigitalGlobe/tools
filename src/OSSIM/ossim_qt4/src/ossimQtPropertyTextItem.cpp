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
// $Id: ossimQtPropertyTextItem.cpp 12141 2007-12-07 18:05:13Z gpotts $
#include <QtGui/QLineEdit>
//Added by qt3to4:
#include <Qt3Support/Q3Frame>
#include <ossim/base/ossimTextProperty.h>
#include <ossimQt/ossimQtPropertyTextItem.h>
#include <ossimQt/ossimQtPropertyListView.h>

ossimQtPropertyTextItem::ossimQtPropertyTextItem(ossimQtPropertyListView *propList,
                                                 ossimQtPropertyItem *after,
                                                 ossimQtPropertyItem *parent,
                                                 ossimRefPtr<ossimProperty> oProp)
   :ossimQtPropertyItem(propList, after, parent, oProp)
{
   theLineEditor = (QLineEdit*)NULL;
   theBox        = 0;
   thePushButton = 0;
   if(getOssimProperty().valid())
   {
      setText( 1, getOssimProperty()->valueToString().c_str());
   }
   else
   {
      setText( 1, "");
   }
   
}

ossimQtPropertyTextItem::~ossimQtPropertyTextItem()
{
   if(theLineEditor)
   {
      delete theLineEditor;
      theLineEditor = NULL;
   }

   if(theBox)
   {
      delete theBox;
      theBox = NULL;
   }
}

void ossimQtPropertyTextItem::showEditor()
{
   if(!getOssimProperty()) return;
   
    ossimQtPropertyItem::showEditor();
    if ( !theLineEditor || theLineEditor->text().length() == 0 )
    {
       lineEditor()->blockSignals( TRUE );
       lineEditor()->setText( getOssimProperty()->valueToString().c_str() );
       lineEditor()->blockSignals( FALSE );
    }

    QWidget* w;
    if(hasMultiLines())
    {
       w = theBox;
    }
    else
    {
       w = lineEditor();
    }
    
    placeEditor( w );
    if ( !w->isVisible() || !lineEditor()->hasFocus() )
    {
       w->show();
       w->setFocus();
    }
}

void ossimQtPropertyTextItem::hideEditor()
{
    ossimQtPropertyItem::hideEditor();
    QWidget* w;
    
    if ( hasMultiLines() )
    {
       w = theBox;
    }
    else
    {
       w = lineEditor();
    }
    
    w->hide();
}

QLineEdit *ossimQtPropertyTextItem::lineEditor()
{
    if ( theLineEditor )
       return theLineEditor;

    bool multiLine = hasMultiLines();
    if ( multiLine )
    {
       theBox = new Q3HBox( theListView->viewport() );
       theBox->setFrameStyle( Q3Frame::StyledPanel | Q3Frame::Sunken );
       theBox->setLineWidth( 2 );
       theBox->hide();
    }

    if ( multiLine )
    {
       theLineEditor = new QLineEdit( theBox );
    }
    else
    {
       theLineEditor = new QLineEdit( theListView->viewport() );
    }
    if(!multiLine)
    {
       theLineEditor->hide();
    }
    else
    {
       thePushButton = new QPushButton( "...", theBox );
//       setupStyle( button );
       thePushButton->setFixedWidth( 20 );
       connect( thePushButton, SIGNAL( clicked() ),
                this, SLOT( getText() ) );
       theLineEditor->setFrame( FALSE );
       
    }
    
    connect( theLineEditor, SIGNAL( returnPressed() ),
	     this, SLOT( setValue() ) );

    theLineEditor->setReadOnly(getOssimProperty()->isReadOnly());
    
    return theLineEditor;
   
}

void ossimQtPropertyTextItem::setValue()
{
   
   if(getOssimProperty().valid())
   {
      if(theLineEditor->text() == getOssimProperty()->valueToString().c_str())
      {
         return;
      }
      getOssimProperty()->setValue(theLineEditor->text().ascii());
      ossimString value;
      getOssimProperty()->valueToString(value);
      setText( 1, value.c_str());
      notifyValueChange();
   }
}

void ossimQtPropertyTextItem::getText()
{
//   bool richText = false;
//   bool doWrap   = false;
//    QString txt = MultiLineEditor::getText( theListView,
//                                            value().toString(),
//                                            richText,
//                                            &doWrap );
//    if ( !txt.isEmpty() )
//    {
//       setText( 1, txt );
//       getOssimProperty()->setValue(txt.ascii());
//       notifyValueChange();
//       lineEditor()->blockSignals( TRUE );
//       lineEditor()->setText( txt );
//       lineEditor()->blockSignals( FALSE );
//    }
}


bool ossimQtPropertyTextItem::hasMultiLines()const
{
   const ossimTextProperty* textProp = PTR_CAST(ossimTextProperty, getOssimProperty().get());

   if(textProp)
   {
      return textProp->isMulitLine();
   }

   return false;
}

void ossimQtPropertyTextItem::resetProperty(bool notify)
{
   ossimQtPropertyItem::resetProperty(notify);
   setText( 1, getOssimProperty()->valueToString().c_str());
   if(theLineEditor)
   {
      theLineEditor->setText(getOssimProperty()->valueToString().c_str());
   }
}
