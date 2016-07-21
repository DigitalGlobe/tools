#include <ossimQt/ossimQtStringListPropertyDialog.h>


#include <ossimQt/ossimQtStringListPropertyController.h>

/*
 *  Constructs a ossimQtStringListPropertyDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
ossimQtStringListPropertyDialog::ossimQtStringListPropertyDialog(QWidget* parent, const char* name, bool modal, Qt::WFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);


    // signals and slots connections
    connect(theCloseButton, SIGNAL(clicked()), this, SLOT(theCloseButton_clicked()));
    connect(theApplyButton, SIGNAL(clicked()), this, SLOT(theApplyButton_clicked()));
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ossimQtStringListPropertyDialog::~ossimQtStringListPropertyDialog()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ossimQtStringListPropertyDialog::languageChange()
{
    retranslateUi(this);
}

void ossimQtStringListPropertyDialog::init()
{
    theController = new ossimQtStringListPropertyController(this);
}

void ossimQtStringListPropertyDialog::destroy()
{
    if(theController)
    {
	delete theController;
	theController = NULL;
    }
}

ossimQtStringListPropertyController* ossimQtStringListPropertyDialog::controller()
{
    return theController;
}


void ossimQtStringListPropertyDialog::theCloseButton_clicked()
{
    close(true);
}

void ossimQtStringListPropertyDialog::theApplyButton_clicked()
{
    theController->applyButton();
}
