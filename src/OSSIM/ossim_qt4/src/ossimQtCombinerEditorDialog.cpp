#include <ossimQt/ossimQtCombinerEditorDialog.h>

#include <QtCore/QVariant>
#include <QtGui/QImage>
#include <QtGui/QPixmap>

#include <ossimQt/ossimQtCombinerEditorDialogController.h>
#include <ossimQt/ossimQtLayerEditor.h>

/*
 *  Constructs a ossimQtCombinerEditorDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
ossimQtCombinerEditorDialog::ossimQtCombinerEditorDialog(QWidget* parent, const char* name, bool modal, Qt::WFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);


    // signals and slots connections
    connect(theCloseButton, SIGNAL(clicked()), this, SLOT(theCloseButton_clicked()));
    connect(theApplyButton, SIGNAL(clicked()), this, SLOT(theApplyButton_clicked()));
    connect(theResetButton, SIGNAL(clicked()), this, SLOT(theResetButton_clicked()));
    connect(theAutoApplyCheckBox, SIGNAL(toggled(bool)), this, SLOT(theAutoApplyCheckBox_toggled(bool)));
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ossimQtCombinerEditorDialog::~ossimQtCombinerEditorDialog()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ossimQtCombinerEditorDialog::languageChange()
{
    retranslateUi(this);
}

void ossimQtCombinerEditorDialog::init()
{
    theController = new ossimQtCombinerEditorDialogController(this);
    theWidgetStack->hide();
}

void ossimQtCombinerEditorDialog::destroy()
{
    if(theController)
    {
	delete theController;
	theController = NULL;
    }
}

void ossimQtCombinerEditorDialog::theCloseButton_clicked()
{
    close(true);
}


ossimQtCombinerEditorDialogController* ossimQtCombinerEditorDialog::controller()
{
    return theController;
}


void ossimQtCombinerEditorDialog::theApplyButton_clicked()
{
    emit apply();
}


void ossimQtCombinerEditorDialog::theResetButton_clicked()
{
    emit reset();
}


void ossimQtCombinerEditorDialog::theAutoApplyCheckBox_toggled( bool value )
{
    theApplyButton->setEnabled(!value);
}


void ossimQtCombinerEditorDialog::propertyValuesChanged()
{
    if(theAutoApplyCheckBox->isOn())
    {
	emit apply();
    }
}
