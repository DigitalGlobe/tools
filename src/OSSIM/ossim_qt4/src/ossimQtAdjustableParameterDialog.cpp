#include <ossimQt/ossimQtAdjustableParameterDialog.h>

#include <QtCore/QVariant>
#include <QtGui/QImage>
#include <QtGui/QPixmap>

#include <ossimQt/ossimQtAdjustableParameterDialogController.h>

/*
 *  Constructs a ossimQtAdjustableParameterDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
ossimQtAdjustableParameterDialog::ossimQtAdjustableParameterDialog(QWidget* parent, const char* name, bool modal, Qt::WFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);


    // signals and slots connections
    connect(theCloseButton, SIGNAL(clicked()), this, SLOT(theCloseButton_clicked()));
    connect(theKeepAdjustmentButton, SIGNAL(clicked()), this, SLOT(theKeepAdjustmentButton_clicked()));
    connect(theAdjustmentSelectionBox, SIGNAL(activated(const QString&)), this, SLOT(theAdjustmentSelectionBox_activated(const QString&)));
    connect(theDeleteAdjustmentButto, SIGNAL(clicked()), this, SLOT(theDeleteAdjustmentButto_clicked()));
    connect(theCopyAdjustmentButton, SIGNAL(clicked()), this, SLOT(theCopyAdjustmentButton_clicked()));
    connect(theAdjustmentDescriptionInput, SIGNAL(textChanged(const QString&)), this, SLOT(theAdjustmentDescriptionInput_textChanged(const QString&)));
    connect(theSaveButton, SIGNAL(clicked()), this, SLOT(theSaveButton_clicked()));
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ossimQtAdjustableParameterDialog::~ossimQtAdjustableParameterDialog()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ossimQtAdjustableParameterDialog::languageChange()
{
    retranslateUi(this);
}

void ossimQtAdjustableParameterDialog::init()
{
    theController = new ossimQtAdjustableParameterDialogController(this);
}

void ossimQtAdjustableParameterDialog::destroy()
{
    if(theController)
    {
	delete theController;
	theController = NULL;
    }
}

void ossimQtAdjustableParameterDialog::theCloseButton_clicked()
{
    close(true);
}

ossimQtAdjustableParameterDialogController* ossimQtAdjustableParameterDialog::controller()
{
    return theController;
}


void ossimQtAdjustableParameterDialog::theKeepAdjustmentButton_clicked()
{
    if(theController)
    {
	theController->keepAdjustment();
    }
}


void ossimQtAdjustableParameterDialog::theAdjustmentSelectionBox_activated( const QString & )
{
    if(theController)
    {
	theController->selectionListChanged();
    }

}


void ossimQtAdjustableParameterDialog::theDeleteAdjustmentButto_clicked()
{
    if(theController)
    {
	theController->deleteAdjustment();
    }
}


void ossimQtAdjustableParameterDialog::theCopyAdjustmentButton_clicked()
{
   if(theController)
    {
	theController->copyAdjustment();
    }
}

void ossimQtAdjustableParameterDialog::theAdjustmentDescriptionInput_textChanged( const QString & )
{
    if(theController)
    {
	theController->adjustmentDescriptionChanged();
    }
}


void ossimQtAdjustableParameterDialog::theSaveButton_clicked()
{
   if(theController)
    {
	theController->saveAdjustment();
    }
}
