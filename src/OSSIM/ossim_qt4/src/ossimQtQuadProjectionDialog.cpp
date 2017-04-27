#include <ossimQt/ossimQtQuadProjectionDialog.h>

#include <QtCore/QVariant>
#include <QtGui/QImage>
#include <QtGui/QPixmap>

#include <ossimQt/ossimQtQuadProjectionController.h>
#include <ossimQt/ossimQtScrollingImageWidget.h>

/*
 *  Constructs a ossimQtQuadProjectionDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
ossimQtQuadProjectionDialog::ossimQtQuadProjectionDialog(QWidget* parent, const char* name, bool modal, Qt::WFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);


    // signals and slots connections
    connect(theSavePushButton, SIGNAL(clicked()), this, SLOT(savePushButtonClicked()));
    connect(theResetPushButton, SIGNAL(clicked()), this, SLOT(resetPushButtonClicked()));
    connect(theClosePushButton, SIGNAL(clicked()), this, SLOT(closePushButtonClicked()));
    connect(theDeleteRowPushButton, SIGNAL(clicked()), this, SLOT(deleteRowPushButtonClicked()));
    connect(theAddRowPushButton, SIGNAL(clicked()), this, SLOT(addRowPushButtonClicked()));
    connect(theLineSampleGeographicTable, SIGNAL(valueChanged(int,int)), this, SLOT(cellValueChanged(int,int)));
    connect(theRoamingEnabledRadioButton, SIGNAL(toggled(bool)), this, SLOT(roamingEnabledRadioButtonToggled(bool)));
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ossimQtQuadProjectionDialog::~ossimQtQuadProjectionDialog()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ossimQtQuadProjectionDialog::languageChange()
{
    retranslateUi(this);
}

void ossimQtQuadProjectionDialog::savePushButtonClicked()
{
   if (theController)
   {
      theController->save();
   }   
}

void ossimQtQuadProjectionDialog::setImageWidget(ossimQtScrollingImageWidget* widget, ossim_uint32 layer )
{
   if (theController)
   {
      theController->setImageWidget(widget, layer);
   }
}

void ossimQtQuadProjectionDialog::resetPushButtonClicked()
{
   if (theController)
   {
      theController->reset();
   }
}

void ossimQtQuadProjectionDialog::closePushButtonClicked()
{
   close(true);
}

void ossimQtQuadProjectionDialog::init()
{
   theController = new ossimQtQuadProjectionController(this);
}

void ossimQtQuadProjectionDialog::destroy()
{
   if (theController)
   {
      delete theController;
      theController = NULL;
   }
}

void ossimQtQuadProjectionDialog::deleteRowPushButtonClicked()
{
   if (theController)
   {
      theController->deleteRow();
   }
}

void ossimQtQuadProjectionDialog::addRowPushButtonClicked()
{

   if (theController)
   {
      theController->addRow();
   }  
}


void ossimQtQuadProjectionDialog::cellValueChanged( int row, int col )
{
   if (theController)
   {
      theController->cellValueChanged(row, col);
   }  
}


void ossimQtQuadProjectionDialog::roamingEnabledRadioButtonToggled( bool flag )
{
   if (theController)
   {
      theController->enableRoaming(flag);
   }
}

