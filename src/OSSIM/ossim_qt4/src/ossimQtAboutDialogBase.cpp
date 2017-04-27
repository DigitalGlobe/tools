//----------------------------------------------------------------------------
// Copyright (C) 2004 Intelligence Data Systems, Inc. All rights reserved.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: OSSIM QT about dialog base class.
//
// $Id: ossimQtAboutDialogBase.cpp 23664 2015-12-14 14:17:27Z dburken $
//----------------------------------------------------------------------------

#include <Qt3Support/Q3VBox>
#include <Qt3Support/Q3HBox>
#include <QtGui/QLayout>
#include <QtGui/QPushButton>
#include <QtGui/QCheckBox>
#include <QtGui/QSizePolicy>
#include <QtGui/QComboBox>
// #include <qhgroupbox.h>
//Added by qt3to4:
#include <Qt3Support/Q3HBoxLayout>
#include <Qt3Support/Q3VBoxLayout>
#include <ossimQt/ossimQtAboutDialogBase.h>
#include <ossim/ossimConfig.h>
#include <ossim/base/ossimString.h>
#include <ctime>
#include <ossim/ossimVersion.h>
ossimQtAboutDialogBase::ossimQtAboutDialogBase(QWidget* parent,
                                               const char* name,
                                               bool modal,
                                               Qt::WFlags f)
   : QDialog( parent, name, modal, (f|Qt::WDestructiveClose) ),
     theParent(parent),
     theMainVBox(NULL),
     theHBox1(NULL),
     theHBox2(NULL),
     theHBox3(NULL),
     theCloseButton(NULL)
{
   setCaption("About OSSIM");

   // Main vertical box.
   theMainVBox = new Q3VBoxLayout(this);

   // First row, holds the pixmap.
   theHBox1 = new Q3HBoxLayout(theMainVBox);
   
   // Second row, holds the text.
   theHBox2 = new Q3HBoxLayout(theMainVBox);

   // Third row, holds the close button.
   theHBox3 = new Q3HBoxLayout(theMainVBox);

   QSpacerItem* leftSpacer = new QSpacerItem( 10,
                                              10,
                                              QSizePolicy::Expanding,
                                              QSizePolicy::Minimum );

   theHBox3->addItem(leftSpacer);
   
   theCloseButton = new QPushButton( this, "theCloseButton" );
   theCloseButton->setText( "Close" );
   theCloseButton->setDefault(false);
   theCloseButton->setAutoDefault(false);   
   theHBox3->addWidget(theCloseButton);

   QSpacerItem* rightSpacer = new QSpacerItem( 10,
                                               10,
                                               QSizePolicy::Expanding,
                                               QSizePolicy::Minimum );
   
   theHBox3->addItem(rightSpacer);
   
   //---
   // Connect all the signals to slots...
   //
   // NOTE:
   // If a push button is set to "default" or "auto default" it will always
   // call that slot when return is pressed in the dialog.
   // Since this isn't what I want, all defaults are set to off!
   //---
   connect( theCloseButton, SIGNAL ( clicked() ),
            this, SLOT ( closeClicked() ) );
}

ossimQtAboutDialogBase::~ossimQtAboutDialogBase()
{
}

QString ossimQtAboutDialogBase::getVersionString() const
{
   ossimString versionString = OSSIM_VERSION;//"Version ";
//   int i;
#if 0
#ifdef OSSIM_MAJOR_VERSION_NUMBER
   i = OSSIM_MAJOR_VERSION_NUMBER;
   versionString += ossimString::toString(i);
#endif

#ifdef OSSIM_MINOR_VERSION_NUMBER
   versionString += ".";
   i = OSSIM_MINOR_VERSION_NUMBER;
   versionString += ossimString::toString(i);
#endif

#ifdef OSSIM_RELEASE_NUMBER
   i = OSSIM_RELEASE_NUMBER;
   versionString += " Release ";
   versionString += ossimString::toString(i);
#endif   
#endif

#if 0
   // Add in date in the format of (yyyymmdd).
   char s[12];
   s[11] = '\0';
   time_t t;
   time(&t);
   tm* lt = localtime(&t);
   strftime(s, 12, " (%Y%m%d)", lt);
   versionString += s;
#endif
   versionString += (ossimString(" Build Date ") + OSSIM_BUILD_DATE);
   QString qs = versionString.c_str();
   
   return qs;
}

void ossimQtAboutDialogBase::closeClicked()
{
   close(true);
}

ossimQtAboutDialogBase::ossimQtAboutDialogBase(const ossimQtAboutDialogBase& /* obj */ )
   : QDialog()
{
}

const ossimQtAboutDialogBase& ossimQtAboutDialogBase::operator=(const ossimQtAboutDialogBase& /* rhs */ )
{
   return *this;
}

