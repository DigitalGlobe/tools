#ifndef OSSIMQTDATAMANAGERDIALOG_H
#define OSSIMQTDATAMANAGERDIALOG_H

#include <QtCore/QVariant>

class ossimDataManager;
class ossimQtDataManagerListBox;

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include "ossimQtDataManagerListBox.h"

class Ui_ossimQtDataManagerDialog
{
public:
    QHBoxLayout *hboxLayout;
    ossimQtDataManagerListBox *theDataManagerListBox;
    QVBoxLayout *vboxLayout;
    QPushButton *theDisplayButton;
    QPushButton *theDeleteButton;
    QPushButton *theDuplicatButton;
    QPushButton *theMosaicButton;
    QPushButton *theBlendButton;
    QPushButton *theFeatherButton;
    QSpacerItem *spacerItem;
    QPushButton *theCloseButton;

    void setupUi(QDialog *ossimQtDataManagerDialog);

    void retranslateUi(QDialog *ossimQtDataManagerDialog);

protected:
    enum IconID
    {
        image0_ID,
        unknown_ID
    };
    static QPixmap icon(IconID id)
    {
       static const char* const image0_data[] = {"0 0 0 1"};

       switch (id)
       {
          case image0_ID: return QPixmap((const char**)image0_data);
          default: return QPixmap();
       } // switch
    } // icon
};

namespace Ui {
    class ossimQtDataManagerDialog: public Ui_ossimQtDataManagerDialog {};
} // namespace Ui

class ossimQtDataManagerDialog : public QDialog, public Ui::ossimQtDataManagerDialog
{
    Q_OBJECT

public:
    ossimQtDataManagerDialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~ossimQtDataManagerDialog();

public slots:
    virtual void setDataManager( ossimDataManager * dataManager );
    virtual void theCloseButton_clicked();
    virtual void theDisplayButton_clicked();

protected slots:
    virtual void languageChange();

};

#endif // OSSIMQTDATAMANAGERDIALOG_H
