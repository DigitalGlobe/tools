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
// $Id: ossimQtTopographicCorrectionDialogController.h 12110 2007-12-04 19:03:09Z gpotts $
#ifndef ossimQtTopographicCorrectionDialogController_HEADER
#define ossimQtTopographicCorrectionDialogController_HEADER
#include <QtCore/QObject>
#include <ossim/base/ossimFilename.h>

class ossimQtTopographicCorrectionDialog;
class ossimConnectableObject;
class ossimQtTopographicCorrectionDialogControllerListener;
class ossimTopographicCorrectionFilter;

class ossimQtTopographicCorrectionDialogController : public QObject
{
  Q_OBJECT
public:
  friend class ossimQtTopographicCorrectionDialogControllerListener;

  ossimQtTopographicCorrectionDialogController(ossimQtTopographicCorrectionDialog* dialog);
  
  void setObject(ossimConnectableObject* obj);

protected:
  ossimQtTopographicCorrectionDialog* theDialog;
  ossimTopographicCorrectionFilter* theObject;

  virtual void transferToDialog();
   ossimFilename getDefaultParameterFile();
   ossimFilename getLandsatHeader(const ossimFilename& imageFileName);

   void transferParametersToDialog(const std::vector<double>& gainValues,
                                   const std::vector<double>& biasValues,
                                   double sunElevation,
                                   double sunAzimuth);
public slots:
   virtual void applyChanges();
   virtual void typeActivated(const QString& type);
   virtual void importParametersButtonClicked();

  
};

#endif
