//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Interface to plugins.
//
// $Id$
//----------------------------------------------------------------------------

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QGroupBox>
#include <QtGui/QListWidget>
#include <QtGui/QListWidgetItem>
#include <QtGui/QTextEdit>

#include <ossimQt/ossimQtPluginsDialog.h>
#include <ossimQt/ossimQtEvent.h>
#include <ossimQt/ossimQtApplicationUtility.h>

#include <ossim/base/ossimString.h>
#include <ossim/plugin/ossimSharedPluginRegistry.h>
#include <ossim/plugin/ossimPluginLibrary.h>


ossimQtPluginsDialog::ossimQtPluginsDialog(QWidget* parent)
   : QDialog(parent, "ossimQtPluginsDialog", Qt::WDestructiveClose),
     thePluginList(0),
     theDescription(0),
     theAddButton(0),
     theRemoveButton(0),
     theCloseButton(0)
{
   setCaption("Plugin Dialog");
   
   // Main box to hold everything.  Parented to this dialog box...
   QVBoxLayout* mainLayout = new QVBoxLayout(this);

   // Row one:
   QGroupBox* row1GroupBox = new QGroupBox();

   // Row one:  2 columns horizontal. 
   QHBoxLayout* row1Layout = new QHBoxLayout();

   // Row one, Column one, a list of plugins.
   QGroupBox* pluginListGroupBox = new QGroupBox(QString("Plugins"));
   QVBoxLayout* pluginListLayout = new QVBoxLayout();

   // plugin list pointer stored by class.
   thePluginList = new QListWidget();

   // Parent list to layout.
   pluginListLayout->addWidget(thePluginList);

   // Parent layout to group box.
   pluginListGroupBox->setLayout(pluginListLayout);

   // Parent group box to row one layout.
   row1Layout->addWidget(pluginListGroupBox);

   // Row one , column two, selected plugin description.
   QGroupBox* descriptionGroupBox = new QGroupBox(QString("Description"));
   QVBoxLayout* descriptionLayout = new QVBoxLayout();

   // plugin description pointer stored by class.
   theDescription = new QTextEdit();

   // Only we can mess with text. 
   theDescription->setReadOnly(true);

   // Parent text edit to layout.
   descriptionLayout->addWidget(theDescription);

   // Parent layout to group box.
   descriptionGroupBox->setLayout(descriptionLayout);

   // Parent group box to row one layout.
   row1Layout->addWidget(descriptionGroupBox);

   // Parent the row1Layout to the row1GroupBox.
   row1GroupBox->setLayout(row1Layout);
   
   // Parent row one group box to main layout
   mainLayout->addWidget(row1GroupBox);

   //---
   // End of row one.
   //---


   //---
   // Row two:
   //---
   QGroupBox* row2GroupBox = new QGroupBox();

   // Row two:  horizontal buttonw. 
   QHBoxLayout* row2Layout = new QHBoxLayout();

   // Row two, column one, add button.
   theAddButton = new QPushButton("Add");
   row2Layout->addWidget(theAddButton);

   // Row two, column two, remove button.
   theRemoveButton = new QPushButton("Remove");
   row2Layout->addWidget(theRemoveButton);

   // Row two, column three, close button.
   theCloseButton = new QPushButton("Close");
   row2Layout->addWidget(theCloseButton);
   
    // Parent the row2Layout to the row2GroupBox.
   row2GroupBox->setLayout(row2Layout);
   
   // Parent row two group box to main layout
   mainLayout->addWidget(row2GroupBox);

   //---
   // End of row two.
   //---

   //---
   // Connect all the signals to slots...
   //---
   connect( thePluginList, SIGNAL ( itemSelectionChanged () ),
            this, SLOT ( pluginSelectionChanged() ) );

   connect( theAddButton, SIGNAL ( pressed() ),
            this, SLOT ( addButtonPressed() ) );

   connect( theRemoveButton, SIGNAL ( pressed() ),
            this, SLOT ( removeButtonPressed() ) );

   connect( theCloseButton, SIGNAL ( pressed() ),
            this, SLOT ( closeButtonPressed() ) );

   //---
   // Set up the plugin list and description.
   //---
   updateDialog();
   
}

ossimQtPluginsDialog::~ossimQtPluginsDialog()
{
}

void ossimQtPluginsDialog::pluginSelectionChanged()
{
   updateDescription();
}

void ossimQtPluginsDialog::addButtonPressed()
{
   QStringList plugins = QFileDialog::getOpenFileNames();
   QStringList::iterator iter = plugins.begin();
   
   while(iter != plugins.end())
   {
      ossimFilename file = (*iter).ascii();
      if (ossimSharedPluginRegistry::instance()->isLoaded(file) == false)
      {
         ossimSharedPluginRegistry::instance()->registerPlugin(file);
      }
      else
      {
         QString caption = "Sorry:";
         QString text = "Plugin loaded already!\n";
         text += file.c_str();
         QMessageBox::information( this,
                                   caption,
                                   text,
                                   QMessageBox::Ok );
      }
      ++iter;
   }
   updateDialog();
}

void ossimQtPluginsDialog::removeButtonPressed()
{
   ossimPluginLibrary* pi = getSelectedPlugin();
   if(pi)
   {
      int answer = QMessageBox::question(
         this,
         tr("Remove selected plugin:\n"),
         tr(pi->getName().c_str(), "\n yes or no"),
         QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton);
      if (answer == QMessageBox::Yes)
      {
         ossimQtQueryObjectAllocationEvent event;
         std::vector<ossimString> queryList;
         pi->getClassNames(queryList);
         event.setQueryList(queryList);
         
         ossimQtApplicationUtility::sendEventToRoot(this, &event);
         
         if(event.getObjectsAllocated() == false)
         {
            ossim_uint32 pluginIdx = ossimSharedPluginRegistry::instance()->
               getIndex(pi);
            ossimSharedPluginRegistry::instance()->unregisterPlugin(pluginIdx);
         }
         else
         {
            QMessageBox::critical(this,
                                  "ERROR",
                                  "Can't remove plugin, please remove any object allocated by this plugin");
         }
         updateDialog();
         
      } // End of "if (answer == QMessageBox::Yes)"
      
   } // End of "if (pi)"
}

void ossimQtPluginsDialog::closeButtonPressed()
{
   close(true);
}

ossimQtPluginsDialog::ossimQtPluginsDialog(
   const ossimQtPluginsDialog& /* obj */ )
   : QDialog(),
     thePluginList(0),
     theDescription(0),
     theAddButton(0),
     theRemoveButton(0),
     theCloseButton(0)
{
}

const ossimQtPluginsDialog& ossimQtPluginsDialog::operator=(
   const ossimQtPluginsDialog& /* rhs */ )
{
   return *this;
}

void ossimQtPluginsDialog::updateDialog()
{
   updatePluginList();
   updateDescription();
}

void ossimQtPluginsDialog::updatePluginList()
{
   if (thePluginList)
   {
      if (thePluginList->count())
      {
         thePluginList->clear();
      }

      ossim_uint32 count =
         ossimSharedPluginRegistry::instance()->getNumberOfPlugins();
      for (ossim_uint32 i = 0; i < count; ++i)
      {
         ossimPluginLibrary* pi =
            ossimSharedPluginRegistry::instance()->getPlugin(i);
         if (pi)
         {
            thePluginList->addItem(pi->getName().c_str());
         }
      }
      if (count)
      {
         thePluginList->setCurrentRow(0);
      }
   }
}

void ossimQtPluginsDialog::updateDescription()
{
   if (thePluginList && theDescription)
   {
      QString text = "";
      ossimPluginLibrary* pi = getSelectedPlugin();
      if (pi)
      {
         text = pi->getDescription().c_str();
      }
      theDescription->setPlainText(text);
   }
}

ossimPluginLibrary* ossimQtPluginsDialog::getSelectedPlugin() const
{
   ossimPluginLibrary* result = 0;
   if (thePluginList)
   {
      QListWidgetItem* currentItem = thePluginList->currentItem();
      if (currentItem)
      {
         ossimString name = currentItem->text().toStdString();
         ossim_uint32 count =
            ossimSharedPluginRegistry::instance()->getNumberOfPlugins();
         
         for (ossim_uint32 i = 0; i < count; ++i)
         {
            ossimPluginLibrary* pi =
               ossimSharedPluginRegistry::instance()->getPlugin(i);
            if (pi)
            {
               if (name == pi->getName())
               {
                  result = pi;
                  break;
               }
            }
         }
      }
   }
   return result;
}
