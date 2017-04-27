//----------------------------------------------------------------------------
//
// License:   See top levle LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Dialog for building image overviews.
//
// $Id: ossimQtOverviewBuilderDialog.cpp 15766 2009-10-20 12:37:09Z gpotts $
//----------------------------------------------------------------------------

#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QPushButton>
#include <QtGui/QMessageBox>

#include <ossimQt/ossimQtOverviewBuilderDialog.h>
#include <ossimQt/ossimQtPropertyDialog.h>
#include <ossimQt/ossimQtPropertyDialogController.h>
#include <ossimQt/ossimQtProgressDialog.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimTiffOverviewBuilder.h>
#include <ossim/imaging/ossimImageHandler.h>

ossimQtOverviewBuilderDialog::ossimQtOverviewBuilderDialog(
   QWidget* parent,
   ossimImageHandler* imageHandler)
   :
      QDialog(parent, "ossimQtOverviewBuilderDialog", true),
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
   setCaption("Build Overviews");

   if (!theImageHandler)
   {
      close();
   }
   
   // Main vertical box.
   theMainVBox = new QVBoxLayout(this);

   // First row, display source image file name.
   theHBox1 = new QHBoxLayout(theMainVBox);
   
   theSourceImageGroupBox = new QGroupBox("source image", this);
   theSourceImageGroupBox->setAlignment(Qt::AlignHCenter);

   QString qs = theImageHandler->getFilename().c_str();
   theSourceImageLabel = new QLabel(qs,
                                    theSourceImageGroupBox,
                                    "theSourceImageLabel");
   theHBox1->addWidget(theSourceImageGroupBox);
   // End of first row.

   // Second row.
   theHBox2 = new QHBoxLayout(theMainVBox);
   
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

ossimQtOverviewBuilderDialog::~ossimQtOverviewBuilderDialog()
{
   if (theWriter)
   {
      delete theWriter;
      theWriter = NULL;
   }
}

void ossimQtOverviewBuilderDialog::buildClicked()
{
   if ( !theImageHandler )
   {
      return;
   }

   if (theWriter)
   {
      delete theWriter;
   }
   
   theWriter = new ossimTiffOverviewBuilder();
   theWriter->setInputSource(theImageHandler);

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
   ossimFilename output_file = theWriter->getOutputFile();
   
   QString qs = "Processing file ";
   qs += output_file.c_str();
   pd->setLabelText(qs);
   pd->show();
   
   // Build the overview.
   if (theWriter->execute())
   {
      theImageHandler->openOverview();
   }

   // Check for cancelation.
   if (pd->wasCanceled())
   {
      pd->close();

      int status = remove(output_file.c_str()); // 0 == success, -1 failure
         
      QString caption = "Processing of file aborted!";
      QString text = "File:  ";
      text += output_file.c_str();
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

   close();
}

void ossimQtOverviewBuilderDialog::abortClicked()
{
   if (theWriter)
   {
      theWriter->abort();
   }
}

void ossimQtOverviewBuilderDialog::closeClicked()
{
   close();
}


ossimQtOverviewBuilderDialog::ossimQtOverviewBuilderDialog(const ossimQtOverviewBuilderDialog& /* obj */ )
   : QDialog()
{
}

const ossimQtOverviewBuilderDialog& ossimQtOverviewBuilderDialog::operator=(const ossimQtOverviewBuilderDialog& /* rhs */ )
{
   return *this;
}
