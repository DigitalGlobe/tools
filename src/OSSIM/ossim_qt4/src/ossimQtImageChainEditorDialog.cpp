#include <ossimQt/ossimQtImageChainEditorDialog.h>

#include <QtCore/QVariant>
#include <QtGui/QImage>
#include <QtGui/QPixmap>

#include <ossimQt/ossimQtImageChainEditorController.h>
#include <ossimQt/ossimQtScrollingImageWidget.h>

/*
 *  Constructs a ossimQtImageChainEditorDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
ossimQtImageChainEditorDialog::ossimQtImageChainEditorDialog(QWidget* parent, const char* name, bool modal, Qt::WFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);


    // signals and slots connections
    connect(theClosePushButton, SIGNAL(clicked()), this, SLOT(closePushButton_clicked()));
    connect(theInsertBeforeButton, SIGNAL(clicked()), this, SLOT(insertBeforeButton_clicked()));
    connect(theLayerComboBox, SIGNAL(activated(int)), this, SLOT(layerComboBox_activated(int)));
    connect(theEditPropertiesPushButton, SIGNAL(clicked()), this, SLOT(editPropertiesPushButton_clicked()));
    connect(theDeletePushButton, SIGNAL(clicked()), this, SLOT(deletePushButton_clicked()));
    connect(theInsertAfterPushButton, SIGNAL(clicked()), this, SLOT(insertAfterPushButton_clicked()));
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ossimQtImageChainEditorDialog::~ossimQtImageChainEditorDialog()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ossimQtImageChainEditorDialog::languageChange()
{
    retranslateUi(this);
}

void ossimQtImageChainEditorDialog::setImageWidget(ossimQtScrollingImageWidget* widget)
{
   if (theController)
   {
      theController->setImageWidget(widget);
   }
}

void ossimQtImageChainEditorDialog::setAvailableFilterList( const std::vector<ossimString> & list )
{
   if (theController)
   {
      theController->setAvailableFilterList(list);
   }   
}

void ossimQtImageChainEditorDialog::init()
{
   theController = new ossimQtImageChainEditorController(this);
}

void ossimQtImageChainEditorDialog::destroy()
{
   if (theController)
   {
      delete theController;
      theController = NULL;
   }
}

void ossimQtImageChainEditorDialog::closePushButton_clicked()
{
   close(true);
}

void ossimQtImageChainEditorDialog::insertBeforeButton_clicked()
{
   if (theController)
   {
      theController->insertBeforeButtonClicked();
   }
}

void ossimQtImageChainEditorDialog::insertAfterPushButton_clicked()
{
   if (theController)
   {
      theController->insertAfterPushButtonClicked();
   }
}

void ossimQtImageChainEditorDialog::layerComboBox_activated( int /* layer */ )
{
   if (theController)
   {
      theController->layerComboBoxActivated();
   }
}

void ossimQtImageChainEditorDialog::editPropertiesPushButton_clicked()
{
   if (theController)
   {
      theController->editPropertiesPushButtonClicked();
   }
}

void ossimQtImageChainEditorDialog::deletePushButton_clicked()
{
   if (theController)
   {
      theController->deletePushButtonClicked();
   }   
}
