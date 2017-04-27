//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
// 
// Description:
// Main interface between the elevation manager and the elevation manager
// dialog box.
//
//*************************************************************************
// $Id: ossimQtElevationManagerController.cpp 16149 2009-12-19 03:16:50Z okramer $

#include <vector>
using namespace std;

#include <QtGui/QCheckBox>
#include <QtCore/QStringList>
#include <QtGui/QMessageBox>
#include <QtGui/QListWidget>
#include <ossimQt/ossimQtElevationManagerController.h>
#include <ossimQt/ossimQtElevationManagerDialog.h>
#include <ossimQt/ossimQtEvent.h>
#include <ossimQt/ossimQtApplicationUtility.h>
#include <ossimQt/ossimDataManager.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/elevation/ossimElevManager.h>

ossimFilename ossimQtElevationManagerController::theLatestOpenCellPath;

#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimOrthoImageMosaic.h>
#include <ossim/base/ossimTrace.h>

static ossimTrace traceDebug("ossimQtElevationManagerController:degug");

ossimQtElevationManagerController::ossimQtElevationManagerController(ossimQtElevationManagerDialog* dialog)
   :
      theDialog(dialog),
      theMgr(NULL)
{
   // Get the pointer to the elevation manager.
   theMgr = ossimElevManager::instance();
   theMgr->addListener( this );
   
   update(); // Initialize the dialog box.
   
}

ossimQtElevationManagerController::~ossimQtElevationManagerController()
{
   if ( theMgr )
   {
      theMgr->removeListener( this );
      theMgr = NULL;
   }
}

void ossimQtElevationManagerController::onAdd(const QList<QListWidgetItem *>&) const
{
   // Give the user an already open
   QMessageBox::warning( theDialog,
                        "ERROR",
                        "NEED TO IMPLEMENT FOR NEW ELEVATION MANAGER",
                        QMessageBox::Ok,
                        QMessageBox::NoButton);
#if 0   
   static const char MODULE[] = "ossimQtElevationManagerController::onAdd";

   QList<QListWidgetItem *>::const_iterator it = cells.begin();
   while( it != cells.end() )
   {
      ossimFilename cell = (*it)->text().ascii();

      if (traceDebug())
      {
         CLOG << "\ncell:  " << cell
              << std::endl;
      }
      
      if (theMgr->isCellOpen(cell))
      {
         QString caption("Notice:");
         QString text = cell.c_str();
         text += " is already loaded by the elevation manager.";

         // Give the user an already open
         QMessageBox::information ( theDialog,
                                    caption,
                                    text,
                                    QMessageBox::Ok);
         
      }
      else if (theMgr->openCell(cell) == false)
      {
         QString caption("Warning:");
         QString text = cell.c_str();
         text += " could not be opened!";

         // Give the user an already open
         QMessageBox::warning( theDialog,
                               caption,
                               text,
                               QMessageBox::Ok,
                               QMessageBox::NoButton);
         
      }
      ++it;
   }

   updateListBox();
#endif
} 

void ossimQtElevationManagerController::onAdd(const QStringList&) const
{
   // Give the user an already open
   QMessageBox::warning( theDialog,
                        "ERROR",
                        "NEED TO IMPLEMENT FOR NEW ELEVATION MANAGER",
                        QMessageBox::Ok,
                        QMessageBox::NoButton);
   
#if 0
   static const char MODULE[] = "ossimQtElevationManagerController::onAdd";

   QStringList::const_iterator it = cells.begin();
   while( it != cells.end() )
   {
      ossimFilename cell = (*it).ascii();

      if (traceDebug())
      {
         CLOG << "\ncell:  " << cell
              << std::endl;
      }

      
      if (theMgr->isCellOpen(cell))
      {
         QString caption("Notice:");
         QString text = cell.c_str();
         text += " is already loaded by the elevation manager.";

         // Give the user an already open
         QMessageBox::information ( theDialog,
                                    caption,
                                    text,
                                    QMessageBox::Ok);
         
      }
      else if (theMgr->openCell(cell) == false)
      {
         QString caption("Warning:");
         QString text = cell.c_str();
         text += " could not be opened!";

         // Give the user an already open
         QMessageBox::warning( theDialog,
                               caption,
                               text,
                               QMessageBox::Ok,
                               QMessageBox::NoButton);
         
      }
      ++it;
   }

   updateListBox();
#endif
} 

void ossimQtElevationManagerController::onAddDirectory(const QString& /*directory*/) const
{
   // Give the user an already open
   QMessageBox::warning( theDialog,
                        "ERROR",
                        "NEED TO IMPLEMENT FOR NEW ELEVATION MANAGER",
                        QMessageBox::Ok,
                        QMessageBox::NoButton);
#if 0   
   ossimFilename dir = (const char*)directory;
   if (theMgr->openDirectory(dir) == false)
   {
      QString caption("Warning:");
      QString text = dir.c_str();
      text += " could not be opened!";
   }

   updateListBox();
#endif
}

void
ossimQtElevationManagerController::onRemove(const QList<QListWidgetItem *>& /*cells*/) const
{
   // Give the user an already open
   QMessageBox::warning( theDialog,
                        "ERROR",
                        "NEED TO IMPLEMENT FOR NEW ELEVATION MANAGER",
                        QMessageBox::Ok,
                        QMessageBox::NoButton);
#if 0
   QList<QListWidgetItem *>::const_iterator it = cells.begin();
   while( it != cells.end() )
   {
      ossimFilename cell((*it)->text().ascii());
      if (!theMgr->closeCell(cell))
      {
         QString caption("Warning:");
         QString text = cell.c_str();
         text += " could not be closed!";
      }

      ++it;
   }

   updateListBox();
#endif
} 

void ossimQtElevationManagerController::onRemoveAll() const
{
   // Give the user an already open
   QMessageBox::warning( theDialog,
                        "ERROR",
                        "NEED TO IMPLEMENT FOR NEW ELEVATION MANAGER",
                        QMessageBox::Ok,
                        QMessageBox::NoButton);
#if 0   
   theMgr->closeAllCells();
   updateListBox();
#endif
}

void ossimQtElevationManagerController::onUp(const QList<QListWidgetItem *>& /*cells*/) const
{
   // Give the user an already open
   QMessageBox::warning( theDialog,
                        "ERROR",
                        "NEED TO IMPLEMENT FOR NEW ELEVATION MANAGER",
                        QMessageBox::Ok,
                        QMessageBox::NoButton);
#if 0   
   QList<QListWidgetItem *>::const_iterator it = cells.begin();
   while( it != cells.end() )
   {
      ossimFilename cell((*it)->text().ascii());
      if (!theMgr->moveCellUpOne(cell))
      {
         QString caption("Warning:");
         QString text = cell.c_str();
         text += " could not be moved up!";

         // Give the user an already open
         QMessageBox::warning( theDialog,
                               caption,
                               text,
                               QMessageBox::Ok,
                               QMessageBox::NoButton);
      }

      ++it;
   }

   updateListBox();
#endif
}


void ossimQtElevationManagerController::onDown(const QList<QListWidgetItem *>& /*cells*/) const
{
   // Give the user an already open
   QMessageBox::warning( theDialog,
                        "ERROR",
                        "NEED TO IMPLEMENT FOR NEW ELEVATION MANAGER",
                        QMessageBox::Ok,
                        QMessageBox::NoButton);
#if 0
   QList<QListWidgetItem *>::const_iterator it = cells.end();
   int count = cells.count();
   --it;

   for (int i = 0; i < count; ++i)
   {
      ossimFilename cell = (*it)->text().ascii();
      if (!theMgr->moveCellDownOne(cell))
      {
         // Send a warning
         QString caption("Warning:");
         QString text = cell.c_str();
         text += " could not be moved down!";
         QMessageBox::warning( theDialog,
                               caption,
                               text,
                               QMessageBox::Ok,
                               QMessageBox::NoButton);
      }
      --it;
   }

   updateListBox();
#endif
}

void ossimQtElevationManagerController::onTop(const QList<QListWidgetItem *>& /*cells*/) const
{
   // Give the user an already open
   QMessageBox::warning( theDialog,
                        "ERROR",
                        "NEED TO IMPLEMENT FOR NEW ELEVATION MANAGER",
                        QMessageBox::Ok,
                        QMessageBox::NoButton);
#if 0
   QList<QListWidgetItem *>::const_iterator it = cells.end();
   int count = cells.count();
   --it;
   
   for (int i = 0; i < count; ++i)
   {
      ossimFilename cell = (*it)->text().ascii();
      if (!theMgr->moveCellToTop(cell))
      {
         // Send a warning
         QString caption("Warning:");
         QString text = cell.c_str();
         text += " could not be moved to top!";
         QMessageBox::warning( theDialog,
                               caption,
                               text,
                               QMessageBox::Ok,
                               QMessageBox::NoButton);
      }
      --it;
   }

   updateListBox();
#endif
}

void ossimQtElevationManagerController::onBottom(const QList<QListWidgetItem *>& /*cells*/) const
{
   // Give the user an already open
   QMessageBox::warning( theDialog,
                        "ERROR",
                        "NEED TO IMPLEMENT FOR NEW ELEVATION MANAGER",
                        QMessageBox::Ok,
                        QMessageBox::NoButton);
#if 0   
   QList<QListWidgetItem *>::const_iterator it = cells.begin();
   while( it != cells.end() )
   {
      ossimFilename cell = (*it)->text().ascii();
      if (!theMgr->moveCellToBottom(cell))
      {
         QString caption("Warning:");
         QString text = cell.c_str();
         text += " could not be moved to bottom!";

         // Give the user an already open
         QMessageBox::warning( theDialog,
                               caption,
                               text,
                               QMessageBox::Ok,
                               QMessageBox::NoButton);
      }

      ++it;
   }

   updateListBox();
#endif
}

void ossimQtElevationManagerController::onClose(const QList<QListWidgetItem *>& /*cells*/) const
{
   // Give the user an already open
   QMessageBox::warning( theDialog,
                        "ERROR",
                        "NEED TO IMPLEMENT FOR NEW ELEVATION MANAGER",
                        QMessageBox::Ok,
                        QMessageBox::NoButton);
#if 0   
   QList<QListWidgetItem *>::const_iterator it = cells.begin();
   while( it != cells.end() )
   {
      ossimFilename cell = (*it)->text().ascii();
      if (!theMgr->closeCell(cell))
      {
         QString caption("Warning:");
         QString text = cell.c_str();
         text += " could not be closed!";

         // Give the user an already open
         QMessageBox::warning( theDialog,
                               caption,
                               text,
                               QMessageBox::Ok,
                               QMessageBox::NoButton);
      }

      ++it;
   }

   updateListBox();
#endif
}

void ossimQtElevationManagerController::updateListBox() const
{
   // Give the user an already open
   QMessageBox::warning( theDialog,
                        "ERROR",
                        "NEED TO IMPLEMENT FOR NEW ELEVATION MANAGER",
                        QMessageBox::Ok,
                        QMessageBox::NoButton);
#if 0   
   if (theDialog && theMgr)
   {
      if (theDialog->theElevationListBox)
      {
         theDialog->theElevationListBox->clear();

         // Get the new list and add it to the list box.
         vector<ossimFilename> cellList;
         theMgr->getOpenCellList(cellList);
         ossim_uint32 number_of_open_cells = cellList.size();
         for (ossim_uint32 i=0; i<number_of_open_cells; ++i)
         {
            QString item = cellList[i].c_str();
            theDialog->theElevationListBox->addItem(item);
         }
      }
   }
#endif
}

void ossimQtElevationManagerController::updateEnableBox() const
{
   if (theDialog && theMgr)
   {
      if (theDialog->theElevationEnabledCheckBox)
      {
         theDialog->theElevationEnabledCheckBox->
            setChecked(theMgr->isSourceEnabled());
      }
   }
}

void ossimQtElevationManagerController::updateAutoLoadEnableBox() const
{
   // Give the user an already open
   QMessageBox::warning( theDialog,
                        "ERROR",
                        "NEED TO IMPLEMENT FOR NEW ELEVATION MANAGER",
                        QMessageBox::Ok,
                        QMessageBox::NoButton);
#if 0   
   if (theDialog && theMgr)
   {
      if (theDialog->theAutoLoadCheckBox)
      {
         theDialog->theAutoLoadCheckBox->
            setChecked(theMgr->isAutoLoadEnabled());
      }
   }
#endif
}

void ossimQtElevationManagerController::updateAutoSortEnableBox() const
{
   // Give the user an already open
   QMessageBox::warning( theDialog,
                        "ERROR",
                        "NEED TO IMPLEMENT FOR NEW ELEVATION MANAGER",
                        QMessageBox::Ok,
                        QMessageBox::NoButton);
#if 0   
   if (theDialog && theMgr)
   {
      if (theDialog->theAutoSortCheckBox)
      {
         theDialog->theAutoSortCheckBox->
            setChecked(theMgr->isAutoSortEnabled());
      }
   }
#endif
}

void ossimQtElevationManagerController::onEnable(bool flag) const
{
   if (theMgr)
   {
      if (flag)
      {
         theMgr->enableSource();
      }
      else
      {
         theMgr->disableSource();
      }
   }
}

void ossimQtElevationManagerController::onAutoLoadEnable(bool /*flag*/) const
{
   // Give the user an already open
   QMessageBox::warning( theDialog,
                        "ERROR",
                        "NEED TO IMPLEMENT FOR NEW ELEVATION MANAGER",
                        QMessageBox::Ok,
                        QMessageBox::NoButton);
#if 0   
   if (theMgr)
   {
      if (flag)
      {
         theMgr->enableAutoLoad();
      }
      else
      {
         theMgr->disableAutoLoad(); 
      }
   }
#endif
}

void ossimQtElevationManagerController::onAutoSortEnable(bool /*flag*/) const
{
   // Give the user an already open
   QMessageBox::warning( theDialog,
                        "ERROR",
                        "NEED TO IMPLEMENT FOR NEW ELEVATION MANAGER",
                        QMessageBox::Ok,
                        QMessageBox::NoButton);
#if 0   
   if (theMgr)
   {
      if (flag)
      {
         theMgr->enableAutoSort();
      }
      else
      {
         theMgr->disableAutoSort(); 
      }
   }
#endif
}   

void ossimQtElevationManagerController::onElevOffsetChanged(double value) const
{
   if (theMgr)
      theMgr->setElevationOffset(value);
}   

void ossimQtElevationManagerController::onMosaic(
   const QList<QListWidgetItem*>& cells) const
{
   std::vector<ossimFilename> fileList;
   ossimQtGetDataManagerEvent tempEvt;
   ossimQtApplicationUtility::sendEventToRoot(theDialog, &tempEvt);

   for(ossim_int32 idx = 0; idx < cells.count(); ++idx)
   {
      fileList.push_back(cells[idx]->text().ascii());
   }
   ossimConnectableObject* result = NULL;
   if(tempEvt.getDataManager())
   {
      result = tempEvt.getDataManager()->createStandardElevMosaic(fileList);
   }

   if(result)
   {
      ossimQtDisplayChainEvent *evt = new ossimQtDisplayChainEvent(result->getId().getId());
      ossimQtApplicationUtility::postEventToRoot(theDialog, evt);
   }
}

void ossimQtElevationManagerController::processEvent(ossimElevationManagerEvent&) const
{
   update();
}

void ossimQtElevationManagerController::update() const
{
   updateListBox();
   updateEnableBox();
   updateAutoLoadEnableBox();
   updateAutoSortEnableBox();
}

// static method...
ossimFilename ossimQtElevationManagerController::getLatestOpenCellPath()
{
   return theLatestOpenCellPath;
}

double ossimQtElevationManagerController::getElevOffset()
{
   if (theMgr)
      return theMgr->getElevationOffset();
   return 0.0;
}

// Private to disallow copy constructor, assignment operator use...
ossimQtElevationManagerController::ossimQtElevationManagerController(const ossimQtElevationManagerController&)
   :
      ossimElevationManagerEventListener()
{
}

ossimQtElevationManagerController& ossimQtElevationManagerController::operator=(const ossimQtElevationManagerController&)
{
   return *this;
}
