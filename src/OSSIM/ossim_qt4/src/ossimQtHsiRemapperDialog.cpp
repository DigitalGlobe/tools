#include <ossimQt/ossimQtHsiRemapperDialog.h>

#include <QtCore/QVariant>
#include <QtGui/QImage>
#include <QtGui/QPixmap>

#include <ossimQt/ossimQtHsiRemapperController.h>
#include <ossim/imaging/ossimHsiRemapper.h>

/*
 *  Constructs a ossimQtHsiRemapperDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
ossimQtHsiRemapperDialog::ossimQtHsiRemapperDialog(QWidget* parent, const char* name, bool modal, Qt::WFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);


    // signals and slots connections
    connect(theRedButton, SIGNAL(clicked()), this, SLOT(redButtonClicked()));
    connect(theYellowButton, SIGNAL(clicked()), this, SLOT(yellowButtonClicked()));
    connect(theGreenButton, SIGNAL(clicked()), this, SLOT(greenButtonClicked()));
    connect(theCyanButton, SIGNAL(clicked()), this, SLOT(cyanButtonClicked()));
    connect(theBlueButton, SIGNAL(clicked()), this, SLOT(blueButtonClicked()));
    connect(theMagentaButton, SIGNAL(clicked()), this, SLOT(magentaButtonClicked()));
    connect(theAllButton, SIGNAL(clicked()), this, SLOT(allButtonClicked()));
    connect(theHueOffsetSlider, SIGNAL(valueChanged(int)), this, SLOT(hueOffsetValueChanged(int)));
    connect(theHueLowRangeSlider, SIGNAL(valueChanged(int)), this, SLOT(hueLowRangeValueChanged(int)));
    connect(theHueHighRangeSlider, SIGNAL(valueChanged(int)), this, SLOT(hueHighRangeValueChanged(int)));
    connect(theSaturationOffsetSlider, SIGNAL(valueChanged(int)), this, SLOT(saturationOffsetValueChanged(int)));
    connect(theIntensityOffsetSlider, SIGNAL(valueChanged(int)), this, SLOT(intensityOffsetValueChanged(int)));
    connect(theLowIntensityClipSlider, SIGNAL(valueChanged(int)), this, SLOT(lowIntensityClipValueChanged(int)));
    connect(theEnableButton, SIGNAL(clicked()), this, SLOT(enableButtonClicked()));
    connect(theResetGroupButton, SIGNAL(clicked()), this, SLOT(resetGroupButtonClicked()));
    connect(theResetAllGroup, SIGNAL(clicked()), this, SLOT(resetAllButtonClicked()));
    connect(theCloseButton, SIGNAL(clicked()), this, SLOT(closeButtonClicked()));
    connect(theHueBlendRangeSlider, SIGNAL(valueChanged(int)), this, SLOT(hueBlendRangeValueChanged(int)));
    connect(theWhiteObjectClipSlider, SIGNAL(valueChanged(int)), this, SLOT(whiteObjectClipValueChanged(int)));
    connect(theHighIntensityClipSlider, SIGNAL(valueChanged(int)), this, SLOT(highIntensityClipValueChanged(int)));
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ossimQtHsiRemapperDialog::~ossimQtHsiRemapperDialog()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ossimQtHsiRemapperDialog::languageChange()
{
    retranslateUi(this);
}

void ossimQtHsiRemapperDialog::redButtonClicked()
{
   if (theController)
   {
      theController->redButtonClicked();
   }
}

void ossimQtHsiRemapperDialog::yellowButtonClicked()
{
   if (theController)
   {
      theController->yellowButtonClicked();
   }
}

void ossimQtHsiRemapperDialog::greenButtonClicked()
{
   if (theController)
   {
      theController->greenButtonClicked();
   }
}

void ossimQtHsiRemapperDialog::cyanButtonClicked()
{
   if (theController)
   {
      theController->cyanButtonClicked();
   }
}

void ossimQtHsiRemapperDialog::blueButtonClicked()
{
   if (theController)
   {
      theController->blueButtonClicked();
   }
}

void ossimQtHsiRemapperDialog::magentaButtonClicked()
{
   if (theController)
   {
      theController->magentaButtonClicked();
   }
}

void ossimQtHsiRemapperDialog::allButtonClicked()
{
   if (theController)
   {
      theController->allButtonClicked();
   }
}

void ossimQtHsiRemapperDialog::hueOffsetValueChanged(int value)
{
   if (theController)
   {
      theController->hueOffsetValueChanged(value);
   }
}

void ossimQtHsiRemapperDialog::hueLowRangeValueChanged(int value)
{
   if (theController)
   {
      theController->hueLowRangeValueChanged(value);
   }
}

void ossimQtHsiRemapperDialog::hueHighRangeValueChanged(int value )
{
   if (theController)
   {
      theController->hueHighRangeValueChanged(value);
   }

}

void ossimQtHsiRemapperDialog::saturationOffsetValueChanged(int value)
{
   if (theController)
   {
      theController->saturationOffsetValueChanged(value);
   }
}

void ossimQtHsiRemapperDialog::intensityOffsetValueChanged(int value)
{
   if (theController)
   {
      theController->intensityOffsetValueChanged(value);
   }
}

void ossimQtHsiRemapperDialog::lowIntensityClipValueChanged(int value)
{
   if (theController)
   {
      theController->lowIntensityClipValueChanged(value);
   }
}

void ossimQtHsiRemapperDialog::highIntensityClipValueChanged(int value)
{
   if (theController)
   {
      theController->highIntensityClipValueChanged(value);
   }
}

void ossimQtHsiRemapperDialog::enableButtonClicked()
{
   if (theController)
   {
      theController->enableButtonClicked();
   }
}

void ossimQtHsiRemapperDialog::resetGroupButtonClicked()
{
   if (theController)
   {
      theController->resetGroupButtonClicked();
   }
}


void ossimQtHsiRemapperDialog::resetAllButtonClicked()
{
   if (theController)
   {
      theController->resetAllButtonClicked();
   }
}

void ossimQtHsiRemapperDialog::closeButtonClicked()
{
   close(true);
}

void ossimQtHsiRemapperDialog::init()
{
   theColorRangeButtonGroup->setButton(6); // all button
   theController = new ossimQtHsiRemapperController(this);
}

void ossimQtHsiRemapperDialog::destroy()
{
   if (theController)
   {
      delete theController;
      theController = NULL;
   }
}

void ossimQtHsiRemapperDialog::setHsiRemapper(ossimHsiRemapper* remapper)
{
   if (theController)
   {
      theController->setHsiRemapper(remapper);
   }
}



void ossimQtHsiRemapperDialog::hueBlendRangeValueChanged( int value )
{
   if (theController)
   {
      theController->hueBlendRangeValueChanged( value );
   }
}


void ossimQtHsiRemapperDialog::whiteObjectClipValueChanged( int value )
{
   if (theController)
   {
      theController->whiteObjectClipValueChanged( value );
   }
}
