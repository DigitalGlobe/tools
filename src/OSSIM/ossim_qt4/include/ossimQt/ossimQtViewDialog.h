#ifndef OSSIMQTVIEWDIALOG_H
#define OSSIMQTVIEWDIALOG_H

#include <QtCore/QVariant>
//Added by qt3to4:
#include <Qt3Support/Q3GridLayout>
#include <Qt3Support/Q3HBoxLayout>
#include <Qt3Support/Q3VBoxLayout>
#include <QtGui/QLabel>

class ossimQtViewDialogController;

#include <Qt3Support/Q3ButtonGroup>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <Qt3Support/Q3MimeSourceFactory>

class Ui_ossimQtViewDialog
{
public:
    Q3VBoxLayout *vboxLayout;
    Q3ButtonGroup *theTransformationGroup;
    Q3HBoxLayout *hboxLayout;
    QRadioButton *theMapRadioButton;
    QRadioButton *theIdentityRadioButton;
    QRadioButton *theStatePlaneRadioButton;
    QGridLayout *gridLayout;
    QComboBox *theHemisphereComboBox;
    QLineEdit *theScaleFactorInput;
    QLineEdit *theGsdInput;
    QLabel *theGsdLabel;
    QComboBox *theDatumComboBox;
    QLabel *theZoneLabel;
    QLabel *theScaleFactorLabel;
    QLineEdit *theStandardParallelInput;
    QLabel *theDatumLabel;
    QLabel *theHemisphereLabel;
    QLineEdit *theZoneInput;
    QLabel *theStandardParallelLabel;
    QLabel *theMapProjectionLabel;
    QComboBox *theProjectionComboBox;
    QLabel *theOriginLabel;
    QLineEdit *theOriginInput;
    Q3HBoxLayout *hboxLayout1;
    QPushButton *theHelpButton;
    QSpacerItem *spacerItem;
    QPushButton *thePropagateButton;
    QPushButton *theApplyButton;
    QPushButton *theCloseButton;

    void setupUi(QDialog *ossimQtViewDialog);

    void retranslateUi(QDialog *ossimQtViewDialog);
};

namespace Ui {
    class ossimQtViewDialog: public Ui_ossimQtViewDialog {};
} // namespace Ui

class ossimQtViewDialog : public QDialog, public Ui::ossimQtViewDialog
{
    Q_OBJECT

public:
    ossimQtViewDialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0);
    ~ossimQtViewDialog();

    ossimQtViewDialogController* theController;

    virtual ossimQtViewDialogController * controller();

public slots:
    virtual void setDefaultView();
    virtual void closeButton_clicked();
    virtual void applyButton_clicked();
    virtual void statePlaneRadioButton_clicked();
    virtual void mapRadioButton_clicked();
    virtual void identityButton_clicked();
    virtual void projectionComboBox_textChanged( const QString & value );
    virtual void projectionComboBox_activated( const QString & value );
    virtual void propagateButton_clicked();

protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

};

#endif // OSSIMQTVIEWDIALOG_H
