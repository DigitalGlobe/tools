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
// $Id: ossimQtPropertyDialogController.h 1972 2011-01-05 00:13:42Z greg.schmidt $
#ifndef ossimQtPropertyDialogController_HEADER
#define ossimQtPropertyDialogController_HEADER
#include <QtCore/QObject>
class ossimQtPropertyDialog;
class ossimObject;
class ossimQtPropertyDialogControllerPrivateListener;

class ossimQtPropertyDialogController : public QObject
{
   Q_OBJECT
public:
   friend class ossimQtPropertyDialogControllerPrivateListener;
   
   ossimQtPropertyDialogController(ossimQtPropertyDialog* dialog);
   virtual ~ossimQtPropertyDialogController();

   virtual void setObject(ossimObject* obj);
   
protected:
   ossimQtPropertyDialog* theDialog;
   ossimObject*           theObject;
   ossimQtPropertyDialogControllerPrivateListener* thePrivateListener;
public slots:
   virtual void closeButton();
   virtual void executeButton();
   virtual void resetButton();
   virtual void applyButton();
   virtual void autoApplyToggledButton(bool value);
};

#endif

