#ifndef OSSIMQTBANDSELECTORDIALOG_H
#define OSSIMQTBANDSELECTORDIALOG_H

// #include <qvariant.h>
#include <QtGui/QDialog>

class ossimQtBandSelectorController;
class ossimBandSelector;
class QListWidget;
class QListWidgetItem;
class QGroupBox;
class QVBoxLayout;
class QHBoxLayout;
class QRadioButton;
class QLabel;
class QListWidget;
class QSpacerItem;
class QFrame;



// #include <QtCore/QVariant>
// #include <QtGui/QAction>
// #include <QtGui/QApplication>
// #include <QtGui/QButtonGroup>
//  #include <QtGui/QDialog>
// #include <QtGui/QFrame>
// #include <QtGui/QHBoxLayout>
// #include <QtGui/QLabel>
// #include <QtGui/QPushButton>
// #include <QtGui/QRadioButton>
// #include <QtGui/QSpacerItem>
// #include <QtGui/QVBoxLayout>
// #include <QListWidget>
// #include <Qt3Support/Q3MimeSourceFactory>

class Ui_ossimQtBandSelectorDialog
{
public:
    QVBoxLayout *vboxLayout;
    QGroupBox *theImageTypeGroupBox;
    QHBoxLayout *hboxLayout;
    QRadioButton *theGreyButton;
    QRadioButton *theTrueButton;
    QRadioButton *theNBandButton;
    QHBoxLayout *hboxLayout1;
    QLabel *theAvailableBandLabel;
    QSpacerItem *spacerItem;
    QLabel *theSelectedBandsLabel;
    QHBoxLayout *hboxLayout2;
    QListWidget *theAvailableBandsListBox;
    QVBoxLayout *vboxLayout1;
    QPushButton *theClearButton;
    QPushButton *theAllButton;
    QPushButton *theRightButton;
    QPushButton *theLeftButton;
    QPushButton *theUpButton;
    QPushButton *theDownButton;
    QPushButton *theMoveToTopButton;
    QPushButton *theMoveToBottomButton;
    QListWidget *theSelectedBandsListBox;
    QFrame *theLine1;
    QHBoxLayout *hboxLayout3;
    QSpacerItem *spacerItem1;
    QPushButton *theApplyButton;
    QSpacerItem *spacerItem2;
    QPushButton *theCloseButton;
    QSpacerItem *spacerItem3;

    void setupUi(QDialog *ossimQtBandSelectorDialog);

    void retranslateUi(QDialog *ossimQtBandSelectorDialog);


};

namespace Ui {
    class ossimQtBandSelectorDialog: public Ui_ossimQtBandSelectorDialog {};
} // namespace Ui

class ossimQtBandSelectorDialog : public QDialog, public Ui::ossimQtBandSelectorDialog
{
    Q_OBJECT

public:
    ossimQtBandSelectorDialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0);
    ~ossimQtBandSelectorDialog();

    virtual void setBandSelector( ossimBandSelector * bandSelector );

public slots:
    virtual void greyButtonClicked();
    virtual void trueButtonClicked();
    virtual void nBandButtonClicked();
    virtual void clearButtonClicked();
    virtual void downButtonClicked();
    virtual void moveToTopButtonClicked();
    virtual void moveToBottomButtonClicked();
    virtual void applyButtonClicked();
    virtual void closeButtonClicked();
    virtual void updateDisplay();
    virtual void allButtonClicked();
    virtual void rightButtonClicked();
    virtual void leftButtonClicked();
    virtual void upButtonClicked();
    virtual void availableBandClicked(QListWidgetItem * );

protected slots:
    virtual void languageChange();

private:
    ossimQtBandSelectorController* theController;

    virtual void init();
    virtual void destroy();

};

#endif // OSSIMQTBANDSELECTORDIALOG_H
