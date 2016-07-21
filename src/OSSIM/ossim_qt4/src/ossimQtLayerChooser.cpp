#include <ossimQt/ossimQtLayerChooser.h>

#include <QtCore/QVariant>
#include <QtGui/QImage>
#include <QtGui/QPixmap>

#include <ossimQt/ossimQtDataManagerListBox.h>

/*
 *  Constructs a ossimQtLayerChooser as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
ossimQtLayerChooser::ossimQtLayerChooser(QWidget* parent, const char* name, bool modal, Qt::WFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);


    // signals and slots connections
    connect(theApplyButton, SIGNAL(clicked()), this, SLOT(theApplyButton_clicked()));
    connect(theCancelButton, SIGNAL(clicked()), this, SLOT(theCancelButton_clicked()));
}

/*
 *  Destroys the object and frees any allocated resources
 */
ossimQtLayerChooser::~ossimQtLayerChooser()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ossimQtLayerChooser::languageChange()
{
    retranslateUi(this);
}

void ossimQtLayerChooser::theApplyButton_clicked()
{
    ossimConnectableObject::ConnectableObjectList selectedObjects;
    theDataManagerListBox->getSelectedObjects(selectedObjects);
    
    emit applySelected(selectedObjects);
}



void ossimQtLayerChooser::theCancelButton_clicked()
{
    close(true);
}
