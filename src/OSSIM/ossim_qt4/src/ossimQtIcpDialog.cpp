//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Image Copy interface.
//
// $Id: ossimQtIcpDialog.cpp 15766 2009-10-20 12:37:09Z gpotts $
//----------------------------------------------------------------------------

#include <exception>

#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QComboBox>
#include <QtGui/QBoxLayout>
#include <QtGui/QGroupBox>

#include <ossimQt/ossimQtIcpDialog.h>
#include <ossimQt/ossimQtPropertyDialog.h>
#include <ossimQt/ossimQtPropertyDialogController.h>
#include <ossimQt/ossimQtProgressDialog.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>
#include <ossim/imaging/ossimImageFileWriter.h>
#include <ossim/imaging/ossimImageHandler.h>

ossimQtIcpDialog::ossimQtIcpDialog(QWidget* parent,
                                   ossimImageChain* image_chain)
   : QDialog(parent, "ossimQtIcpDialog", true, Qt::WDestructiveClose),
     theChain(image_chain),
     theSourceImageLabel(0),
     theStartLineField(0),
     theStopLineField(0),
     theStartSampleField(0),
     theStopSampleField(0),
     theOutputImageField(0),
     theOutputImageButton(0),
     theOutputImageWriterMenu(0),
     theEditOutputImageWriterButton(0),
     theSaveButton(0),
     theCloseButton(0),
     theSourceImageRect(),
     theOutputImageRect()
{
   setCaption("Export Image");

   // Main vertical box to hold everything.  Parented to this dialog box...
   QVBoxLayout* mainVBox = new QVBoxLayout(this);

   // First row, display source image file name.
   QGroupBox* row1GroupBox = new QGroupBox(QString("source image"));
   row1GroupBox->setAlignment(Qt::AlignHCenter);
   QString s = getImageFileName();
   theSourceImageLabel = new QLabel(s);
   QHBoxLayout* hBox1 = new QHBoxLayout();
   hBox1->addWidget(theSourceImageLabel);
   row1GroupBox->setLayout(hBox1);
   mainVBox->addWidget(row1GroupBox);
   // End of first row.

   // Second row, start line, stop line.
   QGroupBox* row2GroupBox = new QGroupBox();
   row2GroupBox->setAlignment(Qt::AlignHCenter);

   // Start line box.
   QGroupBox* startLineGroupBox = new QGroupBox("start line");
   startLineGroupBox->setAlignment(Qt::AlignHCenter);
   theStartLineField = new QLineEdit();
   QHBoxLayout* startLineLayout = new QHBoxLayout();
   startLineLayout->addWidget(theStartLineField);
   startLineGroupBox->setLayout(startLineLayout);

   // Stop line box.
   QGroupBox* stopLineGroupBox = new QGroupBox("stop line");
   stopLineGroupBox->setAlignment(Qt::AlignHCenter);
   theStopLineField = new QLineEdit();
   QHBoxLayout* stopLineLayout = new QHBoxLayout();
   stopLineLayout->addWidget(theStopLineField);
   stopLineGroupBox->setLayout(stopLineLayout);

   // Layout
   QHBoxLayout* hBox2 = new QHBoxLayout();
   hBox2->addWidget(startLineGroupBox);
   hBox2->addWidget(stopLineGroupBox);
   row2GroupBox->setLayout(hBox2);
   mainVBox->addWidget(row2GroupBox);

   // Third row, start sample, stop sample.
   QGroupBox* row3GroupBox = new QGroupBox();
   row3GroupBox->setAlignment(Qt::AlignHCenter);

   // Start sample box.
   QGroupBox* startSampleGroupBox = new QGroupBox("start sample");
   startSampleGroupBox->setAlignment(Qt::AlignHCenter);
   // startSampleGroupBox->setFlat(true);
   theStartSampleField = new QLineEdit();
   QHBoxLayout* startSampleLayout = new QHBoxLayout();
   startSampleLayout->addWidget(theStartSampleField);
   startSampleGroupBox->setLayout(startSampleLayout);

   // Stop sample box.
   QGroupBox* stopSampleGroupBox = new QGroupBox("stop sample");
   stopSampleGroupBox->setAlignment(Qt::AlignHCenter);
   // stopSampleGroupBox->setFlat(true);
   theStopSampleField = new QLineEdit();
   QHBoxLayout* stopSampleLayout = new QHBoxLayout();
   stopSampleLayout->addWidget(theStopSampleField);
   stopSampleGroupBox->setLayout(stopSampleLayout);

   // Layout
   QHBoxLayout* hBox3 = new QHBoxLayout();
   hBox3->addWidget(startSampleGroupBox);
   hBox3->addWidget(stopSampleGroupBox);
   row3GroupBox->setLayout(hBox3);
   mainVBox->addWidget(row3GroupBox);

   // Fourth row, output file, file dialog button.
   QGroupBox* row4GroupBox = new QGroupBox("output image");
   row4GroupBox->setAlignment(Qt::AlignHCenter);
   theOutputImageField = new QLineEdit();
   theOutputImageButton = new QPushButton();
   theOutputImageButton->setText( "..." );
   theOutputImageButton->setDefault(false);
   theOutputImageButton->setAutoDefault(false);

   QHBoxLayout* hBox4 = new QHBoxLayout();
   hBox4->addWidget(theOutputImageField);
   hBox4->addWidget(theOutputImageButton);
   row4GroupBox->setLayout(hBox4);
   mainVBox->addWidget(row4GroupBox);
   // End of fourth row...
   
   // Fifth row.
   QGroupBox* row5GroupBox = new QGroupBox("output image type");
   row5GroupBox->setAlignment(Qt::AlignHCenter);
   
   theOutputImageWriterMenu = new QComboBox();
   buildWriterMenu();
   
   theEditOutputImageWriterButton = new QPushButton();
   theEditOutputImageWriterButton->setText( "edit writer" );
   theEditOutputImageWriterButton->setDefault(false);
   theEditOutputImageWriterButton->setAutoDefault(false);

   QHBoxLayout* hBox5 = new QHBoxLayout();
   hBox5->addWidget(theOutputImageWriterMenu);
   hBox5->addWidget(theEditOutputImageWriterButton);
   row5GroupBox->setLayout(hBox5);
   mainVBox->addWidget(row5GroupBox);

   // Sixth row.
   QGroupBox* row6GroupBox = new QGroupBox();
   row6GroupBox->setAlignment(Qt::AlignHCenter);
   row6GroupBox->setFlat(true);  

   theSaveButton = new QPushButton();
   theSaveButton->setText( "Save" );
   theSaveButton->setDefault(false);
   theSaveButton->setAutoDefault(false);   
   
   theCloseButton = new QPushButton();
   theCloseButton->setText( "Close" );
   theCloseButton->setDefault(false);
   theCloseButton->setAutoDefault(false);   
   
   QHBoxLayout* hBox6 = new QHBoxLayout();
   hBox6->addWidget(theSaveButton);
   hBox6->addWidget(theCloseButton);
   row6GroupBox->setLayout(hBox6);
   mainVBox->addWidget(row6GroupBox);

   //---
   // Connect all the signals to slots...
   //
   // NOTE:
   // If a push button is set to "default" or "auto default" it will always
   // call that slot when return is pressed in the dialog.
   // Since this isn't what I want all defaults are set to off!
   //---
   connect( theStartLineField, SIGNAL ( returnPressed() ),
            this, SLOT ( startLinePressed() ) );
   
   connect( theStopLineField, SIGNAL ( returnPressed() ),
            this, SLOT ( stopLinePressed() ) );
   
   connect( theStartSampleField, SIGNAL ( returnPressed() ),
            this, SLOT ( startSamplePressed() ) );
   
   connect( theStopSampleField, SIGNAL ( returnPressed() ),
            this, SLOT ( stopSamplePressed() ) );

   connect( theOutputImageField, SIGNAL ( returnPressed() ),
            this, SLOT ( outputImagePressed() ) );

   connect( theOutputImageButton, SIGNAL ( clicked() ),
            this, SLOT ( outputImageClicked() ) );

   connect( theOutputImageWriterMenu, SIGNAL ( activated( const QString& ) ),
            this, SLOT ( outputImageWriterMenuActivated( const QString& ) ) );

   connect( theEditOutputImageWriterButton,
            SIGNAL ( clicked() ),
            this,
            SLOT ( editOutputImageWriterClicked() ) );

   connect( theSaveButton, SIGNAL ( clicked() ),
            this, SLOT ( saveClicked() ) );
   
   connect( theCloseButton, SIGNAL ( clicked() ),
            this, SLOT ( closeClicked() ) );

   initializeDialog();
}

ossimQtIcpDialog::~ossimQtIcpDialog()
{
}

QString ossimQtIcpDialog::getImageFileName()
{
   QString qs;
   
   const ossimImageHandler* ih = getImageHandler();
   if (ih)
   {
      qs = ih->getFilename().c_str();
   }

   return qs;
}

ossimImageHandler* ossimQtIcpDialog::getImageHandler()
{
   ossimImageHandler* handler = 0;
   
   if (!theChain.valid())
   {
      return handler;
   }

   // Image handler usually at the end of chain so start there...
   const ossim_uint32 NUMBER_OF_LINKS = theChain->getNumberOfObjects(false);
   for (ossim_uint32 idx = NUMBER_OF_LINKS; idx > 0; --idx) 
   { 
      ossimConnectableObject* filter = (*theChain.get())[(idx-1)];
      if (filter)
      {
         if ( PTR_CAST(ossimImageHandler, filter) )
         {
            handler = PTR_CAST(ossimImageHandler, filter);
            break;
         }
      }
   }
   return handler;
}

void ossimQtIcpDialog::startLinePressed()
{
   updateStartLine();
}

void ossimQtIcpDialog::stopLinePressed()
{
   updateStopline();
}

void ossimQtIcpDialog::startSamplePressed()
{
   updateStartSample();
}

void ossimQtIcpDialog::stopSamplePressed()
{
   updateStopSample();
}

void ossimQtIcpDialog::outputImagePressed()
{
   if (!theOutputImageField)
   {
      return;
   }

   QString qs = theOutputImageField->text();

   // Make sure output filename is not the same as input.
   QString source_file = getImageFileName();
   if (source_file == qs)
   {
      QString caption = "Sorry:";
      QString text = "Output file cannot be the same as input file.";
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );
      theOutputImageField->setText("");
      return;      
   }
   
   theOutputImageField->setText(qs);
}

void ossimQtIcpDialog::outputImageClicked()
{
   if (!theOutputImageField)
   {
      return;
   }
   
   QFileDialog* fd = new QFileDialog( this, "file dialog");
   fd->setMode( QFileDialog::AnyFile );
   
   QString fileName;
   
   if ( fd->exec() == QDialog::Accepted )
   {
      fileName = fd->selectedFile();
   }
   delete fd;
   fd = 0;
   
   if (fileName.isEmpty())
   {
      return;
   }
   
   // Make sure output filename is not the same as input.
   QString source_file = getImageFileName();
   if (source_file == fileName)
   {
      QString caption = "Sorry:";
      QString text = "Output file cannot be the same as input file.";
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );
      theOutputImageField->setText("");
      return;      
   }
   
   theOutputImageField->setText(fileName);
}

void ossimQtIcpDialog::updateOutputFilenameFromWriter()
{
   if (!theOutputImageField || !theWriter.valid())
   {
      return;
   }

   ossimFilename writersOutputFile = theWriter->getFilename();
   if (writersOutputFile.empty())
   {
      return;
   }
   
   QString fileName = writersOutputFile.c_str();
   
   if (fileName == theOutputImageField->text())
   {
      return;
   }
   
   // Make sure output filename is not the same as input.
   QString source_file = getImageFileName();
   if (source_file == fileName)
   {
      QString caption = "Sorry:";
      QString text = "Output file cannot be the same as input file.";
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );
      theOutputImageField->setText("");
      return;      
   }
   
   theOutputImageField->setText(fileName);
}

void ossimQtIcpDialog::outputImageWriterMenuActivated(const QString& qs)
{
   if (!theOutputImageWriterMenu)
   {
      return;
   }

   // Get the writer type.
   ossimString os = qs.ascii();

   // Make the writer.
   ossimRefPtr<ossimImageFileWriter> writer =
      ossimImageWriterFactoryRegistry::instance()->createWriter(os);

   if (!writer.valid())
   {
      return; // Should never happen...
   }

   //---
   // Set the writer to create overviews and a histogram as the default.
   // The user can override this with the  "edit writer" interface.
   //---
   writer->setWriteOverviewFlag(true);
   writer->setWriteHistogramFlag(true);   

   if (theWriter.valid())
   {
      if (writer->getClassName() != theWriter->getClassName())
      {
         theWriter = writer;
      }
      else // Writer the same class set the type.
      {
         theWriter->setOutputImageType(writer->getOutputImageTypeString());
      }
   }
   else
   {
      theWriter = writer;
   }
}

void ossimQtIcpDialog::editOutputImageWriterClicked()
{
   if (!theWriter.valid())
   {
      return;
   }

   ossimQtPropertyDialog* dialog = new ossimQtPropertyDialog(this,
                                                             "Property editor",
                                                             true);
   dialog->controller()->setObject(theWriter.get());
   dialog->setModal(true);
   dialog->exec();

   //---
   // Since the writers have an output image property that the user could
   // have just changed, see if we need to update the line edit field.
   //---
   updateOutputFilenameFromWriter();
}

void ossimQtIcpDialog::saveClicked()
{
   if ( !theOutputImageField || !theWriter.valid() || !theChain.valid() )
   {
      return;
   }

   // Make sure the output rectangle is up to date.
   if (updateOuputRect() == false)
   {
      return;
   }
   
   // Make sure output file is not empty.
   QString output_file = theOutputImageField->text();
   if (output_file.isEmpty())
   {
      QString caption = "Sorry:";
      QString text = "You must set an output file name.";
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );
      return;
   }

   // Check to see if file exist and prompt user for overrite.
   ossimFilename f = output_file.ascii();
   if (f.exists())
   {
      QString caption("Question:");
      QString text = "Overwrite existing file:  ";
      text += output_file;
      int answer = QMessageBox::question( this,
                                          caption,
                                          text,
                                          QMessageBox::Yes,
                                          QMessageBox::No);
      if (answer == QMessageBox::No)
      {
         theOutputImageField->setText("");
         return;
      }
   }

   // Make sure output filename is not the same as input.
   QString source_file = getImageFileName();
   if (source_file == output_file)
   {
      QString caption = "Sorry:";
      QString text = "Output file cannot be the same as input file.";
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );
      theOutputImageField->setText("");
      return;      
   }

   // Connect it up to the source.
   theWriter->connectMyInputTo(0, theChain.get());

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
   qs += output_file        ;
   pd->setLabelText(qs);
   pd->show();
   
   //---
   // Process the tile...
   //---
   theWriter->open(output_file.ascii());
   theWriter->initialize();
   theWriter->setAreaOfInterest(theOutputImageRect);

   // Process the image...
   bool exceptionCaught = false;
   try
   {
      theWriter->execute();
   }
   catch(std::exception& e)
   {
      pd->close();
      QString caption = "Exception caught!\n";
      QString text = e.what();
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );
      exceptionCaught = true;
   }
   catch (...)
   {
      pd->close();
      QString caption = "Unknown exception caught!\n";
      QString text = "";
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );
      exceptionCaught = true;
   }

   theWriter->close();

   if (exceptionCaught)
   {
      int status = remove(output_file.ascii()); // 0 == success, -1 failure
      
      QString caption = "Processing of file aborted!";
      QString text = "File:  ";
      text += output_file;
      if (status == 0)
      {
         text += "\nFile removed successfully...";
      }
      
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );
   }
   else if (pd->wasCanceled()) // Check for cancelation.
   {
      pd->close();

      int status = remove(output_file.ascii()); // 0 == success, -1 failure
         
      QString caption = "Processing of file aborted!";
      QString text = "File:  ";
      text += output_file;
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
   pd = 0;
}

void ossimQtIcpDialog::abortClicked()
{
   if (theWriter.valid())
   {
      theWriter->abort();
   }
}

void ossimQtIcpDialog::closeClicked()
{
   close(true);
}

void ossimQtIcpDialog::initializeDialog()
{
   if ( !theChain.valid() )
   {
      return;
   }
   
   theSourceImageRect = theChain->getBoundingRect();
   
   if (theSourceImageRect.hasNans())
   {
      return; // Should never happen...
   }
   
   theOutputImageRect = theSourceImageRect;

   setStartStopFields();  // This will set fields to theOutputImageRect.
}

void ossimQtIcpDialog::setStartStopFields()
{
   if ( !theStartLineField   || !theStopLineField ||
        !theStartSampleField || !theStopSampleField )
   {
      return;
   }
   
   QString qs; // Used throughout...
   
   qs = ossimString::toString(theOutputImageRect.ul().y).c_str();
   theStartLineField->setText(qs);

   qs = ossimString::toString(theOutputImageRect.ul().x).c_str();
   theStartSampleField->setText(qs);
   
   qs = ossimString::toString(theOutputImageRect.lr().y).c_str();
   theStopLineField->setText(qs);

   qs = ossimString::toString(theOutputImageRect.lr().x).c_str();
   theStopSampleField->setText(qs);
}

void ossimQtIcpDialog::buildWriterMenu()
{
   const QString DEFAULT_WRITER_TYPE = "tiff_tiled_band_separate";
   
   if (!theOutputImageWriterMenu)
   {
      return;
   }

   theOutputImageWriterMenu->clear();
   
   vector<ossimString> writerList;
   ossimImageWriterFactoryRegistry::instance()->getImageTypeList(writerList);

   vector<ossimString>::const_iterator i = writerList.begin();
   while (i != writerList.end())
   {
      QString qs = (*i).c_str();
      theOutputImageWriterMenu->insertItem(qs);
      ++i;
   }

   // Set the default item.
   for (int index = 0; index < theOutputImageWriterMenu->count(); ++index)
   {
      if (theOutputImageWriterMenu->text(index) == DEFAULT_WRITER_TYPE)
      {
         theOutputImageWriterMenu->setCurrentItem(index);
         break;
      }
   }

   // Initialize the writer.
   ossimString os = theOutputImageWriterMenu->currentText().ascii();
   theWriter = ossimImageWriterFactoryRegistry::instance()->createWriter(os);
   if (theWriter.valid())
   {
      //---
      // Set the writer to create overviews and a histogram as the default.
      // The user can override this with the  "edit writer" interface.
      //---
      theWriter->setWriteOverviewFlag(true);
      theWriter->setWriteHistogramFlag(true);
   }
}

bool ossimQtIcpDialog::updateOuputRect()
{
   if (updateStartLine() == false)
   {
      return false;
   }
   if (updateStopline() == false)
   {
      return false;
   }
   if (updateStartSample() == false)
   {
      return false;
   }
   if (updateStopSample() == false)
   {
      return false;
   }

   return true;
}

bool ossimQtIcpDialog::updateStartLine()
{
   if (!theStartLineField)
   {
      return false;
   }
   
   QString     qs   = theStartLineField->text();
   ossimString os   = qs.ascii();
   ossim_int32 line = os.toInt32();
   
   if (line < theSourceImageRect.ul().y)
   {
      QString caption = "Range Error:";
      QString text =
         "Start line cannot be less than the start of the image!\n";
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );

      setStartStopFields();  // This will reset fields to theOutputImageRect.
      return false;
   }
   if (line >= theOutputImageRect.lr().y)
   {
      QString caption = "Range Error:";
      QString text =
         "Start line must be less than the stop line!\n";
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );
      setStartStopFields();  // This will reset fields to theOutputImageRect.
      return false;
   }

   theOutputImageRect.set_uly(line);

   return true;
}

bool ossimQtIcpDialog::updateStopline()
{
   if (!theStopLineField)
   {
      return false;
   }
   
   QString     qs   = theStopLineField->text();
   ossimString os   = qs.ascii();
   ossim_int32 line = os.toInt32();
   
   if (line > theSourceImageRect.lr().y)
   {
      QString caption = "Range Error:";
      QString text =
         "Stop line cannot be greater than the end of the image!\n";
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );
      
      setStartStopFields();  // This will reset fields to theOutputImageRect.
      return false;
   }
   if (line <= theOutputImageRect.ul().y)
   {
      QString caption = "Range Error:";
      QString text =
         "Stop line must be greater than the start line!\n";
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );
      setStartStopFields();  // This will reset fields to theOutputImageRect.
      return false;
   }

   theOutputImageRect.set_lry(line);

   return true;
}

bool ossimQtIcpDialog::updateStartSample()
{
   if (!theStartSampleField)
   {
      return false;
   }
   
   QString     qs     = theStartSampleField->text();
   ossimString os     = qs.ascii();
   ossim_int32 sample = os.toInt32();
   
   if (sample < theSourceImageRect.ul().x)
   {
      QString caption = "Range Error:";
      QString text =
         "Start sample cannot be less than the start of the image!\n";
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );

      setStartStopFields();  // This will reset fields to theOutputImageRect.
      return false;
   }
   if (sample >= theOutputImageRect.lr().x)
   {
      QString caption = "Range Error:";
      QString text =
         "Start sample must be less than the stop sample!\n";
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );
      setStartStopFields(); // This will reset fields to theOutputImageRect. 
      return false;
   }

   theOutputImageRect.set_ulx(sample);

   return true;
}

bool ossimQtIcpDialog::updateStopSample()
{
   if (!theStopSampleField)
   {
      return false;
   }

   QString     qs     = theStopSampleField->text();
   ossimString os     = qs.ascii();
   ossim_int32 sample = os.toInt32();
   
   if (sample > theSourceImageRect.lr().x)
   {
      QString caption = "Range Error:";
      QString text =
         "Stop sample cannot be greater than the end of the image!\n";
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );
      
      setStartStopFields();  // This will reset fields to theOutputImageRect.
      return false;
   }
   if (sample <= theOutputImageRect.ul().x)
   {
      QString caption = "Range Error:";
      QString text =
         "Stop sample must be greater than the start sample!\n";
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );
      setStartStopFields();  // This will reset fields to theOutputImageRect.
      return false;
   }

   theOutputImageRect.set_lrx(sample);

   return true;
}

ossimQtIcpDialog::ossimQtIcpDialog(const ossimQtIcpDialog& /* obj */ )
   : QDialog()
{
}

const ossimQtIcpDialog& ossimQtIcpDialog::operator=(const ossimQtIcpDialog& /* rhs */ )
{
   return *this;
}
