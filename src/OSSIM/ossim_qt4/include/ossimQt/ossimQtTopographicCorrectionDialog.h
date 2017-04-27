#ifndef OSSIMQTTOPOGRAPHICCORRECTIONDIALOG_H
#define OSSIMQTTOPOGRAPHICCORRECTIONDIALOG_H

#include <QtCore/QVariant>
//Added by qt3to4:
#include <Qt3Support/Q3HBoxLayout>
#include <Qt3Support/Q3VBoxLayout>
#include <QtGui/QLabel>

class ossimQtTopographicCorrectionDialogController;

#include <Qt3Support/Q3GroupBox>
#include <Qt3Support/Q3Header>
#include <Qt3Support/Q3Table>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
// #include <QtGui/Q3ButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
// #include <QtGui/Q3HBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
// #include <QtGui/Q3VBoxLayout>
#include <Qt3Support/Q3MimeSourceFactory>

class Ui_ossimQtTopographicCorrectionDialog
{
public:
    Q3VBoxLayout *vboxLayout;
    Q3GroupBox *theCorrectionTypeGroup;
    Q3HBoxLayout *hboxLayout;
    QComboBox *theCorrectionTypeComboBox;
    Q3GroupBox *theCorrectionParameterGroup;
    Q3HBoxLayout *hboxLayout1;
    Q3Table *theAdjustableParameterTable;
    Q3VBoxLayout *vboxLayout1;
    Q3Table *theGainBiasTable;
    Q3HBoxLayout *hboxLayout2;
    QLabel *theSunElevationLabel;
    QLineEdit *theSunElevationInput;
    Q3HBoxLayout *hboxLayout3;
    QLabel *theSunAzimuthLabel;
    QLineEdit *theSunAzimuthInput;
    QPushButton *theImportParametersButton;
    Q3HBoxLayout *hboxLayout4;
    QCheckBox *theAutoApplyCheckBox;
    QPushButton *theApplyButton;
    QPushButton *theResetButton;
    QPushButton *theCloseButton;

    void setupUi(QDialog *ossimQtTopographicCorrectionDialog)
    {
    ossimQtTopographicCorrectionDialog->setObjectName(QString::fromUtf8("ossimQtTopographicCorrectionDialog"));
    ossimQtTopographicCorrectionDialog->resize(QSize(610, 386).expandedTo(ossimQtTopographicCorrectionDialog->minimumSizeHint()));
    vboxLayout = new Q3VBoxLayout(ossimQtTopographicCorrectionDialog);
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(11);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    theCorrectionTypeGroup = new Q3GroupBox(ossimQtTopographicCorrectionDialog);
    theCorrectionTypeGroup->setObjectName(QString::fromUtf8("theCorrectionTypeGroup"));
    theCorrectionTypeGroup->setColumnLayout(0, Qt::Vertical);
    theCorrectionTypeGroup->layout()->setSpacing(6);
    theCorrectionTypeGroup->layout()->setMargin(11);
    hboxLayout = new Q3HBoxLayout(theCorrectionTypeGroup->layout());
    hboxLayout->setAlignment(Qt::AlignTop);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    theCorrectionTypeComboBox = new QComboBox(theCorrectionTypeGroup);
    theCorrectionTypeComboBox->setObjectName(QString::fromUtf8("theCorrectionTypeComboBox"));

    hboxLayout->addWidget(theCorrectionTypeComboBox);


    vboxLayout->addWidget(theCorrectionTypeGroup);

    theCorrectionParameterGroup = new Q3GroupBox(ossimQtTopographicCorrectionDialog);
    theCorrectionParameterGroup->setObjectName(QString::fromUtf8("theCorrectionParameterGroup"));
    theCorrectionParameterGroup->setColumnLayout(0, Qt::Vertical);
    theCorrectionParameterGroup->layout()->setSpacing(6);
    theCorrectionParameterGroup->layout()->setMargin(11);
    hboxLayout1 = new Q3HBoxLayout(theCorrectionParameterGroup->layout());
    hboxLayout1->setAlignment(Qt::AlignTop);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    theAdjustableParameterTable = new Q3Table(theCorrectionParameterGroup);
    theAdjustableParameterTable->setObjectName(QString::fromUtf8("theAdjustableParameterTable"));
    theAdjustableParameterTable->setNumRows(0);
    theAdjustableParameterTable->setNumCols(1);

    hboxLayout1->addWidget(theAdjustableParameterTable);

    vboxLayout1 = new Q3VBoxLayout();
    vboxLayout1->setSpacing(6);
    vboxLayout1->setMargin(0);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    theGainBiasTable = new Q3Table(theCorrectionParameterGroup);
    theGainBiasTable->setObjectName(QString::fromUtf8("theGainBiasTable"));
    theGainBiasTable->setNumRows(0);
    theGainBiasTable->setNumCols(2);

    vboxLayout1->addWidget(theGainBiasTable);

    hboxLayout2 = new Q3HBoxLayout();
    hboxLayout2->setSpacing(6);
    hboxLayout2->setMargin(0);
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    theSunElevationLabel = new QLabel(theCorrectionParameterGroup);
    theSunElevationLabel->setObjectName(QString::fromUtf8("theSunElevationLabel"));

    hboxLayout2->addWidget(theSunElevationLabel);

    theSunElevationInput = new QLineEdit(theCorrectionParameterGroup);
    theSunElevationInput->setObjectName(QString::fromUtf8("theSunElevationInput"));

    hboxLayout2->addWidget(theSunElevationInput);


    vboxLayout1->addLayout(hboxLayout2);

    hboxLayout3 = new Q3HBoxLayout();
    hboxLayout3->setSpacing(6);
    hboxLayout3->setMargin(0);
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
    theSunAzimuthLabel = new QLabel(theCorrectionParameterGroup);
    theSunAzimuthLabel->setObjectName(QString::fromUtf8("theSunAzimuthLabel"));

    hboxLayout3->addWidget(theSunAzimuthLabel);

    theSunAzimuthInput = new QLineEdit(theCorrectionParameterGroup);
    theSunAzimuthInput->setObjectName(QString::fromUtf8("theSunAzimuthInput"));

    hboxLayout3->addWidget(theSunAzimuthInput);


    vboxLayout1->addLayout(hboxLayout3);

    theImportParametersButton = new QPushButton(theCorrectionParameterGroup);
    theImportParametersButton->setObjectName(QString::fromUtf8("theImportParametersButton"));

    vboxLayout1->addWidget(theImportParametersButton);


    hboxLayout1->addLayout(vboxLayout1);


    vboxLayout->addWidget(theCorrectionParameterGroup);

    hboxLayout4 = new Q3HBoxLayout();
    hboxLayout4->setSpacing(6);
    hboxLayout4->setMargin(0);
    hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
    theAutoApplyCheckBox = new QCheckBox(ossimQtTopographicCorrectionDialog);
    theAutoApplyCheckBox->setObjectName(QString::fromUtf8("theAutoApplyCheckBox"));
    theAutoApplyCheckBox->setChecked(true);

    hboxLayout4->addWidget(theAutoApplyCheckBox);

    theApplyButton = new QPushButton(ossimQtTopographicCorrectionDialog);
    theApplyButton->setObjectName(QString::fromUtf8("theApplyButton"));

    hboxLayout4->addWidget(theApplyButton);

    theResetButton = new QPushButton(ossimQtTopographicCorrectionDialog);
    theResetButton->setObjectName(QString::fromUtf8("theResetButton"));
    theResetButton->setEnabled(false);

    hboxLayout4->addWidget(theResetButton);

    theCloseButton = new QPushButton(ossimQtTopographicCorrectionDialog);
    theCloseButton->setObjectName(QString::fromUtf8("theCloseButton"));

    hboxLayout4->addWidget(theCloseButton);


    vboxLayout->addLayout(hboxLayout4);

    retranslateUi(ossimQtTopographicCorrectionDialog);

    QMetaObject::connectSlotsByName(ossimQtTopographicCorrectionDialog);
    } // setupUi

    void retranslateUi(QDialog *ossimQtTopographicCorrectionDialog)
    {
    ossimQtTopographicCorrectionDialog->setWindowTitle(QApplication::translate("ossimQtTopographicCorrectionDialog", "Topographic Correction Editor", 0, QApplication::UnicodeUTF8));
    theCorrectionTypeGroup->setTitle(QApplication::translate("ossimQtTopographicCorrectionDialog", "Correction Type", 0, QApplication::UnicodeUTF8));
    theCorrectionTypeComboBox->clear();
    theCorrectionTypeComboBox->addItem(QApplication::translate("ossimQtTopographicCorrectionDialog", "Cosine", 0, QApplication::UnicodeUTF8));
    theCorrectionTypeComboBox->addItem(QApplication::translate("ossimQtTopographicCorrectionDialog", "Minnaert", 0, QApplication::UnicodeUTF8));
    theCorrectionParameterGroup->setTitle(QApplication::translate("ossimQtTopographicCorrectionDialog", "Correction Parameters", 0, QApplication::UnicodeUTF8));
    theAdjustableParameterTable->horizontalHeader()->setLabel(0, QApplication::translate("ossimQtTopographicCorrectionDialog", "C Values", 0, QApplication::UnicodeUTF8));
    theGainBiasTable->horizontalHeader()->setLabel(0, QApplication::translate("ossimQtTopographicCorrectionDialog", "Gains", 0, QApplication::UnicodeUTF8));
    theGainBiasTable->horizontalHeader()->setLabel(1, QApplication::translate("ossimQtTopographicCorrectionDialog", "Bias", 0, QApplication::UnicodeUTF8));
    theSunElevationLabel->setText(QApplication::translate("ossimQtTopographicCorrectionDialog", "Sun Elevation:", 0, QApplication::UnicodeUTF8));
    theSunAzimuthLabel->setText(QApplication::translate("ossimQtTopographicCorrectionDialog", "Sun Azimuth:", 0, QApplication::UnicodeUTF8));
    theImportParametersButton->setText(QApplication::translate("ossimQtTopographicCorrectionDialog", "Import...", 0, QApplication::UnicodeUTF8));
    theAutoApplyCheckBox->setText(QApplication::translate("ossimQtTopographicCorrectionDialog", "Auto apply", 0, QApplication::UnicodeUTF8));
    theApplyButton->setText(QApplication::translate("ossimQtTopographicCorrectionDialog", "Apply", 0, QApplication::UnicodeUTF8));
    theResetButton->setText(QApplication::translate("ossimQtTopographicCorrectionDialog", "Reset", 0, QApplication::UnicodeUTF8));
    theCloseButton->setText(QApplication::translate("ossimQtTopographicCorrectionDialog", "Close", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ossimQtTopographicCorrectionDialog);
    } // retranslateUi

};

namespace Ui {
    class ossimQtTopographicCorrectionDialog: public Ui_ossimQtTopographicCorrectionDialog {};
} // namespace Ui

class ossimQtTopographicCorrectionDialog : public QDialog, public Ui::ossimQtTopographicCorrectionDialog
{
    Q_OBJECT

public:
    ossimQtTopographicCorrectionDialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0);
    ~ossimQtTopographicCorrectionDialog();

    virtual ossimQtTopographicCorrectionDialogController * controller();

public slots:
    virtual void closeButtonClicked();

protected:
    ossimQtTopographicCorrectionDialogController* theController;

protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

};

#endif // OSSIMQTTOPOGRAPHICCORRECTIONDIALOG_H
