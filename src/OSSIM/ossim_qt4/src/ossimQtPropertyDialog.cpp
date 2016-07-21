#include <ossimQt/ossimQtPropertyDialog.h>

#include <QtCore/QVariant>
#include <QtGui/QImage>
#include <QtGui/QPixmap>

#include <ossimQt/ossimQtPropertyDialogController.h>
#include <ossimQt/ossimQtPropertyListView.h>

/*
 *  Constructs a ossimQtPropertyDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
ossimQtPropertyDialog::ossimQtPropertyDialog(QWidget* parent, const char* name, bool modal, Qt::WFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ossimQtPropertyDialog::~ossimQtPropertyDialog()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ossimQtPropertyDialog::languageChange()
{
    retranslateUi(this);
}

void ossimQtPropertyDialog::init()
{
    theController = new ossimQtPropertyDialogController(this);
}

void ossimQtPropertyDialog::destroy()
{
    if(theController)
    {
	delete theController;
	theController = NULL;
    }
}

ossimQtPropertyDialogController* ossimQtPropertyDialog::controller()
{
    return theController;
}
