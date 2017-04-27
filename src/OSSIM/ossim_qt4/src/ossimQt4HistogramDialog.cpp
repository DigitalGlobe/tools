//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  Marla Bartel
//
// Description:
//
// Histogram dialog.
//
// Filename: ossimQtHistogramDialog.cpp
//
// $Id$
//----------------------------------------------------------------------------


#include <QWidget>
#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <iostream>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QPainter>
   // #include <>


#include <ossimQt/ossimQt4HistogramDialog.h>
#include <ossimQt/ossimQtHistogramController.h>
#include <ossim/imaging/ossimHistogramRemapper.h>
#include <ossimQt/ossimQtHistogramWidget.h>

ossimQt4HistogramDialog::ossimQt4HistogramDialog(QWidget *parent)
   : QDialog(parent, Qt::WDestructiveClose), // Set to have destructive close.
     
     theController(0),
     theLowClipPercentLineEdit(0),
     theLowClipValueLineEdit(0),
     theHighClipPercentLineEdit(0),
     theHighClipValueLineEdit(0),
     theOutputMinValue(0),
     theOutputMaxValue(0),
     theMidPointLineEdit(0),
     theHistogramFileLineEdit(0),
     theHistogramWidget(0),
     theBandComboBox(0),
     theStretchModeComboBox(0),
     theEnableCheckBox(0),
     theApplyButton(0),
     theResetButton(0),
     theCloseButton(0)
{
   
   // Horizontal box 1
   QGroupBox*   clipPointsGroupBox       = new QGroupBox(tr("Clip Points"));

   // box 1 will be a 4x3 grid
   QGridLayout* r1lo = new QGridLayout;
   
   // (r1c1lo) row 1 grid layout 
   QGroupBox* r1c1 = new QGroupBox();
   QGridLayout* r1c1lo = new QGridLayout;

   QLabel* lowClipLabel = new QLabel(tr("Low Clip Percent:"));
   theLowClipPercentLineEdit = new QLineEdit();
   QLabel* lowClipValueLabel = new QLabel(tr("Low Clip Value:"));
   theLowClipValueLineEdit = new QLineEdit();
   
   QLabel* highClipLabel = new QLabel(tr("High Clip Percent:"));
   theHighClipPercentLineEdit = new QLineEdit();
   QLabel* highClipValueLabel = new QLabel(tr("High Clip Value:"));
   theHighClipValueLineEdit = new QLineEdit();
   
   QLabel* midPointLabel = new QLabel(tr("Mid Point:"));
   theMidPointLineEdit = new QLineEdit();
   
   QLabel* outputMinLabel = new QLabel(tr("Output Min Value:"));
   theOutputMinValue = new QLineEdit();
   QLabel* outputMaxLabel = new QLabel(tr("Output Max Value:"));
   theOutputMaxValue = new QLineEdit();
   
   QLabel* labStretchMode    = new QLabel(tr("Stretch Mode"));
   QLabel* labDisplayChannel = new QLabel(tr("Display Channel"));
   theStretchModeComboBox    = new QComboBox;
   theBandComboBox           = new QComboBox;

   r1c1lo->addWidget(lowClipLabel, 0, 0);               // row 1, col 1
   r1c1lo->addWidget(theLowClipPercentLineEdit, 0, 1);  // row 1, col 2
   r1c1lo->addWidget(lowClipValueLabel, 0, 2);          // row 1, col 3
   r1c1lo->addWidget(theLowClipValueLineEdit, 0, 3);    // row 1, col 4
   r1c1lo->addWidget(highClipLabel, 1, 0);              // row 2, col 1
   r1c1lo->addWidget(theHighClipPercentLineEdit, 1, 1); // row 2, col 2
   r1c1lo->addWidget(highClipValueLabel, 1, 2);         // row 2, col 3
   r1c1lo->addWidget(theHighClipValueLineEdit, 1, 3);   // row 2, col 4
   r1c1lo->addWidget(midPointLabel, 2, 0);          // row 3, col 1
   r1c1lo->addWidget(theMidPointLineEdit, 2, 1);    // row 3, col 2
   r1c1lo->addWidget(outputMinLabel, 3, 0);         // row 4, col 1
   r1c1lo->addWidget(theOutputMinValue, 3, 1);      // row 4, col 2
   r1c1lo->addWidget(outputMaxLabel, 3, 2);         // row 4, col 3
   r1c1lo->addWidget(theOutputMaxValue, 3, 3);      // row 4, col 4
   r1c1lo->addWidget(labStretchMode, 4, 0);         // row 5, col 1
   r1c1lo->addWidget(theStretchModeComboBox, 4, 1); // row 5, col 2
   r1c1lo->addWidget(labDisplayChannel, 4, 2);      // row 5, col 3
   r1c1lo->addWidget(theBandComboBox, 4, 3);        // row 5, col 4
   r1c1->setLayout(r1c1lo);
   
   r1lo->addWidget(r1c1, 0, 0);            // end box 1
   clipPointsGroupBox->setLayout(r1lo);

   // Horizontal box 2
   QGroupBox*   histogramGroupBox       = new QGroupBox(tr("Histogram"));

   // box 2 will be a 6x2 grid
   QGridLayout* r2lo = new QGridLayout;
   theHistogramFileLineEdit  = new QLineEdit();
   
   // (r2c1lo) row 1 grid layout 
   QGroupBox* r2c1 = new QGroupBox();
   QGridLayout* r2c1lo = new QGridLayout;
   
   QLabel* labHistogramFile  = new QLabel(tr("Histogram File:"));
   QPushButton* theHistogramFilePushButton = new QPushButton(tr("Browse"));
   
   theHistogramWidget = new ossimQtHistogramWidget;

   r2c1lo->addWidget(theHistogramWidget, 0, 0);            // row 1, col 1
   r2c1lo->addWidget(labHistogramFile, 1, 0);              // row 2, col 1
   r2c1lo->addWidget(theHistogramFileLineEdit, 2, 0);      // row 3, col 1
   r2c1lo->addWidget(theHistogramFilePushButton, 2, 1);    // row 3, col 2
   
   r2c1->setLayout(r2c1lo);
   r2lo->addWidget(r2c1, 0, 0);            // end box 2
   histogramGroupBox->setLayout(r2lo);

   
     // Horizontal box 3
   QGroupBox*   buttonGroupBox = new QGroupBox;

   // box 3 will be a 1x5 grid
   QGridLayout* r4lo = new QGridLayout;
   QGroupBox* r4c1 = new QGroupBox();
   QGridLayout* r4c1lo = new QGridLayout;
   
   QLabel* labEnable = new QLabel(tr("Filter Enable:"));
   theEnableCheckBox = new QCheckBox;
   
   QPushButton* theApplyButton = new QPushButton(tr("Apply"));
   QPushButton* theResetButton = new QPushButton(tr("Reset"));
   QPushButton* theCloseButton = new QPushButton(tr("Close"));
    
   r4c1lo->addWidget(theEnableCheckBox, 0, 1);           // row 1, col 2
   r4c1lo->addWidget(labEnable, 0, 0);                   // row 1, col 1
   
   r4c1lo->addWidget(theApplyButton, 0, 2);              // row 1, col 3
   r4c1lo->addWidget(theResetButton, 0, 3);              // row 1, col 4
   r4c1lo->addWidget(theCloseButton, 0, 4);              // row 1, col 5
   
   r4c1->setLayout(r4c1lo);
   r4lo->addWidget(r4c1, 0, 0);            // end box 3
   buttonGroupBox->setLayout(r4lo);
   
    // signals and slots connections
    connect(theCloseButton, SIGNAL(clicked()), this, SLOT(closeButtonClicked()));
    connect(theApplyButton, SIGNAL(clicked()), this, SLOT(applyButtonClicked()));
    connect(theResetButton, SIGNAL(clicked()), this, SLOT(resetButtonClicked()));
    connect(theEnableCheckBox, SIGNAL(toggled(bool)), this, SLOT(enableCheckBoxClicked(bool)));
    connect(theHighClipPercentLineEdit, SIGNAL(returnPressed()), this, SLOT(highClipPercentReturnPressed()));
    connect(theLowClipPercentLineEdit, SIGNAL(returnPressed()), this, SLOT(lowClipPercentReturnPressed()));
    connect(theLowClipValueLineEdit, SIGNAL(returnPressed()), this, SLOT(lowClipValueReturnPressed()));
    connect(theHighClipValueLineEdit, SIGNAL(returnPressed()), this, SLOT(highClipValueReturnPressed()));
    connect(theMidPointLineEdit, SIGNAL(returnPressed()), this, SLOT(midPointReturnPressed()));
    connect(theStretchModeComboBox, SIGNAL(activated(int)), this, SLOT(stretchModeComboBoxActivated(int)));
    connect(theEnableCheckBox, SIGNAL(clicked()), this, SLOT(enableCheckBoxClicked()));
    connect(theBandComboBox, SIGNAL(activated(int)), this, SLOT(bandComboBoxActivated(int)));
    connect(theHistogramFileLineEdit, SIGNAL(returnPressed()), this, SLOT(histogramFileLineEditReturnPressed()));
    connect(theHistogramFilePushButton, SIGNAL(clicked()), this, SLOT(histogramFilePushButtonClicked()));
    init();

    QVBoxLayout* mainLayout = new QVBoxLayout;

   // Parent group boxes to main layout.
   mainLayout->addWidget(clipPointsGroupBox);
   mainLayout->addWidget(histogramGroupBox);
   mainLayout->addWidget(buttonGroupBox);

   // Parent main layout to this widget.
   setLayout(mainLayout);
   
   setWindowTitle(tr("Histogram Dialog"));
   
}

ossimQt4HistogramDialog::~ossimQt4HistogramDialog()
{
}

void ossimQt4HistogramDialog::init()
{
   theController = new ossimQtHistogramController(this);
}

void ossimQt4HistogramDialog::destroy()
{
   if (theController)
   {
      delete theController;
      theController = 0;
   }
}

void ossimQt4HistogramDialog::closeButtonClicked()
{
   close(true);
}


void ossimQt4HistogramDialog::applyButtonClicked()
{
   if (theController)
   {
      theController->applyButtonClicked();
   }
}

void ossimQt4HistogramDialog::resetButtonClicked()
{
   if (theController)
   {
      theController->resetButtonClicked();
   }
}

void ossimQt4HistogramDialog::enableCheckBoxClicked()
{
   if (theController)
   {
      theController->enableCheckBoxClicked();
   }
}

void ossimQt4HistogramDialog::setHistogramRemapper(ossimHistogramRemapper* remapper)
{

   if (remapper)
   {
      // Build theStretchModeComboBox menu.
      theStretchModeComboBox->clear();
      const ossim_uint32 MODES = remapper->getNumberOfStretchModes();
      for (ossim_uint32 mode = 0; mode < MODES; ++mode)
      {
         QString item = remapper->getStretchModeString(mode).c_str();
         theStretchModeComboBox->insertItem(item);
      }
      
      // Build up theBandComboBox menu.
      QString item = "master";
      theBandComboBox->clear();
      theBandComboBox->insertItem(item);

      const ossim_uint32 BANDS = remapper->getNumberOfOutputBands();
      for (ossim_uint32 band = 0; band < BANDS; ++band)
      {
         item = ossimString::toString(band+1).c_str();
         theBandComboBox->insertItem(item);
      }
   }

   if (theController)
   {
      theController->setHistogramRemapper(remapper);
   }

   theHistogramWidget->setHistogramRemapper(remapper);

   // This will paint an average of all bands on the widget.
   theHistogramWidget->update();

}

void ossimQt4HistogramDialog::lowClipPercentReturnPressed()
{
   if (theController)
   {
      theController->lowClipPercentReturnPressed();
      cout << "Low Clip Return Pressed....Qt4Dialog" << endl;

   }
}

void ossimQt4HistogramDialog::highClipPercentReturnPressed()
{
   if (theController)
   {
      theController->highClipPercentReturnPressed();
   }
}

void ossimQt4HistogramDialog::lowClipValueReturnPressed()
{
   if (theController)
   {
      theController->lowClipValueReturnPressed();
   }
}

void ossimQt4HistogramDialog::highClipValueReturnPressed()
{
   if (theController)
   {
      theController->highClipValueReturnPressed();
   }
}

void ossimQt4HistogramDialog::midPointReturnPressed()
{
   if (theController)
   {
      theController->midPointReturnPressed();
   }
}


void ossimQt4HistogramDialog::stretchModeComboBoxActivated( int item )
{
   if (theController)
   {
      theController->stretchModeComboBoxActivated(item);
   }  
}


void ossimQt4HistogramDialog::bandComboBoxActivated( int item )
{
   if (theController)
   {
      theController->bandComboBoxActivated(item);
   } 
}


void ossimQt4HistogramDialog::histogramFileLineEditReturnPressed()
{
#if 0
   if (theController)
   {
      ossimFilename f = theHistogramFileLineEdit->text().ascii();
      theController->setHistogramFile(f);
   }
#endif
}

void ossimQt4HistogramDialog::histogramFilePushButtonClicked()
{
   QFileDialog* fd = new QFileDialog( this );
   fd->setMode(QFileDialog::AnyFile);
   fd->setDir(ossimQtHistogramController::getLatestOutputFilePath().c_str());
   
   QString file;
   if (fd->exec() == QDialog::Accepted )
   {
      file = fd->selectedFile();
      ossimFilename f = file.ascii();
      theController->setHistogramFile(f);
   }

   // Delete fd???
   delete fd;
   fd = 0;
}


