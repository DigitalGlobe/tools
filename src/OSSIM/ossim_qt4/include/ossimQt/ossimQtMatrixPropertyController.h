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
// $Id: ossimQtMatrixPropertyController.h 12108 2007-12-04 17:42:05Z gpotts $
#ifndef ossimQtMatrixPropertyController_HEADER
#define ossimQtMatrixPropertyController_HEADER
#include <QtCore/QObject>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimMatrixProperty.h>

class ossimQtMatrixPropertyDialog;

class ossimQtMatrixPropertyController : public QObject
{
   Q_OBJECT
public:
   ossimQtMatrixPropertyController(ossimQtMatrixPropertyDialog* dialog);

   void setOssimProperty(ossimRefPtr<ossimMatrixProperty> prop);
   const ossimRefPtr<ossimMatrixProperty> getOssimProperty()const;

public slots:
   virtual void applyButton();
   virtual void zeroButton();
   virtual void normalizeButton();
   virtual void valueChanged(int row, int col);
   
protected:
   ossimQtMatrixPropertyDialog *theDialog;
   ossimRefPtr<ossimMatrixProperty> theProperty;

   void transferPropertyToDialog();
   void transferPropertyFromDialog();

signals:
   void apply(ossimRefPtr<ossimMatrixProperty> prop);
   void changed(ossimRefPtr<ossimMatrixProperty> prop);
};

#endif
