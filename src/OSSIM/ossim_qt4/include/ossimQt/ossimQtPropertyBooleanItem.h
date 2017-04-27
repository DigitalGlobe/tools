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
// $Id: ossimQtPropertyBooleanItem.h 12122 2007-12-06 13:27:09Z gpotts $
#ifndef ossimQtPropertyBooleanItem_HEADER
#define ossimQtPropertyBooleanItem_HEADER
#include <QtCore/QPointer>
#include <QtGui/QCheckBox>
#include "ossimQtPropertyItem.h"

class ossimQtPropertyBooleanItem : public ossimQtPropertyItem
{
   Q_OBJECT
public:
   ossimQtPropertyBooleanItem(ossimQtPropertyListView *propList,
                          ossimQtPropertyItem *after,
                          ossimQtPropertyItem *parent,
                          ossimRefPtr<ossimProperty> oProp);
   virtual ~ossimQtPropertyBooleanItem();
   
   virtual void showEditor();
   virtual void hideEditor();
   virtual void toggle();
   virtual void setValue( const QVariant &v );
   virtual void updateGUI();
   

protected slots:
   void setValue();
   virtual void resetProperty(bool notify=true);

protected:
   QCheckBox* checkBox();
   QPointer<QCheckBox> theCheckBox;
};

#endif

