// $Id: ossimQtPreferencesDialog.h 12108 2007-12-04 17:42:05Z gpotts $
#ifndef OSSIMQTPREFERENCESDIALOG_H
#define OSSIMQTPREFERENCESDIALOG_H

#include <QtGui/QDialog>

class ossimQtPreferencesDialogController;

class QGroupBox;
class QListWidget;
class QListWidgetItem;
class QTextEdit;
class QVariant;
class QAction;
class QApplication;
class QButtonGroup;
class QHBoxLayout;
class QPushButton;
class QSpacerItem;
class QTabWidget;
class QVBoxLayout;
class QWidget;

class ossimQtPreferencesDialog : public QDialog
{
   Q_OBJECT
      
public:
   ossimQtPreferencesDialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
   ~ossimQtPreferencesDialog();
   
   QVBoxLayout *vboxLayout;
    QTabWidget *theTabWidget;
    QWidget *TabPage;
    QHBoxLayout *hboxLayout;
    QGroupBox *groupBox1;
    QVBoxLayout *vboxLayout1;
    QVBoxLayout *vboxLayout2;
    QListWidget *thePluginList;
    QHBoxLayout *hboxLayout1;
    QPushButton *theAddPluginButton;
    QPushButton *theSelectAllPluginButton;
    QPushButton *theReloadPluginButton;
    QPushButton *theRemovePluginButton;
    QGroupBox *groupBox2;
    QVBoxLayout *vboxLayout3;
    QTextEdit *thePluginDescription;
    QHBoxLayout *hboxLayout2;
    QSpacerItem *spacerItem;
    QPushButton *theCloseButton;
    QSpacerItem *spacerItem1;
    QHBoxLayout *hboxLayout3;

    void setupUi(QDialog *ossimQtPreferencesDialog);
    void retranslateUi(QDialog *ossimQtPreferencesDialog);


    virtual ossimQtPreferencesDialogController * controller();

public slots:
    virtual void theAddPluginButton_clicked();
    virtual void theRemovePluginButton_clicked();
    virtual void theCloseButton_clicked();
    virtual void thePluginList_currentChanged( QListWidgetItem * item );
    virtual void theSelectAllPluginButton_clicked();
    virtual void theReloadPluginButton_clicked();

protected:
    ossimQtPreferencesDialogController* theController;

protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

};

#endif // OSSIMQTPREFERENCESDIALOG_H
