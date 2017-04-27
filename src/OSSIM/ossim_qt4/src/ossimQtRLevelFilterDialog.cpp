//----------------------------------------------------------------------------
// Copyright (C) 2004 Intelligence Data Systems, Inc. All rights reserved.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Property editor dialog for ossimRLevelFilter.
//
// $Id: ossimQtRLevelFilterDialog.cpp 23664 2015-12-14 14:17:27Z dburken $
//----------------------------------------------------------------------------

#include <Qt3Support/Q3VBox>
#include <Qt3Support/Q3HBox>
#include <QtGui/QLayout>
#include <QtGui/QPushButton>
#include <QtGui/QCheckBox>
#include <QtGui/QSizePolicy>
#include <QtGui/QComboBox>
#include <Qt3Support/Q3GroupBox>
//Added by qt3to4:
#include <Qt3Support/Q3HBoxLayout>
#include <Qt3Support/Q3VBoxLayout>

#include <ossimQt/ossimQtRLevelFilterDialog.h>
#include <ossimQt/ossimQtDisplayUtility.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/imaging/ossimRLevelFilter.h>

static ossimTrace traceDebug("ossimQtRLevelFilterDialog:degug");


ossimQtRLevelFilterDialog::ossimQtRLevelFilterDialog(QWidget* parent,
                             const char* name,
                             bool modal,
                             Qt::WFlags f)
   : QDialog(parent, name, modal, f),
     ossimConnectableObjectListener(),
     theParent(parent),
     theFilter(NULL),
     theMainVBox(NULL),
     theHBox1(NULL),
     theRLevelGroupBox(NULL),
     theRLevelMenu(NULL),
     theHBox2(NULL),
     theEnableGroupBox(NULL),
     theEnableCheckBox(NULL),
     theHBox3(NULL),
     theCloseButton(NULL)
{
   setCaption("Reduced Resolution Property Editor");

   // Main vertical box.
   theMainVBox = new Q3VBoxLayout(this);

   // First row, the rlevel pull down.
   theHBox1 = new Q3HBoxLayout(theMainVBox);

   // Make a box to put the pull down menu in.
   theRLevelGroupBox = new Q3GroupBox("current resolution level", this);
   theRLevelGroupBox->setAlignment(Qt::AlignHCenter);
   
   // Make the "Resolution Level" pull down menu.
   theRLevelMenu = new QComboBox(theRLevelGroupBox);

   theHBox1->addWidget(theRLevelGroupBox);
   
   // Second row, filter enable disable check box.
   theHBox2 = new Q3HBoxLayout(theMainVBox);

   // Make a box to put the check box in.
   theEnableGroupBox = new Q3GroupBox("filter enabled / disabled", this);
   theEnableGroupBox->setAlignment(Qt::AlignHCenter);

   // Make the check box.
   theEnableCheckBox = new  QCheckBox(theEnableGroupBox);
   theEnableCheckBox->setText("enabled");

   theHBox2->addWidget(theEnableGroupBox);

   // Third row, output image base name.
   theHBox3 = new Q3HBoxLayout(theMainVBox);

   theCloseButton = new QPushButton( this, "theCloseButton" );
   theCloseButton->setText( "Close" );
   theCloseButton->setDefault(false);
   theCloseButton->setAutoDefault(false);   

   theHBox3->addWidget(theCloseButton);

   //---
   // Connect all the signals to slots...
   //
   // NOTE:
   // If a push button is set to "default" or "auto default" it will always
   // call that slot when return is pressed in the dialog.
   // Since this isn't what I want, all defaults are set to off!
   //---
   connect( theRLevelMenu, SIGNAL ( activated(int) ),
            this, SLOT ( rlevelChanged(int) ) );

   connect( theEnableCheckBox, SIGNAL ( clicked() ),
            this, SLOT ( enableClicked() ) );
   
   connect( theCloseButton, SIGNAL ( clicked() ),
            this, SLOT ( closeClicked() ) );
}

ossimQtRLevelFilterDialog::~ossimQtRLevelFilterDialog()
{
   if (theFilter)
   {
      theFilter->removeListener((ossimConnectableObjectListener*)this);
      theFilter = NULL;
   }
}

void ossimQtRLevelFilterDialog::objectDestructingEvent(ossimObjectDestructingEvent& event)
{
   if (theFilter)
   {
      if(event.getObject() == PTR_CAST(ossimObject, theFilter))
      {      
         close(true);
      }
   }
}

void ossimQtRLevelFilterDialog::setRLevelFilter(ossimRLevelFilter* filter)
{
   if (theFilter)
   {
      theFilter->removeListener((ossimConnectableObjectListener*)this);
   }
    
   theFilter = filter;

   if(theFilter)
   {
      theFilter->addListener((ossimConnectableObjectListener*)this);
   }
   
   buildRLevelMenu();
   updateRLevelMenu();
   updateEnableMenu();
}

void ossimQtRLevelFilterDialog::rlevelChanged(int level)
{
   if (!theFilter || !theRLevelMenu || !theEnableCheckBox)
   {
      return;
   }
   ossim_uint32 new_level = static_cast<ossim_uint32>(level);
   ossim_uint32 current_level = theFilter->getCurrentRLevel();
   if (new_level != current_level)
   {
      theFilter->setCurrentRLevel(new_level);

      if (new_level == 0)
      {
         theFilter->disableSource();
         theFilter->setOverrideGeometryFlag(false);
      }
      else
      {
         theFilter->enableSource();
         theFilter->setOverrideGeometryFlag(true);
      }

      // Update the gui enable.
      updateEnableMenu();

      // Force the window to repaint.
      ossimQtDisplayUtility::flushAllOutputs(theFilter, true);
   }
}

void ossimQtRLevelFilterDialog::enableClicked()
{
   if (!theFilter || !theEnableCheckBox)
   {
      return;
   }
   
   bool gui_status    = theEnableCheckBox->isChecked();
   bool filter_status = theFilter->getEnableFlag();

   if (gui_status != filter_status)
   {
      // Set the filter.
      theFilter->setEnableFlag(gui_status);

      // Force the window to repaint.
      ossimQtDisplayUtility::flushAllOutputs(theFilter, true);
   }
}

void ossimQtRLevelFilterDialog::closeClicked()
{
   close(true);
}

void ossimQtRLevelFilterDialog::updateEnableMenu() const
{
   if (!theFilter || !theEnableCheckBox)
   {
      return;
   }
   theEnableCheckBox->setChecked(theFilter->getEnableFlag());
}

void ossimQtRLevelFilterDialog::updateRLevelMenu() const
{
   if (!theFilter || !theRLevelMenu)
   {
      return;
   }
   int item = static_cast<int>(theFilter->getCurrentRLevel());
   theRLevelMenu->setCurrentItem(item);
}

void ossimQtRLevelFilterDialog::buildRLevelMenu() const
{
   if (!theRLevelMenu || !theFilter)
   {
      return;
   }

   ossim_uint32 levels = theFilter->getNumberOfDecimationLevels();
   for (ossim_uint32 i = 0; i < levels; ++i)
   {
      ossimString os = ossimString::toString(i);
      QString qs = "Resolution level ";
      qs += os.c_str();
      theRLevelMenu->insertItem(qs, i);
   }

   QSize size = theRLevelMenu->sizeHint();
   theRLevelMenu->resize(size);
}

ossimQtRLevelFilterDialog::ossimQtRLevelFilterDialog(const ossimQtRLevelFilterDialog& /* obj */ )
   : QDialog(),
     ossimConnectableObjectListener()
{
}

const ossimQtRLevelFilterDialog& ossimQtRLevelFilterDialog::operator=(const ossimQtRLevelFilterDialog& /* rhs */ )
{
   return *this;
}

