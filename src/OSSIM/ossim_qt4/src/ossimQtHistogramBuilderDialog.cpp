//----------------------------------------------------------------------------
// Copyright (C) 2005 David Burken, all rights reserved.
//
// License:   See LICENSE.txt file in the top level directory.
//
// Author:  David Burken
//
// Description:  Dialog for building image histograms.
//
// $Id: ossimQtHistogramBuilderDialog.cpp 18007 2010-08-30 18:38:20Z gpotts $
//----------------------------------------------------------------------------

#include <Qt3Support/Q3GroupBox>
// #include <qhgroupbox.h>
#include <Qt3Support/Q3VBox>
#include <Qt3Support/Q3HBox>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QPushButton>
#include <QtGui/QMessageBox>
//Added by qt3to4:
#include <Qt3Support/Q3HBoxLayout>
#include <Qt3Support/Q3VBoxLayout>

#include <ossimQt/ossimQtHistogramBuilderDialog.h>
#include <ossimQt/ossimQtPropertyDialog.h>
#include <ossimQt/ossimQtPropertyDialogController.h>
#include <ossimQt/ossimQtProgressDialog.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimHistogramWriter.h>
#include <ossim/imaging/ossimImageHistogramSource.h>
#include <ossim/imaging/ossimImageHandler.h>

ossimQtHistogramBuilderDialog::ossimQtHistogramBuilderDialog(
   QWidget* parent,
   ossimImageHandler* imageHandler)
   :
      QDialog(parent,
              "ossimQtHistogramBuilderDialog",
              true),
      theImageHandler(imageHandler),
      theWriter(NULL),
      theMainVBox(NULL),
     
      theHBox1(NULL),
      theSourceImageGroupBox(NULL),         
      theSourceImageLabel(NULL),
      
      theHBox2(NULL),
      theSpacer1(NULL), 
      theBuildButton(NULL),
      theCloseButton(NULL)
{
   setCaption("Build Histograms");

   if (!theImageHandler)
   {
      close();
   }
   
   // Main vertical box.
   theMainVBox = new Q3VBoxLayout(this);

   // First row, display source image file name.
   theHBox1 = new Q3HBoxLayout(theMainVBox);
   
   theSourceImageGroupBox = new Q3GroupBox(1,
                                          Qt::Horizontal,
                                          "source image",
                                          this,
                                          "theSourceImageGroupBox");
   theSourceImageGroupBox->setAlignment(Qt::AlignHCenter);
   QString qs = theImageHandler->getFilename().c_str();
   theSourceImageLabel = new QLabel(qs,
                                    theSourceImageGroupBox,
                                    "theSourceImageLabel");
   theHBox1->addWidget(theSourceImageGroupBox);
   // End of first row.

   // Second row.
   theHBox2 = new Q3HBoxLayout(theMainVBox);

   // theSpacer1 = new QSpacerItem(-1, -1);
   
   theBuildButton = new QPushButton( this, "theBuildButton" );
   theBuildButton->setText( "build" );
   theBuildButton->setDefault(false);
   theBuildButton->setAutoDefault(false);   
   
   theCloseButton = new QPushButton( this, "theCloseButton" );
   theCloseButton->setText( "Close" );
   theCloseButton->setDefault(false);
   theCloseButton->setAutoDefault(false);   

   // theHBox2->addWidget(theSpacer1);
   theHBox2->addWidget(theBuildButton);
   theHBox2->addWidget(theCloseButton);

   //---
   // Connect all the signals to slots...
   //
   // NOTE:
   // If a push button is set to "default" or "auto default" it will always
   // call that slot when return is pressed in the dialog.
   // Since this isn't what I want all defaults are set to off!
   //---
   connect( theBuildButton, SIGNAL ( clicked() ),
            this, SLOT ( buildClicked() ) );
   
   connect( theCloseButton, SIGNAL ( clicked() ),
            this, SLOT ( closeClicked() ) );
}

ossimQtHistogramBuilderDialog::~ossimQtHistogramBuilderDialog()
{
}

void ossimQtHistogramBuilderDialog::buildClicked()
{
   if ( !theImageHandler.valid() )
   {
      return;
   }

   // Set up the histogram source.
   ossimRefPtr<ossimImageHistogramSource> histoSource =
      new ossimImageHistogramSource;
   histoSource->setMaxNumberOfRLevels(1);
   histoSource->connectMyInputTo(0, theImageHandler.get());
   histoSource->enableSource();

   // Set up the histogram writer.
   theWriter = new ossimHistogramWriter;
   theWriter->connectMyInputTo(0, histoSource.get());
   ossimFilename histoFile =
      theImageHandler->createDefaultHistogramFilename();
   theWriter->setFilename(histoFile);

   // Make a progress dialog.
   ossimQtProgressDialog* pd = new ossimQtProgressDialog("");
   pd->setMinimumDuration(250); // Update 4 times a second.

   //---
   // Connect the progress dialog's signal "canceled()" up to our slot
   // "saveCanceled()" so that we can tell the writer to abort.
   //---
   connect( pd, SIGNAL(canceled()), this, SLOT(abortClicked()) );
   
   theWriter->addListener(pd); // Progress dialog listens to writer...

   // Set up the progress dialog...
   QString qs = "Processing file ";
   qs += histoFile.c_str();
   pd->setLabelText(qs);
   pd->show();
   
   // Process the tile...
   theWriter->execute();

   // Check for cancelation.
   if (pd->wasCanceled())
   {
      pd->close();

      int status = remove(histoFile.c_str()); // 0 == success, -1 failure
         
      QString caption = "Processing of file aborted!";
      QString text = "File:  ";
      text += histoFile.c_str();
      if (status == 0)
      {
         text += "\nFile removed successfully...";
      }

      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );
   }

   theWriter->removeListener(pd);

   // Cleanup...
   delete pd;
   pd = NULL;
   theWriter = NULL;

   close();
   
}

void ossimQtHistogramBuilderDialog::abortClicked()
{
   if (theWriter.valid())
   {
      theWriter->abort();
   }
}

void ossimQtHistogramBuilderDialog::closeClicked()
{
   close();
}

ossimQtHistogramBuilderDialog::ossimQtHistogramBuilderDialog(
   const ossimQtHistogramBuilderDialog& /* obj */ )
   : QDialog()
{
}

const ossimQtHistogramBuilderDialog& ossimQtHistogramBuilderDialog::operator=(
   const ossimQtHistogramBuilderDialog& /* rhs */ )
{
   return *this;
}
