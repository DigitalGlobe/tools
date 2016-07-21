//----------------------------------------------------------------------------
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author:  David Burken
//
// Description: Single image window.
//
// $Id: ossimQtSingleImageWindow.cpp 20394 2011-12-19 20:47:53Z dburken $
//----------------------------------------------------------------------------

#include <cstdlib> /* for getenv */
#include <iostream>
#include <sstream>
#include <iomanip>

#include <QtGui/QStatusBar>
#include <QtGui/QMessageBox>
#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QToolButton>
#include <QtGui/QFileDialog>
#include <QtCore/QCustomEvent>
#include <QtCore/QEvent>
#include <ossim/vpfutil/set.h>
#include <QtGui/QMainWindow>
#include <QtGui/QComboBox>
#include <ossimQt/ossimQtSingleImageWindow.h>
#include <ossimQt/ossimQtIcpDialog.h>
#include <ossimQt/ossimQtIviewAboutDialog.h>
#include <ossimQt/ossimQtOverviewBuilderDialog.h>
#include <ossimQt/ossimQtProgressDialog.h>
#include <ossimQt/ossimQtScrollingImageWidget.h>
#include <ossimQt/ossimQtImageChainEditorDialog.h>
#include <ossimQt/ossimQtImageWindowControllerEventFilter.h>
#include <ossimQt/ossimQtEditorWidgetManager.h>
#include <ossimQt/ossimQtEvent.h>
#include <ossimQt/ossimQtDisplayUtility.h>
#include <ossim/base/ossimContainerEvent.h>
#include <ossim/base/ossimVisitor.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimTiffOverviewBuilder.h>
#include <ossim/imaging/ossimBandSelector.h>
#include <ossim/imaging/ossimBrightnessContrastSource.h>
#include <ossim/imaging/ossimCacheTileSource.h>
#include <ossim/imaging/ossimHistogramRemapper.h>
#include <ossim/imaging/ossimHsiRemapper.h>
#include <ossim/imaging/ossimRLevelFilter.h>
#include <ossim/imaging/ossimWatermarkFilter.h>
#include <ossim/imaging/ossimTiffTileSource.h>

static const int DEFAULT_WIDTH  = 512;
static const int DEFAULT_HEIGHT = 512;

RTTI_DEF2(ossimQtSingleImageWindow,
          "ossimQtSingleImageWindow",
          ossimConnectableObject,
          ossimConnectableDisplayListener);

ossimQtSingleImageWindow::ossimQtSingleImageWindow(QWidget* parent,
                                                   const char* name,
                                                   Qt::WFlags f)
   : QMainWindow(parent, name, f),
     ossimConnectableObject(),
     ossimConnectableDisplayListener(),
     theImageWidget(0),
     theLastOpenedDirectory(),
     theResolutionLevelMenu(0)
{
   ossimReferenced::ref();
   setCaption("iview");
   
   QSize size(DEFAULT_WIDTH, DEFAULT_HEIGHT);
   setBaseSize(size);

   // This set the window's widget size.
   setGeometry(0,0,DEFAULT_WIDTH-1,DEFAULT_HEIGHT-1);
   
   // Create the image widget parenting it to this.
   theImageWidget = new ossimQtScrollingImageWidget(this, "image_widget");   

   // This will clear out any initial garbage in the widget.
   theImageWidget->refresh();

   // Disable random caching scheme.
   theImageWidget->setRandomPriorityQueueEnabledFlag(false);
   
   // Set the the width and height of the window.
   theImageWidget->resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);

   // Make the image widget the centralized widget.
   setCentralWidget(theImageWidget);

   // Make the "File" pull down menu.
   QMenu* file = new QMenu( this );
   menuBar()->insertItem( "&File", file );
   file->insertItem( "&Open Image",  this, SLOT(openImage()), Qt::CTRL+Qt::Key_O );
   file->insertItem( "&Close Image",  this, SLOT(closeImage()), Qt::CTRL+Qt::Key_C );
   file->insertItem( "&Export",  this, SLOT(exportImage()), Qt::CTRL+Qt::Key_E );
   file->insertItem( "&Quit",  this, SLOT(closeWindow()), Qt::CTRL+Qt::Key_Q );

   // Make the "Edit" pull down menu.
   QMenu* edit = new QMenu( this );
   menuBar()->insertItem( "&Edit", edit );
   edit->insertItem( "Image Chain",  this, SLOT(editImageChain()));

   // Make the "Utilities" pull down menu.
   QMenu* utilities = new QMenu( this );
   menuBar()->insertItem( "&Utilities", utilities );
   utilities->insertItem( "Refresh",  this, SLOT(refreshDisplay()));

   // Make the "Resolution_Level" pull down menu.
   theResolutionLevelMenu = new QMenu( this );
   menuBar()->insertItem( "&Resolution_Level",  theResolutionLevelMenu);

   QAction* rsetAct = new QAction(QString("0"), this);
   rsetAct->setCheckable(true);
   rsetAct->setChecked(true);
   rsetAct->setData(0);
   rsetAct->setEnabled(false);

   theResolutionLevelMenu->addAction( rsetAct );
   
   // Connect the activated signal to the changeResolutionLevel slot.
   connect( theResolutionLevelMenu, SIGNAL( triggered( QAction * ) ),
            this, SLOT ( changeResolutionLevel( QAction * ) ) );

   // Make the "Help" pull down menu.
   QMenu* help = new QMenu( this );
   menuBar()->insertItem( "&Help", help );
   help->insertItem( "&About",  this, SLOT(about()), Qt::CTRL+Qt::Key_A );

   // Connect up the slot to capture mouse events.
   connect(theImageWidget,
           SIGNAL(scrollingImageWidgetMouseEvent(ossimQtMouseEvent*)),
           this,
           SLOT(trackImageWidget(ossimQtMouseEvent*)));

   // Add us in as a listener for display events like refresh. 
   addListener((ossimConnectableDisplayListener*)this);
   
   //---
   // Connect this display up to "theImageWidget" so that event propagation
   // comes up the chain to us.
   //---
   connectMyInputTo(theImageWidget);

   // Send a dummy message to make the status bar show.
   statusBar()->message( QString(" ") );

   //---
   // Set the last open directory to the current working dir so the open image
   // dialog will come up where you started.
   //---
   theLastOpenedDirectory = getenv("PWD");
}

ossimQtSingleImageWindow::~ossimQtSingleImageWindow()
{
   if (theImageChain.valid())
   {
      theImageChain->disconnect();
      theImageChain = 0;
   }
   theImageWidget->disconnectAllInputs();
   theImageWidget->disconnectAllOutputs();
   ossimConnectableObject::disconnect();
   ossimReferenced::unref_nodelete();
}

void ossimQtSingleImageWindow::addObjectEvent(ossimContainerEvent& event)
{
   ossimContainerEvent::ObjectList& objects = event.getObjectList();
   ossim_uint32 idx = 0;
   bool buildResFlag = false;
   for(idx = 0; ((idx < objects.size())&&!buildResFlag); ++idx)
   {
      if ( PTR_CAST(ossimRLevelFilter, objects[idx].get()) ||
          PTR_CAST(ossimTiffTileSource, objects[idx].get()) )
      {
         buildResFlag = true;
      }
   }
   if(buildResFlag)
   {
      buildResolutionLevelMenu();
   }
}

void ossimQtSingleImageWindow::removeObjectEvent(ossimContainerEvent& event)
{
   ossimContainerEvent::ObjectList& objects = event.getObjectList();
   if(!objects.empty())
   {
      ossimRLevelFilter* rf = PTR_CAST(ossimRLevelFilter, objects[0].get());
      if (rf)
      {
         if (theResolutionLevelMenu)
         {
            theResolutionLevelMenu->clear();
            theResolutionLevelMenu->insertItem( QString("0"), 0, 0 );
         }
      }
   }
}

void ossimQtSingleImageWindow::refreshDisplay()
{
   if (theImageWidget)
   {
      theImageWidget->refresh();
   }
}

void ossimQtSingleImageWindow::changeResolutionLevel(QAction* action )
{
   ossimRefPtr<ossimRLevelFilter> rf = getRLevelFilter();
   if ( rf.valid() )
   {
      ossim_uint32 new_level =
         static_cast<ossim_uint32>( action->data().toInt() );
      ossim_uint32 current_level = rf->getCurrentRLevel();
      if (new_level != current_level)
      {
         rf->setCurrentRLevel(new_level);
         
         if (new_level == 0)
         {
            rf->disableSource();
         }
         else
         {
            rf->enableSource();
         }

         // Force the window to repaint.
         ossimQtDisplayUtility::flushAllOutputs( rf.get(), true );
      }
   }
}

void ossimQtSingleImageWindow::openImage()
{
   QString caption = "iview : open image";
   QString directory;
   if (!theLastOpenedDirectory.empty())
   {
      if (theLastOpenedDirectory.exists())
      {
         directory = theLastOpenedDirectory.c_str();
      }
   }
   
   QFileDialog* fd = new QFileDialog( this, caption, directory );
   fd->setFileMode( QFileDialog::ExistingFile );
   
   ossimFilename fileName;
  
   if ( fd->exec() == QDialog::Accepted )
   {
      fileName = fd->selectedFile().ascii();
      theLastOpenedDirectory = fileName.path();
   }

   delete fd;
   fd = 0;
   
   if (fileName.empty())
   {
      return;
   }

   ossimFilename f;
   getImageFile(f);

   if ( f != fileName )
   {
      displayImage(fileName);
   }
   else
   {
      QString caption = "NOTICE:";
      QString text = "Image is already open!";
      text += "  \nUse \"File->Close Image\"\n";
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );
   }
}

void ossimQtSingleImageWindow::closeImage()
{
   if ( theImageChain.valid() )
   {
      ossimConnectableObject* obj = dynamic_cast<ossimConnectableObject*>(theImageWidget);
      if ( obj ) obj->disconnect();
      theImageChain = 0;
      QString caption = "iview";
      setCaption(caption);
      theImageWidget->refresh();
      if (theResolutionLevelMenu)
      {
         theResolutionLevelMenu->clear();
      }
      theImageChain = 0;
   }
}

void ossimQtSingleImageWindow::exportImage()
{
   if (!theImageChain.valid())
   {
      QString caption = "Sorry:";
      QString text = "You must open an image first.";
      text += "  Use \"File->Open Image\"\n";
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );
      return;
   }
   
   ossimString name = "ossimQtIcpDialog";

   // First see if we already have a dialog open for this.
   QWidget* w = ossimQtEditorWidgetManager::instance()->getFromList(name,
                                                                    this);
   if (w)
   {
      w->raise();
      return;
   }

   w = new ossimQtIcpDialog(this, theImageChain.get());

   // Add to the list so we don't make a duplicate dialog.
   ossimQtEditorWidgetManager::instance()->addToList(w, this);

   w->show();
}

void ossimQtSingleImageWindow::closeWindow()
{
   close(true);
}

void ossimQtSingleImageWindow::displayImage(const ossimFilename& file)
{
   // Close any existing file open:
   closeImage();

   ossimRefPtr<ossimImageHandler> ih = ossimImageHandlerRegistry::instance()->open(file);
   if (!ih)
   {
      QString caption = "Sorry:";
      QString text = "Could not find the image handler for file:\n";
      text += file.c_str();
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );
      return;
   }

   if (ih->getNumberOfDecimationLevels() == 1)
   {
      QString caption("Question:");
      QString text = "Would you like to build reduced resolution data sets?\n";
      text += "Note:\n";
      text += "This can take some time depending on the size of your image.";
      text += "\nAlternatively use the command line application:  \"img2rr\"";

      int answer = QMessageBox::question( this,
                                          caption,
                                          text,
                                          QMessageBox::Yes,
                                          QMessageBox::No);
      if (answer == QMessageBox::Yes)
      {
         //---
         // We need to listen for the open overview signal to rebuild the
         // theResolutionLevelMenu.
         //---
         ih->addListener(this);
         buildOverViews( ih.get() );
      }
   }

   createImageChain( ih.get() );

   // Build the resolution level menu.
   buildResolutionLevelMenu();

   // Give it to the widget.
   theImageWidget->connectMyInputTo(theImageChain.get());
   theImageWidget->refresh();

   connectMyInputTo(theImageChain.get());
   QString caption = "iview : ";
   caption += file.file().c_str();
   setCaption(caption);
}

void ossimQtSingleImageWindow::createImageChain(ossimImageHandler* ih)
{
   if ( !ih )
   {
      return;
   }
   
   // Build an image chain.
   if (theImageChain.valid())
   {
      //---
      // Send a dummy message to the status bar to clear the values from a
      // previous.
      //---
      statusBar()->message( QString(" ") );
   }

   theImageChain = new ossimImageChain;

   // We need to listen to the chain.
   theImageChain->addListener(this);

   // Add the image handler.
   if (theImageChain->addLast(ih) == false)
   {
      theImageChain = 0;
      return;
   }

   if (ih->getNumberOfOutputBands() > 1)
   {
      // Add a band selector.
      ossimBandSelector* bs = new ossimBandSelector();
      theImageChain->addFirst(bs);
   }

   if (ih->getNumberOfDecimationLevels())
   {
      ossimRLevelFilter* rf = new ossimRLevelFilter();

      // Start with this filter disabled since we're coming up in r0.
      rf->disableSource();
      theImageChain->addFirst(rf);
   }

   // Add a cache at the end of the chain.
   ossimCacheTileSource* cache = new ossimCacheTileSource();
   theImageChain->addFirst(cache);

   theImageChain->initialize();
}

void ossimQtSingleImageWindow::trackImageWidget(ossimQtMouseEvent* event)
{
   if (!event)
   {
      return;
   }
   
   if (!theImageChain.valid())
   {
      event->getEvent()->ignore();
      return;
   }

   switch(event->getQtEventType())
   {
      case QEvent::MouseButtonPress:
      case QEvent::MouseMove:
      {
         //---
         // We want to track only left clicks or left down and drag.
         //---
         if( (event->getState()  == Qt::LeftButton) ||
             (event->getButton() == Qt::LeftButton) )
         {
            ossimIpt pt = event->getShiftPoint();
            std::ostringstream os;
            os << setiosflags(ios::right)
               << "line:"    << setw(9) << pt.y
               << "    sample:" << setw(9) << pt.x
               << "    pixel value";
            
            ossim_uint8 r;
            ossim_uint8 g;
            ossim_uint8 b;
            theImageWidget->getRgb(pt, r, g, b);
            
            const ossim_uint32 BANDS =
               theImageChain->getNumberOfOutputBands();
            switch (BANDS)
            {
               case 1:
                  os << ":" << setw(6) << int(r);
                  break;
               case 2:
                  os << "s:" << setw(6) << int(r) << setw(6) << int(g);
                  break;
               default:
                  os << "s:" << setw(6) << int(r) << setw(6) << int(g)
                     << setw(6) << int(b);
                  break;
            }
            
            QString qs = os.str().c_str();
            statusBar()->message(qs);
         }
         else
         {
            event->getEvent()->ignore();
         }
         break;
      }
      
      default:
      {
         event->getEvent()->ignore();
         break;
      }
   }
}


void ossimQtSingleImageWindow::editImageChain()
{
   if (!theImageChain.valid())
   {
      QString caption = "Sorry:";
      QString text = "You must open an image first.";
      text += "  Use \"File->Open Image\"\n";
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );
      return;
   }

   ossimString name = "ossimQtImageChainEditorDialog";

   // First see if we already have a dialog open for this.
   QWidget* w = ossimQtEditorWidgetManager::instance()->getFromList(name,
                                                                    this);
   if (w)
   {
      w->raise();
      return;
   }
      
   ossimQtImageChainEditorDialog* chainEditor
      = new ossimQtImageChainEditorDialog(this,
                                          name.c_str(),
                                          false,
                                          Qt::WDestructiveClose);
   chainEditor->setImageWidget(theImageWidget);
//   vector<ossimString> list;
//   getAllowedFilterList(list);
//   chainEditor->setAvailableFilterList(list);

   // Add to the list so we don't make a duplicate dialog.
   ossimQtEditorWidgetManager::instance()->addToList(chainEditor, this);

   chainEditor->show();
}

void ossimQtSingleImageWindow::about()
{
   ossimString name = "ossimQtIviewAboutDialog";

   // First see if we already have a dialog open for this.
   QWidget* w = ossimQtEditorWidgetManager::instance()->getFromList(name);
   if (w)
   {
      w->raise();
      return;
   }
   
   w = new ossimQtIviewAboutDialog(this);

   // Add to the list so we don't make a duplicate dialog.
   ossimQtEditorWidgetManager::instance()->addToList(w);

   w->show();
}

void ossimQtSingleImageWindow::getAllowedFilterList(vector<ossimString>& list) const
{
   list.push_back(ossimString("ossimBandSelector"));
   list.push_back(ossimString("ossimBrightnessContrastSource"));   
   list.push_back(ossimString("ossimCastTileSourceFilter"));
   list.push_back(ossimString("ossimHistogramRemapper"));   
   list.push_back(ossimString("ossimHsiRemapper"));
   list.push_back(ossimString("ossimMeanMedianFilter"));
   list.push_back(ossimString("ossimNBandToIndexFilter"));
   list.push_back(ossimString("ossimPixelFlipper"));
   list.push_back(ossimString("ossimRgbToGreyFilter"));
   list.push_back(ossimString("ossimRLevelFilter"));
   list.push_back(ossimString("ossimScalarRemapper"));
   list.push_back(ossimString("ossimWatermarkFilter"));
   list.push_back(ossimString("ossimOpenC.*"));
}

void ossimQtSingleImageWindow::customEvent(QEvent* event)
{
   if (!theImageChain.valid()) // No image open...
   {
      return;
   }

   ossimQtEventIdType type = static_cast<ossimQtEventIdType>(event->type());
   switch(type)
   {
      case OSSIM_QT_EVENT_REFRESH_DISPLAY_ID:
      {
         theImageWidget->refresh(false);
         break;
      }
      case OSSIM_QT_EVENT_FLUSH_DISPLAY_ID:
      {
         theImageWidget->refresh(true);
         break;
      }
      default:
      {
         break;
      }
   }
}

void ossimQtSingleImageWindow::displayListEvent(ossimDisplayListEvent& event)
{
   event.addDisplay(this);
}

void ossimQtSingleImageWindow::displayRefreshEvent(ossimDisplayRefreshEvent& /* event */)
{
   theImageWidget->refresh(false);
}

bool ossimQtSingleImageWindow::canConnectMyInputTo(ossim_int32 /*myInputIndex*/,
                                    const ossimConnectableObject* /* object */)const
{
   return true;
}

void ossimQtSingleImageWindow::buildOverViews(ossimImageHandler* ih)
{
   if (!ih)
   {
      QString caption = "Sorry:";
      QString text = "You must open an image first.";
      text += "  Use \"File->Open Image\"\n";
      QMessageBox::information( this,
                                caption,
                                text,
                                QMessageBox::Ok );
      return;
   }

   ossimQtOverviewBuilderDialog* builder = new ossimQtOverviewBuilderDialog(this, ih);
   builder->buildClicked();
   delete builder;
}

void ossimQtSingleImageWindow::buildResolutionLevelMenu()
{
   if (!theResolutionLevelMenu) return;
   
   ossim_uint32 count = theResolutionLevelMenu->count();
   ossim_uint32 numberOfReducedResSets = 1;
   
   ossimRefPtr<ossimRLevelFilter> rf = getRLevelFilter();
   if ( rf.valid() )
   {
      numberOfReducedResSets = rf->getNumberOfDecimationLevels();
   }

   if (count != numberOfReducedResSets)
   {
      theResolutionLevelMenu->clear();
      QActionGroup* rsetGroup = new QActionGroup(theResolutionLevelMenu);
      
      for (ossim_uint32 i = 0; i < numberOfReducedResSets; ++i)
      {
         int id = static_cast<int>(i);
         QString qs = ossimString::toString(i).c_str();

         //---
         //Create the QAction button (Rset button) and assign it to the
         // rset group so that only one Rset is checked at any given time.
         //---
         QAction* rsetAct = new QAction(qs, rsetGroup);
         rsetAct->setCheckable(true);
         
         //Set the Rset 0 button to checked upon initial entry
         if (i == 0)rsetAct->setChecked(true);

         rsetAct->setData(id);
         rsetAct->setEnabled(true);
         theResolutionLevelMenu->addAction( rsetAct );
         rsetGroup->addAction( rsetAct );
      }
   }
}

ossimRefPtr<ossimRLevelFilter> ossimQtSingleImageWindow::getRLevelFilter()
{
   ossimRefPtr<ossimRLevelFilter> rf = 0;
   if ( theImageChain.valid() )
   {
      ossimTypeNameVisitor visitor(ossimString("ossimRLevelFilter"),
                                   true,
                                   ossimVisitor::VISIT_CHILDREN|ossimVisitor::VISIT_INPUTS);
      
      theImageChain->accept(visitor);
      ossimRefPtr<ossimObject> obj = visitor.getObject();
      if ( obj.valid() )
      {
         rf = dynamic_cast<ossimRLevelFilter*>( obj.get() );
      }
   }
   return rf;
}

bool ossimQtSingleImageWindow::getImageFile(ossimFilename& file)
{
   bool status = false;
   {
      if ( theImageChain.valid() )
      {
         ossimTypeNameVisitor visitor(ossimString("ossimImageHandler"),
                                      true,
                                      ossimVisitor::VISIT_CHILDREN|ossimVisitor::VISIT_INPUTS);
         
         theImageChain->accept(visitor);
         ossimRefPtr<ossimObject> obj = visitor.getObject();
         if ( obj.valid() )
         {
            ossimRefPtr<ossimImageHandler> ih = dynamic_cast<ossimImageHandler*>( obj.get() );
            if ( ih.valid() )
            {
               file = ih->getFilename();
               status = true;
            }
         }
      }
   }
   return status;
}

// Hidden from use...
ossimQtSingleImageWindow::ossimQtSingleImageWindow(const ossimQtSingleImageWindow& /* obj */)
   : QMainWindow(),
     ossimConnectableObject(),
     ossimConnectableDisplayListener()
{
   ossimReferenced::ref();
}

// Hidden from use...
const ossimQtSingleImageWindow& ossimQtSingleImageWindow::operator=(const ossimQtSingleImageWindow& /* rhs */)
{
   return *this;
}

   
