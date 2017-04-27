#ifndef OSSIMQTHSIREMAPPERDIALOG_H
#define OSSIMQTHSIREMAPPERDIALOG_H

#include <QtCore/QVariant>
//Added by qt3to4:
#include <Qt3Support/Q3HBoxLayout>
#include <Qt3Support/Q3Frame>
#include <QtGui/QLabel>
#include <Qt3Support/Q3VBoxLayout>

class ossimQtHsiRemapperController;
class ossimHsiRemapper;

#include <Qt3Support/Q3ButtonGroup>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
// #include <QtGui/Q3ButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
// #include <QtGui/Q3Frame>
// #include <QtGui/Q3HBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
// #include <QtGui/Q3VBoxLayout>
#include <Qt3Support/Q3MimeSourceFactory>

class Ui_ossimQtHsiRemapperDialog
{
public:
    Q3VBoxLayout *vboxLayout;
    Q3ButtonGroup *theColorRangeButtonGroup;
    Q3HBoxLayout *hboxLayout;
    Q3HBoxLayout *hboxLayout1;
    QRadioButton *theRedButton;
    QRadioButton *theYellowButton;
    QRadioButton *theGreenButton;
    QRadioButton *theCyanButton;
    QRadioButton *theBlueButton;
    QRadioButton *theMagentaButton;
    QRadioButton *theAllButton;
    Q3VBoxLayout *vboxLayout1;
    Q3HBoxLayout *hboxLayout2;
    QLabel *theHueOffsetLabel;
    QSlider *theHueOffsetSlider;
    QLabel *theHueOffsetValueLabel;
    Q3HBoxLayout *hboxLayout3;
    QLabel *theHueLowRangeLabel;
    QSlider *theHueLowRangeSlider;
    QLabel *theHueLowRangeValueLabel;
    Q3HBoxLayout *hboxLayout4;
    QLabel *theHueHighRangeLabel;
    QSlider *theHueHighRangeSlider;
    QLabel *theHueHighRangeValueLabel;
    Q3HBoxLayout *hboxLayout5;
    QLabel *theHueBlendRangeLabel;
    QSlider *theHueBlendRangeSlider;
    QLabel *theHueBlendRangeValueLabel;
    Q3HBoxLayout *hboxLayout6;
    QLabel *theSaturationOffsetLabel;
    QSlider *theSaturationOffsetSlider;
    QLabel *theSaturationOffsetValueLabel;
    Q3HBoxLayout *hboxLayout7;
    QLabel *theIntensityOffsetLabel;
    QSlider *theIntensityOffsetSlider;
    QLabel *theIntensityOffsetValueLabel;
    Q3HBoxLayout *hboxLayout8;
    QLabel *theLowIntensityClipLabel;
    QSlider *theLowIntensityClipSlider;
    QLabel *theLowIntensityClipValueLabel;
    Q3HBoxLayout *hboxLayout9;
    QLabel *theHighIntensityClipLabel;
    QSlider *theHighIntensityClipSlider;
    QLabel *theHighIntensityClipValueLabel;
    Q3HBoxLayout *hboxLayout10;
    QLabel *theWhiteObjectClipLabel;
    QSlider *theWhiteObjectClipSlider;
    QLabel *theWhiteObjectClipValueLabel;
    Q3Frame *line1;
    Q3HBoxLayout *hboxLayout11;
    QCheckBox *theEnableButton;
    QPushButton *theResetGroupButton;
    QPushButton *theResetAllGroup;
    QPushButton *theCloseButton;

    void setupUi(QDialog *ossimQtHsiRemapperDialog)
    {
    ossimQtHsiRemapperDialog->setObjectName(QString::fromUtf8("ossimQtHsiRemapperDialog"));
    ossimQtHsiRemapperDialog->resize(QSize(478, 391).expandedTo(ossimQtHsiRemapperDialog->minimumSizeHint()));
    vboxLayout = new Q3VBoxLayout(ossimQtHsiRemapperDialog);
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(11);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    theColorRangeButtonGroup = new Q3ButtonGroup(ossimQtHsiRemapperDialog);
    theColorRangeButtonGroup->setObjectName(QString::fromUtf8("theColorRangeButtonGroup"));
    theColorRangeButtonGroup->setMaximumSize(QSize(500, 50));
    theColorRangeButtonGroup->setAlignment(Qt::AlignTop);
    theColorRangeButtonGroup->setExclusive(true);
    theColorRangeButtonGroup->setColumnLayout(0, Qt::Vertical);
    theColorRangeButtonGroup->layout()->setSpacing(6);
    theColorRangeButtonGroup->layout()->setMargin(11);
    hboxLayout = new Q3HBoxLayout(theColorRangeButtonGroup->layout());
    hboxLayout->setAlignment(Qt::AlignTop);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    hboxLayout1 = new Q3HBoxLayout();
    hboxLayout1->setSpacing(6);
    hboxLayout1->setMargin(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    theRedButton = new QRadioButton(theColorRangeButtonGroup);
    theRedButton->setObjectName(QString::fromUtf8("theRedButton"));

    hboxLayout1->addWidget(theRedButton);

    theYellowButton = new QRadioButton(theColorRangeButtonGroup);
    theYellowButton->setObjectName(QString::fromUtf8("theYellowButton"));

    hboxLayout1->addWidget(theYellowButton);

    theGreenButton = new QRadioButton(theColorRangeButtonGroup);
    theGreenButton->setObjectName(QString::fromUtf8("theGreenButton"));

    hboxLayout1->addWidget(theGreenButton);

    theCyanButton = new QRadioButton(theColorRangeButtonGroup);
    theCyanButton->setObjectName(QString::fromUtf8("theCyanButton"));

    hboxLayout1->addWidget(theCyanButton);

    theBlueButton = new QRadioButton(theColorRangeButtonGroup);
    theBlueButton->setObjectName(QString::fromUtf8("theBlueButton"));

    hboxLayout1->addWidget(theBlueButton);

    theMagentaButton = new QRadioButton(theColorRangeButtonGroup);
    theMagentaButton->setObjectName(QString::fromUtf8("theMagentaButton"));

    hboxLayout1->addWidget(theMagentaButton);

    theAllButton = new QRadioButton(theColorRangeButtonGroup);
    theAllButton->setObjectName(QString::fromUtf8("theAllButton"));

    hboxLayout1->addWidget(theAllButton);


    hboxLayout->addLayout(hboxLayout1);


    vboxLayout->addWidget(theColorRangeButtonGroup);

    vboxLayout1 = new Q3VBoxLayout();
    vboxLayout1->setSpacing(8);
    vboxLayout1->setMargin(0);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    hboxLayout2 = new Q3HBoxLayout();
    hboxLayout2->setSpacing(6);
    hboxLayout2->setMargin(0);
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    theHueOffsetLabel = new QLabel(ossimQtHsiRemapperDialog);
    theHueOffsetLabel->setObjectName(QString::fromUtf8("theHueOffsetLabel"));
    theHueOffsetLabel->setMinimumSize(QSize(110, 0));
    theHueOffsetLabel->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

    hboxLayout2->addWidget(theHueOffsetLabel);

    theHueOffsetSlider = new QSlider(ossimQtHsiRemapperDialog);
    theHueOffsetSlider->setObjectName(QString::fromUtf8("theHueOffsetSlider"));
    theHueOffsetSlider->setMinimumSize(QSize(290, 0));
    theHueOffsetSlider->setMinimum(-180);
    theHueOffsetSlider->setMaximum(180);
    theHueOffsetSlider->setPageStep(1);
    theHueOffsetSlider->setTracking(false);
    theHueOffsetSlider->setOrientation(Qt::Horizontal);
    theHueOffsetSlider->setTickInterval(60);

    hboxLayout2->addWidget(theHueOffsetSlider);

    theHueOffsetValueLabel = new QLabel(ossimQtHsiRemapperDialog);
    theHueOffsetValueLabel->setObjectName(QString::fromUtf8("theHueOffsetValueLabel"));
    theHueOffsetValueLabel->setMinimumSize(QSize(40, 0));
    theHueOffsetValueLabel->setAlignment(Qt::AlignVCenter);

    hboxLayout2->addWidget(theHueOffsetValueLabel);


    vboxLayout1->addLayout(hboxLayout2);

    hboxLayout3 = new Q3HBoxLayout();
    hboxLayout3->setSpacing(6);
    hboxLayout3->setMargin(0);
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
    theHueLowRangeLabel = new QLabel(ossimQtHsiRemapperDialog);
    theHueLowRangeLabel->setObjectName(QString::fromUtf8("theHueLowRangeLabel"));
    theHueLowRangeLabel->setMinimumSize(QSize(110, 0));
    theHueLowRangeLabel->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

    hboxLayout3->addWidget(theHueLowRangeLabel);

    theHueLowRangeSlider = new QSlider(ossimQtHsiRemapperDialog);
    theHueLowRangeSlider->setObjectName(QString::fromUtf8("theHueLowRangeSlider"));
    theHueLowRangeSlider->setMinimumSize(QSize(290, 0));
    theHueLowRangeSlider->setMinimum(-30);
    theHueLowRangeSlider->setMaximum(30);
    theHueLowRangeSlider->setPageStep(1);
    theHueLowRangeSlider->setTracking(false);
    theHueLowRangeSlider->setOrientation(Qt::Horizontal);
    theHueLowRangeSlider->setTickInterval(10);

    hboxLayout3->addWidget(theHueLowRangeSlider);

    theHueLowRangeValueLabel = new QLabel(ossimQtHsiRemapperDialog);
    theHueLowRangeValueLabel->setObjectName(QString::fromUtf8("theHueLowRangeValueLabel"));
    theHueLowRangeValueLabel->setMinimumSize(QSize(40, 0));
    theHueLowRangeValueLabel->setAlignment(Qt::AlignVCenter);

    hboxLayout3->addWidget(theHueLowRangeValueLabel);


    vboxLayout1->addLayout(hboxLayout3);

    hboxLayout4 = new Q3HBoxLayout();
    hboxLayout4->setSpacing(6);
    hboxLayout4->setMargin(0);
    hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
    theHueHighRangeLabel = new QLabel(ossimQtHsiRemapperDialog);
    theHueHighRangeLabel->setObjectName(QString::fromUtf8("theHueHighRangeLabel"));
    theHueHighRangeLabel->setMinimumSize(QSize(110, 0));
    theHueHighRangeLabel->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

    hboxLayout4->addWidget(theHueHighRangeLabel);

    theHueHighRangeSlider = new QSlider(ossimQtHsiRemapperDialog);
    theHueHighRangeSlider->setObjectName(QString::fromUtf8("theHueHighRangeSlider"));
    theHueHighRangeSlider->setMinimumSize(QSize(290, 0));
    theHueHighRangeSlider->setMinimum(-30);
    theHueHighRangeSlider->setMaximum(30);
    theHueHighRangeSlider->setPageStep(1);
    theHueHighRangeSlider->setTracking(false);
    theHueHighRangeSlider->setOrientation(Qt::Horizontal);
    theHueHighRangeSlider->setTickInterval(10);

    hboxLayout4->addWidget(theHueHighRangeSlider);

    theHueHighRangeValueLabel = new QLabel(ossimQtHsiRemapperDialog);
    theHueHighRangeValueLabel->setObjectName(QString::fromUtf8("theHueHighRangeValueLabel"));
    theHueHighRangeValueLabel->setMinimumSize(QSize(40, 0));
    theHueHighRangeValueLabel->setAlignment(Qt::AlignVCenter);

    hboxLayout4->addWidget(theHueHighRangeValueLabel);


    vboxLayout1->addLayout(hboxLayout4);

    hboxLayout5 = new Q3HBoxLayout();
    hboxLayout5->setSpacing(6);
    hboxLayout5->setMargin(0);
    hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
    theHueBlendRangeLabel = new QLabel(ossimQtHsiRemapperDialog);
    theHueBlendRangeLabel->setObjectName(QString::fromUtf8("theHueBlendRangeLabel"));
    theHueBlendRangeLabel->setMinimumSize(QSize(110, 0));

    hboxLayout5->addWidget(theHueBlendRangeLabel);

    theHueBlendRangeSlider = new QSlider(ossimQtHsiRemapperDialog);
    theHueBlendRangeSlider->setObjectName(QString::fromUtf8("theHueBlendRangeSlider"));
    theHueBlendRangeSlider->setMinimumSize(QSize(290, 0));
    theHueBlendRangeSlider->setMaximum(30);
    theHueBlendRangeSlider->setPageStep(1);
    theHueBlendRangeSlider->setValue(15);
    theHueBlendRangeSlider->setTracking(false);
    theHueBlendRangeSlider->setOrientation(Qt::Horizontal);
    theHueBlendRangeSlider->setTickInterval(5);

    hboxLayout5->addWidget(theHueBlendRangeSlider);

    theHueBlendRangeValueLabel = new QLabel(ossimQtHsiRemapperDialog);
    theHueBlendRangeValueLabel->setObjectName(QString::fromUtf8("theHueBlendRangeValueLabel"));
    theHueBlendRangeValueLabel->setMinimumSize(QSize(40, 0));

    hboxLayout5->addWidget(theHueBlendRangeValueLabel);


    vboxLayout1->addLayout(hboxLayout5);

    hboxLayout6 = new Q3HBoxLayout();
    hboxLayout6->setSpacing(6);
    hboxLayout6->setMargin(0);
    hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
    theSaturationOffsetLabel = new QLabel(ossimQtHsiRemapperDialog);
    theSaturationOffsetLabel->setObjectName(QString::fromUtf8("theSaturationOffsetLabel"));
    theSaturationOffsetLabel->setMinimumSize(QSize(110, 0));
    theSaturationOffsetLabel->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

    hboxLayout6->addWidget(theSaturationOffsetLabel);

    theSaturationOffsetSlider = new QSlider(ossimQtHsiRemapperDialog);
    theSaturationOffsetSlider->setObjectName(QString::fromUtf8("theSaturationOffsetSlider"));
    theSaturationOffsetSlider->setMinimumSize(QSize(290, 0));
    theSaturationOffsetSlider->setMinimum(-200);
    theSaturationOffsetSlider->setMaximum(200);
    theSaturationOffsetSlider->setPageStep(1);
    theSaturationOffsetSlider->setTracking(false);
    theSaturationOffsetSlider->setOrientation(Qt::Horizontal);
    theSaturationOffsetSlider->setTickInterval(40);

    hboxLayout6->addWidget(theSaturationOffsetSlider);

    theSaturationOffsetValueLabel = new QLabel(ossimQtHsiRemapperDialog);
    theSaturationOffsetValueLabel->setObjectName(QString::fromUtf8("theSaturationOffsetValueLabel"));
    theSaturationOffsetValueLabel->setMinimumSize(QSize(40, 0));

    hboxLayout6->addWidget(theSaturationOffsetValueLabel);


    vboxLayout1->addLayout(hboxLayout6);

    hboxLayout7 = new Q3HBoxLayout();
    hboxLayout7->setSpacing(6);
    hboxLayout7->setMargin(0);
    hboxLayout7->setObjectName(QString::fromUtf8("hboxLayout7"));
    theIntensityOffsetLabel = new QLabel(ossimQtHsiRemapperDialog);
    theIntensityOffsetLabel->setObjectName(QString::fromUtf8("theIntensityOffsetLabel"));
    theIntensityOffsetLabel->setMinimumSize(QSize(110, 0));
    theIntensityOffsetLabel->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

    hboxLayout7->addWidget(theIntensityOffsetLabel);

    theIntensityOffsetSlider = new QSlider(ossimQtHsiRemapperDialog);
    theIntensityOffsetSlider->setObjectName(QString::fromUtf8("theIntensityOffsetSlider"));
    theIntensityOffsetSlider->setMinimumSize(QSize(290, 0));
    theIntensityOffsetSlider->setMinimum(-200);
    theIntensityOffsetSlider->setMaximum(200);
    theIntensityOffsetSlider->setPageStep(1);
    theIntensityOffsetSlider->setTracking(false);
    theIntensityOffsetSlider->setOrientation(Qt::Horizontal);
    theIntensityOffsetSlider->setTickInterval(40);

    hboxLayout7->addWidget(theIntensityOffsetSlider);

    theIntensityOffsetValueLabel = new QLabel(ossimQtHsiRemapperDialog);
    theIntensityOffsetValueLabel->setObjectName(QString::fromUtf8("theIntensityOffsetValueLabel"));
    theIntensityOffsetValueLabel->setMinimumSize(QSize(40, 0));

    hboxLayout7->addWidget(theIntensityOffsetValueLabel);


    vboxLayout1->addLayout(hboxLayout7);

    hboxLayout8 = new Q3HBoxLayout();
    hboxLayout8->setSpacing(6);
    hboxLayout8->setMargin(0);
    hboxLayout8->setObjectName(QString::fromUtf8("hboxLayout8"));
    theLowIntensityClipLabel = new QLabel(ossimQtHsiRemapperDialog);
    theLowIntensityClipLabel->setObjectName(QString::fromUtf8("theLowIntensityClipLabel"));
    theLowIntensityClipLabel->setMinimumSize(QSize(110, 0));
    theLowIntensityClipLabel->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

    hboxLayout8->addWidget(theLowIntensityClipLabel);

    theLowIntensityClipSlider = new QSlider(ossimQtHsiRemapperDialog);
    theLowIntensityClipSlider->setObjectName(QString::fromUtf8("theLowIntensityClipSlider"));
    theLowIntensityClipSlider->setMinimumSize(QSize(290, 0));
    theLowIntensityClipSlider->setMaximum(400);
    theLowIntensityClipSlider->setPageStep(1);
    theLowIntensityClipSlider->setTracking(false);
    theLowIntensityClipSlider->setOrientation(Qt::Horizontal);
    theLowIntensityClipSlider->setTickInterval(40);

    hboxLayout8->addWidget(theLowIntensityClipSlider);

    theLowIntensityClipValueLabel = new QLabel(ossimQtHsiRemapperDialog);
    theLowIntensityClipValueLabel->setObjectName(QString::fromUtf8("theLowIntensityClipValueLabel"));
    theLowIntensityClipValueLabel->setMinimumSize(QSize(40, 0));

    hboxLayout8->addWidget(theLowIntensityClipValueLabel);


    vboxLayout1->addLayout(hboxLayout8);

    hboxLayout9 = new Q3HBoxLayout();
    hboxLayout9->setSpacing(6);
    hboxLayout9->setMargin(0);
    hboxLayout9->setObjectName(QString::fromUtf8("hboxLayout9"));
    theHighIntensityClipLabel = new QLabel(ossimQtHsiRemapperDialog);
    theHighIntensityClipLabel->setObjectName(QString::fromUtf8("theHighIntensityClipLabel"));
    theHighIntensityClipLabel->setMinimumSize(QSize(110, 0));
    theHighIntensityClipLabel->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

    hboxLayout9->addWidget(theHighIntensityClipLabel);

    theHighIntensityClipSlider = new QSlider(ossimQtHsiRemapperDialog);
    theHighIntensityClipSlider->setObjectName(QString::fromUtf8("theHighIntensityClipSlider"));
    theHighIntensityClipSlider->setMinimumSize(QSize(290, 0));
    theHighIntensityClipSlider->setMaximum(400);
    theHighIntensityClipSlider->setPageStep(1);
    theHighIntensityClipSlider->setValue(400);
    theHighIntensityClipSlider->setTracking(false);
    theHighIntensityClipSlider->setOrientation(Qt::Horizontal);
    theHighIntensityClipSlider->setTickInterval(40);

    hboxLayout9->addWidget(theHighIntensityClipSlider);

    theHighIntensityClipValueLabel = new QLabel(ossimQtHsiRemapperDialog);
    theHighIntensityClipValueLabel->setObjectName(QString::fromUtf8("theHighIntensityClipValueLabel"));
    theHighIntensityClipValueLabel->setMinimumSize(QSize(40, 0));

    hboxLayout9->addWidget(theHighIntensityClipValueLabel);


    vboxLayout1->addLayout(hboxLayout9);

    hboxLayout10 = new Q3HBoxLayout();
    hboxLayout10->setSpacing(6);
    hboxLayout10->setMargin(0);
    hboxLayout10->setObjectName(QString::fromUtf8("hboxLayout10"));
    theWhiteObjectClipLabel = new QLabel(ossimQtHsiRemapperDialog);
    theWhiteObjectClipLabel->setObjectName(QString::fromUtf8("theWhiteObjectClipLabel"));
    theWhiteObjectClipLabel->setMinimumSize(QSize(110, 0));

    hboxLayout10->addWidget(theWhiteObjectClipLabel);

    theWhiteObjectClipSlider = new QSlider(ossimQtHsiRemapperDialog);
    theWhiteObjectClipSlider->setObjectName(QString::fromUtf8("theWhiteObjectClipSlider"));
    theWhiteObjectClipSlider->setMinimumSize(QSize(290, 0));
    theWhiteObjectClipSlider->setMinimum(320);
    theWhiteObjectClipSlider->setMaximum(400);
    theWhiteObjectClipSlider->setPageStep(1);
    theWhiteObjectClipSlider->setValue(400);
    theWhiteObjectClipSlider->setTracking(false);
    theWhiteObjectClipSlider->setOrientation(Qt::Horizontal);
    theWhiteObjectClipSlider->setTickInterval(8);

    hboxLayout10->addWidget(theWhiteObjectClipSlider);

    theWhiteObjectClipValueLabel = new QLabel(ossimQtHsiRemapperDialog);
    theWhiteObjectClipValueLabel->setObjectName(QString::fromUtf8("theWhiteObjectClipValueLabel"));
    theWhiteObjectClipValueLabel->setMinimumSize(QSize(40, 0));

    hboxLayout10->addWidget(theWhiteObjectClipValueLabel);


    vboxLayout1->addLayout(hboxLayout10);


    vboxLayout->addLayout(vboxLayout1);

    line1 = new Q3Frame(ossimQtHsiRemapperDialog);
    line1->setObjectName(QString::fromUtf8("line1"));
    line1->setFrameShape(Q3Frame::HLine);
    line1->setFrameShadow(Q3Frame::Sunken);

    vboxLayout->addWidget(line1);

    hboxLayout11 = new Q3HBoxLayout();
    hboxLayout11->setSpacing(6);
    hboxLayout11->setMargin(0);
    hboxLayout11->setObjectName(QString::fromUtf8("hboxLayout11"));
    theEnableButton = new QCheckBox(ossimQtHsiRemapperDialog);
    theEnableButton->setObjectName(QString::fromUtf8("theEnableButton"));

    hboxLayout11->addWidget(theEnableButton);

    theResetGroupButton = new QPushButton(ossimQtHsiRemapperDialog);
    theResetGroupButton->setObjectName(QString::fromUtf8("theResetGroupButton"));

    hboxLayout11->addWidget(theResetGroupButton);

    theResetAllGroup = new QPushButton(ossimQtHsiRemapperDialog);
    theResetAllGroup->setObjectName(QString::fromUtf8("theResetAllGroup"));

    hboxLayout11->addWidget(theResetAllGroup);

    theCloseButton = new QPushButton(ossimQtHsiRemapperDialog);
    theCloseButton->setObjectName(QString::fromUtf8("theCloseButton"));

    hboxLayout11->addWidget(theCloseButton);


    vboxLayout->addLayout(hboxLayout11);

    retranslateUi(ossimQtHsiRemapperDialog);

    QMetaObject::connectSlotsByName(ossimQtHsiRemapperDialog);
    } // setupUi

    void retranslateUi(QDialog *ossimQtHsiRemapperDialog)
    {
    ossimQtHsiRemapperDialog->setWindowTitle(QApplication::translate("ossimQtHsiRemapperDialog", "HSI Remapper Property Editor", 0, QApplication::UnicodeUTF8));
    theColorRangeButtonGroup->setTitle(QApplication::translate("ossimQtHsiRemapperDialog", "", 0, QApplication::UnicodeUTF8));
    theRedButton->setText(QApplication::translate("ossimQtHsiRemapperDialog", "red", 0, QApplication::UnicodeUTF8));
    theYellowButton->setText(QApplication::translate("ossimQtHsiRemapperDialog", "yellow", 0, QApplication::UnicodeUTF8));
    theGreenButton->setText(QApplication::translate("ossimQtHsiRemapperDialog", "green", 0, QApplication::UnicodeUTF8));
    theCyanButton->setText(QApplication::translate("ossimQtHsiRemapperDialog", "cyan", 0, QApplication::UnicodeUTF8));
    theBlueButton->setText(QApplication::translate("ossimQtHsiRemapperDialog", "blue", 0, QApplication::UnicodeUTF8));
    theMagentaButton->setText(QApplication::translate("ossimQtHsiRemapperDialog", "magenta", 0, QApplication::UnicodeUTF8));
    theAllButton->setText(QApplication::translate("ossimQtHsiRemapperDialog", "all", 0, QApplication::UnicodeUTF8));
    theHueOffsetLabel->setText(QApplication::translate("ossimQtHsiRemapperDialog", "hue offset:", 0, QApplication::UnicodeUTF8));
    theHueOffsetValueLabel->setText(QApplication::translate("ossimQtHsiRemapperDialog", "0", 0, QApplication::UnicodeUTF8));
    theHueLowRangeLabel->setText(QApplication::translate("ossimQtHsiRemapperDialog", "hue low range:", 0, QApplication::UnicodeUTF8));
    theHueLowRangeValueLabel->setText(QApplication::translate("ossimQtHsiRemapperDialog", "0", 0, QApplication::UnicodeUTF8));
    theHueHighRangeLabel->setText(QApplication::translate("ossimQtHsiRemapperDialog", "hue high range:", 0, QApplication::UnicodeUTF8));
    theHueHighRangeValueLabel->setText(QApplication::translate("ossimQtHsiRemapperDialog", "0", 0, QApplication::UnicodeUTF8));
    theHueBlendRangeLabel->setText(QApplication::translate("ossimQtHsiRemapperDialog", "hue blend range:", 0, QApplication::UnicodeUTF8));
    theHueBlendRangeValueLabel->setText(QApplication::translate("ossimQtHsiRemapperDialog", "0", 0, QApplication::UnicodeUTF8));
    theSaturationOffsetLabel->setText(QApplication::translate("ossimQtHsiRemapperDialog", "saturation offset:", 0, QApplication::UnicodeUTF8));
    theSaturationOffsetValueLabel->setText(QApplication::translate("ossimQtHsiRemapperDialog", "0", 0, QApplication::UnicodeUTF8));
    theIntensityOffsetLabel->setText(QApplication::translate("ossimQtHsiRemapperDialog", "intensity offset", 0, QApplication::UnicodeUTF8));
    theIntensityOffsetValueLabel->setText(QApplication::translate("ossimQtHsiRemapperDialog", "0", 0, QApplication::UnicodeUTF8));
    theLowIntensityClipLabel->setText(QApplication::translate("ossimQtHsiRemapperDialog", "low intensity clip:", 0, QApplication::UnicodeUTF8));
    theLowIntensityClipValueLabel->setText(QApplication::translate("ossimQtHsiRemapperDialog", "0", 0, QApplication::UnicodeUTF8));
    theHighIntensityClipLabel->setText(QApplication::translate("ossimQtHsiRemapperDialog", "high intensity clip:", 0, QApplication::UnicodeUTF8));
    theHighIntensityClipValueLabel->setText(QApplication::translate("ossimQtHsiRemapperDialog", "0", 0, QApplication::UnicodeUTF8));
    theWhiteObjectClipLabel->setText(QApplication::translate("ossimQtHsiRemapperDialog", "white object clip", 0, QApplication::UnicodeUTF8));
    theWhiteObjectClipValueLabel->setText(QApplication::translate("ossimQtHsiRemapperDialog", "0", 0, QApplication::UnicodeUTF8));
    theEnableButton->setText(QApplication::translate("ossimQtHsiRemapperDialog", "enable", 0, QApplication::UnicodeUTF8));
    theResetGroupButton->setText(QApplication::translate("ossimQtHsiRemapperDialog", "reset group", 0, QApplication::UnicodeUTF8));
    theResetAllGroup->setText(QApplication::translate("ossimQtHsiRemapperDialog", "reset all", 0, QApplication::UnicodeUTF8));
    theCloseButton->setText(QApplication::translate("ossimQtHsiRemapperDialog", "close", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ossimQtHsiRemapperDialog);
    } // retranslateUi

};

namespace Ui {
    class ossimQtHsiRemapperDialog: public Ui_ossimQtHsiRemapperDialog {};
} // namespace Ui

class ossimQtHsiRemapperDialog : public QDialog, public Ui::ossimQtHsiRemapperDialog
{
    Q_OBJECT

public:
    ossimQtHsiRemapperDialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0);
    ~ossimQtHsiRemapperDialog();

    virtual void setHsiRemapper( ossimHsiRemapper * remapper );

public slots:
    virtual void redButtonClicked();
    virtual void yellowButtonClicked();
    virtual void greenButtonClicked();
    virtual void cyanButtonClicked();
    virtual void blueButtonClicked();
    virtual void magentaButtonClicked();
    virtual void allButtonClicked();
    virtual void hueOffsetValueChanged( int value );
    virtual void hueLowRangeValueChanged( int value );
    virtual void hueHighRangeValueChanged( int value );
    virtual void saturationOffsetValueChanged( int value );
    virtual void intensityOffsetValueChanged( int value );
    virtual void lowIntensityClipValueChanged( int value );
    virtual void highIntensityClipValueChanged( int value );
    virtual void enableButtonClicked();
    virtual void resetGroupButtonClicked();
    virtual void resetAllButtonClicked();
    virtual void closeButtonClicked();
    virtual void hueBlendRangeValueChanged( int );
    virtual void whiteObjectClipValueChanged( int );

protected slots:
    virtual void languageChange();

private:
    ossimQtHsiRemapperController* theController;

    virtual void init();
    virtual void destroy();

};

#endif // OSSIMQTHSIREMAPPERDIALOG_H
