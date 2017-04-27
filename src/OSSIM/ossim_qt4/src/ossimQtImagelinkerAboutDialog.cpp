//----------------------------------------------------------------------------
// Copyright (C) 2004 Intelligence Data Systems, Inc. All rights reserved.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: ImageLinker about dialog.
//
// $Id: ossimQtImagelinkerAboutDialog.cpp 23664 2015-12-14 14:17:27Z dburken $
//----------------------------------------------------------------------------

#include <QtGui/QPixmap>
#include <Qt3Support/Q3HBox>
#include <QtGui/QLayout>
#include <QtGui/QLabel>
//Added by qt3to4:
#include <Qt3Support/Q3MimeSourceFactory>

#include <ossimQt/ossimQtImagelinkerAboutDialog.h>

ossimQtImagelinkerAboutDialog::ossimQtImagelinkerAboutDialog(QWidget* parent,
                                                             const char* name,
                                                             bool modal,
                                                             Qt::WFlags f)
   : ossimQtAboutDialogBase(parent, name, modal, f)
{
   setCaption("About ImageLinker");
   QLabel* pixLabel = new QLabel( this );
   pixLabel->setPixmap(qPixmapFromMimeSource("ImageLinkerSplash"));
   theHBox1->addWidget(pixLabel);

   QSpacerItem* leftSpacer = new QSpacerItem( 10,
                                              10,
                                              QSizePolicy::Expanding,
                                              QSizePolicy::Minimum );
   theHBox2->addItem(leftSpacer);
   
   QLabel* textLabel = new QLabel( this );
   QString text = "\nImageLinker (qt4)\n";
   text +=        getVersionString();
   text +=        "\nhttp://www.ossim.org\n\n";
   textLabel->setText(text);
   theHBox2->addWidget(textLabel);
   
   QSpacerItem* rightSpacer = new QSpacerItem( 10,
                                               10,
                                               QSizePolicy::Expanding,
                                               QSizePolicy::Minimum );
   
   theHBox2->addItem(rightSpacer);
}

ossimQtImagelinkerAboutDialog::~ossimQtImagelinkerAboutDialog()
{
}

ossimQtImagelinkerAboutDialog::ossimQtImagelinkerAboutDialog(const ossimQtImagelinkerAboutDialog& /* obj */ )
   : ossimQtAboutDialogBase()
{
}

const ossimQtImagelinkerAboutDialog& ossimQtImagelinkerAboutDialog::operator=(const ossimQtImagelinkerAboutDialog& /* rhs */ )
{
   return *this;
}

