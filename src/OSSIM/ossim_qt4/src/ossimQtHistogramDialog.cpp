#include <ossimQt/ossimQtHistogramDialog.h>

#include <QtGui/QFileDialog>
#include <QtCore/QVariant>
#include <Qt3Support/Q3FileDialog>
#include <QtGui/QImage>
#include <QtGui/QPixmap>

#include <ossimQt/ossimQtHistogramController.h>
#include <ossim/base/ossimString.h>
#include <ossim/imaging/ossimHistogramRemapper.h>
#include <ossimQt/ossimQtHistogramWidget.h>

/*
 *  Constructs a ossimQtHistogramDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
ossimQtHistogramDialog::ossimQtHistogramDialog(QWidget* parent, const char* name, bool modal, Qt::WFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);


    // signals and slots connections
    connect(theCloseButton, SIGNAL(clicked()), this, SLOT(closeButtonClicked()));
    connect(theApplyButton, SIGNAL(clicked()), this, SLOT(applyButtonClicked()));
    connect(theResetButton, SIGNAL(clicked()), this, SLOT(resetButtonClicked()));
    connect(theEnableCheckBox, SIGNAL(clicked()), this, SLOT(enableCheckBoxClicked()));
    connect(theHighClipPercentLineEdit, SIGNAL(returnPressed()), this, SLOT(highClipPercentReturnPressed()));
    connect(theLowClipPercentLineEdit, SIGNAL(returnPressed()), this, SLOT(lowClipPercentReturnPressed()));
    connect(theLowClipValueLineEdit, SIGNAL(returnPressed()), this, SLOT(lowClipValueReturnPressed()));
    connect(theHighClipValueLineEdit, SIGNAL(returnPressed()), this, SLOT(highClipValueReturnPressed()));
    connect(theMidPointLineEdit, SIGNAL(returnPressed()), this, SLOT(midPointReturnPressed()));
    connect(theOutputMinValue, SIGNAL(returnPressed()), this, SLOT(outputMinValueReturnPressed()));
    connect(theOutputMaxValue, SIGNAL(returnPressed()), this, SLOT(outputMaxValueReturnPressed()));
    connect(theStretchModeComboBox, SIGNAL(activated(int)), this, SLOT(stretchModeComboBoxActivated(int)));
    connect(theBandComboBox, SIGNAL(activated(int)), this, SLOT(bandComboBoxActivated(int)));
    connect(theHistogramFileLineEdit, SIGNAL(returnPressed()), this, SLOT(histogramFileLineEditReturnPressed()));
    connect(theHistogramFilePushButton, SIGNAL(clicked()), this, SLOT(histogramFilePushButtonClicked()));
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ossimQtHistogramDialog::~ossimQtHistogramDialog()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ossimQtHistogramDialog::languageChange()
{
    retranslateUi(this);
}

void ossimQtHistogramDialog::init()
{
   theController = new ossimQtHistogramController(this);
}

void ossimQtHistogramDialog::destroy()
{
   if (theController)
   {
      delete theController;
      theController = NULL;
   }
}

void ossimQtHistogramDialog::closeButtonClicked()
{
   close(true);
}


void ossimQtHistogramDialog::applyButtonClicked()
{
   if (theController)
   {
      theController->applyButtonClicked();
   }
}

void ossimQtHistogramDialog::resetButtonClicked()
{
   if (theController)
   {
      theController->resetButtonClicked();
   }
}

void ossimQtHistogramDialog::enableCheckBoxClicked()
{
   if (theController)
   {
      theController->enableCheckBoxClicked();
   }
}

void ossimQtHistogramDialog::setHistogramRemapper(ossimHistogramRemapper* remapper)
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

void ossimQtHistogramDialog::lowClipPercentReturnPressed()
{
   if (theController)
   {
      theController->lowClipPercentReturnPressed();
   }
}

void ossimQtHistogramDialog::highClipPercentReturnPressed()
{
   if (theController)
   {
      theController->highClipPercentReturnPressed();
   }
}

void ossimQtHistogramDialog::lowClipValueReturnPressed()
{
   if (theController)
   {
      theController->lowClipValueReturnPressed();
   }
}

void ossimQtHistogramDialog::highClipValueReturnPressed()
{
   if (theController)
   {
      theController->highClipValueReturnPressed();
   }
}

void ossimQtHistogramDialog::midPointReturnPressed()
{
   if (theController)
   {
      theController->midPointReturnPressed();
   }
}

void ossimQtHistogramDialog::outputMinValueReturnPressed()
{
   if (theController)
   {
      theController->outputMinValueReturnPressed();
   }
}

void ossimQtHistogramDialog::outputMaxValueReturnPressed()
{
   if (theController)
   {
      theController->outputMaxValueReturnPressed();
   }
}


void ossimQtHistogramDialog::stretchModeComboBoxActivated( int item )
{
   if (theController)
   {
      theController->stretchModeComboBoxActivated(item);
   }  
}


void ossimQtHistogramDialog::bandComboBoxActivated( int item )
{
   if (theController)
   {
      theController->bandComboBoxActivated(item);
   } 
}


void ossimQtHistogramDialog::histogramFileLineEditReturnPressed()
{
   if (theController)
   {
      ossimFilename f = theHistogramFileLineEdit->text().ascii();
      theController->setHistogramFile(f);
   }
}

void ossimQtHistogramDialog::histogramFilePushButtonClicked()
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
   fd = NULL;
}
