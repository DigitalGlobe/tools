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
// $Id: ossimQtPropertyStringListItem.cpp 12141 2007-12-07 18:05:13Z gpotts $
#include <Qt3Support/Q3HBox>
#include <QtCore/QVariant>
//Added by qt3to4:
#include <Qt3Support/Q3Frame>
#include <ossimQt/ossimQtPropertyListView.h>
#include <ossimQt/ossimQtPropertyStringListItem.h>
#include <ossim/base/ossimStringListProperty.h>

#include <ossimQt/ossimQtStringListPropertyDialog.h>
#include <ossimQt/ossimQtStringListPropertyController.h>

ossimQtPropertyStringListItem::ossimQtPropertyStringListItem(ossimQtPropertyListView *propList,
                                                     ossimQtPropertyItem *after,
                                                     ossimQtPropertyItem *parent,
                                                     ossimRefPtr<ossimProperty> oProp)
   :ossimQtPropertyItem(propList, after, parent, oProp),
    theBox(0),
    theButton(0),
    theStringListDialog(0)
{
    theBox = new Q3HBox( theListView->viewport() );
    theBox->hide();
    theBox->setFrameStyle( Q3Frame::StyledPanel | Q3Frame::Sunken );
    theBox->setLineWidth( 2 );
    theButton = new QPushButton( "...", theBox );
    theButton->setFixedWidth( 20 );

    connect( theButton, SIGNAL( clicked() ),
             this, SLOT( getStringList() ) );
}

ossimQtPropertyStringListItem::~ossimQtPropertyStringListItem()
{
   if(theBox)
   {
      delete theBox;
      theBox = 0;
   }
   if(theStringListDialog)
   {
      delete theStringListDialog;
      theStringListDialog = 0;
   }
}

void ossimQtPropertyStringListItem::showEditor()
{
   ossimQtPropertyItem::showEditor();
   placeEditor( theBox );
   
   if ( !theBox->isVisible() )
   {
      theBox->show();
   }
}

void ossimQtPropertyStringListItem::hideEditor()
{
   ossimQtPropertyItem::hideEditor();
   theBox->hide();
}

void ossimQtPropertyStringListItem::resetProperty(bool notify)
{
   ossimQtPropertyItem::resetProperty(notify);

   if(theStringListDialog)
   {
      theStringListDialog->controller()->setOssimProperty(PTR_CAST(ossimStringListProperty,
                                                               getOssimProperty().get()));
   }
}

void ossimQtPropertyStringListItem::getStringList()
{
   if(!theStringListDialog)
   {
      theStringListDialog = new ossimQtStringListPropertyDialog(theListView->viewport(),
                                                        "StringList dialog",
                                                        false,
                                                        Qt::WDestructiveClose);
      connect(theStringListDialog->controller(),
              SIGNAL(apply(ossimStringListProperty*)),
              this, SLOT(stringListChanged(ossimStringListProperty*)));
      connect(theStringListDialog->controller(),
              SIGNAL(changed(ossimStringListProperty*)),
              this, SLOT(stringListChanged(ossimStringListProperty*)));
   }
   theStringListDialog->controller()->setOssimProperty(PTR_CAST(ossimStringListProperty,
                                                            getOssimProperty().get()));

   theStringListDialog->show();
}

void ossimQtPropertyStringListItem::stringListChanged(ossimStringListProperty* prop)
{
   if(getOssimProperty().valid())
   {
      if(getOssimProperty()->valueToString() != ((ossimProperty*)prop)->valueToString())
      {
         getOssimProperty()->assign(*prop);

         notifyValueChange();
      }
   }
}

