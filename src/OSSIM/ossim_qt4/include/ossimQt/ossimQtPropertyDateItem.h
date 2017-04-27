//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE: LGPL see top level LICENSE.txt
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimQtPropertyDateItem.h 12122 2007-12-06 13:27:09Z gpotts $
#ifndef ossimQtPropertyDateItem_HEADER
#define ossimQtPropertyDateItem_HEADER
#include <QtCore/QPointer>
#include <Qt3Support/Q3HBox>
#include <Qt3Support/Q3DateTimeEdit>
#include "ossimQtPropertyItem.h"

class ossimQtPropertyDateItem : public ossimQtPropertyItem
{
   Q_OBJECT
public:
   ossimQtPropertyDateItem(ossimQtPropertyListView *propList,
                             ossimQtPropertyItem *after,
                             ossimQtPropertyItem *parent,
                             ossimRefPtr<ossimProperty> oProp);
   virtual ~ossimQtPropertyDateItem();
   virtual void showEditor();
   virtual void hideEditor();
   
protected:
   Q3DateTimeEdit* theDateTimeEditor;

   Q3DateTimeEdit* dateTimeEditor();

public slots:
   virtual void returnPressed();
   virtual void setValue();
   virtual void valueChanged(const QDateTime& value);
   virtual void resetProperty(bool notify = true);

};

#endif
