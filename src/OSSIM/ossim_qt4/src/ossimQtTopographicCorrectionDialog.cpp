#include <ossimQt/ossimQtTopographicCorrectionDialog.h>


#include <ossimQt/ossimQtTopographicCorrectionDialogController.h>

/*
 *  Constructs a ossimQtTopographicCorrectionDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
ossimQtTopographicCorrectionDialog::ossimQtTopographicCorrectionDialog(QWidget* parent, const char* name, bool modal, Qt::WFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);


    // signals and slots connections
    connect(theCloseButton, SIGNAL(clicked()), this, SLOT(closeButtonClicked()));
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ossimQtTopographicCorrectionDialog::~ossimQtTopographicCorrectionDialog()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ossimQtTopographicCorrectionDialog::languageChange()
{
    retranslateUi(this);
}

void ossimQtTopographicCorrectionDialog::init()
{
    theController = new ossimQtTopographicCorrectionDialogController(this);
}

void ossimQtTopographicCorrectionDialog::destroy()
{
    if(theController)
    {
	delete theController;
	theController = NULL;
    }
}

ossimQtTopographicCorrectionDialogController* ossimQtTopographicCorrectionDialog::controller()
{
    return theController;
}


void ossimQtTopographicCorrectionDialog::closeButtonClicked()
{
   close(true);
}
