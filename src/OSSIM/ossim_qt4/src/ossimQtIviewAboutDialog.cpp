//----------------------------------------------------------------------------
// Copyright (C) 2004 David Burken, All rights reserved.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: ImageLinker about dialog.
//
// $Id: ossimQtIviewAboutDialog.cpp 23664 2015-12-14 14:17:27Z dburken $
//----------------------------------------------------------------------------

#include <QtGui/QPixmap>
#include <Qt3Support/Q3HBox>
#include <QtGui/QLayout>
#include <QtGui/QLabel>
//Added by qt3to4:
#include <Qt3Support/Q3MimeSourceFactory>

#include <ossimQt/ossimQtIviewAboutDialog.h>

ossimQtIviewAboutDialog::ossimQtIviewAboutDialog(QWidget* parent,
                                                 const char* name,
                                                 bool modal,
                                                 Qt::WFlags f)
   : ossimQtAboutDialogBase(parent, name, modal, f)
{
   setCaption("About iview");
   QLabel* pixLabel = new QLabel( this );
   pixLabel->setPixmap(qPixmapFromMimeSource("iviewsplash"));
   theHBox1->addWidget(pixLabel);

   QSpacerItem* leftSpacer = new QSpacerItem( 10,
                                              10,
                                              QSizePolicy::Expanding,
                                              QSizePolicy::Minimum );
   theHBox2->addItem(leftSpacer);
   
   QLabel* textLabel = new QLabel( this );
   QString text = "\n             iview      \n";
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

ossimQtIviewAboutDialog::~ossimQtIviewAboutDialog()
{
}

ossimQtIviewAboutDialog::ossimQtIviewAboutDialog(const ossimQtIviewAboutDialog& /* obj */ )
   : ossimQtAboutDialogBase()
{
}

const ossimQtIviewAboutDialog& ossimQtIviewAboutDialog::operator=(const ossimQtIviewAboutDialog& /* rhs */ )
{
   return *this;
}

