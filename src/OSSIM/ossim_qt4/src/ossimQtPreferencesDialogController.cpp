//*******************************************************************
//
// License:  See top LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimQtPreferencesDialogController.cpp 12141 2007-12-07 18:05:13Z gpotts $

#include <QtGui/QGroupBox>
#include <QtGui/QListWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

#include <ossimQt/ossimQtPreferencesDialogController.h>
#include <ossimQt/ossimQtPreferencesDialog.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/plugin/ossimSharedPluginRegistry.h>
#include <ossim/plugin/ossimPluginLibrary.h>
#include <ossimQt/ossimQtEvent.h>
#include <ossimQt/ossimQtApplicationUtility.h>

ossimQtPreferencesDialogController::ossimQtPreferencesDialogController(ossimQtPreferencesDialog* dialog)
{
   theDialog = dialog;
   
   transferToDialog();
}

ossimQtPreferencesDialogController::~ossimQtPreferencesDialogController()
{
   theDialog = 0;
}

void ossimQtPreferencesDialogController::close()
{
}

void ossimQtPreferencesDialogController::addPlugin()
{
   QStringList plugins = QFileDialog::getOpenFileNames();
   QStringList::iterator iter = plugins.begin();
   
   while(iter != plugins.end())
   {
      ossimFilename file = (*iter).ascii();

      if(ossimSharedPluginRegistry::instance()->registerPlugin(file))
      {
         theDialog->thePluginList->addItem(file.c_str());
      }
      ++iter;
   }
}

void ossimQtPreferencesDialogController::removePlugin()
{
   if(!theDialog)
   {
      return;
   }
   int numRows = theDialog->thePluginList->count();
   int idx = 0;

   if(numRows)
   {
      for(idx = 0; idx < numRows; ++idx)
      {
         QListWidgetItem* item = theDialog->thePluginList->item(idx);

         if(item)
         {
            if(item->isSelected())
            {
               ossimPluginLibrary* library = ossimSharedPluginRegistry::instance()->getPlugin(item->text().ascii());

               if(library)
               {
                  ossimQtQueryObjectAllocationEvent event;
                  std::vector<ossimString> queryList;
                  library->getClassNames(queryList);
                  event.setQueryList(queryList);

                  ossimQtApplicationUtility::sendEventToRoot(theDialog,
                                                             &event);

                  if(event.getObjectsAllocated() == false)
                  {
                     ossim_uint32 pluginIdx = ossimSharedPluginRegistry::instance()->getIndex(library);
                     ossimSharedPluginRegistry::instance()->unregisterPlugin(pluginIdx);
                  }
                  else
                  {
                     QMessageBox::critical(theDialog, "ERROR", "Can't remove plugin, please remove any object allocated by this plugin");
                  }
               }
            }
         }
      }

      transferToDialog();
   }
}

void ossimQtPreferencesDialogController::reloadPlugin()
{
   if(!theDialog)
   {
      return;
   }
   int numRows = theDialog->thePluginList->count();
   int idx = 0;

   if(numRows)
   {
      ossimString nonReloads;
      for(idx = 0; idx < numRows; ++idx)
      {
         QListWidgetItem* item = theDialog->thePluginList->item(idx);

         if(item)
         {
            if(item->isSelected())
            {
               ossimPluginLibrary* library = ossimSharedPluginRegistry::instance()->getPlugin(item->text().ascii());

               if(library)
               {
                  ossimFilename libraryName = library->getName();
                  ossimQtQueryObjectAllocationEvent event;
                  std::vector<ossimString> queryList;
                  library->getClassNames(queryList);
                  event.setQueryList(queryList);

                  ossimQtApplicationUtility::sendEventToRoot(theDialog,
                                                             &event);

                  if(event.getObjectsAllocated() == false)
                  {
                     ossim_uint32 pluginIdx = ossimSharedPluginRegistry::instance()->getIndex(library);
                     ossimSharedPluginRegistry::instance()->unregisterPlugin(pluginIdx);
                     ossimSharedPluginRegistry::instance()->registerPlugin(libraryName);
                  }
                  else
                  {
                     nonReloads +=libraryName + "\n";
                  }
               }
            }
         }
      }
      if(nonReloads != "")
      {
         QMessageBox::critical(theDialog, "Error reloading Plugins", (nonReloads+"\nhad errors since allocated objects exist from the plugin\n  Delete objects then reload").c_str());
      }
      transferToDialog();
   }
}

void ossimQtPreferencesDialogController::selectAllPlugin()
{
   theDialog->thePluginList->selectAll();
}

void ossimQtPreferencesDialogController::transferToDialog()
{
   if(!theDialog)
   {
      return;
   }
   ossimString label = theDialog->theTabWidget->tabLabel(theDialog->theTabWidget->currentPage()).ascii();
   label = label.downcase();
   if(label.contains("plugin"))
   {
      transferPluginToDialog();
   }
   else
   {
   }
   
}

void ossimQtPreferencesDialogController::transferPluginToDialog()
{
   if(!theDialog)
   {
      return;
   }

   theDialog->thePluginList->clear();
   ossim_uint32 idx = 0;
   theDialog->thePluginDescription->setText("");
   for(idx = 0; idx < ossimSharedPluginRegistry::instance()->getNumberOfPlugins(); ++idx)
   {
      ossimPluginLibrary* library = ossimSharedPluginRegistry::instance()->getPlugin(idx);
      if(library)
      {
         theDialog->thePluginList->addItem(library->getName().c_str());
      }
   }
}

void ossimQtPreferencesDialogController::currentPluginSelectionChanged(QListWidgetItem* item)
{
   if(item)
   {
      if(item->text() != "")
      {
         ossimPluginLibrary* library = ossimSharedPluginRegistry::instance()->getPlugin(item->text().ascii());
         if(library)
         {
            theDialog->thePluginDescription->setText(library->getDescription().c_str());
         }
      }
   }
}
