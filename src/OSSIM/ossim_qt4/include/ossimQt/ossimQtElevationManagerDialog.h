// $Id: ossimQtElevationManagerDialog.h 12108 2007-12-04 17:42:05Z gpotts $
#ifndef OSSIMQTELEVATIONMANAGERDIALOG_H
#define OSSIMQTELEVATIONMANAGERDIALOG_H

#include <QtGui/QDialog>

class ossimQtElevationManagerController;

class QGroupBox;
class QVariant;
class QAction;
class QApplication;
class QButtonGroup;
class QCheckBox;
class QDialog;
class QFrame;
class QGridLayout;
class QHBoxLayout;
class QPushButton;
class QSpacerItem;
class QVBoxLayout;
class QListWidget;
class QSlider;
class QLabel;
class ossimQtMainWindowController;

class ossimQtElevationManagerDialog : public QDialog
{
    Q_OBJECT

public:
   ossimQtElevationManagerDialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
   ~ossimQtElevationManagerDialog();
   
   QVBoxLayout *vboxLayout;
   QVBoxLayout *vboxLayout1;
   QHBoxLayout *hboxLayout;
   QGroupBox *theElevationCellGroup;
   QGridLayout *gridLayout;
   QListWidget *theElevationListBox;
   QVBoxLayout *vboxLayout2;
   QPushButton *theAddButton;
   QPushButton *theAddDirectoryButton;
   QPushButton *theRemoveButton;
   QPushButton *theRemoveAllButton;
   QPushButton *theMoveUpButton;
   QPushButton *theMoveDownButton;
   QPushButton *theMoveToTopButton;
   QPushButton *theMoveToBottomButton;
   QPushButton *theMosaicButton;
   QHBoxLayout *hboxLayout1;
   QCheckBox *theElevationEnabledCheckBox;
   QCheckBox *theAutoLoadCheckBox;
   QCheckBox *theAutoSortCheckBox;
   QFrame *line1;
   QHBoxLayout *hboxLayout3;
   QHBoxLayout *hboxLayout4;
   QVBoxLayout *vboxLayout3;
   QLabel *theHgtOffsetLabel;
   QLabel *theHgtOffsetValueLabel;
   QSlider *theElevOffsetWidget;
   QCheckBox *theAutoRefreshCheckBox;
   QHBoxLayout *hboxLayout2;
   QSpacerItem *spacerItem;
   QPushButton *theCloseButton;
   QSpacerItem *spacerItem1;
   void setupUi(QDialog *ossimQtElevationManagerDialog);
   void retranslateUi(QDialog *ossimQtElevationManagerDialog);

public slots:
    virtual void elevationEnabledCheckBoxClicked();
    virtual void closeButtonClicked();
    virtual void addDirectoryClicked();
    virtual void removeButtonClicked();
    virtual void moveUpButtonClicked();
    virtual void removeAllButtonClicked();
    virtual void moveDownButtonClicked();
    virtual void moveToTopButtonClicked();
    virtual void moveToBottomButtonClicked();
    virtual void mosaicButtonClicked();
    virtual void autoLoadCheckBoxClicked();
    virtual void autoSortingCheckBoxClicked();
    virtual void addButtonClicked();
    virtual void elevOffsetChanged(int);
    virtual void autoRefreshCheckBoxClicked();


protected:
    ossimQtElevationManagerController* theController;
    ossimQtMainWindowController* theParent;
    virtual void destroy();

protected slots:
    virtual void languageChange();

private:
    virtual void init();

};

#endif // OSSIMQTELEVATIONMANAGERDIALOG_H
