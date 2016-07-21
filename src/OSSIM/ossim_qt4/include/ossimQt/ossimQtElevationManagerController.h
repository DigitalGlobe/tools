//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
// 
// Description:
// Main interface between the elevation manager and the elevation manager
// dialog box.
//
//*************************************************************************
// $Id: ossimQtElevationManagerController.h 10334 2007-01-21 15:04:48Z dburken $

#ifndef ossimQtElevationManagerController_HEADER
#define ossimQtElevationManagerController_HEADER

#include <ossimQt/ossimQtDatumConverterDialog.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimElevationManagerEventListener.h>
#include <ossim/base/ossimElevationManagerEvent.h>

// Forward class declarations.
class ossimElevManager;
class ossimQtElevationManagerDialog;
class QListWidgetItem;

class ossimQtElevationManagerController : public ossimElevationManagerEventListener
{
public:

   enum
   {
      ADD_BUTTON,
      ADD_DIRECTORY_BUTTON,
      REMOVE_BUTTON,
      REMOVE_ALL_BUTTON,
      UP_BUTTON,
      DOWN_BUTTON,
      TOP_BUTTON,
      BOTTOM_BUTTON,
      CLOSE_BUTTON,
      ENABLE_BUTTON,
      AUTO_LOAD_ENABLE_BUTTON,
      AUTO_SORT_ENABLE_BUTTON,
      DEFAULT_ELEV_SPINBOX,
      MOSAIC_BUTTON
   };

   ossimQtElevationManagerController(ossimQtElevationManagerDialog* dialog);
   virtual ~ossimQtElevationManagerController();

   virtual void processEvent(ossimElevationManagerEvent&) const;

   void onAdd(const QList<QListWidgetItem *>& cells) const;
   void onAdd(const QStringList& cells) const;
   void onAddDirectory(const QString& directory) const;
   void onRemove(const QList<QListWidgetItem *>& cells) const;
   void onRemoveAll() const;
   void onUp(const QList<QListWidgetItem *>& cells) const;
   void onDown(const QList<QListWidgetItem *>& cells) const;
   void onTop(const QList<QListWidgetItem *>& cells) const;
   void onBottom(const QList<QListWidgetItem *>& cells) const;
   void onEnable(bool flag) const;
   void onAutoLoadEnable(bool flag) const;
   void onAutoSortEnable(bool flag) const;
   void onElevOffsetChanged(double value) const;
   void onClose(const QList<QListWidgetItem *>& cells) const;
   void onMosaic(const QList<QListWidgetItem *>& cells) const;

   static ossimFilename getLatestOpenCellPath();
   double getElevOffset();

private:
   void update() const;
   void updateListBox() const;
   void updateEnableBox() const;
   void updateAutoLoadEnableBox() const;
   void updateAutoSortEnableBox() const;

   // Disallow copy  constructor, assignment operator.
   ossimQtElevationManagerController( const ossimQtElevationManagerController&);
   ossimQtElevationManagerController& operator=( const ossimQtElevationManagerController&);
   
   ossimQtElevationManagerDialog* theDialog;
   ossimElevManager*              theMgr;

   // Static data member to capture the last directory.
   static ossimFilename theLatestOpenCellPath;
};

#endif
