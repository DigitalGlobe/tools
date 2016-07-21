//----------------------------------------------------------------------------
// Copyright (C) 2004 David Burken, all rights reserved.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Property editor dialog for ossimRLevelFilter.
//
// $Id: ossimQtRLevelFilterDialog.h 12109 2007-12-04 18:09:45Z gpotts $
//----------------------------------------------------------------------------
#ifndef ossimQtRLevelFilterDialog_HEADER
#define ossimQtRLevelFilterDialog_HEADER

#include <QtGui/QDialog>
//Added by qt3to4:
#include <Qt3Support/Q3VBoxLayout>
#include <Qt3Support/Q3HBoxLayout>
#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimObjectDestructingEvent.h>

// Forward class declarations.
class ossimRLevelFilter;
class Q3HBoxLayout;
class Q3VBoxLayout;
class QPushButton;
class QWidget;
class QComboBox;
class QCheckBox;
class Q3GroupBox;

class ossimQtRLevelFilterDialog :
   public QDialog,
   public ossimConnectableObjectListener
{
   Q_OBJECT

public:

   /** public constructor */
   ossimQtRLevelFilterDialog(QWidget* parent = 0,
                             const char* name = "ossimQtRLevelFilterDialog",
                             bool modal = false,
                             Qt::WFlags f = 0 );

   /** virtual destructor */
   virtual ~ossimQtRLevelFilterDialog();

   /**
    * Method to catch destruction event sent by the ossimRLevelFilter.
    */
   virtual void objectDestructingEvent(ossimObjectDestructingEvent& event);

   void setRLevelFilter(ossimRLevelFilter* filter);

public slots:

   /**
    * Slot connected to theOutputImageField selection changed signal.
    */
   virtual void rlevelChanged(int level);

   /**
    * Slot connected to theOutputImageButton clicked signal.
    */
   virtual void enableClicked();

   /**
    * Slot connected to theCloseButton clicked signal.
    */
   virtual void closeClicked();

protected:

   QWidget*              theParent;
   ossimRLevelFilter*    theFilter;

   Q3VBoxLayout*          theMainVBox;

   Q3HBoxLayout*          theHBox1;
   Q3GroupBox*            theRLevelGroupBox;
   QComboBox*            theRLevelMenu;

   Q3HBoxLayout*          theHBox2;
   Q3GroupBox*            theEnableGroupBox;
   QCheckBox*            theEnableCheckBox;

   Q3HBoxLayout*          theHBox3;
   QPushButton*          theCloseButton;

private:
   /** Hide from use copy constructor. */
   ossimQtRLevelFilterDialog(const ossimQtRLevelFilterDialog& obj);

   /** Hide from use assignment operator. */
   const ossimQtRLevelFilterDialog& operator=(const ossimQtRLevelFilterDialog& rhs);

   void buildRLevelMenu()  const;
   void updateEnableMenu() const;
   void updateRLevelMenu() const;
};

#endif /* End of "#ifndef ossimQtRLevelFilterDialog_HEADER" */

