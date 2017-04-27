//----------------------------------------------------------------------------
// Copyright (C) 2004 David Burken, all rights reserved.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: OSSIM QT about dialog base class.
//
// $Id: ossimQtAboutDialogBase.h 12108 2007-12-04 17:42:05Z gpotts $
//----------------------------------------------------------------------------
#ifndef ossimQtAboutDialogBase_HEADER
#define ossimQtAboutDialogBase_HEADER

#include <QtGui/QDialog>
#include <QtCore/QString>
//Added by qt3to4:
#include <Qt3Support/Q3VBoxLayout>
#include <Qt3Support/Q3HBoxLayout>
#include "ossimQtExport.h"

// Forward class declarations.
class Q3HBoxLayout;
class Q3VBoxLayout;
class QPushButton;
class QWidget;
class QComboBox;
class QCheckBox;
class Q3GroupBox;

class OSSIMQT_DLL ossimQtAboutDialogBase : public QDialog
{
   Q_OBJECT

public:

   /** public constructor */
   ossimQtAboutDialogBase(QWidget* parent = 0,
                          const char* name = "ossimQtAboutDialogBase",
                          bool modal = false,
                          Qt::WFlags f = 0 );

   /** virtual destructor */
   virtual ~ossimQtAboutDialogBase();

   /**
    * @return The current version number as a QString in the format of
    * "Version 1.5 Release 6" where 1=major, 5=minor and 6=release.
    */
   virtual QString getVersionString() const;

public slots:

   /**
    * Slot connected to theCloseButton clicked signal.
    */
   virtual void closeClicked();

protected:

   QWidget*              theParent;
   Q3VBoxLayout*          theMainVBox;
   Q3HBoxLayout*          theHBox1;
   Q3HBoxLayout*          theHBox2;
   Q3HBoxLayout*          theHBox3;
   QPushButton*          theCloseButton;

private:
   /** Hide from use copy constructor. */
   ossimQtAboutDialogBase(const ossimQtAboutDialogBase& obj);

   /** Hide from use assignment operator. */
   const ossimQtAboutDialogBase& operator=(const ossimQtAboutDialogBase& rhs);
};

#endif /* End of "#ifndef ossimQtAboutDialogBase_HEADER" */

