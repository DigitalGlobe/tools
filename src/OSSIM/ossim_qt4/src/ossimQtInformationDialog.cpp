#include <ossimQt/ossimQtInformationDialog.h>
#include <QtGui/QLayout>
#include <Qt3Support/Q3TextEdit>
//Added by qt3to4:
#include <Qt3Support/Q3VBoxLayout>

ossimQtInformationDialog::ossimQtInformationDialog(QWidget* parent,
                                                   bool modal,
                                                   Qt::WFlags f)
   :QDialog(parent, "ossimQtInformationDialog", modal, f)
{
   theMainVBox = new Q3VBoxLayout(this);

   theTextEditor = new Q3TextEdit(this);
   theMainVBox->addWidget(theTextEditor);
}

void ossimQtInformationDialog::setText(const QString& value)
{
   theTextEditor->setText(value);
}

Q3TextEdit*  ossimQtInformationDialog::textEditor()
{
   return theTextEditor;
}
