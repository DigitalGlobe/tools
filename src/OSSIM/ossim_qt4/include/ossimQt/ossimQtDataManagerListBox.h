//*******************************************************************
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimQtDataManagerListBox.h 12108 2007-12-04 17:42:05Z gpotts $
#ifndef ossimQtDataManagerListBox_HEADER
#define ossimQtDataManagerListBox_HEADER
#include <Qt3Support/Q3ListBox>
#include <vector>
#include <map>
#include <ossim/base/ossimId.h>
#include <ossim/base/ossimConnectableObject.h>
class ossimDataManager;
class ossimConnectableObject;

class ossimQtDataManagerListBox : public Q3ListBox
{
    Q_OBJECT
public:
    ossimQtDataManagerListBox(QWidget* parent=0, 
			      const char* name=0, 
			      Qt::WFlags f=0);

   ossimQtDataManagerListBox(ossimConnectableObject::ConnectableObjectList& layers,
                             QWidget* parent=0, 
                             const char* name=0, 
                             Qt::WFlags f=0);
   
   void setDataManager(ossimDataManager* dataManager);

   void getSelectedObjects(ossimConnectableObject::ConnectableObjectList& objList);
   void getSelectedObjectsAsIds(std::vector<ossimId>& idList);
   void setObjects(ossimConnectableObject::ConnectableObjectList& objList);
   ossimConnectableObject::ConnectableObjectList getSelectedObjects();
   std::vector<ossimId> getSelectedObjectsAsIds();

public slots:
  void deleteSelectedObjects();
  void duplicateSelectedObjects();
  void mosaicSelectedObjects();
  void blendSelectedObjects();
  void featherSelectedObjects();

protected:
   ossimDataManager* theDataManager;
   mutable bool      theDragStartFlag;
   virtual void contentsMousePressEvent(QMouseEvent* e);
   virtual void contentsMouseMoveEvent(QMouseEvent* e);
   void addObjects(ossimConnectableObject::ConnectableObjectList& objList);
   void addObject(ossimConnectableObject* objList);
   virtual void startDrag();
};

#endif
