#ifndef ossimQtInformationDialog_HEADER
#define ossimQtInformationDialog_HEADER
#include <QtGui/QDialog>
//Added by qt3to4:
#include <Qt3Support/Q3VBoxLayout>
class Q3VBoxLayout;
class Q3TextEdit;

class ossimQtInformationDialog : public QDialog
{
public:
   ossimQtInformationDialog(QWidget* parent=0,
                            bool modal = false,
                            Qt::WFlags f = 0);
   void setText(const QString& value);
   Q3TextEdit*  textEditor();
protected:
   Q3VBoxLayout* theMainVBox;
   Q3TextEdit*   theTextEditor;
};

#endif
