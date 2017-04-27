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
// $Id: ossimQtViewDialogController.h 12108 2007-12-04 17:42:05Z gpotts $
#ifndef ossimQtViewDialogController_HEADER
#define ossimQtViewDialogController_HEADER

#include <QtCore/QObject>
#include <ossim/base/ossimRefPtr.h>

  // #include <qstrlist.h>

class ossimQtViewDialog;
class ossimObject;
class ossimQtViewDialogController : public QObject
{
   Q_OBJECT

public:
   ossimQtViewDialogController(ossimQtViewDialog* dialog);
   virtual ~ossimQtViewDialogController();
   
   virtual void init();
   
   virtual void apply();
   virtual void propagate();
   virtual void close();

   virtual void setDatumList();
   virtual void setProjectionList();
   virtual void setView(const ossimObject* view);
   virtual void transferFromDialog();
   virtual void transferToDialog();

   virtual void transformationTypeChanged();
   
   virtual void setDefaultView();

   /**
    * @brief Method to hide and un-hide the Identity button.  This should be
    * hidden if image window is a combiner.
    *
    * @param flag If true button is hidden; else, displayed.
    */
   virtual void hideIdentityButton(bool flag);

   /**
    * @brief Method to check and uncheck Identity button.
    *
    * @param flag If true button will be check, else unchecked.
    */
   virtual void setIdentityButtonChecked(bool flag);   
   
signals:
    void propagateView();
    void applyView(const ossimObject* view);
    void setIdentityMode(bool flag);
    
public slots:
    virtual void dialogDestroyed();
    virtual void currentProjectionChanged(const QString& str);
   
protected:
    virtual void showStatePlaneInputs();
    virtual void showProjectionInputs(const QString& proj);
    virtual bool isIdentityMode() const;
    
    ossimQtViewDialog*       theViewDialog;
    ossimRefPtr<ossimObject> theViewObject;
};

#endif
