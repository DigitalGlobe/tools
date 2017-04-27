//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
// 
// Description:
// Main interface between the histogram remapper dialog box and the histogram
// remapper from the image chain.
//
//*************************************************************************
// $Id: ossimQtIgenController.cpp 21853 2012-10-21 20:20:17Z dburken $

#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>
#include <QtCore/QStringList>
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>
#include <QtGui/QButtonGroup>
#include <QtGui/QSlider>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtCore/QProcess>

#include <ossimQt/ossimQtIgenController.h>
#include <ossimQt/ossimQtEvent.h>
#include <ossimQt/ossimQtApplicationUtility.h>
#include <ossimQt/ossimQtIgenDialog.h>
#include <ossimQt/ossimQtScrollingImageWidget.h>
#include <ossimQt/ossimQtImageWindowController.h>
#include <ossimQt/ossimQtProgressDialog.h>
#include <ossimQt/ossimQtPropertyDialog.h>
#include <ossimQt/ossimQtPropertyDialogController.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimIdManager.h>
#include <ossim/base/ossimObjectDestructingEvent.h>
#include <ossim/base/ossimROIEvent.h>
#include <ossim/base/ossimConnectableObject.h>
#include <ossim/base/ossimConnectableContainer.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/base/ossimVisitor.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimIgenGenerator.h>
#include <ossim/imaging/ossimImageFileWriter.h>
#include <ossim/imaging/ossimGeoPolyCutter.h>
#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/projection/ossimImageViewProjectionTransform.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossimQt/ossimQtDisplayUtility.h>

#include <vector>
#include <exception>

ossimFilename ossimQtIgenController::theLatestOutputFilePath = ".";

static ossimTrace traceDebug("ossimQtIgenController:degug");
ossimQtIgenController::ossimQtIgenController(ossimQtIgenDialog* dialog)
   :
      ossimConnectableObjectListener(),
      ossimROIEventListener(),
      theDialog(dialog),
      theWidget(NULL),
      theWindowView(NULL),
      theOutputView(NULL),
      theWriter(NULL),
      theAnnotator(),
      theGsd(0.0, 0.0),
      theLines(0),
      theSamples(0),
      theOutputFile(),
      theOutputGeoPolygon(),
      theCallBackDisabled(false)
{
   // Listen for roi events from the annotator.
   theAnnotator.addListener((ossimROIEventListener*)this);
   
   theOutputGeoPolygon.clear();

   if (!theDialog || !theDialog->theOutputTypeComboBox)
   {
      return;
   }
   
   // Build the output type combo box.
   theDialog->theOutputTypeComboBox->clear();

   std::vector<ossimString> writerList;
   ossimImageWriterFactoryRegistry::instance()->getImageTypeList(writerList);

   std::vector<ossimString>::const_iterator i = writerList.begin();
   while (i != writerList.end())
   {
      QString qs = (*i).c_str();
      theDialog->theOutputTypeComboBox->insertItem(qs);
      ++i;
   }

   // Set the default output type.
   const QString DEFAULT_WRITER_TYPE = "tiff_tiled_band_separate";
   for (int index = 0; index < theDialog->theOutputTypeComboBox->count();
        ++index)
   {
      if (theDialog->theOutputTypeComboBox->text(index) == DEFAULT_WRITER_TYPE)
      {
         theDialog->theOutputTypeComboBox->setCurrentItem(index);
         break;
      }
   }

   // Make a writer so the user can edit it.
   setOutputType(getWriterString());
}

ossimQtIgenController::~ossimQtIgenController()
{
   theAnnotator.removeListener((ossimROIEventListener*)this);
}

void ossimQtIgenController::objectDestructingEvent(ossimObjectDestructingEvent& /* event */)
{
}

void ossimQtIgenController::handleRectangleROIEvent( ossimROIEvent& event)
{
   if (event.getEventType() == ossimROIEvent::OSSIM_RECTANGLE_ROI)
   {
      if (event.getMovingFlag() == false)
      {
         // Mouse drag, update lines and samples.
         theLines   = getLines();
         theSamples = getSamples();
      }
      
      // Update the geo rect from ROI rect.
      updateOutputGrect();
      updateDialog();
   }
}

void ossimQtIgenController::refresh() const
{
}

ossimFilename ossimQtIgenController::getLatestOutputFilePath()
{
   return theLatestOutputFilePath;
}

void ossimQtIgenController::setImageWidget(ossimQtScrollingImageWidget* widget)
{
   theWidget = widget;

   // Give the widget to the annotator to do the drawing.
   theAnnotator.setImageWidget(widget);
}

void ossimQtIgenController::setView(const ossimObject* view)
{
   if (view)
   {
      theWindowView = PTR_CAST(ossimProjection, view);

      //---
      // Duplicate the view since we will change the gsd for writing purposes.
      //---
      ossimRefPtr<ossimProjection> obj = (ossimProjection*) theWindowView->dup();
      theOutputView = PTR_CAST(ossimMapProjection, obj.get());
   }

   if ((theOutputGeoPolygon.size() == 0)&&theWindowView.valid())
   {
      // Must be first time through.
      theGsd = theWindowView->getMetersPerPixel();
      setSceneBoundingRect();
   }
   else
   {
      updateRoiRect();
   }
}

void ossimQtIgenController::setSceneBoundingRect()
{
   if (!theWidget) return;

   //---
   // theWidget is actually on a tile boundary so we want the fitted
   // rect and not any null buffer pixels.
   //---
   setWidgetRect(theWidget->getSceneBoundingRect());

   theLines   = getLines();
   theSamples = getSamples();

   updateOutputGrect();
   updateDialog();
}

void ossimQtIgenController::updateOutputGrect()
{
   if (!theWindowView || !theWidget) return;

   //---
   // First make sure the gsd is set.
   // Later we'll want to make this in decimal degrees if the projection
   // is geographic.  This interface will need to be added to the igen dialog.
   //---
   theOutputView->setMetersPerPixel(theGsd);

   theOutputGeoPolygon.clear();

   ossimIrect rect = theAnnotator.getShiftedRoiRect();
   ossimDpt dpt;
   ossimGpt gpt;

   // Upper left tie point from window view to ground space.
   dpt = rect.ul();
   theWindowView->lineSampleToWorld(dpt, gpt);

   // Convert the ground point output view space.
   theOutputView->worldToLineSample(gpt, dpt);

   // Snap it to an even view point.
   dpt.x = ossim::round<ossim_float64>(dpt.x);
   dpt.y = ossim::round<ossim_float64>(dpt.y);

   // Convert to ground point.
   theOutputView->lineSampleToWorld(dpt, gpt);

   // Add upper left.
   theOutputGeoPolygon.addPoint(gpt);
   
   // Add upper right
   dpt.x += theSamples - 1;
   theOutputView->lineSampleToWorld(dpt, gpt);
   theOutputGeoPolygon.addPoint(gpt);

   // Add lower right
   dpt.y += theLines - 1;
   theOutputView->lineSampleToWorld(dpt, gpt);
   theOutputGeoPolygon.addPoint(gpt);

   // lower left
   dpt.x -= theSamples -1;
   theOutputView->lineSampleToWorld(dpt, gpt);
   theOutputGeoPolygon.addPoint(gpt);

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimQtIgenController::updateOutputGrect DEBUG:"
         << "\nAnnotator rect:  " << rect
         << "\nlines: " << theLines
         << "\nsamples: " << theSamples
         << "\ntheOutputGeoPolygon\n" << theOutputGeoPolygon
         << endl;
   }

   // updateDialog();
}

void ossimQtIgenController::updateRoiRect()
{
   if (!theWindowView || !theWidget || theOutputGeoPolygon.size() != 4) return;

   ossimIrect rect;
   ossimDpt dpt;

   // upper left
   theWindowView->worldToLineSample(theOutputGeoPolygon[0], dpt);
   rect.set_ul(dpt);

   // lower right
   theWindowView->worldToLineSample(theOutputGeoPolygon[2], dpt);
   rect.set_lr(dpt);

   setWidgetRect(rect);

   updateDialog();
}

void ossimQtIgenController::recalculateRect()
{
   if (!theWidget || !theWindowView) return;

   ossimIrect rect = theAnnotator.getShiftedRoiRect();
   
   if (rect.hasNans()) return;

   ossimDpt view_gsd = theWindowView->getMetersPerPixel();
   
   double meters_in_line_dir = theLines*theGsd.y;
   double meters_in_samp_dir = theSamples*theGsd.x;

   double view_lines = meters_in_line_dir / view_gsd.y;
   double view_samps = meters_in_samp_dir / view_gsd.x;

   ossimDpt center;
   rect.getCenter(center);

   ossimDrect drect;

   // upper left
   ossimDpt ul;
   ul.x = center.x - (view_samps/2.0);
   ul.y = center.y - (view_lines/2.0);
   drect.set_ul(ul);

   ossimDpt lr;
   // lower right
   lr.x = ul.x + view_samps-1;
   lr.y = ul.y + view_lines-1;
   drect.set_lr(lr);

   // Now make an ossimIrect for the widget.
   rect = drect;

   setWidgetRect(rect);

   // Update the stored geographic points.
   updateOutputGrect();
}

void ossimQtIgenController::updateDialog()
{
   if (!theWindowView || !theWidget) return;
   
   //---
   // Since with the dialog box can trigger the callbacks set this
   // so that the receivers of callback events don't execute.
   //---
   theCallBackDisabled = true;

   // Set the gsd.  Currently just using x value.
   ossimString s = ossimString::toString(theGsd.x, 15);
   QString qs = s.c_str();
   theDialog->theGsdLineEdit->setText(qs);

   // Set the lines.
   s = ossimString::toString(theLines);
   qs = s.c_str();
   theDialog->theLinesLineEdit->setText(qs);

   // Set the samples.
   s = ossimString::toString(theSamples);
   qs = s.c_str();
   theDialog->theSamplesLineEdit->setText(qs);

   // Set the output filename.
   theDialog->theOutputFileLineEdit->setText(theOutputFile.c_str());
   
   // Enable the valueChanged event methods...
   theCallBackDisabled = false;
}

void ossimQtIgenController::setOutputFile(const QString& file)
{
   theOutputFile = file.ascii();

   theLatestOutputFilePath = theOutputFile.path();

   // This is from the dialog so update the line edit.
   theDialog->theOutputFileLineEdit->setText(file);
}

void ossimQtIgenController::saveSpecFile(const QString& file)
{
   if (!theWidget || !theOutputView || !theWriter) return;

   if (theOutputFile == ossimFilename::NIL)
   {
      QString caption("Notice:");
      QString text = "You must specify an output file!";
      
      // Give the user an already open
      QMessageBox::warning( theDialog,
                            caption,
                            text,
                            QMessageBox::Ok,
                            QMessageBox::NoButton);
      return;
   }

   if (theOutputFile == file.ascii())
   {
      QString caption("Notice:");
      QString text = "Your spec file cannot be the same as the output file!";
      
      // Give the user an already open
      QMessageBox::warning( theDialog,
                            caption,
                            text,
                            QMessageBox::Ok,
                            QMessageBox::NoButton);
      return;
   }

   // Update the ground rectangle in case someone changed the view.
   updateOutputGrect();
   
   ossimIgenGenerator* igen = new ossimIgenGenerator();

   igen->setOutputPolygon(theOutputGeoPolygon);
   theOutputView->setMetersPerPixel(theGsd);

   igen->setView(theOutputView.get());
   igen->setInput(theWidget->getInput());

   theWriter->setOutputName(theOutputFile);
   igen->setOutput(theWriter.get());
   
   igen->generateSpecList();
   ossimKeywordlist kwl;
   igen->getSpec(kwl, 0);
   
   ossimFilename spec_file = file.ascii();

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_INFO)
         << "INFO ossimQtIgenController::saveSpecFile: Writing spec file = "
         << spec_file << std::endl;
   }
   
   kwl.write(spec_file);

   delete igen;
}

void ossimQtIgenController::runIgen()
{
   // Pointer sanity checks.
   if (!theWidget || !theWriter || !theDialog)
   {
      return;
   }

   // Make sure the output file name has been set.
   if (theOutputFile == ossimFilename::NIL)
   {
      QString caption("Notice:");
      QString text = "You must specify an output file!";
      
      // Give the user an already open
      QMessageBox::warning( theDialog,
                            caption,
                            text,
                            QMessageBox::Ok,
                            QMessageBox::NoButton);
      return;
   }

   // Check to see if file exist and prompt user for overrite.
   if (theOutputFile.exists())
   {
      QString caption("Question:");
      QString text = "Overwrite existing file:  ";
      text += theOutputFile.c_str();
      int answer = QMessageBox::question( theDialog,
                                          caption,
                                          text,
                                          QMessageBox::Yes,
                                          QMessageBox::No );
      if (answer == QMessageBox::No)
      {
         theDialog->theOutputFileLineEdit->setText("");
         return;
      }
   }

#if 0  /* Temporarily commented out.  (drb) */
   //---
   // Check to see if user wants batch mode in a separate process or to run
   // in this process with a progress dialog?
   //---
   QString caption("Question:");
   QString text = "Run in batch mode?\n";
   text += "yes = Separtate process with no feed back.\n";
   text += "no  = In this process with a percent complete dialog.";
   int answer = QMessageBox::question( theDialog,
                                       caption,
                                       text,
                                       QMessageBox::Yes,
                                       QMessageBox::No);
   if (answer == QMessageBox::Yes)
   {
      runBatchIgen();
      return;
   }
#endif


   //---
   // Check the chain to make sure the output file is not one of the input
   // files.
   //---
   if ( isInChain(theOutputFile) )
   {
      QString caption("Notice:");
      QString text = "Your output file cannot be one of the input files!\n";
      text += "Please select a new output file.";

      // Give the user a warning.
      QMessageBox::warning( theDialog,
                            caption,
                            text,
                            QMessageBox::Ok,
                            QMessageBox::NoButton);
      theDialog->theOutputFileLineEdit->setText("");
      return;
   }
   
   // Make a spec file
   //---
   ossimFilename spec_file = theOutputFile;
   spec_file.setExtension("spec");

   if (theOutputFile == spec_file)
   {
      QString caption("Notice:");
      QString text = "Your output file ends with \".spec\"\n";
      text += "Please select a new output file.";

      // Give the user a warning.
      QMessageBox::warning( theDialog,
                            caption,
                            text,
                            QMessageBox::Ok,
                            QMessageBox::NoButton);
      theDialog->theOutputFileLineEdit->setText("");
      return;
   }

   QString s = spec_file.c_str();
   saveSpecFile(s);

   const ossimConnectableObject* widgetInput = theWidget->getInput();
   if (!widgetInput)
   {
      return;
   }

   // Duplicate the widget's input
   ossimRefPtr<ossimConnectableObject> writerInput = duplicate(widgetInput);
   if (writerInput.valid() == false)
   {
      return;
   }
   
   //  Set up the view(s) in the container and sync them by firing event.
   setContainerView(writerInput.get());
   ossimPropertyEvent propEvt(writerInput.get());
   writerInput->fireEvent(propEvt);
   writerInput->propagateEventToOutputs(propEvt);

   // Set the area of interest using a cutter.
   ossimGeoPolyCutter* cutter = new ossimGeoPolyCutter;
   cutter->setPolygon(theOutputGeoPolygon);
   cutter->setView(theOutputView.get());

   // Put the cutter into the chain at the end.
   ossimImageChain* chain = PTR_CAST(ossimImageChain, writerInput.get());
   if (chain)
   {
      chain->addFirst(cutter);
   }
   else
   {
      return;
   }
   
   // Connect writer to the cutter.
   theWriter->connectMyInputTo(0, writerInput.get());

   // Set the output file.
   theWriter->setOutputName(theOutputFile);

   // Initialize.
   theWriter->initialize();
   
   // Make a progress dialog.
   ossimQtProgressDialog* pd = new ossimQtProgressDialog("");
   pd->setMinimumDuration(250); // Update 4 times a second.

   //---
   // Connect the progress dialog's signal "canceled()" up to our slot
   // "saveCanceled()" so that we can tell the writer to abort.
   //---
   connect( pd, SIGNAL(canceled()), this, SLOT(abortClicked()) );

   ossimProcessListener* pl = PTR_CAST(ossimProcessListener, pd);
   if (pl)
   {
      // Make the progress dialog a listener to the writer.
      theWriter->addListener(pl);
   }

   // Set up the progress dialog...
   QString qs = "Processing file ";
   qs += theOutputFile.c_str();
   pd->setLabelText(qs);
   pd->show();

   // Process the tile...
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
      QMessageBox::information( theDialog,
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
      QMessageBox::information( theDialog,
                                caption,
                                text,
                                QMessageBox::Ok );
      exceptionCaught = true;
   }

   // Close and disconnect for next run.
   theWriter->close();
   theWriter->disconnectAllInputs();
   cutter = 0;
   writerInput = 0;

   if (exceptionCaught)
   {
      removeFile(); 
   }
   else if (pd->wasCanceled())
   {
      pd->close();
      removeFile();
   }

   if (pl)
   {
      theWriter->removeListener(pl);
   }

   // Cleanup...
   delete pd;
   pd = NULL;
   if(theOutputFile.exists())
   {
     ossimQtAddImageFileEvent event(theOutputFile);

     ossimQtApplicationUtility::sendEventToRoot(theDialog,
						&event); 
   }
}

void ossimQtIgenController::removeFile() const
{
   // 0 == success, -1 failure
   int status = ossimFilename::remove(theOutputFile.c_str());
   
   QString caption = "Processing of file aborted!";
   QString text = "File:  ";
   text += theOutputFile.c_str();
   if (status == 0)
   {
      text += "\nFile removed successfully...";
   }
   
   QMessageBox::information( theDialog,
                             caption,
                             text,
                             QMessageBox::Ok );
}

void ossimQtIgenController::runBatchIgen()
{
   ossimFilename spec_file = theOutputFile;
   spec_file.setExtension("spec");
   QString s = spec_file.c_str();
   
   saveSpecFile(s);
   
   ossimFilename omd_file = theOutputFile;
   omd_file.setExtension("omd");
   if(omd_file.exists())
   {
      ossimFilename::remove(omd_file);
   }
   
   QProcess igen;
   igen.start("igen", QStringList() << spec_file.c_str());

   // tmp drb research exit status.
   
//    if (proc->error())
//    {
//       ossimString command = "igen ";
//       command += spec_file;
      
//       ossimNotify(ossimNotifyLevel_FATAL)
//          << "FATAL ossimQtIgenController::runIgen: "
//          << "Can't execute command line = " << command
//          << std::endl;
//    }

   // Note:  From QT's assistant, they never delete "proc"???
}

bool ossimQtIgenController::isInChain(const ossimFilename& outputFile) const
{
   const ossimConnectableObject* widgetInput = theWidget->getInput();
   if (!widgetInput)
   {
      // Oops...
      return false;
   }

   // First get the keyword list for the chain.
   ossimKeywordlist kwl;
   widgetInput->saveStateOfAllInputs(kwl);

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimQtIgenController::isInChain DEBUG:"
         << "\nInput keyword list\n" << kwl
         << "\noutputFile:  " << outputFile 
         << std::endl;
   }

   // Check for filenames.
   std::vector<ossimString> keys =
      kwl.findAllKeysThatContains(ossimKeywordNames::FILENAME_KW);
   ossim_uint32 index = 0;
   for (index = 0; index < keys.size(); ++index)
   {
      // Find the filename for key.
      ossimFilename f = kwl.find(keys[index]);
      
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimQtIgenController::isInChain DEBUG:"
            << "keys[" << index << "]:  " << keys[index]
            << "\ninput file for key:  " << f << std::endl;
      }
      
      if (f == outputFile)
      {
         return true;
      }
   }

   keys.clear();

   // Check for overviews.
   keys = kwl.findAllKeysThatContains(ossimKeywordNames::OVERVIEW_FILE_KW);
   for (index = 0; index < keys.size(); ++index)
   {
      // Find the overview file for key.
      ossimFilename f = kwl.find(keys[index]);

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimQtIgenController::isInChain DEBUG:"
            << "keys[" << index << "]:  " << keys[index]
            << "\ninput overview file for key:  " << f << std::endl;
      }

      if (f == outputFile)
      {
         return true;
      }
   }

   // Output file not found in input chain.
   return false;
}

void ossimQtIgenController::gsdLineEditReturnPressed()
{
   if (theCallBackDisabled) return;

   ossimString s = theDialog->theGsdLineEdit->text().ascii();
   theGsd.x = s.toDouble();
   theGsd.y = theGsd.x;

   recalculateRect();
}

void ossimQtIgenController::linesLineEditReturnPressed()
{
   if (theCallBackDisabled) return;

   ossimString s = theDialog->theLinesLineEdit->text().ascii();
   theLines = s.toUInt32();

   recalculateRect();
}

void ossimQtIgenController::samplesLineEditReturnPressed()
{
   if (theCallBackDisabled) return;

   ossimString s = theDialog->theSamplesLineEdit->text().ascii();
   theSamples = s.toUInt32();

   recalculateRect();
}

void ossimQtIgenController::outputFileLineEditReturnPressed()
{
   if (theCallBackDisabled) return;
   
   theOutputFile = theDialog->theOutputFileLineEdit->text().ascii();

   theLatestOutputFilePath = theOutputFile.path();
}

ossim_uint32 ossimQtIgenController::getLines() const
{
   if (!theWidget || !theWindowView) return 0;

   ossimIrect rect = theAnnotator.getShiftedRoiRect();
   
   if (rect.isNan()) return 0;
   
   double size = (rect.height() * theWindowView->getMetersPerPixel().y) / theGsd.y;
   return static_cast<ossim_uint32>(floor(size));
}

ossim_uint32 ossimQtIgenController::getSamples() const
{
   if (!theWidget || !theWindowView) return 0;

   ossimIrect rect = theAnnotator.getShiftedRoiRect();
   
   if (rect.isNan()) return 0;
   
   double size = (rect.width() * theWindowView->getMetersPerPixel().x) / theGsd.x;
   return static_cast<ossim_uint32>(floor(size));
}

void ossimQtIgenController::setWidgetRect(const ossimIrect& rect)
{
   theAnnotator.setShiftedRoiRect(rect);
}

QString ossimQtIgenController::getWriterString() const 
{
   return theDialog->theOutputTypeComboBox->currentText();
}

void ossimQtIgenController::viewChanged(const ossimObject* viewObj)
{
   setView(viewObj);
}

void ossimQtIgenController::abortClicked()
{
   if (theWriter.valid())
      theWriter->abort();
}

void ossimQtIgenController::editWriter()
{
   if (!theWriter || !theDialog)
      return;

   // Before we make a dialog, make sure the filename is up to date.
   theWriter->setOutputName(theOutputFile);
   
   ossimQtPropertyDialog* dialog = new ossimQtPropertyDialog(theDialog,
                                                             "Property editor",
                                                             true);
   dialog->controller()->setObject(theWriter.get());
   dialog->setModal(true);
   dialog->exec();

   // Note: The dialog destroys itself as best I can tell.

   //---
   // Since the writers have an output image property that the user could
   // have just changed, see if we need to update the line edit field.
   //---
   updateOutputFilenameFromWriter();
}

void ossimQtIgenController::setOutputType(const QString& type)
{
   if (!theDialog || !theDialog->theOutputTypeComboBox)
   {
      return;
   }

   // Get the writer type.
   ossimString os = type.ascii();

   // Make the writer.
   ossimRefPtr<ossimImageFileWriter> writer =
      ossimImageWriterFactoryRegistry::instance()->createWriter(os);
   if (!writer.valid())
      return; // Should never happen...


   if (theWriter.valid())
   {
      if (writer->getClassName() != theWriter->getClassName())
         theWriter = writer;
      else // Writer the same class set the type.
         theWriter->setOutputImageType(writer->getOutputImageTypeString());
   }
   else
      theWriter = writer;

   if (writer.valid())
   {
      //---
      // Writer new.
      // Set the writer to create overviews and a histogram as the default.
      // The user can override this with the  "edit writer" interface.
      //---
      theWriter->setWriteOverviewFlag(true);
      theWriter->setWriteHistogramFlag(true);
   }
}

QString ossimQtIgenController::getOutputFile() const
{
   if (!theDialog || !theDialog->theOutputFileLineEdit)
   {
      return QString();
   }
   
   return theDialog->theOutputFileLineEdit->text();
}

void ossimQtIgenController::updateOutputFilenameFromWriter()
{
   if (!theDialog || !theDialog->theOutputFileLineEdit || !theWriter)
   {
      return;
   }

   ossimFilename writersOutputFile = theWriter->getFilename();
   if (writersOutputFile.empty())
   {
      return;
   }
   
   QString fileName = writersOutputFile.c_str();
   
   if (fileName == getOutputFile())
   {
      return;
   }
   
   // Make sure output filename is not the same as input.
#if 0
   QString source_file = getImageFileName();
   if (source_file == fileName)
   {
      QString caption = "Sorry:";
      QString text = "Output file cannot be the same as input file.";
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );
      return;      
   }
#endif

   // Check to see if file exist and prompt user for overwrite.
   if (writersOutputFile.exists())
   {
      QString caption("Question:");
      QString text = "Overwrite existing file:  ";
      text += fileName;
      int answer = QMessageBox::question( theDialog,
                                          caption,
                                          text,
                                          QMessageBox::Yes,
                                          QMessageBox::No);
      if (answer == QMessageBox::No)
      {
         theDialog->theOutputFileLineEdit->setText("");
         return;
      }
   }
   
   theDialog->theOutputFileLineEdit->setText(fileName);
}

void ossimQtIgenController::setContainerView(ossimConnectableObject* container)
{
   if ( !container || !theOutputView )
   {
      return;
   }

   // Set the resolution.
   theOutputView->setMetersPerPixel(theGsd);

   //---
   // Snap the tie point to something that makes sense.
   //---
   if (theOutputView->isGeographic())
   {
      // If geographic, snap to an arc second.
      ossim_float64 arcSecond = 1.0/3600.0;
      theOutputView->snapTiePointTo(arcSecond, OSSIM_DEGREES);
   }
   else
   {
      // If not geographic, snap the tie point to an even meter.
      theOutputView->snapTiePointTo(1.0, OSSIM_METERS);
   }

   // Find all the views from input.
   ossimTypeNameVisitor visitor( ossimString("ossimViewInterface"),
                                 false, // firstofTypeFlag
                                 (ossimVisitor::VISIT_INPUTS|
                                  ossimVisitor::VISIT_CHILDREN) );
   container->accept( visitor );

   ossim_uint32 idx = 0;
   const ossim_uint32 SIZE = visitor.getObjects().size();
   for( idx = 0; idx < SIZE; ++idx )
   {
      ossimViewInterface* viewInterface = visitor.getObjectAs<ossimViewInterface>( idx );
      if (viewInterface)
      {
         viewInterface->setView( theOutputView.get() );
      }
   }
   
   // Lets initialize everyone else after we set all views just incase there are dependencies.
   for( idx = 0; idx < SIZE; ++idx )
   {
      ossimRefPtr<ossimConnectableObject> obj =
         visitor.getObjectAs<ossimConnectableObject>( idx );
      if ( obj.valid() )
      {
         ossimRefreshEvent evt( obj.get() );
         obj->fireEvent(evt);
         obj->propagateEventToOutputs(evt);
      }
   }

#if 0 /* Replaced deprecated code. (drb) */    
   
   ossimConnectableObject::ConnectableObjectList viewList;
   container->findAllInputsOfType(viewList,
                                  STATIC_TYPE_INFO(ossimViewInterface),
                                  true,
                                  true);

   for(ossim_uint32 i = 0; i < viewList.size(); ++i)
   {
      ossimViewInterface* viewInterface = PTR_CAST(ossimViewInterface,
                                                   viewList[i].get());
      if(viewInterface)
      {
         viewInterface->setView(theOutputView.get()); 
         ossimPropertyEvent propEvt(viewList[i].get());
         viewList[i]->fireEvent(propEvt);
         viewList[i]->propagateEventToOutputs(propEvt);
         
      }
   }
#endif /* Replaced deprecated code. (drb) */  
}

ossimConnectableObject* ossimQtIgenController::duplicate(const ossimConnectableObject* obj) const
{
   if(!obj)
   {
      return NULL;
   }

   ossimKeywordlist kwl;
   obj->saveState(kwl);
   ossimObject* tempObj =
      ossimObjectFactoryRegistry::instance()->createObject(kwl);

   if (!tempObj)
   {
      return NULL;
   }
   
   ossimConnectableObject* connectable = PTR_CAST(ossimConnectableObject,
                                                  tempObj);

   if ( !connectable )
   {
      return NULL;
   }

   ossimConnectableContainerInterface* inter =
      PTR_CAST(ossimConnectableContainerInterface, connectable);

   if(inter)
   {
      inter->makeUniqueIds();
   }
   else
   {
      connectable->setId(ossimIdManager::instance()->generateId());
   }
   
   for(ossim_uint32 i = 0; i < obj->getNumberOfInputs(); ++i)
   {
      if(obj->getInput(i))
      {
         ossimConnectableObject* newInput = duplicate(obj->getInput(i));
         if(newInput)
         {
            connectable->connectMyInputTo(newInput);
         }
      }
   }
   
   return connectable;
}


// Private to disallow copy constructor, assignment operator use...
ossimQtIgenController::ossimQtIgenController(const ossimQtIgenController&)
   :
      QObject(),
      ossimConnectableObjectListener(),
      ossimROIEventListener(),
      theDialog(NULL),
      theWidget(NULL),
      theCallBackDisabled(false)
{
}

ossimQtIgenController& ossimQtIgenController::operator=(const ossimQtIgenController&)
{
   return *this;
}
