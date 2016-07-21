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
// $Id: ossimQtAdjustableParameterDialogController.h 12122 2007-12-06 13:27:09Z gpotts $
#ifndef ossimQtAdjustableParameterDialogController_HEADER
#define ossimQtAdjustableParameterDialogController_HEADER
#include <QtCore/QObject>
#include <ossim/base/ossimFilename.h>

class ossimAdjustableParameterInterface;
class ossimConnectableObject;
class ossimQtAdjustableParameterDialog;
class ossimQtAdjustableParameterDialogController : public QObject
{
  Q_OBJECT
public:
  ossimQtAdjustableParameterDialogController(ossimQtAdjustableParameterDialog* dialog);
   
   void setInformation(ossimConnectableObject* object,
                       ossimAdjustableParameterInterface* adjInterface);
   
   void setTypeToGeomAdjustment();
   void clearTypeAdjustment();
   
protected:
   ossimQtAdjustableParameterDialog*  theDialog;
   ossimConnectableObject*            theObject;
   ossimAdjustableParameterInterface* theAdjustableParameterInterface;
   bool                               theGeomAdjustmentFlag;
   ossimFilename                      theFilename;

   ossimFilename getDefaultFilename()const;          
   void transferToDialog();
   void clearAllFields();
   void clearTable();
   void transferToTable();
   void transferToList();

protected slots:
   void valueChanged(int row, int col);

public slots:
   void resetTable();
   void keepAdjustment();
   void saveAdjustment();
   void copyAdjustment();
   void deleteAdjustment();
   void adjustmentDescriptionChanged();
   void selectionListChanged();
};

#endif
