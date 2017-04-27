#include <ossimQt/ossimQtImageInfoDialog.h>

#include <QtCore/QVariant>
#include <QtGui/QImage>
#include <QtGui/QPixmap>

#include <ossimQt/ossimQtImageInfoController.h>
#include <ossimQt/ossimQtScrollingImageWidget.h>

/*
 *  Constructs a ossimQtImageInfoDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
ossimQtImageInfoDialog::ossimQtImageInfoDialog(QWidget* parent, const char* name, bool modal, Qt::WFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);


    // signals and slots connections
    connect(theHelpPushButton, SIGNAL(clicked()), this, SLOT(helpButtonClicked()));
    connect(theScanForValidVerticesPushButton, SIGNAL(clicked()), this, SLOT(scanForValidVerticesButtonClicked()));
    connect(theClosePushButton, SIGNAL(clicked()), this, SLOT(closeButtonClicked()));
    connect(theLayerComboBox, SIGNAL(activated(int)), this, SLOT(layerComboBoxActivated(int)));
    connect(theBandSpinBox, SIGNAL(valueChanged(int)), this, SLOT(bandSpinBoxValueChanged(int)));
    connect(theProjectionComboBox, SIGNAL(activated(const QString&)), this, SLOT(projectionComboBoxActivated(const QString&)));
    connect(theDatumComboBox, SIGNAL(activated(const QString&)), this, SLOT(datumComboBoxActivated(const QString&)));
    connect(thePixelTypeComboBox, SIGNAL(activated(const QString&)), this, SLOT(pixelTypeComboBoxActivated(const QString&)));
    connect(theZoneComboBox, SIGNAL(activated(const QString&)), this, SLOT(zoneComboBoxActivated(const QString&)));
    connect(theHemisphereComboBox, SIGNAL(activated(const QString&)), this, SLOT(hemisphereComboBoxActivated(const QString&)));
    connect(theOriginLatitudeLineEdit, SIGNAL(returnPressed()), this, SLOT(originLatitudeLineEditReturnPressed()));
    connect(theCentralMeridianLineEdit, SIGNAL(returnPressed()), this, SLOT(centralMeridianLineEditReturnPressed()));
    connect(theTiePointX_LineEdit, SIGNAL(returnPressed()), this, SLOT(tiePointX_LineEditReturnPressed()));
    connect(theTiePointY_LineEdit, SIGNAL(returnPressed()), this, SLOT(tiePointY_LineEditReturnPressed()));
    connect(theParallelOneLineEdit, SIGNAL(returnPressed()), this, SLOT(parallelOneLineEditReturnPressed()));
    connect(theParallelTwoLineEdit, SIGNAL(returnPressed()), this, SLOT(parallelTwoLineEditReturnPressed()));
    connect(theResetPushButton, SIGNAL(clicked()), this, SLOT(resetProjectionPushButtonClicked()));
    connect(theScanForMinMaxPushButton, SIGNAL(clicked()), this, SLOT(scanForMinMaxButtonClicked()));
    connect(theSavePushButton, SIGNAL(clicked()), this, SLOT(saveProjectionPushButtonClicked()));
    connect(theImageInfoTabWidget, SIGNAL(selected(const QString&)), this, SLOT(imageInfoTabWidgetSelected(const QString&)));
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ossimQtImageInfoDialog::~ossimQtImageInfoDialog()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ossimQtImageInfoDialog::languageChange()
{
    retranslateUi(this);
}

void ossimQtImageInfoDialog::setImageWidget(ossimQtScrollingImageWidget* widget)
{
   if (theController)
   {
      theController->setImageWidget(widget);
   }
}

void ossimQtImageInfoDialog::init()
{
   theController = new ossimQtImageInfoController(this);
}

void ossimQtImageInfoDialog::destroy()
{
   if (theController)
   {
      delete theController;
      theController = NULL;
   }
}

void ossimQtImageInfoDialog::closeButtonClicked()
{
   close(true);
}

void ossimQtImageInfoDialog::helpButtonClicked()
{
}

void ossimQtImageInfoDialog::scanForMinMaxButtonClicked()
{
   if (theController)
   {
      theController->scanForMinMax();
   }
}

void ossimQtImageInfoDialog::scanForValidVerticesButtonClicked()
{
   if (theController)
   {
      theController->scanForValidVertices();
   }
}

void ossimQtImageInfoDialog::layerComboBoxActivated( int /* layer */)
{
   if (theController)
   {
      theController->layerComboBoxActivated();	
   }
}

void ossimQtImageInfoDialog::bandSpinBoxValueChanged( int /* band */ )
{
   if (theController)
   {
      theController->initializeMinMaxNulls();
   }
}

void ossimQtImageInfoDialog::projectionComboBoxActivated( const QString & )
{
   if (theController)
   {
      theController->projectionComboBoxActivated();
   }
}

void ossimQtImageInfoDialog::datumComboBoxActivated( const QString & )
{
}

void ossimQtImageInfoDialog::pixelTypeComboBoxActivated( const QString & )
{
}

void ossimQtImageInfoDialog::zoneComboBoxActivated( const QString & )
{
   if (theController)
   {
      theController->zoneComboBoxActivated();
   }
}

void ossimQtImageInfoDialog::hemisphereComboBoxActivated( const QString & )
{
}

void ossimQtImageInfoDialog::originLatitudeLineEditReturnPressed()
{
   if (theController)
   {
      theController->originLineEditActivated();
   }
}

void ossimQtImageInfoDialog::centralMeridianLineEditReturnPressed()
{
   if (theController)
   {
      theController->centralMeridianLineEditActivated();
   }
}

void ossimQtImageInfoDialog::tiePointX_LineEditReturnPressed()
{
   if (theController)
   {
      theController->tiePointX_activated();
   }
}

void ossimQtImageInfoDialog::tiePointY_LineEditReturnPressed()
{
   if (theController)
   {
      theController->tiePointY_activated();
   }
}

void ossimQtImageInfoDialog::parallelOneLineEditReturnPressed()
{
   if (theController)
   {
      theController->parallelOneLineEditActivated();
   }
}

void ossimQtImageInfoDialog::parallelTwoLineEditReturnPressed()
{
   if (theController)
   {
      theController->parallelTwoLineEditActivated();
   }
}

void ossimQtImageInfoDialog::resetProjectionPushButtonClicked()
{
   if (theController)
   {
      theController->resetProjection();
   }
}


void ossimQtImageInfoDialog::saveProjectionPushButtonClicked()
{
   if (theController)
   {
      theController->saveProjection();
   }
}

void ossimQtImageInfoDialog::customDialogHasClosed()
{
   if (theController)
   {
      theController->customDialogHasClosed();
   }
}


void ossimQtImageInfoDialog::imageInfoTabWidgetSelected( const QString& qs )
{
   if (theController)
   {
      theController->tabChanged(qs);
   }
}
