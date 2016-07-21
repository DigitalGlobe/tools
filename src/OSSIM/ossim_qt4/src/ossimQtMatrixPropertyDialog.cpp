#include <ossimQt/ossimQtMatrixPropertyDialog.h>

#include <QtCore/QVariant>
#include <QtGui/QImage>
#include <QtGui/QPixmap>

#include <ossimQt/ossimQtMatrixPropertyController.h>

/*
 *  Constructs a ossimQtMatrixPropertyDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
ossimQtMatrixPropertyDialog::ossimQtMatrixPropertyDialog(QWidget* parent, const char* name, bool modal, Qt::WFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);


    // signals and slots connections
    connect(theNormalizeButton, SIGNAL(clicked()), this, SLOT(theNormalizeButton_clicked()));
    connect(theZeroButton, SIGNAL(clicked()), this, SLOT(theZeroButton_clicked()));
    connect(theApplyButton, SIGNAL(clicked()), this, SLOT(theApplyButton_clicked()));
    connect(theCloseButton, SIGNAL(clicked()), this, SLOT(theCloseButton_clicked()));
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ossimQtMatrixPropertyDialog::~ossimQtMatrixPropertyDialog()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ossimQtMatrixPropertyDialog::languageChange()
{
    retranslateUi(this);
}

void ossimQtMatrixPropertyDialog::init()
{
    theController = new ossimQtMatrixPropertyController(this);
}

void ossimQtMatrixPropertyDialog::destroy()
{
    if(theController)
    {
	delete theController;
	theController = 0;
    }
}

ossimQtMatrixPropertyController* ossimQtMatrixPropertyDialog::controller()
{
    return theController;
}

void ossimQtMatrixPropertyDialog::theNormalizeButton_clicked()
{
    theController->normalizeButton();
}

void ossimQtMatrixPropertyDialog::theZeroButton_clicked()
{
    theController->zeroButton();
}

void ossimQtMatrixPropertyDialog::theApplyButton_clicked()
{
    theController->applyButton();
}

void ossimQtMatrixPropertyDialog::theCloseButton_clicked()
{
    close(true);
}
