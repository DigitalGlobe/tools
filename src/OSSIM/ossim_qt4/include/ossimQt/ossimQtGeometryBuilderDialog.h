// $Id: ossimQtGeometryBuilderDialog.h 12108 2007-12-04 17:42:05Z gpotts $
#ifndef OSSIMQTGEOMETRYBUILDERDIALOG_H
#define OSSIMQTGEOMETRYBUILDERDIALOG_H

#include <QtGui/QDialog>

class ossimQtGeometryBuilderDialogController;

class QGroupBox;
class QTextEdit;
class QVariant;
class QAction;
class QApplication;
class QButtonGroup;
class QComboBox;
class QDialog;
class QHBoxLayout;
class QLabel;
class QPushButton;
class QSpacerItem;
class QTabWidget;
class QVBoxLayout;
class QWidget;

class ossimQtGeometryBuilderDialog : public QDialog
{
    Q_OBJECT

public:
   ossimQtGeometryBuilderDialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
   ~ossimQtGeometryBuilderDialog();
   
    QVBoxLayout *vboxLayout;
    QTabWidget *theTab;
    QWidget *TabPage;
    QVBoxLayout *vboxLayout1;
    QHBoxLayout *hboxLayout;
    QLabel *textLabel1;
    QComboBox *theOssimProjectionList;
    QTextEdit *theOssimGeometryString;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacerItem;
    QPushButton *theOssimSaveGeomButton;
    QPushButton *theMakeAffineAdjustableButton;
    QSpacerItem *spacerItem1;
    QWidget *tab;
    QVBoxLayout *vboxLayout2;
    QGroupBox *groupBox1;
    QVBoxLayout *vboxLayout3;
    QTextEdit *theWktProjectionString;
    QHBoxLayout *hboxLayout2;
    QSpacerItem *spacerItem2;
    QPushButton *theGdalWktLoadFromFileButton;
    QPushButton *theGdalWktConvertButton;
    QSpacerItem *spacerItem3;
    QHBoxLayout *hboxLayout3;
    QSpacerItem *spacerItem4;
    QPushButton *theCancelButton;
    QSpacerItem *spacerItem5;

    void setupUi(QDialog *ossimQtGeometryBuilderDialog);

    void retranslateUi(QDialog *ossimQtGeometryBuilderDialog);

    virtual ossimQtGeometryBuilderDialogController * controller();

public slots:
    virtual void theGdalWktConvertButton_clicked();
    virtual void theCancelButton_clicked();
    virtual void theOssimSaveGeomButton_clicked();
    virtual void theGdalWktLoadFromFileButton_clicked();
    virtual void theOssimProjectionList_activated( const QString & );

protected:
    ossimQtGeometryBuilderDialogController* theController;

protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

};

#endif // OSSIMQTGEOMETRYBUILDERDIALOG_H
