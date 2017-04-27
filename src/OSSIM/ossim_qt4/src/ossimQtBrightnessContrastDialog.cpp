//----------------------------------------------------------------------------
// Copyright (C) 2005 Intelligence Data Systems, Inc. All rights reserved.
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Custom dialog for ossimBrightnessContrastSource.
//
//----------------------------------------------------------------------------
// $Id: ossimQtBrightnessContrastDialog.cpp 12141 2007-12-07 18:05:13Z gpotts $

#include <Qt3Support/Q3VBox>
#include <Qt3Support/Q3HBox>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QPushButton>
#include <QtGui/QCheckBox>
#include <QtGui/QSizePolicy>
#include <QtGui/QComboBox>
// #include <qhgroupbox.h>
#include <QtGui/QSlider>
//Added by qt3to4:
#include <Qt3Support/Q3HBoxLayout>
#include <Qt3Support/Q3VBoxLayout>
#include <Qt3Support/Q3Frame>

#include <ossimQt/ossimQtBrightnessContrastDialog.h>
#include <ossimQt/ossimQtDisplayUtility.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/imaging/ossimBrightnessContrastSource.h>

static ossimTrace traceDebug("ossimQtBrightnessContrastDialog:degug");


ossimQtBrightnessContrastDialog::ossimQtBrightnessContrastDialog(
   QWidget* parent,
   const char* name,
   bool modal,
   Qt::WFlags f)
   : QDialog(parent, name, modal, f),
     ossimConnectableObjectListener(),
     theParent(parent),
     theFilter(NULL),
     theMainVBox(NULL),

     theHBox1(NULL),
     theBrightnessLabel(NULL),
     theBrightnessSlider(NULL),
     theBrightnessValueLabel(NULL),
     
     theHBox2(NULL),
     theContrastLabel(NULL),
     theContrastSlider(NULL),
     theContrastValueLabel(NULL),

     theHBox3(NULL),
     theEnableCheckBox(NULL),
     theResetButton(NULL),
     theCloseButton(NULL)
{
   setCaption("Brightness Contrast Property Editor");
   
   // Main vertical box.
   theMainVBox = new Q3VBoxLayout(this);

   // First row, for the brightness slider.
   theHBox1 = new Q3HBoxLayout(theMainVBox);

   // Brightness label.
   theBrightnessLabel = new QLabel( this, "theBrightnessLabel" );
   theBrightnessLabel->setMinimumSize( QSize( 90, 0 ) );
   theBrightnessLabel->setAlignment( int( Qt::AlignVCenter | Qt::AlignLeft ) );
   theBrightnessLabel->setText("brightness: ");
   theHBox1->addWidget( theBrightnessLabel );

   // Brightness slider.
   theBrightnessSlider = new QSlider( this, "theBrightnessSlider" );
   theBrightnessSlider->setMinimumSize( QSize( 290, 0 ) );
   theBrightnessSlider->setMaxValue( 400 );
   theBrightnessSlider->setPageStep( 1 );
   theBrightnessSlider->setValue( 400 );
   theBrightnessSlider->setTracking( FALSE );
   theBrightnessSlider->setOrientation( Qt::Horizontal );
   theBrightnessSlider->setTickmarks( QSlider::TicksRight );
   theBrightnessSlider->setTickInterval( 40 );
   theHBox1->addWidget( theBrightnessSlider );

   // Brightness value
   theBrightnessValueLabel = new QLabel( this, "theBrightnessValueLabel" );
   theBrightnessValueLabel->setMinimumSize( QSize( 40, 0 ) );
   theBrightnessValueLabel->setText("0.0");
   theHBox1->addWidget( theBrightnessValueLabel );

   // Second row, for the contrast slider.
   theHBox2 = new Q3HBoxLayout(theMainVBox);

   // Contrast label.
   theContrastLabel = new QLabel( this, "theContrastLabel" );
   theContrastLabel->setMinimumSize( QSize( 90, 0 ) );
   theContrastLabel->setAlignment( int( Qt::AlignVCenter | Qt::AlignLeft ) );
   theContrastLabel->setText("contrast: ");
   theHBox2->addWidget( theContrastLabel );

   // Contrast slider.
   theContrastSlider = new QSlider( this, "theContrastSlider" );
   theContrastSlider->setMinimumSize( QSize( 290, 0 ) );
   theContrastSlider->setMaxValue( 400 );
   theContrastSlider->setPageStep( 1 );
   theContrastSlider->setValue( 400 );
   theContrastSlider->setTracking( FALSE );
   theContrastSlider->setOrientation( Qt::Horizontal );
   theContrastSlider->setTickmarks( QSlider::TicksRight );
   theContrastSlider->setTickInterval( 40 );
   theHBox2->addWidget( theContrastSlider );

   // Contrast value
   theContrastValueLabel = new QLabel( this, "theContrastValueLabel" );
   theContrastValueLabel->setMinimumSize( QSize( 40, 0 ) );
   theContrastValueLabel->setText("0.0");
   theHBox2->addWidget( theContrastValueLabel );

   // Let's put a line in between things...
   Q3Frame* line1 = new Q3Frame( this, "line1" );
   line1->setFrameShape( Q3Frame::HLine );
   line1->setFrameShadow( Q3Frame::Sunken );
   theMainVBox->addWidget( line1 );
   
   // Third row, filter enable disable check box and close.
   theHBox3 = new Q3HBoxLayout(theMainVBox);
   
   // Make the check box.
   theEnableCheckBox = new  QCheckBox(this, "theEnableButton");
   theEnableCheckBox->setText("enable");
   theHBox3->addWidget(theEnableCheckBox);

   // Make the reset button...
   theResetButton = new QPushButton( this, "theResetButton" );
   theResetButton->setText( "Reset" );
   theResetButton->setDefault(false);
   theResetButton->setAutoDefault(false);
   theHBox3->addWidget(theResetButton);

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

   connect( theBrightnessSlider,
            SIGNAL( valueChanged(int) ),
            this,
            SLOT( brightnessSliderChanged(int) ) );
   
   connect( theContrastSlider,
            SIGNAL( valueChanged(int) ),
            this,
            SLOT( contrastSliderChanged(int) ) );
   
   connect( theEnableCheckBox, SIGNAL ( clicked() ),
            this, SLOT ( enableClicked() ) );
   
   connect( theResetButton, SIGNAL ( clicked() ),
            this, SLOT ( resetClicked() ) );
   
   connect( theCloseButton, SIGNAL ( clicked() ),
            this, SLOT ( closeClicked() ) );
}

ossimQtBrightnessContrastDialog::~ossimQtBrightnessContrastDialog()
{
   if (theFilter)
   {
      theFilter->removeListener((ossimConnectableObjectListener*)this);
      theFilter = NULL;
   }
}

void ossimQtBrightnessContrastDialog::objectDestructingEvent(ossimObjectDestructingEvent& event)
{
   if (theFilter)
   {
      if(event.getObject() == PTR_CAST(ossimObject, theFilter))
      {      
         close(true);
      }
   }
}

void ossimQtBrightnessContrastDialog::setFilter(
   ossimBrightnessContrastSource* filter)
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

   updateDialog();
}

void ossimQtBrightnessContrastDialog::enableClicked()
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

void ossimQtBrightnessContrastDialog::brightnessSliderChanged(int value)
{
   if (!theFilter)
   {
      return;
   }

   //---
   // Brightnes of:
   // -1.0 = 0
   //  0.0 = 200
   // +1.0 = 400
   //---
   double convertedValue = 0.0;
   if (value)
   {
      convertedValue = (value/200.0) - 1.0;
   }
   else
   {
      convertedValue = -1.0;
   }
   
   double currentValue = theFilter->getBrightness();

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimQtBrightnessContrastDialog::brightnessSliderChanged DEBUG:"
         << "\nslider value: " << value
         << "\nconverted value:  " << convertedValue
         << "\ncurrent brightness:  " << currentValue
         << std::endl;
   }
   
   if ( (convertedValue < -1.0) || (convertedValue > 1.0) )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimQtBrightnessContrastDialog::brightnessSliderChanged WARN:"
         << "\nRange error..."
         << std::endl;
      return;
   }

   if (convertedValue == currentValue)
   {
      return; // nothing to do...
   }
   
   theFilter->setBrightness(convertedValue);

   updateBrightnessSlider();

   // Force the window to repaint.
   ossimQtDisplayUtility::flushAllOutputs(theFilter, true);
}

void ossimQtBrightnessContrastDialog::contrastSliderChanged(int value)
{
   if (!theFilter)
   {
      return;
   }
   
   //---
   // Contrast of:
   //  0.0 = 0
   //  1.0 = 200
   // +2.0 = 400
   //---

   double convertedValue = (value/200.0);
   double currentValue   = theFilter->getContrast();

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimQtBrightnessContrastDialog::contrastSliderChanged DEBUG:"
         << "\nslider value: " << value
         << "\nconverted value:  " << convertedValue
         << "\ncurrent contrast:  " << currentValue
         << std::endl;
   }
   
   if ( (convertedValue < 0.0) || (convertedValue > 2.0) )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimQtBrightnessContrastDialog::contrastSliderChanged WARN:"
         << "\nRange error..."
         << std::endl;
      return;
   }

   if (convertedValue == currentValue)
   {
      return; // nothing to do...
   }
   
   theFilter->setContrast(convertedValue);

   updateContrastSlider();

   // Force the window to repaint.
   ossimQtDisplayUtility::flushAllOutputs(theFilter, true);   
   
}

void ossimQtBrightnessContrastDialog::resetClicked()
{
   if (theFilter)
   {
      theFilter->setBrightness(0.0);
      theFilter->setContrast(1.0);
      updateDialog();

      // Force the window to repaint.
      ossimQtDisplayUtility::flushAllOutputs(theFilter, true);   
   }
}

void ossimQtBrightnessContrastDialog::closeClicked()
{
   close(true);
}


void ossimQtBrightnessContrastDialog::updateDialog() const
{
   updateEnableMenu();
   updateBrightnessSlider();
   updateContrastSlider();
}

void ossimQtBrightnessContrastDialog::updateEnableMenu() const
{
   if (!theFilter)
   {
      return;
   }
   theEnableCheckBox->setChecked(theFilter->getEnableFlag());
}
   
void ossimQtBrightnessContrastDialog::updateBrightnessSlider() const
{
   if (!theFilter)
   {
      return;
   }

   //---
   // Update the brightness slider.
   //---
   
   // Disable to avoid callback on value change.
   theBrightnessSlider->setDisabled(true);

   //---
   // Brightnes of:
   // -1.0 = 0
   //  0.0 = 200
   // +1.0 = 400
   //---
   double d = theFilter->getBrightness();

   // Update the text...
   QString qs = ossimString::toString(d, 2).c_str();
   theBrightnessValueLabel->setText(qs);

   // Update the slider.
   d += 1.0;
   int value = static_cast<int>(d*200.0);
   theBrightnessSlider->setValue(value);

   // Turn it back on...
   theBrightnessSlider->setEnabled(true);
}

void ossimQtBrightnessContrastDialog::updateContrastSlider()   const
{
   if (!theFilter)
   {
      return;
   }

   //---
   // Update the contrast slider.
   //---

   // Disable to avoid callback on value change.
   theContrastSlider->setDisabled(true);

   //---
   // Contrast of:
   //  0.0 = 0
   //  1.0 = 200
   // +2.0 = 400
   //---

   double d = theFilter->getContrast();

   // Update the text.
   QString qs = ossimString::toString(d, 2).c_str();
   theContrastValueLabel->setText(qs);

   // Update the slider.
   int value = static_cast<int>(d*200.0);
   theContrastSlider->setValue(value);

   // Turn it back on...
   theContrastSlider->setEnabled(true);
}

ossimQtBrightnessContrastDialog::ossimQtBrightnessContrastDialog(const ossimQtBrightnessContrastDialog& /* obj */ )
   : QDialog(),
     ossimConnectableObjectListener()
{
}

const ossimQtBrightnessContrastDialog& ossimQtBrightnessContrastDialog::operator=(const ossimQtBrightnessContrastDialog& /* rhs */ )
{
   return *this;
}
