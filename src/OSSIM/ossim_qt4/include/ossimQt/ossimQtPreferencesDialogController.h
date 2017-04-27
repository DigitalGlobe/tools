//*******************************************************************
//
// See top level LINCENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimQtPreferencesDialogController.h 12108 2007-12-04 17:42:05Z gpotts $
#ifndef ossimQtPreferencesDialogController_HEADER
#define ossimQtPreferencesDialogController_HEADER
#include <QtCore/QObject>

class QListWidgetItem;
class ossimQtPreferencesDialog;

class ossimQtPreferencesDialogController : public QObject
{
   Q_OBJECT

public:
   ossimQtPreferencesDialogController(ossimQtPreferencesDialog* dialog);

   virtual ~ossimQtPreferencesDialogController();
   
public slots:
   virtual void close();
   virtual void addPlugin();
   virtual void removePlugin();
   virtual void reloadPlugin();
   virtual void selectAllPlugin();
   virtual void currentPluginSelectionChanged(QListWidgetItem* item);

protected:
   ossimQtPreferencesDialog* theDialog;

   void transferToDialog();

   void transferPluginToDialog();
};

#endif
