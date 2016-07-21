#ifndef OSSIMQTQUADPROJECTIONDIALOG_H
#define OSSIMQTQUADPROJECTIONDIALOG_H

#include <QtCore/QVariant>
//Added by qt3to4:
#include <Qt3Support/Q3HBoxLayout>
#include <Qt3Support/Q3VBoxLayout>
#include <QtGui/QLabel>

class ossimQtQuadProjectionController;
class ossimQtScrollingImageWidget;
class ossimQtImageInfoController;

#include <Qt3Support/Q3ButtonGroup>
#include <Qt3Support/Q3Header>
#include <Qt3Support/Q3Table>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include "ossim/base/ossimConstants.h"
#include <Qt3Support/Q3MimeSourceFactory>

class Ui_ossimQtQuadProjectionDialog
{
public:
    Q3VBoxLayout *vboxLayout;
    QLabel *theTextLabel;
    Q3Table *theLineSampleGeographicTable;
    Q3HBoxLayout *hboxLayout;
    Q3ButtonGroup *theDatumButtonGroup;
    Q3HBoxLayout *hboxLayout1;
    QComboBox *theDatumComboBox;
    QSpacerItem *spacerItem;
    QRadioButton *theRoamingEnabledRadioButton;
    Q3ButtonGroup *theSaveResetCloseButtonGroup;
    Q3HBoxLayout *hboxLayout2;
    QPushButton *theAddRowPushButton;
    QPushButton *theDeleteRowPushButton;
    QPushButton *theSavePushButton;
    QPushButton *theResetPushButton;
    QPushButton *theClosePushButton;

    void setupUi(QDialog *ossimQtQuadProjectionDialog)
    {
    ossimQtQuadProjectionDialog->setObjectName(QString::fromUtf8("ossimQtQuadProjectionDialog"));
    ossimQtQuadProjectionDialog->resize(QSize(613, 299).expandedTo(ossimQtQuadProjectionDialog->minimumSizeHint()));
    vboxLayout = new Q3VBoxLayout(ossimQtQuadProjectionDialog);
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(11);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    theTextLabel = new QLabel(ossimQtQuadProjectionDialog);
    theTextLabel->setObjectName(QString::fromUtf8("theTextLabel"));

    vboxLayout->addWidget(theTextLabel);

    theLineSampleGeographicTable = new Q3Table(ossimQtQuadProjectionDialog);
    theLineSampleGeographicTable->setObjectName(QString::fromUtf8("theLineSampleGeographicTable"));
    theLineSampleGeographicTable->setNumRows(4);
    theLineSampleGeographicTable->setNumCols(5);
    theLineSampleGeographicTable->setSelectionMode(Q3Table::SingleRow);

    vboxLayout->addWidget(theLineSampleGeographicTable);

    hboxLayout = new Q3HBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    theDatumButtonGroup = new Q3ButtonGroup(ossimQtQuadProjectionDialog);
    theDatumButtonGroup->setObjectName(QString::fromUtf8("theDatumButtonGroup"));
    theDatumButtonGroup->setMinimumSize(QSize(360, 0));
    theDatumButtonGroup->setAlignment(Qt::AlignHCenter);
    theDatumButtonGroup->setColumnLayout(0, Qt::Vertical);
    theDatumButtonGroup->layout()->setSpacing(6);
    theDatumButtonGroup->layout()->setMargin(11);
    hboxLayout1 = new Q3HBoxLayout(theDatumButtonGroup->layout());
    hboxLayout1->setAlignment(Qt::AlignTop);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    theDatumComboBox = new QComboBox(theDatumButtonGroup);
    theDatumComboBox->setObjectName(QString::fromUtf8("theDatumComboBox"));

    hboxLayout1->addWidget(theDatumComboBox);


    hboxLayout->addWidget(theDatumButtonGroup);

    spacerItem = new QSpacerItem(70, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    theRoamingEnabledRadioButton = new QRadioButton(ossimQtQuadProjectionDialog);
    theRoamingEnabledRadioButton->setObjectName(QString::fromUtf8("theRoamingEnabledRadioButton"));

    hboxLayout->addWidget(theRoamingEnabledRadioButton);


    vboxLayout->addLayout(hboxLayout);

    theSaveResetCloseButtonGroup = new Q3ButtonGroup(ossimQtQuadProjectionDialog);
    theSaveResetCloseButtonGroup->setObjectName(QString::fromUtf8("theSaveResetCloseButtonGroup"));
    theSaveResetCloseButtonGroup->setColumnLayout(0, Qt::Vertical);
    theSaveResetCloseButtonGroup->layout()->setSpacing(6);
    theSaveResetCloseButtonGroup->layout()->setMargin(11);
    hboxLayout2 = new Q3HBoxLayout(theSaveResetCloseButtonGroup->layout());
    hboxLayout2->setAlignment(Qt::AlignTop);
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    theAddRowPushButton = new QPushButton(theSaveResetCloseButtonGroup);
    theAddRowPushButton->setObjectName(QString::fromUtf8("theAddRowPushButton"));

    hboxLayout2->addWidget(theAddRowPushButton);

    theDeleteRowPushButton = new QPushButton(theSaveResetCloseButtonGroup);
    theDeleteRowPushButton->setObjectName(QString::fromUtf8("theDeleteRowPushButton"));

    hboxLayout2->addWidget(theDeleteRowPushButton);

    theSavePushButton = new QPushButton(theSaveResetCloseButtonGroup);
    theSavePushButton->setObjectName(QString::fromUtf8("theSavePushButton"));

    hboxLayout2->addWidget(theSavePushButton);

    theResetPushButton = new QPushButton(theSaveResetCloseButtonGroup);
    theResetPushButton->setObjectName(QString::fromUtf8("theResetPushButton"));

    hboxLayout2->addWidget(theResetPushButton);

    theClosePushButton = new QPushButton(theSaveResetCloseButtonGroup);
    theClosePushButton->setObjectName(QString::fromUtf8("theClosePushButton"));

    hboxLayout2->addWidget(theClosePushButton);


    vboxLayout->addWidget(theSaveResetCloseButtonGroup);

    retranslateUi(ossimQtQuadProjectionDialog);

    QMetaObject::connectSlotsByName(ossimQtQuadProjectionDialog);
    } // setupUi

    void retranslateUi(QDialog *ossimQtQuadProjectionDialog)
    {
    ossimQtQuadProjectionDialog->setWindowTitle(QApplication::translate("ossimQtQuadProjectionDialog", "quad projection dialog", 0, QApplication::UnicodeUTF8));
    theTextLabel->setText(QApplication::translate("ossimQtQuadProjectionDialog", "Select row, use left button on image to drop point.  Edit cells as needed.", 0, QApplication::UnicodeUTF8));
    theDatumButtonGroup->setTitle(QApplication::translate("ossimQtQuadProjectionDialog", "Datum", 0, QApplication::UnicodeUTF8));
    theRoamingEnabledRadioButton->setText(QApplication::translate("ossimQtQuadProjectionDialog", "roaming enabled", 0, QApplication::UnicodeUTF8));
    theSaveResetCloseButtonGroup->setTitle(QApplication::translate("ossimQtQuadProjectionDialog", "", 0, QApplication::UnicodeUTF8));
    theAddRowPushButton->setText(QApplication::translate("ossimQtQuadProjectionDialog", "add row", 0, QApplication::UnicodeUTF8));
    theDeleteRowPushButton->setText(QApplication::translate("ossimQtQuadProjectionDialog", "delete row", 0, QApplication::UnicodeUTF8));
    theSavePushButton->setText(QApplication::translate("ossimQtQuadProjectionDialog", "save", 0, QApplication::UnicodeUTF8));
    theResetPushButton->setText(QApplication::translate("ossimQtQuadProjectionDialog", "reset", 0, QApplication::UnicodeUTF8));
    theClosePushButton->setText(QApplication::translate("ossimQtQuadProjectionDialog", "close", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ossimQtQuadProjectionDialog);
    } // retranslateUi

};

namespace Ui {
    class ossimQtQuadProjectionDialog: public Ui_ossimQtQuadProjectionDialog {};
} // namespace Ui

class ossimQtQuadProjectionDialog : public QDialog, public Ui::ossimQtQuadProjectionDialog
{
    Q_OBJECT

public:
    ossimQtQuadProjectionDialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0);
    ~ossimQtQuadProjectionDialog();

    virtual void setImageWidget( ossimQtScrollingImageWidget * widget , ossim_uint32 layer );

public slots:
    virtual void savePushButtonClicked();
    virtual void resetPushButtonClicked();
    virtual void closePushButtonClicked();
    virtual void deleteRowPushButtonClicked();
    virtual void addRowPushButtonClicked();
    virtual void cellValueChanged( int row, int col );
    virtual void roamingEnabledRadioButtonToggled( bool );

protected slots:
    virtual void languageChange();

private:
    ossimQtQuadProjectionController* theController;

    virtual void init();
    virtual void destroy();

};

#endif // OSSIMQTQUADPROJECTIONDIALOG_H
