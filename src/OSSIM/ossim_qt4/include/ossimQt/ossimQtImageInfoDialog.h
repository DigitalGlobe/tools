#ifndef OSSIMQTIMAGEINFODIALOG_H
#define OSSIMQTIMAGEINFODIALOG_H

#include <QtCore/QVariant>
//Added by qt3to4:
#include <Qt3Support/Q3HBoxLayout>
#include <QtGui/QLabel>
#include <Qt3Support/Q3VBoxLayout>

class ossimQtImageInfoController;
class ossimQtScrollingImageWidget;

#include <Qt3Support/Q3ButtonGroup>
#include <Qt3Support/Q3GroupBox>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <Qt3Support/Q3MimeSourceFactory>

class Ui_ossimQtImageInfoDialog
{
public:
    Q3VBoxLayout *vboxLayout;
    Q3ButtonGroup *theImageLayerButtonGroup;
    Q3HBoxLayout *hboxLayout;
    QComboBox *theLayerComboBox;
    QTabWidget *theImageInfoTabWidget;
    QWidget *theImageGeomPage;
    Q3VBoxLayout *vboxLayout1;
    Q3HBoxLayout *hboxLayout1;
    Q3GroupBox *theImageTypeGroupBox;
    Q3HBoxLayout *hboxLayout2;
    QLineEdit *theImageTypeLineEdit;
    Q3GroupBox *theRadiometryGroupBox;
    Q3HBoxLayout *hboxLayout3;
    QLineEdit *theRadiometryLineEdit;
    Q3GroupBox *theByteOrderGroupBox;
    Q3HBoxLayout *hboxLayout4;
    QLineEdit *theByteOrderLineEdit;
    Q3HBoxLayout *hboxLayout5;
    Q3GroupBox *theLinesGroupBox;
    Q3HBoxLayout *hboxLayout6;
    QLineEdit *theLinesLineEdit;
    Q3GroupBox *theSamplesGroupBox;
    Q3HBoxLayout *hboxLayout7;
    QLineEdit *theSamplesLineEdit;
    Q3GroupBox *theBandsGroupBox;
    Q3HBoxLayout *hboxLayout8;
    QLineEdit *theBandsLineEdit;
    Q3ButtonGroup *buttonGroup13;
    Q3HBoxLayout *hboxLayout9;
    QLineEdit *theRLevelLineEdit;
    Q3GroupBox *theBandValueGroupBox;
    Q3HBoxLayout *hboxLayout10;
    Q3GroupBox *theSelectedBandGroupBox;
    Q3HBoxLayout *hboxLayout11;
    QSpinBox *theBandSpinBox;
    Q3GroupBox *theMinGroupBox;
    Q3HBoxLayout *hboxLayout12;
    QLineEdit *theMinLineEdit;
    Q3GroupBox *theMaxGroupBox;
    Q3HBoxLayout *hboxLayout13;
    QLineEdit *theMaxLineEdit;
    Q3GroupBox *theNullGroupBox;
    Q3HBoxLayout *hboxLayout14;
    QLineEdit *theNullLineEdit;
    Q3HBoxLayout *hboxLayout15;
    QSpacerItem *spacerItem;
    QPushButton *theScanForMinMaxPushButton;
    QSpacerItem *spacerItem1;
    QSpacerItem *spacerItem2;
    QWidget *theProjectionPage;
    Q3VBoxLayout *vboxLayout2;
    Q3HBoxLayout *hboxLayout16;
    QLabel *theProjectionTextLable;
    QComboBox *theProjectionComboBox;
    Q3HBoxLayout *hboxLayout17;
    QLabel *theDatumTextLabel;
    QComboBox *theDatumComboBox;
    Q3HBoxLayout *hboxLayout18;
    QLabel *theUnitsTextLabel;
    QComboBox *theUnitsComboBox;
    Q3HBoxLayout *hboxLayout19;
    QLabel *thePixelTypeTextLabel;
    QComboBox *thePixelTypeComboBox;
    Q3HBoxLayout *hboxLayout20;
    QLabel *theZoneTextLabel;
    QComboBox *theZoneComboBox;
    Q3HBoxLayout *hboxLayout21;
    QLabel *theHemisphereTextLabel;
    QComboBox *theHemisphereComboBox;
    Q3HBoxLayout *hboxLayout22;
    QLabel *theOriginLatitudeTextLabel;
    QLineEdit *theOriginLatitudeLineEdit;
    Q3HBoxLayout *hboxLayout23;
    QLabel *theCentralMeridianTextLabel;
    QLineEdit *theCentralMeridianLineEdit;
    Q3HBoxLayout *hboxLayout24;
    QLabel *theFalseEastingTextLabel;
    QLineEdit *theFalseEastingLineEdit;
    Q3HBoxLayout *hboxLayout25;
    QLabel *theFalseNorthingTextLabel;
    QLineEdit *theFalseNorthingLineEdit;
    Q3HBoxLayout *hboxLayout26;
    QLabel *theTiePointX_TextLabel;
    QLineEdit *theTiePointX_LineEdit;
    Q3HBoxLayout *hboxLayout27;
    QLabel *theTiePointY_TextLabel;
    QLineEdit *theTiePointY_LineEdit;
    Q3HBoxLayout *hboxLayout28;
    QLabel *theScaleX_TextLabel;
    QLineEdit *theScaleX_LineEdit;
    Q3HBoxLayout *hboxLayout29;
    QLabel *theScaleY_TextLabel;
    QLineEdit *theScaleY_LineEdit;
    Q3HBoxLayout *hboxLayout30;
    QLabel *theScaleFactorTextLabel;
    QLineEdit *theScaleFactorLineEdit;
    Q3HBoxLayout *hboxLayout31;
    QLabel *theParallelOneTextLabel;
    QLineEdit *theParallelOneLineEdit;
    Q3HBoxLayout *hboxLayout32;
    QLabel *theParallelTwoTextLabel;
    QLineEdit *theParallelTwoLineEdit;
    Q3HBoxLayout *hboxLayout33;
    QPushButton *theSavePushButton;
    QSpacerItem *spacerItem3;
    QPushButton *theResetPushButton;
    QWidget *theValidVerticesPage;
    Q3VBoxLayout *vboxLayout3;
    Q3GroupBox *theValidVeticesGroupBox;
    Q3VBoxLayout *vboxLayout4;
    Q3HBoxLayout *hboxLayout34;
    Q3GroupBox *theVertex0GroupBox;
    Q3HBoxLayout *hboxLayout35;
    Q3HBoxLayout *hboxLayout36;
    Q3GroupBox *theVertex0LineGroupBox;
    Q3HBoxLayout *hboxLayout37;
    QLineEdit *theVertex0LineLineEdit;
    Q3GroupBox *theVertex0SampleGroupBox;
    Q3HBoxLayout *hboxLayout38;
    QLineEdit *theVertex0SampleLineEdit;
    Q3GroupBox *theVertex1GroupBox;
    Q3HBoxLayout *hboxLayout39;
    Q3HBoxLayout *hboxLayout40;
    Q3GroupBox *theVertex1LineGroupBox;
    Q3HBoxLayout *hboxLayout41;
    QLineEdit *theVertex1LineLineEdit;
    Q3GroupBox *theVertex1SampleGroupBox;
    Q3HBoxLayout *hboxLayout42;
    QLineEdit *theVertex1SampleLineEdit;
    Q3HBoxLayout *hboxLayout43;
    Q3GroupBox *theVertex2GroupBox;
    Q3HBoxLayout *hboxLayout44;
    Q3HBoxLayout *hboxLayout45;
    Q3GroupBox *theVertex2LineGroupBox;
    Q3HBoxLayout *hboxLayout46;
    QLineEdit *theVertex2LineLineEdit;
    Q3GroupBox *theVertex2SampleGroupBox;
    Q3HBoxLayout *hboxLayout47;
    QLineEdit *theVertex2SampleLineEdit;
    Q3GroupBox *theVertex3GroupBox;
    Q3HBoxLayout *hboxLayout48;
    Q3HBoxLayout *hboxLayout49;
    Q3GroupBox *theVertex3LineGroupBox;
    Q3HBoxLayout *hboxLayout50;
    QLineEdit *theVertex0LineLineEdit_4;
    Q3GroupBox *theVertex3SampleGroupBox;
    Q3HBoxLayout *hboxLayout51;
    QLineEdit *theVertex0SampleLineEdit_4;
    Q3HBoxLayout *hboxLayout52;
    QSpacerItem *spacerItem4;
    QPushButton *theScanForValidVerticesPushButton;
    QSpacerItem *spacerItem5;
    QSpacerItem *spacerItem6;
    Q3HBoxLayout *hboxLayout53;
    QPushButton *theHelpPushButton;
    QSpacerItem *spacerItem7;
    QPushButton *theClosePushButton;

    void setupUi(QDialog *ossimQtImageInfoDialog)
    {
    ossimQtImageInfoDialog->setObjectName(QString::fromUtf8("ossimQtImageInfoDialog"));
    ossimQtImageInfoDialog->resize(QSize(466, 750).expandedTo(ossimQtImageInfoDialog->minimumSizeHint()));
    QFont font;
    font.setPointSize(9);
    font.setBold(false);
    font.setItalic(false);
    font.setUnderline(false);
    font.setStrikeOut(false);
    ossimQtImageInfoDialog->setFont(font);
    ossimQtImageInfoDialog->setSizeGripEnabled(true);
    vboxLayout = new Q3VBoxLayout(ossimQtImageInfoDialog);
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(11);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    theImageLayerButtonGroup = new Q3ButtonGroup(ossimQtImageInfoDialog);
    theImageLayerButtonGroup->setObjectName(QString::fromUtf8("theImageLayerButtonGroup"));
    theImageLayerButtonGroup->setAlignment(Qt::AlignHCenter);
    theImageLayerButtonGroup->setColumnLayout(0, Qt::Vertical);
    theImageLayerButtonGroup->layout()->setSpacing(6);
    theImageLayerButtonGroup->layout()->setMargin(11);
    hboxLayout = new Q3HBoxLayout(theImageLayerButtonGroup->layout());
    hboxLayout->setAlignment(Qt::AlignTop);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    theLayerComboBox = new QComboBox(theImageLayerButtonGroup);
    theLayerComboBox->setObjectName(QString::fromUtf8("theLayerComboBox"));

    hboxLayout->addWidget(theLayerComboBox);


    vboxLayout->addWidget(theImageLayerButtonGroup);

    theImageInfoTabWidget = new QTabWidget(ossimQtImageInfoDialog);
    theImageInfoTabWidget->setObjectName(QString::fromUtf8("theImageInfoTabWidget"));
    theImageInfoTabWidget->setEnabled(true);
    theImageGeomPage = new QWidget();
    theImageGeomPage->setObjectName(QString::fromUtf8("theImageGeomPage"));
    vboxLayout1 = new Q3VBoxLayout(theImageGeomPage);
    vboxLayout1->setSpacing(6);
    vboxLayout1->setMargin(0);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    hboxLayout1 = new Q3HBoxLayout();
    hboxLayout1->setSpacing(6);
    hboxLayout1->setMargin(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    theImageTypeGroupBox = new Q3GroupBox(theImageGeomPage);
    theImageTypeGroupBox->setObjectName(QString::fromUtf8("theImageTypeGroupBox"));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(1));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(theImageTypeGroupBox->sizePolicy().hasHeightForWidth());
    theImageTypeGroupBox->setSizePolicy(sizePolicy);
    theImageTypeGroupBox->setAlignment(Qt::AlignHCenter);
    theImageTypeGroupBox->setColumnLayout(0, Qt::Vertical);
    theImageTypeGroupBox->layout()->setSpacing(6);
    theImageTypeGroupBox->layout()->setMargin(11);
    hboxLayout2 = new Q3HBoxLayout(theImageTypeGroupBox->layout());
    hboxLayout2->setAlignment(Qt::AlignTop);
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    theImageTypeLineEdit = new QLineEdit(theImageTypeGroupBox);
    theImageTypeLineEdit->setObjectName(QString::fromUtf8("theImageTypeLineEdit"));
    theImageTypeLineEdit->setMinimumSize(QSize(96, 0));

    hboxLayout2->addWidget(theImageTypeLineEdit);


    hboxLayout1->addWidget(theImageTypeGroupBox);

    theRadiometryGroupBox = new Q3GroupBox(theImageGeomPage);
    theRadiometryGroupBox->setObjectName(QString::fromUtf8("theRadiometryGroupBox"));
    QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(1));
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(theRadiometryGroupBox->sizePolicy().hasHeightForWidth());
    theRadiometryGroupBox->setSizePolicy(sizePolicy1);
    theRadiometryGroupBox->setAlignment(Qt::AlignHCenter);
    theRadiometryGroupBox->setColumnLayout(0, Qt::Vertical);
    theRadiometryGroupBox->layout()->setSpacing(6);
    theRadiometryGroupBox->layout()->setMargin(11);
    hboxLayout3 = new Q3HBoxLayout(theRadiometryGroupBox->layout());
    hboxLayout3->setAlignment(Qt::AlignTop);
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
    theRadiometryLineEdit = new QLineEdit(theRadiometryGroupBox);
    theRadiometryLineEdit->setObjectName(QString::fromUtf8("theRadiometryLineEdit"));
    theRadiometryLineEdit->setMinimumSize(QSize(96, 0));

    hboxLayout3->addWidget(theRadiometryLineEdit);


    hboxLayout1->addWidget(theRadiometryGroupBox);

    theByteOrderGroupBox = new Q3GroupBox(theImageGeomPage);
    theByteOrderGroupBox->setObjectName(QString::fromUtf8("theByteOrderGroupBox"));
    QSizePolicy sizePolicy2(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(1));
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(theByteOrderGroupBox->sizePolicy().hasHeightForWidth());
    theByteOrderGroupBox->setSizePolicy(sizePolicy2);
    theByteOrderGroupBox->setMinimumSize(QSize(96, 0));
    theByteOrderGroupBox->setAlignment(Qt::AlignHCenter);
    theByteOrderGroupBox->setColumnLayout(0, Qt::Vertical);
    theByteOrderGroupBox->layout()->setSpacing(6);
    theByteOrderGroupBox->layout()->setMargin(11);
    hboxLayout4 = new Q3HBoxLayout(theByteOrderGroupBox->layout());
    hboxLayout4->setAlignment(Qt::AlignTop);
    hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
    theByteOrderLineEdit = new QLineEdit(theByteOrderGroupBox);
    theByteOrderLineEdit->setObjectName(QString::fromUtf8("theByteOrderLineEdit"));
    theByteOrderLineEdit->setMinimumSize(QSize(96, 0));

    hboxLayout4->addWidget(theByteOrderLineEdit);


    hboxLayout1->addWidget(theByteOrderGroupBox);


    vboxLayout1->addLayout(hboxLayout1);

    hboxLayout5 = new Q3HBoxLayout();
    hboxLayout5->setSpacing(6);
    hboxLayout5->setMargin(0);
    hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
    theLinesGroupBox = new Q3GroupBox(theImageGeomPage);
    theLinesGroupBox->setObjectName(QString::fromUtf8("theLinesGroupBox"));
    QSizePolicy sizePolicy3(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(1));
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(theLinesGroupBox->sizePolicy().hasHeightForWidth());
    theLinesGroupBox->setSizePolicy(sizePolicy3);
    theLinesGroupBox->setAlignment(Qt::AlignHCenter);
    theLinesGroupBox->setColumnLayout(0, Qt::Vertical);
    theLinesGroupBox->layout()->setSpacing(6);
    theLinesGroupBox->layout()->setMargin(11);
    hboxLayout6 = new Q3HBoxLayout(theLinesGroupBox->layout());
    hboxLayout6->setAlignment(Qt::AlignTop);
    hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
    theLinesLineEdit = new QLineEdit(theLinesGroupBox);
    theLinesLineEdit->setObjectName(QString::fromUtf8("theLinesLineEdit"));
    theLinesLineEdit->setMinimumSize(QSize(80, 0));

    hboxLayout6->addWidget(theLinesLineEdit);


    hboxLayout5->addWidget(theLinesGroupBox);

    theSamplesGroupBox = new Q3GroupBox(theImageGeomPage);
    theSamplesGroupBox->setObjectName(QString::fromUtf8("theSamplesGroupBox"));
    QSizePolicy sizePolicy4(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(1));
    sizePolicy4.setHorizontalStretch(0);
    sizePolicy4.setVerticalStretch(0);
    sizePolicy4.setHeightForWidth(theSamplesGroupBox->sizePolicy().hasHeightForWidth());
    theSamplesGroupBox->setSizePolicy(sizePolicy4);
    theSamplesGroupBox->setAlignment(Qt::AlignHCenter);
    theSamplesGroupBox->setColumnLayout(0, Qt::Vertical);
    theSamplesGroupBox->layout()->setSpacing(6);
    theSamplesGroupBox->layout()->setMargin(11);
    hboxLayout7 = new Q3HBoxLayout(theSamplesGroupBox->layout());
    hboxLayout7->setAlignment(Qt::AlignTop);
    hboxLayout7->setObjectName(QString::fromUtf8("hboxLayout7"));
    theSamplesLineEdit = new QLineEdit(theSamplesGroupBox);
    theSamplesLineEdit->setObjectName(QString::fromUtf8("theSamplesLineEdit"));
    theSamplesLineEdit->setMinimumSize(QSize(80, 0));

    hboxLayout7->addWidget(theSamplesLineEdit);


    hboxLayout5->addWidget(theSamplesGroupBox);

    theBandsGroupBox = new Q3GroupBox(theImageGeomPage);
    theBandsGroupBox->setObjectName(QString::fromUtf8("theBandsGroupBox"));
    QSizePolicy sizePolicy5(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(1));
    sizePolicy5.setHorizontalStretch(0);
    sizePolicy5.setVerticalStretch(0);
    sizePolicy5.setHeightForWidth(theBandsGroupBox->sizePolicy().hasHeightForWidth());
    theBandsGroupBox->setSizePolicy(sizePolicy5);
    theBandsGroupBox->setAlignment(Qt::AlignHCenter);
    theBandsGroupBox->setColumnLayout(0, Qt::Vertical);
    theBandsGroupBox->layout()->setSpacing(6);
    theBandsGroupBox->layout()->setMargin(11);
    hboxLayout8 = new Q3HBoxLayout(theBandsGroupBox->layout());
    hboxLayout8->setAlignment(Qt::AlignTop);
    hboxLayout8->setObjectName(QString::fromUtf8("hboxLayout8"));
    theBandsLineEdit = new QLineEdit(theBandsGroupBox);
    theBandsLineEdit->setObjectName(QString::fromUtf8("theBandsLineEdit"));
    theBandsLineEdit->setMinimumSize(QSize(50, 0));

    hboxLayout8->addWidget(theBandsLineEdit);


    hboxLayout5->addWidget(theBandsGroupBox);

    buttonGroup13 = new Q3ButtonGroup(theImageGeomPage);
    buttonGroup13->setObjectName(QString::fromUtf8("buttonGroup13"));
    QSizePolicy sizePolicy6(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(5));
    sizePolicy6.setHorizontalStretch(0);
    sizePolicy6.setVerticalStretch(0);
    sizePolicy6.setHeightForWidth(buttonGroup13->sizePolicy().hasHeightForWidth());
    buttonGroup13->setSizePolicy(sizePolicy6);
    buttonGroup13->setAlignment(Qt::AlignHCenter);
    buttonGroup13->setColumnLayout(0, Qt::Vertical);
    buttonGroup13->layout()->setSpacing(6);
    buttonGroup13->layout()->setMargin(11);
    hboxLayout9 = new Q3HBoxLayout(buttonGroup13->layout());
    hboxLayout9->setAlignment(Qt::AlignTop);
    hboxLayout9->setObjectName(QString::fromUtf8("hboxLayout9"));
    theRLevelLineEdit = new QLineEdit(buttonGroup13);
    theRLevelLineEdit->setObjectName(QString::fromUtf8("theRLevelLineEdit"));
    theRLevelLineEdit->setMinimumSize(QSize(50, 0));

    hboxLayout9->addWidget(theRLevelLineEdit);


    hboxLayout5->addWidget(buttonGroup13);


    vboxLayout1->addLayout(hboxLayout5);

    theBandValueGroupBox = new Q3GroupBox(theImageGeomPage);
    theBandValueGroupBox->setObjectName(QString::fromUtf8("theBandValueGroupBox"));
    QSizePolicy sizePolicy7(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(1));
    sizePolicy7.setHorizontalStretch(0);
    sizePolicy7.setVerticalStretch(0);
    sizePolicy7.setHeightForWidth(theBandValueGroupBox->sizePolicy().hasHeightForWidth());
    theBandValueGroupBox->setSizePolicy(sizePolicy7);
    theBandValueGroupBox->setAlignment(Qt::AlignHCenter);
    theBandValueGroupBox->setColumnLayout(0, Qt::Vertical);
    theBandValueGroupBox->layout()->setSpacing(6);
    theBandValueGroupBox->layout()->setMargin(11);
    hboxLayout10 = new Q3HBoxLayout(theBandValueGroupBox->layout());
    hboxLayout10->setAlignment(Qt::AlignTop);
    hboxLayout10->setObjectName(QString::fromUtf8("hboxLayout10"));
    theSelectedBandGroupBox = new Q3GroupBox(theBandValueGroupBox);
    theSelectedBandGroupBox->setObjectName(QString::fromUtf8("theSelectedBandGroupBox"));
    QSizePolicy sizePolicy8(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(5));
    sizePolicy8.setHorizontalStretch(0);
    sizePolicy8.setVerticalStretch(0);
    sizePolicy8.setHeightForWidth(theSelectedBandGroupBox->sizePolicy().hasHeightForWidth());
    theSelectedBandGroupBox->setSizePolicy(sizePolicy8);
    theSelectedBandGroupBox->setAlignment(Qt::AlignHCenter);
    theSelectedBandGroupBox->setColumnLayout(0, Qt::Vertical);
    theSelectedBandGroupBox->layout()->setSpacing(6);
    theSelectedBandGroupBox->layout()->setMargin(11);
    hboxLayout11 = new Q3HBoxLayout(theSelectedBandGroupBox->layout());
    hboxLayout11->setAlignment(Qt::AlignTop);
    hboxLayout11->setObjectName(QString::fromUtf8("hboxLayout11"));
    theBandSpinBox = new QSpinBox(theSelectedBandGroupBox);
    theBandSpinBox->setObjectName(QString::fromUtf8("theBandSpinBox"));
    theBandSpinBox->setMinimumSize(QSize(60, 0));

    hboxLayout11->addWidget(theBandSpinBox);


    hboxLayout10->addWidget(theSelectedBandGroupBox);

    theMinGroupBox = new Q3GroupBox(theBandValueGroupBox);
    theMinGroupBox->setObjectName(QString::fromUtf8("theMinGroupBox"));
    QSizePolicy sizePolicy9(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(5));
    sizePolicy9.setHorizontalStretch(0);
    sizePolicy9.setVerticalStretch(0);
    sizePolicy9.setHeightForWidth(theMinGroupBox->sizePolicy().hasHeightForWidth());
    theMinGroupBox->setSizePolicy(sizePolicy9);
    theMinGroupBox->setAlignment(Qt::AlignHCenter);
    theMinGroupBox->setColumnLayout(0, Qt::Vertical);
    theMinGroupBox->layout()->setSpacing(6);
    theMinGroupBox->layout()->setMargin(11);
    hboxLayout12 = new Q3HBoxLayout(theMinGroupBox->layout());
    hboxLayout12->setAlignment(Qt::AlignTop);
    hboxLayout12->setObjectName(QString::fromUtf8("hboxLayout12"));
    theMinLineEdit = new QLineEdit(theMinGroupBox);
    theMinLineEdit->setObjectName(QString::fromUtf8("theMinLineEdit"));
    theMinLineEdit->setMinimumSize(QSize(60, 0));

    hboxLayout12->addWidget(theMinLineEdit);


    hboxLayout10->addWidget(theMinGroupBox);

    theMaxGroupBox = new Q3GroupBox(theBandValueGroupBox);
    theMaxGroupBox->setObjectName(QString::fromUtf8("theMaxGroupBox"));
    QSizePolicy sizePolicy10(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(5));
    sizePolicy10.setHorizontalStretch(0);
    sizePolicy10.setVerticalStretch(0);
    sizePolicy10.setHeightForWidth(theMaxGroupBox->sizePolicy().hasHeightForWidth());
    theMaxGroupBox->setSizePolicy(sizePolicy10);
    theMaxGroupBox->setAlignment(Qt::AlignHCenter);
    theMaxGroupBox->setColumnLayout(0, Qt::Vertical);
    theMaxGroupBox->layout()->setSpacing(6);
    theMaxGroupBox->layout()->setMargin(11);
    hboxLayout13 = new Q3HBoxLayout(theMaxGroupBox->layout());
    hboxLayout13->setAlignment(Qt::AlignTop);
    hboxLayout13->setObjectName(QString::fromUtf8("hboxLayout13"));
    theMaxLineEdit = new QLineEdit(theMaxGroupBox);
    theMaxLineEdit->setObjectName(QString::fromUtf8("theMaxLineEdit"));
    theMaxLineEdit->setMinimumSize(QSize(60, 0));

    hboxLayout13->addWidget(theMaxLineEdit);


    hboxLayout10->addWidget(theMaxGroupBox);

    theNullGroupBox = new Q3GroupBox(theBandValueGroupBox);
    theNullGroupBox->setObjectName(QString::fromUtf8("theNullGroupBox"));
    QSizePolicy sizePolicy11(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(1));
    sizePolicy11.setHorizontalStretch(0);
    sizePolicy11.setVerticalStretch(0);
    sizePolicy11.setHeightForWidth(theNullGroupBox->sizePolicy().hasHeightForWidth());
    theNullGroupBox->setSizePolicy(sizePolicy11);
    theNullGroupBox->setAlignment(Qt::AlignHCenter);
    theNullGroupBox->setColumnLayout(0, Qt::Vertical);
    theNullGroupBox->layout()->setSpacing(6);
    theNullGroupBox->layout()->setMargin(11);
    hboxLayout14 = new Q3HBoxLayout(theNullGroupBox->layout());
    hboxLayout14->setAlignment(Qt::AlignTop);
    hboxLayout14->setObjectName(QString::fromUtf8("hboxLayout14"));
    theNullLineEdit = new QLineEdit(theNullGroupBox);
    theNullLineEdit->setObjectName(QString::fromUtf8("theNullLineEdit"));
    theNullLineEdit->setMinimumSize(QSize(60, 0));

    hboxLayout14->addWidget(theNullLineEdit);


    hboxLayout10->addWidget(theNullGroupBox);


    vboxLayout1->addWidget(theBandValueGroupBox);

    hboxLayout15 = new Q3HBoxLayout();
    hboxLayout15->setSpacing(6);
    hboxLayout15->setMargin(0);
    hboxLayout15->setObjectName(QString::fromUtf8("hboxLayout15"));
    spacerItem = new QSpacerItem(79, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

    hboxLayout15->addItem(spacerItem);

    theScanForMinMaxPushButton = new QPushButton(theImageGeomPage);
    theScanForMinMaxPushButton->setObjectName(QString::fromUtf8("theScanForMinMaxPushButton"));

    hboxLayout15->addWidget(theScanForMinMaxPushButton);

    spacerItem1 = new QSpacerItem(100, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

    hboxLayout15->addItem(spacerItem1);


    vboxLayout1->addLayout(hboxLayout15);

    spacerItem2 = new QSpacerItem(20, 250, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

    vboxLayout1->addItem(spacerItem2);

    theImageInfoTabWidget->addTab(theImageGeomPage, QApplication::translate("ossimQtImageInfoDialog", "Image Geometry", 0, QApplication::UnicodeUTF8));
    theProjectionPage = new QWidget();
    theProjectionPage->setObjectName(QString::fromUtf8("theProjectionPage"));
    vboxLayout2 = new Q3VBoxLayout(theProjectionPage);
    vboxLayout2->setSpacing(6);
    vboxLayout2->setMargin(0);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    hboxLayout16 = new Q3HBoxLayout();
    hboxLayout16->setSpacing(6);
    hboxLayout16->setMargin(0);
    hboxLayout16->setObjectName(QString::fromUtf8("hboxLayout16"));
    theProjectionTextLable = new QLabel(theProjectionPage);
    theProjectionTextLable->setObjectName(QString::fromUtf8("theProjectionTextLable"));
    theProjectionTextLable->setMinimumSize(QSize(110, 0));

    hboxLayout16->addWidget(theProjectionTextLable);

    theProjectionComboBox = new QComboBox(theProjectionPage);
    theProjectionComboBox->setObjectName(QString::fromUtf8("theProjectionComboBox"));
    QSizePolicy sizePolicy12(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy12.setHorizontalStretch(0);
    sizePolicy12.setVerticalStretch(0);
    sizePolicy12.setHeightForWidth(theProjectionComboBox->sizePolicy().hasHeightForWidth());
    theProjectionComboBox->setSizePolicy(sizePolicy12);
    theProjectionComboBox->setMinimumSize(QSize(170, 0));

    hboxLayout16->addWidget(theProjectionComboBox);


    vboxLayout2->addLayout(hboxLayout16);

    hboxLayout17 = new Q3HBoxLayout();
    hboxLayout17->setSpacing(6);
    hboxLayout17->setMargin(0);
    hboxLayout17->setObjectName(QString::fromUtf8("hboxLayout17"));
    theDatumTextLabel = new QLabel(theProjectionPage);
    theDatumTextLabel->setObjectName(QString::fromUtf8("theDatumTextLabel"));
    theDatumTextLabel->setMinimumSize(QSize(110, 0));

    hboxLayout17->addWidget(theDatumTextLabel);

    theDatumComboBox = new QComboBox(theProjectionPage);
    theDatumComboBox->setObjectName(QString::fromUtf8("theDatumComboBox"));
    QSizePolicy sizePolicy13(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy13.setHorizontalStretch(0);
    sizePolicy13.setVerticalStretch(0);
    sizePolicy13.setHeightForWidth(theDatumComboBox->sizePolicy().hasHeightForWidth());
    theDatumComboBox->setSizePolicy(sizePolicy13);
    theDatumComboBox->setMinimumSize(QSize(170, 0));

    hboxLayout17->addWidget(theDatumComboBox);


    vboxLayout2->addLayout(hboxLayout17);

    hboxLayout18 = new Q3HBoxLayout();
    hboxLayout18->setSpacing(6);
    hboxLayout18->setMargin(0);
    hboxLayout18->setObjectName(QString::fromUtf8("hboxLayout18"));
    theUnitsTextLabel = new QLabel(theProjectionPage);
    theUnitsTextLabel->setObjectName(QString::fromUtf8("theUnitsTextLabel"));
    theUnitsTextLabel->setMinimumSize(QSize(110, 0));

    hboxLayout18->addWidget(theUnitsTextLabel);

    theUnitsComboBox = new QComboBox(theProjectionPage);
    theUnitsComboBox->setObjectName(QString::fromUtf8("theUnitsComboBox"));
    theUnitsComboBox->setEnabled(true);
    QSizePolicy sizePolicy14(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy14.setHorizontalStretch(0);
    sizePolicy14.setVerticalStretch(0);
    sizePolicy14.setHeightForWidth(theUnitsComboBox->sizePolicy().hasHeightForWidth());
    theUnitsComboBox->setSizePolicy(sizePolicy14);
    theUnitsComboBox->setMinimumSize(QSize(170, 0));

    hboxLayout18->addWidget(theUnitsComboBox);


    vboxLayout2->addLayout(hboxLayout18);

    hboxLayout19 = new Q3HBoxLayout();
    hboxLayout19->setSpacing(6);
    hboxLayout19->setMargin(0);
    hboxLayout19->setObjectName(QString::fromUtf8("hboxLayout19"));
    thePixelTypeTextLabel = new QLabel(theProjectionPage);
    thePixelTypeTextLabel->setObjectName(QString::fromUtf8("thePixelTypeTextLabel"));
    thePixelTypeTextLabel->setMinimumSize(QSize(110, 0));

    hboxLayout19->addWidget(thePixelTypeTextLabel);

    thePixelTypeComboBox = new QComboBox(theProjectionPage);
    thePixelTypeComboBox->setObjectName(QString::fromUtf8("thePixelTypeComboBox"));
    QSizePolicy sizePolicy15(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy15.setHorizontalStretch(0);
    sizePolicy15.setVerticalStretch(0);
    sizePolicy15.setHeightForWidth(thePixelTypeComboBox->sizePolicy().hasHeightForWidth());
    thePixelTypeComboBox->setSizePolicy(sizePolicy15);
    thePixelTypeComboBox->setMinimumSize(QSize(170, 0));

    hboxLayout19->addWidget(thePixelTypeComboBox);


    vboxLayout2->addLayout(hboxLayout19);

    hboxLayout20 = new Q3HBoxLayout();
    hboxLayout20->setSpacing(6);
    hboxLayout20->setMargin(0);
    hboxLayout20->setObjectName(QString::fromUtf8("hboxLayout20"));
    theZoneTextLabel = new QLabel(theProjectionPage);
    theZoneTextLabel->setObjectName(QString::fromUtf8("theZoneTextLabel"));
    theZoneTextLabel->setMinimumSize(QSize(110, 0));

    hboxLayout20->addWidget(theZoneTextLabel);

    theZoneComboBox = new QComboBox(theProjectionPage);
    theZoneComboBox->setObjectName(QString::fromUtf8("theZoneComboBox"));
    QSizePolicy sizePolicy16(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy16.setHorizontalStretch(0);
    sizePolicy16.setVerticalStretch(0);
    sizePolicy16.setHeightForWidth(theZoneComboBox->sizePolicy().hasHeightForWidth());
    theZoneComboBox->setSizePolicy(sizePolicy16);
    theZoneComboBox->setMinimumSize(QSize(170, 0));

    hboxLayout20->addWidget(theZoneComboBox);


    vboxLayout2->addLayout(hboxLayout20);

    hboxLayout21 = new Q3HBoxLayout();
    hboxLayout21->setSpacing(6);
    hboxLayout21->setMargin(0);
    hboxLayout21->setObjectName(QString::fromUtf8("hboxLayout21"));
    theHemisphereTextLabel = new QLabel(theProjectionPage);
    theHemisphereTextLabel->setObjectName(QString::fromUtf8("theHemisphereTextLabel"));
    theHemisphereTextLabel->setMinimumSize(QSize(110, 0));

    hboxLayout21->addWidget(theHemisphereTextLabel);

    theHemisphereComboBox = new QComboBox(theProjectionPage);
    theHemisphereComboBox->setObjectName(QString::fromUtf8("theHemisphereComboBox"));
    QSizePolicy sizePolicy17(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy17.setHorizontalStretch(0);
    sizePolicy17.setVerticalStretch(0);
    sizePolicy17.setHeightForWidth(theHemisphereComboBox->sizePolicy().hasHeightForWidth());
    theHemisphereComboBox->setSizePolicy(sizePolicy17);
    theHemisphereComboBox->setMinimumSize(QSize(170, 0));

    hboxLayout21->addWidget(theHemisphereComboBox);


    vboxLayout2->addLayout(hboxLayout21);

    hboxLayout22 = new Q3HBoxLayout();
    hboxLayout22->setSpacing(6);
    hboxLayout22->setMargin(0);
    hboxLayout22->setObjectName(QString::fromUtf8("hboxLayout22"));
    theOriginLatitudeTextLabel = new QLabel(theProjectionPage);
    theOriginLatitudeTextLabel->setObjectName(QString::fromUtf8("theOriginLatitudeTextLabel"));
    theOriginLatitudeTextLabel->setMinimumSize(QSize(110, 0));

    hboxLayout22->addWidget(theOriginLatitudeTextLabel);

    theOriginLatitudeLineEdit = new QLineEdit(theProjectionPage);
    theOriginLatitudeLineEdit->setObjectName(QString::fromUtf8("theOriginLatitudeLineEdit"));
    QSizePolicy sizePolicy18(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy18.setHorizontalStretch(0);
    sizePolicy18.setVerticalStretch(0);
    sizePolicy18.setHeightForWidth(theOriginLatitudeLineEdit->sizePolicy().hasHeightForWidth());
    theOriginLatitudeLineEdit->setSizePolicy(sizePolicy18);
    theOriginLatitudeLineEdit->setMinimumSize(QSize(170, 0));
    theOriginLatitudeLineEdit->setAlignment(Qt::AlignHCenter);

    hboxLayout22->addWidget(theOriginLatitudeLineEdit);


    vboxLayout2->addLayout(hboxLayout22);

    hboxLayout23 = new Q3HBoxLayout();
    hboxLayout23->setSpacing(6);
    hboxLayout23->setMargin(0);
    hboxLayout23->setObjectName(QString::fromUtf8("hboxLayout23"));
    theCentralMeridianTextLabel = new QLabel(theProjectionPage);
    theCentralMeridianTextLabel->setObjectName(QString::fromUtf8("theCentralMeridianTextLabel"));
    theCentralMeridianTextLabel->setMinimumSize(QSize(110, 0));

    hboxLayout23->addWidget(theCentralMeridianTextLabel);

    theCentralMeridianLineEdit = new QLineEdit(theProjectionPage);
    theCentralMeridianLineEdit->setObjectName(QString::fromUtf8("theCentralMeridianLineEdit"));
    QSizePolicy sizePolicy19(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy19.setHorizontalStretch(0);
    sizePolicy19.setVerticalStretch(0);
    sizePolicy19.setHeightForWidth(theCentralMeridianLineEdit->sizePolicy().hasHeightForWidth());
    theCentralMeridianLineEdit->setSizePolicy(sizePolicy19);
    theCentralMeridianLineEdit->setMinimumSize(QSize(170, 0));
    theCentralMeridianLineEdit->setAlignment(Qt::AlignHCenter);

    hboxLayout23->addWidget(theCentralMeridianLineEdit);


    vboxLayout2->addLayout(hboxLayout23);

    hboxLayout24 = new Q3HBoxLayout();
    hboxLayout24->setSpacing(6);
    hboxLayout24->setMargin(0);
    hboxLayout24->setObjectName(QString::fromUtf8("hboxLayout24"));
    theFalseEastingTextLabel = new QLabel(theProjectionPage);
    theFalseEastingTextLabel->setObjectName(QString::fromUtf8("theFalseEastingTextLabel"));
    theFalseEastingTextLabel->setMinimumSize(QSize(110, 0));

    hboxLayout24->addWidget(theFalseEastingTextLabel);

    theFalseEastingLineEdit = new QLineEdit(theProjectionPage);
    theFalseEastingLineEdit->setObjectName(QString::fromUtf8("theFalseEastingLineEdit"));
    QSizePolicy sizePolicy20(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy20.setHorizontalStretch(0);
    sizePolicy20.setVerticalStretch(0);
    sizePolicy20.setHeightForWidth(theFalseEastingLineEdit->sizePolicy().hasHeightForWidth());
    theFalseEastingLineEdit->setSizePolicy(sizePolicy20);
    theFalseEastingLineEdit->setMinimumSize(QSize(170, 0));
    theFalseEastingLineEdit->setAlignment(Qt::AlignHCenter);

    hboxLayout24->addWidget(theFalseEastingLineEdit);


    vboxLayout2->addLayout(hboxLayout24);

    hboxLayout25 = new Q3HBoxLayout();
    hboxLayout25->setSpacing(6);
    hboxLayout25->setMargin(0);
    hboxLayout25->setObjectName(QString::fromUtf8("hboxLayout25"));
    theFalseNorthingTextLabel = new QLabel(theProjectionPage);
    theFalseNorthingTextLabel->setObjectName(QString::fromUtf8("theFalseNorthingTextLabel"));
    theFalseNorthingTextLabel->setMinimumSize(QSize(110, 0));

    hboxLayout25->addWidget(theFalseNorthingTextLabel);

    theFalseNorthingLineEdit = new QLineEdit(theProjectionPage);
    theFalseNorthingLineEdit->setObjectName(QString::fromUtf8("theFalseNorthingLineEdit"));
    QSizePolicy sizePolicy21(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy21.setHorizontalStretch(0);
    sizePolicy21.setVerticalStretch(0);
    sizePolicy21.setHeightForWidth(theFalseNorthingLineEdit->sizePolicy().hasHeightForWidth());
    theFalseNorthingLineEdit->setSizePolicy(sizePolicy21);
    theFalseNorthingLineEdit->setMinimumSize(QSize(170, 0));
    theFalseNorthingLineEdit->setAlignment(Qt::AlignHCenter);

    hboxLayout25->addWidget(theFalseNorthingLineEdit);


    vboxLayout2->addLayout(hboxLayout25);

    hboxLayout26 = new Q3HBoxLayout();
    hboxLayout26->setSpacing(6);
    hboxLayout26->setMargin(0);
    hboxLayout26->setObjectName(QString::fromUtf8("hboxLayout26"));
    theTiePointX_TextLabel = new QLabel(theProjectionPage);
    theTiePointX_TextLabel->setObjectName(QString::fromUtf8("theTiePointX_TextLabel"));
    theTiePointX_TextLabel->setMinimumSize(QSize(110, 0));

    hboxLayout26->addWidget(theTiePointX_TextLabel);

    theTiePointX_LineEdit = new QLineEdit(theProjectionPage);
    theTiePointX_LineEdit->setObjectName(QString::fromUtf8("theTiePointX_LineEdit"));
    QSizePolicy sizePolicy22(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy22.setHorizontalStretch(0);
    sizePolicy22.setVerticalStretch(0);
    sizePolicy22.setHeightForWidth(theTiePointX_LineEdit->sizePolicy().hasHeightForWidth());
    theTiePointX_LineEdit->setSizePolicy(sizePolicy22);
    theTiePointX_LineEdit->setMinimumSize(QSize(170, 0));
    theTiePointX_LineEdit->setAlignment(Qt::AlignHCenter);

    hboxLayout26->addWidget(theTiePointX_LineEdit);


    vboxLayout2->addLayout(hboxLayout26);

    hboxLayout27 = new Q3HBoxLayout();
    hboxLayout27->setSpacing(6);
    hboxLayout27->setMargin(0);
    hboxLayout27->setObjectName(QString::fromUtf8("hboxLayout27"));
    theTiePointY_TextLabel = new QLabel(theProjectionPage);
    theTiePointY_TextLabel->setObjectName(QString::fromUtf8("theTiePointY_TextLabel"));
    theTiePointY_TextLabel->setMinimumSize(QSize(110, 0));

    hboxLayout27->addWidget(theTiePointY_TextLabel);

    theTiePointY_LineEdit = new QLineEdit(theProjectionPage);
    theTiePointY_LineEdit->setObjectName(QString::fromUtf8("theTiePointY_LineEdit"));
    QSizePolicy sizePolicy23(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy23.setHorizontalStretch(0);
    sizePolicy23.setVerticalStretch(0);
    sizePolicy23.setHeightForWidth(theTiePointY_LineEdit->sizePolicy().hasHeightForWidth());
    theTiePointY_LineEdit->setSizePolicy(sizePolicy23);
    theTiePointY_LineEdit->setMinimumSize(QSize(170, 0));
    theTiePointY_LineEdit->setAlignment(Qt::AlignHCenter);

    hboxLayout27->addWidget(theTiePointY_LineEdit);


    vboxLayout2->addLayout(hboxLayout27);

    hboxLayout28 = new Q3HBoxLayout();
    hboxLayout28->setSpacing(6);
    hboxLayout28->setMargin(0);
    hboxLayout28->setObjectName(QString::fromUtf8("hboxLayout28"));
    theScaleX_TextLabel = new QLabel(theProjectionPage);
    theScaleX_TextLabel->setObjectName(QString::fromUtf8("theScaleX_TextLabel"));
    theScaleX_TextLabel->setMinimumSize(QSize(110, 0));

    hboxLayout28->addWidget(theScaleX_TextLabel);

    theScaleX_LineEdit = new QLineEdit(theProjectionPage);
    theScaleX_LineEdit->setObjectName(QString::fromUtf8("theScaleX_LineEdit"));
    QSizePolicy sizePolicy24(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy24.setHorizontalStretch(0);
    sizePolicy24.setVerticalStretch(0);
    sizePolicy24.setHeightForWidth(theScaleX_LineEdit->sizePolicy().hasHeightForWidth());
    theScaleX_LineEdit->setSizePolicy(sizePolicy24);
    theScaleX_LineEdit->setMinimumSize(QSize(170, 0));
    theScaleX_LineEdit->setAlignment(Qt::AlignHCenter);

    hboxLayout28->addWidget(theScaleX_LineEdit);


    vboxLayout2->addLayout(hboxLayout28);

    hboxLayout29 = new Q3HBoxLayout();
    hboxLayout29->setSpacing(6);
    hboxLayout29->setMargin(0);
    hboxLayout29->setObjectName(QString::fromUtf8("hboxLayout29"));
    theScaleY_TextLabel = new QLabel(theProjectionPage);
    theScaleY_TextLabel->setObjectName(QString::fromUtf8("theScaleY_TextLabel"));
    theScaleY_TextLabel->setMinimumSize(QSize(110, 0));

    hboxLayout29->addWidget(theScaleY_TextLabel);

    theScaleY_LineEdit = new QLineEdit(theProjectionPage);
    theScaleY_LineEdit->setObjectName(QString::fromUtf8("theScaleY_LineEdit"));
    QSizePolicy sizePolicy25(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy25.setHorizontalStretch(0);
    sizePolicy25.setVerticalStretch(0);
    sizePolicy25.setHeightForWidth(theScaleY_LineEdit->sizePolicy().hasHeightForWidth());
    theScaleY_LineEdit->setSizePolicy(sizePolicy25);
    theScaleY_LineEdit->setMinimumSize(QSize(170, 0));
    theScaleY_LineEdit->setAlignment(Qt::AlignHCenter);

    hboxLayout29->addWidget(theScaleY_LineEdit);


    vboxLayout2->addLayout(hboxLayout29);

    hboxLayout30 = new Q3HBoxLayout();
    hboxLayout30->setSpacing(6);
    hboxLayout30->setMargin(0);
    hboxLayout30->setObjectName(QString::fromUtf8("hboxLayout30"));
    theScaleFactorTextLabel = new QLabel(theProjectionPage);
    theScaleFactorTextLabel->setObjectName(QString::fromUtf8("theScaleFactorTextLabel"));
    theScaleFactorTextLabel->setMinimumSize(QSize(110, 0));

    hboxLayout30->addWidget(theScaleFactorTextLabel);

    theScaleFactorLineEdit = new QLineEdit(theProjectionPage);
    theScaleFactorLineEdit->setObjectName(QString::fromUtf8("theScaleFactorLineEdit"));
    QSizePolicy sizePolicy26(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy26.setHorizontalStretch(0);
    sizePolicy26.setVerticalStretch(0);
    sizePolicy26.setHeightForWidth(theScaleFactorLineEdit->sizePolicy().hasHeightForWidth());
    theScaleFactorLineEdit->setSizePolicy(sizePolicy26);
    theScaleFactorLineEdit->setMinimumSize(QSize(170, 0));
    theScaleFactorLineEdit->setAlignment(Qt::AlignHCenter);

    hboxLayout30->addWidget(theScaleFactorLineEdit);


    vboxLayout2->addLayout(hboxLayout30);

    hboxLayout31 = new Q3HBoxLayout();
    hboxLayout31->setSpacing(6);
    hboxLayout31->setMargin(0);
    hboxLayout31->setObjectName(QString::fromUtf8("hboxLayout31"));
    theParallelOneTextLabel = new QLabel(theProjectionPage);
    theParallelOneTextLabel->setObjectName(QString::fromUtf8("theParallelOneTextLabel"));
    theParallelOneTextLabel->setMinimumSize(QSize(110, 0));

    hboxLayout31->addWidget(theParallelOneTextLabel);

    theParallelOneLineEdit = new QLineEdit(theProjectionPage);
    theParallelOneLineEdit->setObjectName(QString::fromUtf8("theParallelOneLineEdit"));
    QSizePolicy sizePolicy27(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy27.setHorizontalStretch(0);
    sizePolicy27.setVerticalStretch(0);
    sizePolicy27.setHeightForWidth(theParallelOneLineEdit->sizePolicy().hasHeightForWidth());
    theParallelOneLineEdit->setSizePolicy(sizePolicy27);
    theParallelOneLineEdit->setMinimumSize(QSize(170, 0));
    theParallelOneLineEdit->setAlignment(Qt::AlignHCenter);

    hboxLayout31->addWidget(theParallelOneLineEdit);


    vboxLayout2->addLayout(hboxLayout31);

    hboxLayout32 = new Q3HBoxLayout();
    hboxLayout32->setSpacing(6);
    hboxLayout32->setMargin(0);
    hboxLayout32->setObjectName(QString::fromUtf8("hboxLayout32"));
    theParallelTwoTextLabel = new QLabel(theProjectionPage);
    theParallelTwoTextLabel->setObjectName(QString::fromUtf8("theParallelTwoTextLabel"));
    theParallelTwoTextLabel->setMinimumSize(QSize(110, 0));

    hboxLayout32->addWidget(theParallelTwoTextLabel);

    theParallelTwoLineEdit = new QLineEdit(theProjectionPage);
    theParallelTwoLineEdit->setObjectName(QString::fromUtf8("theParallelTwoLineEdit"));
    QSizePolicy sizePolicy28(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy28.setHorizontalStretch(0);
    sizePolicy28.setVerticalStretch(0);
    sizePolicy28.setHeightForWidth(theParallelTwoLineEdit->sizePolicy().hasHeightForWidth());
    theParallelTwoLineEdit->setSizePolicy(sizePolicy28);
    theParallelTwoLineEdit->setMinimumSize(QSize(170, 0));
    theParallelTwoLineEdit->setAlignment(Qt::AlignHCenter);

    hboxLayout32->addWidget(theParallelTwoLineEdit);


    vboxLayout2->addLayout(hboxLayout32);

    hboxLayout33 = new Q3HBoxLayout();
    hboxLayout33->setSpacing(6);
    hboxLayout33->setMargin(0);
    hboxLayout33->setObjectName(QString::fromUtf8("hboxLayout33"));
    theSavePushButton = new QPushButton(theProjectionPage);
    theSavePushButton->setObjectName(QString::fromUtf8("theSavePushButton"));

    hboxLayout33->addWidget(theSavePushButton);

    spacerItem3 = new QSpacerItem(90, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    hboxLayout33->addItem(spacerItem3);

    theResetPushButton = new QPushButton(theProjectionPage);
    theResetPushButton->setObjectName(QString::fromUtf8("theResetPushButton"));

    hboxLayout33->addWidget(theResetPushButton);


    vboxLayout2->addLayout(hboxLayout33);

    theImageInfoTabWidget->addTab(theProjectionPage, QApplication::translate("ossimQtImageInfoDialog", "Image Projection", 0, QApplication::UnicodeUTF8));
    theValidVerticesPage = new QWidget();
    theValidVerticesPage->setObjectName(QString::fromUtf8("theValidVerticesPage"));
    vboxLayout3 = new Q3VBoxLayout(theValidVerticesPage);
    vboxLayout3->setSpacing(6);
    vboxLayout3->setMargin(0);
    vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
    theValidVeticesGroupBox = new Q3GroupBox(theValidVerticesPage);
    theValidVeticesGroupBox->setObjectName(QString::fromUtf8("theValidVeticesGroupBox"));
    theValidVeticesGroupBox->setAlignment(Qt::AlignHCenter);
    theValidVeticesGroupBox->setColumnLayout(0, Qt::Vertical);
    theValidVeticesGroupBox->layout()->setSpacing(6);
    theValidVeticesGroupBox->layout()->setMargin(11);
    vboxLayout4 = new Q3VBoxLayout(theValidVeticesGroupBox->layout());
    vboxLayout4->setAlignment(Qt::AlignTop);
    vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
    hboxLayout34 = new Q3HBoxLayout();
    hboxLayout34->setSpacing(6);
    hboxLayout34->setMargin(0);
    hboxLayout34->setObjectName(QString::fromUtf8("hboxLayout34"));
    theVertex0GroupBox = new Q3GroupBox(theValidVeticesGroupBox);
    theVertex0GroupBox->setObjectName(QString::fromUtf8("theVertex0GroupBox"));
    theVertex0GroupBox->setAlignment(Qt::AlignHCenter);
    theVertex0GroupBox->setColumnLayout(0, Qt::Vertical);
    theVertex0GroupBox->layout()->setSpacing(6);
    theVertex0GroupBox->layout()->setMargin(11);
    hboxLayout35 = new Q3HBoxLayout(theVertex0GroupBox->layout());
    hboxLayout35->setAlignment(Qt::AlignTop);
    hboxLayout35->setObjectName(QString::fromUtf8("hboxLayout35"));
    hboxLayout36 = new Q3HBoxLayout();
    hboxLayout36->setSpacing(6);
    hboxLayout36->setMargin(0);
    hboxLayout36->setObjectName(QString::fromUtf8("hboxLayout36"));
    theVertex0LineGroupBox = new Q3GroupBox(theVertex0GroupBox);
    theVertex0LineGroupBox->setObjectName(QString::fromUtf8("theVertex0LineGroupBox"));
    theVertex0LineGroupBox->setAlignment(Qt::AlignHCenter);
    theVertex0LineGroupBox->setColumnLayout(0, Qt::Vertical);
    theVertex0LineGroupBox->layout()->setSpacing(6);
    theVertex0LineGroupBox->layout()->setMargin(11);
    hboxLayout37 = new Q3HBoxLayout(theVertex0LineGroupBox->layout());
    hboxLayout37->setAlignment(Qt::AlignTop);
    hboxLayout37->setObjectName(QString::fromUtf8("hboxLayout37"));
    theVertex0LineLineEdit = new QLineEdit(theVertex0LineGroupBox);
    theVertex0LineLineEdit->setObjectName(QString::fromUtf8("theVertex0LineLineEdit"));
    QSizePolicy sizePolicy29(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy29.setHorizontalStretch(0);
    sizePolicy29.setVerticalStretch(0);
    sizePolicy29.setHeightForWidth(theVertex0LineLineEdit->sizePolicy().hasHeightForWidth());
    theVertex0LineLineEdit->setSizePolicy(sizePolicy29);
    theVertex0LineLineEdit->setMinimumSize(QSize(60, 0));

    hboxLayout37->addWidget(theVertex0LineLineEdit);


    hboxLayout36->addWidget(theVertex0LineGroupBox);

    theVertex0SampleGroupBox = new Q3GroupBox(theVertex0GroupBox);
    theVertex0SampleGroupBox->setObjectName(QString::fromUtf8("theVertex0SampleGroupBox"));
    theVertex0SampleGroupBox->setAlignment(Qt::AlignHCenter);
    theVertex0SampleGroupBox->setColumnLayout(0, Qt::Vertical);
    theVertex0SampleGroupBox->layout()->setSpacing(6);
    theVertex0SampleGroupBox->layout()->setMargin(11);
    hboxLayout38 = new Q3HBoxLayout(theVertex0SampleGroupBox->layout());
    hboxLayout38->setAlignment(Qt::AlignTop);
    hboxLayout38->setObjectName(QString::fromUtf8("hboxLayout38"));
    theVertex0SampleLineEdit = new QLineEdit(theVertex0SampleGroupBox);
    theVertex0SampleLineEdit->setObjectName(QString::fromUtf8("theVertex0SampleLineEdit"));
    QSizePolicy sizePolicy30(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy30.setHorizontalStretch(0);
    sizePolicy30.setVerticalStretch(0);
    sizePolicy30.setHeightForWidth(theVertex0SampleLineEdit->sizePolicy().hasHeightForWidth());
    theVertex0SampleLineEdit->setSizePolicy(sizePolicy30);
    theVertex0SampleLineEdit->setMinimumSize(QSize(60, 0));

    hboxLayout38->addWidget(theVertex0SampleLineEdit);


    hboxLayout36->addWidget(theVertex0SampleGroupBox);


    hboxLayout35->addLayout(hboxLayout36);


    hboxLayout34->addWidget(theVertex0GroupBox);

    theVertex1GroupBox = new Q3GroupBox(theValidVeticesGroupBox);
    theVertex1GroupBox->setObjectName(QString::fromUtf8("theVertex1GroupBox"));
    theVertex1GroupBox->setAlignment(Qt::AlignHCenter);
    theVertex1GroupBox->setColumnLayout(0, Qt::Vertical);
    theVertex1GroupBox->layout()->setSpacing(6);
    theVertex1GroupBox->layout()->setMargin(11);
    hboxLayout39 = new Q3HBoxLayout(theVertex1GroupBox->layout());
    hboxLayout39->setAlignment(Qt::AlignTop);
    hboxLayout39->setObjectName(QString::fromUtf8("hboxLayout39"));
    hboxLayout40 = new Q3HBoxLayout();
    hboxLayout40->setSpacing(6);
    hboxLayout40->setMargin(0);
    hboxLayout40->setObjectName(QString::fromUtf8("hboxLayout40"));
    theVertex1LineGroupBox = new Q3GroupBox(theVertex1GroupBox);
    theVertex1LineGroupBox->setObjectName(QString::fromUtf8("theVertex1LineGroupBox"));
    theVertex1LineGroupBox->setAlignment(Qt::AlignHCenter);
    theVertex1LineGroupBox->setColumnLayout(0, Qt::Vertical);
    theVertex1LineGroupBox->layout()->setSpacing(6);
    theVertex1LineGroupBox->layout()->setMargin(11);
    hboxLayout41 = new Q3HBoxLayout(theVertex1LineGroupBox->layout());
    hboxLayout41->setAlignment(Qt::AlignTop);
    hboxLayout41->setObjectName(QString::fromUtf8("hboxLayout41"));
    theVertex1LineLineEdit = new QLineEdit(theVertex1LineGroupBox);
    theVertex1LineLineEdit->setObjectName(QString::fromUtf8("theVertex1LineLineEdit"));
    QSizePolicy sizePolicy31(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy31.setHorizontalStretch(0);
    sizePolicy31.setVerticalStretch(0);
    sizePolicy31.setHeightForWidth(theVertex1LineLineEdit->sizePolicy().hasHeightForWidth());
    theVertex1LineLineEdit->setSizePolicy(sizePolicy31);
    theVertex1LineLineEdit->setMinimumSize(QSize(60, 0));

    hboxLayout41->addWidget(theVertex1LineLineEdit);


    hboxLayout40->addWidget(theVertex1LineGroupBox);

    theVertex1SampleGroupBox = new Q3GroupBox(theVertex1GroupBox);
    theVertex1SampleGroupBox->setObjectName(QString::fromUtf8("theVertex1SampleGroupBox"));
    theVertex1SampleGroupBox->setAlignment(Qt::AlignHCenter);
    theVertex1SampleGroupBox->setColumnLayout(0, Qt::Vertical);
    theVertex1SampleGroupBox->layout()->setSpacing(6);
    theVertex1SampleGroupBox->layout()->setMargin(11);
    hboxLayout42 = new Q3HBoxLayout(theVertex1SampleGroupBox->layout());
    hboxLayout42->setAlignment(Qt::AlignTop);
    hboxLayout42->setObjectName(QString::fromUtf8("hboxLayout42"));
    theVertex1SampleLineEdit = new QLineEdit(theVertex1SampleGroupBox);
    theVertex1SampleLineEdit->setObjectName(QString::fromUtf8("theVertex1SampleLineEdit"));
    QSizePolicy sizePolicy32(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy32.setHorizontalStretch(0);
    sizePolicy32.setVerticalStretch(0);
    sizePolicy32.setHeightForWidth(theVertex1SampleLineEdit->sizePolicy().hasHeightForWidth());
    theVertex1SampleLineEdit->setSizePolicy(sizePolicy32);
    theVertex1SampleLineEdit->setMinimumSize(QSize(60, 0));

    hboxLayout42->addWidget(theVertex1SampleLineEdit);


    hboxLayout40->addWidget(theVertex1SampleGroupBox);


    hboxLayout39->addLayout(hboxLayout40);


    hboxLayout34->addWidget(theVertex1GroupBox);


    vboxLayout4->addLayout(hboxLayout34);

    hboxLayout43 = new Q3HBoxLayout();
    hboxLayout43->setSpacing(6);
    hboxLayout43->setMargin(0);
    hboxLayout43->setObjectName(QString::fromUtf8("hboxLayout43"));
    theVertex2GroupBox = new Q3GroupBox(theValidVeticesGroupBox);
    theVertex2GroupBox->setObjectName(QString::fromUtf8("theVertex2GroupBox"));
    theVertex2GroupBox->setAlignment(Qt::AlignHCenter);
    theVertex2GroupBox->setColumnLayout(0, Qt::Vertical);
    theVertex2GroupBox->layout()->setSpacing(6);
    theVertex2GroupBox->layout()->setMargin(11);
    hboxLayout44 = new Q3HBoxLayout(theVertex2GroupBox->layout());
    hboxLayout44->setAlignment(Qt::AlignTop);
    hboxLayout44->setObjectName(QString::fromUtf8("hboxLayout44"));
    hboxLayout45 = new Q3HBoxLayout();
    hboxLayout45->setSpacing(6);
    hboxLayout45->setMargin(0);
    hboxLayout45->setObjectName(QString::fromUtf8("hboxLayout45"));
    theVertex2LineGroupBox = new Q3GroupBox(theVertex2GroupBox);
    theVertex2LineGroupBox->setObjectName(QString::fromUtf8("theVertex2LineGroupBox"));
    theVertex2LineGroupBox->setAlignment(Qt::AlignHCenter);
    theVertex2LineGroupBox->setColumnLayout(0, Qt::Vertical);
    theVertex2LineGroupBox->layout()->setSpacing(6);
    theVertex2LineGroupBox->layout()->setMargin(11);
    hboxLayout46 = new Q3HBoxLayout(theVertex2LineGroupBox->layout());
    hboxLayout46->setAlignment(Qt::AlignTop);
    hboxLayout46->setObjectName(QString::fromUtf8("hboxLayout46"));
    theVertex2LineLineEdit = new QLineEdit(theVertex2LineGroupBox);
    theVertex2LineLineEdit->setObjectName(QString::fromUtf8("theVertex2LineLineEdit"));
    QSizePolicy sizePolicy33(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy33.setHorizontalStretch(0);
    sizePolicy33.setVerticalStretch(0);
    sizePolicy33.setHeightForWidth(theVertex2LineLineEdit->sizePolicy().hasHeightForWidth());
    theVertex2LineLineEdit->setSizePolicy(sizePolicy33);
    theVertex2LineLineEdit->setMinimumSize(QSize(60, 0));

    hboxLayout46->addWidget(theVertex2LineLineEdit);


    hboxLayout45->addWidget(theVertex2LineGroupBox);

    theVertex2SampleGroupBox = new Q3GroupBox(theVertex2GroupBox);
    theVertex2SampleGroupBox->setObjectName(QString::fromUtf8("theVertex2SampleGroupBox"));
    QSizePolicy sizePolicy34(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(1));
    sizePolicy34.setHorizontalStretch(0);
    sizePolicy34.setVerticalStretch(0);
    sizePolicy34.setHeightForWidth(theVertex2SampleGroupBox->sizePolicy().hasHeightForWidth());
    theVertex2SampleGroupBox->setSizePolicy(sizePolicy34);
    theVertex2SampleGroupBox->setAlignment(Qt::AlignHCenter);
    theVertex2SampleGroupBox->setColumnLayout(0, Qt::Vertical);
    theVertex2SampleGroupBox->layout()->setSpacing(6);
    theVertex2SampleGroupBox->layout()->setMargin(11);
    hboxLayout47 = new Q3HBoxLayout(theVertex2SampleGroupBox->layout());
    hboxLayout47->setAlignment(Qt::AlignTop);
    hboxLayout47->setObjectName(QString::fromUtf8("hboxLayout47"));
    theVertex2SampleLineEdit = new QLineEdit(theVertex2SampleGroupBox);
    theVertex2SampleLineEdit->setObjectName(QString::fromUtf8("theVertex2SampleLineEdit"));
    QSizePolicy sizePolicy35(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy35.setHorizontalStretch(0);
    sizePolicy35.setVerticalStretch(0);
    sizePolicy35.setHeightForWidth(theVertex2SampleLineEdit->sizePolicy().hasHeightForWidth());
    theVertex2SampleLineEdit->setSizePolicy(sizePolicy35);
    theVertex2SampleLineEdit->setMinimumSize(QSize(60, 0));

    hboxLayout47->addWidget(theVertex2SampleLineEdit);


    hboxLayout45->addWidget(theVertex2SampleGroupBox);


    hboxLayout44->addLayout(hboxLayout45);


    hboxLayout43->addWidget(theVertex2GroupBox);

    theVertex3GroupBox = new Q3GroupBox(theValidVeticesGroupBox);
    theVertex3GroupBox->setObjectName(QString::fromUtf8("theVertex3GroupBox"));
    theVertex3GroupBox->setAlignment(Qt::AlignHCenter);
    theVertex3GroupBox->setColumnLayout(0, Qt::Vertical);
    theVertex3GroupBox->layout()->setSpacing(6);
    theVertex3GroupBox->layout()->setMargin(11);
    hboxLayout48 = new Q3HBoxLayout(theVertex3GroupBox->layout());
    hboxLayout48->setAlignment(Qt::AlignTop);
    hboxLayout48->setObjectName(QString::fromUtf8("hboxLayout48"));
    hboxLayout49 = new Q3HBoxLayout();
    hboxLayout49->setSpacing(6);
    hboxLayout49->setMargin(0);
    hboxLayout49->setObjectName(QString::fromUtf8("hboxLayout49"));
    theVertex3LineGroupBox = new Q3GroupBox(theVertex3GroupBox);
    theVertex3LineGroupBox->setObjectName(QString::fromUtf8("theVertex3LineGroupBox"));
    theVertex3LineGroupBox->setAlignment(Qt::AlignHCenter);
    theVertex3LineGroupBox->setColumnLayout(0, Qt::Vertical);
    theVertex3LineGroupBox->layout()->setSpacing(6);
    theVertex3LineGroupBox->layout()->setMargin(11);
    hboxLayout50 = new Q3HBoxLayout(theVertex3LineGroupBox->layout());
    hboxLayout50->setAlignment(Qt::AlignTop);
    hboxLayout50->setObjectName(QString::fromUtf8("hboxLayout50"));
    theVertex0LineLineEdit_4 = new QLineEdit(theVertex3LineGroupBox);
    theVertex0LineLineEdit_4->setObjectName(QString::fromUtf8("theVertex0LineLineEdit_4"));
    QSizePolicy sizePolicy36(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy36.setHorizontalStretch(0);
    sizePolicy36.setVerticalStretch(0);
    sizePolicy36.setHeightForWidth(theVertex0LineLineEdit_4->sizePolicy().hasHeightForWidth());
    theVertex0LineLineEdit_4->setSizePolicy(sizePolicy36);
    theVertex0LineLineEdit_4->setMinimumSize(QSize(60, 0));

    hboxLayout50->addWidget(theVertex0LineLineEdit_4);


    hboxLayout49->addWidget(theVertex3LineGroupBox);

    theVertex3SampleGroupBox = new Q3GroupBox(theVertex3GroupBox);
    theVertex3SampleGroupBox->setObjectName(QString::fromUtf8("theVertex3SampleGroupBox"));
    theVertex3SampleGroupBox->setAlignment(Qt::AlignHCenter);
    theVertex3SampleGroupBox->setColumnLayout(0, Qt::Vertical);
    theVertex3SampleGroupBox->layout()->setSpacing(6);
    theVertex3SampleGroupBox->layout()->setMargin(11);
    hboxLayout51 = new Q3HBoxLayout(theVertex3SampleGroupBox->layout());
    hboxLayout51->setAlignment(Qt::AlignTop);
    hboxLayout51->setObjectName(QString::fromUtf8("hboxLayout51"));
    theVertex0SampleLineEdit_4 = new QLineEdit(theVertex3SampleGroupBox);
    theVertex0SampleLineEdit_4->setObjectName(QString::fromUtf8("theVertex0SampleLineEdit_4"));
    QSizePolicy sizePolicy37(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy37.setHorizontalStretch(0);
    sizePolicy37.setVerticalStretch(0);
    sizePolicy37.setHeightForWidth(theVertex0SampleLineEdit_4->sizePolicy().hasHeightForWidth());
    theVertex0SampleLineEdit_4->setSizePolicy(sizePolicy37);
    theVertex0SampleLineEdit_4->setMinimumSize(QSize(60, 0));

    hboxLayout51->addWidget(theVertex0SampleLineEdit_4);


    hboxLayout49->addWidget(theVertex3SampleGroupBox);


    hboxLayout48->addLayout(hboxLayout49);


    hboxLayout43->addWidget(theVertex3GroupBox);


    vboxLayout4->addLayout(hboxLayout43);

    hboxLayout52 = new Q3HBoxLayout();
    hboxLayout52->setSpacing(6);
    hboxLayout52->setMargin(0);
    hboxLayout52->setObjectName(QString::fromUtf8("hboxLayout52"));
    spacerItem4 = new QSpacerItem(40, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    hboxLayout52->addItem(spacerItem4);

    theScanForValidVerticesPushButton = new QPushButton(theValidVeticesGroupBox);
    theScanForValidVerticesPushButton->setObjectName(QString::fromUtf8("theScanForValidVerticesPushButton"));

    hboxLayout52->addWidget(theScanForValidVerticesPushButton);

    spacerItem5 = new QSpacerItem(50, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    hboxLayout52->addItem(spacerItem5);


    vboxLayout4->addLayout(hboxLayout52);


    vboxLayout3->addWidget(theValidVeticesGroupBox);

    spacerItem6 = new QSpacerItem(20, 80, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout3->addItem(spacerItem6);

    theImageInfoTabWidget->addTab(theValidVerticesPage, QApplication::translate("ossimQtImageInfoDialog", "Valid Vertices", 0, QApplication::UnicodeUTF8));

    vboxLayout->addWidget(theImageInfoTabWidget);

    hboxLayout53 = new Q3HBoxLayout();
    hboxLayout53->setSpacing(6);
    hboxLayout53->setMargin(0);
    hboxLayout53->setObjectName(QString::fromUtf8("hboxLayout53"));
    theHelpPushButton = new QPushButton(ossimQtImageInfoDialog);
    theHelpPushButton->setObjectName(QString::fromUtf8("theHelpPushButton"));

    hboxLayout53->addWidget(theHelpPushButton);

    spacerItem7 = new QSpacerItem(250, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    hboxLayout53->addItem(spacerItem7);

    theClosePushButton = new QPushButton(ossimQtImageInfoDialog);
    theClosePushButton->setObjectName(QString::fromUtf8("theClosePushButton"));

    hboxLayout53->addWidget(theClosePushButton);


    vboxLayout->addLayout(hboxLayout53);

    retranslateUi(ossimQtImageInfoDialog);

    QMetaObject::connectSlotsByName(ossimQtImageInfoDialog);
    } // setupUi

    void retranslateUi(QDialog *ossimQtImageInfoDialog)
    {
    ossimQtImageInfoDialog->setWindowTitle(QApplication::translate("ossimQtImageInfoDialog", "Image Information", 0, QApplication::UnicodeUTF8));
    theImageLayerButtonGroup->setTitle(QApplication::translate("ossimQtImageInfoDialog", "Image Layer", 0, QApplication::UnicodeUTF8));
    theImageTypeGroupBox->setTitle(QApplication::translate("ossimQtImageInfoDialog", "Image Type", 0, QApplication::UnicodeUTF8));
    theRadiometryGroupBox->setTitle(QApplication::translate("ossimQtImageInfoDialog", "Radiometry", 0, QApplication::UnicodeUTF8));
    theByteOrderGroupBox->setTitle(QApplication::translate("ossimQtImageInfoDialog", "Byte Order", 0, QApplication::UnicodeUTF8));
    theLinesGroupBox->setTitle(QApplication::translate("ossimQtImageInfoDialog", "Lines", 0, QApplication::UnicodeUTF8));
    theSamplesGroupBox->setTitle(QApplication::translate("ossimQtImageInfoDialog", "Samples", 0, QApplication::UnicodeUTF8));
    theBandsGroupBox->setTitle(QApplication::translate("ossimQtImageInfoDialog", "Bands", 0, QApplication::UnicodeUTF8));
    buttonGroup13->setTitle(QApplication::translate("ossimQtImageInfoDialog", "R Levels", 0, QApplication::UnicodeUTF8));
    theBandValueGroupBox->setTitle(QApplication::translate("ossimQtImageInfoDialog", "Selected Band min/max/null values", 0, QApplication::UnicodeUTF8));
    theSelectedBandGroupBox->setTitle(QApplication::translate("ossimQtImageInfoDialog", "Band", 0, QApplication::UnicodeUTF8));
    theMinGroupBox->setTitle(QApplication::translate("ossimQtImageInfoDialog", "Min", 0, QApplication::UnicodeUTF8));
    theMaxGroupBox->setTitle(QApplication::translate("ossimQtImageInfoDialog", "Max", 0, QApplication::UnicodeUTF8));
    theNullGroupBox->setTitle(QApplication::translate("ossimQtImageInfoDialog", "Null", 0, QApplication::UnicodeUTF8));
    theScanForMinMaxPushButton->setText(QApplication::translate("ossimQtImageInfoDialog", "Scan For Min Max", 0, QApplication::UnicodeUTF8));
    theImageInfoTabWidget->setTabText(theImageInfoTabWidget->indexOf(theImageGeomPage), QApplication::translate("ossimQtImageInfoDialog", "Image Geometry", 0, QApplication::UnicodeUTF8));
    theProjectionTextLable->setText(QApplication::translate("ossimQtImageInfoDialog", "Projection:", 0, QApplication::UnicodeUTF8));
    theDatumTextLabel->setText(QApplication::translate("ossimQtImageInfoDialog", "Datum:", 0, QApplication::UnicodeUTF8));
    theUnitsTextLabel->setText(QApplication::translate("ossimQtImageInfoDialog", "Units:", 0, QApplication::UnicodeUTF8));
    thePixelTypeTextLabel->setText(QApplication::translate("ossimQtImageInfoDialog", "Pixel type:", 0, QApplication::UnicodeUTF8));
    theZoneTextLabel->setText(QApplication::translate("ossimQtImageInfoDialog", "Zone:", 0, QApplication::UnicodeUTF8));
    theHemisphereTextLabel->setText(QApplication::translate("ossimQtImageInfoDialog", "Hemisphere:", 0, QApplication::UnicodeUTF8));
    theOriginLatitudeTextLabel->setText(QApplication::translate("ossimQtImageInfoDialog", "Origin Latitude:", 0, QApplication::UnicodeUTF8));
    theCentralMeridianTextLabel->setText(QApplication::translate("ossimQtImageInfoDialog", "Central Meridian:", 0, QApplication::UnicodeUTF8));
    theFalseEastingTextLabel->setText(QApplication::translate("ossimQtImageInfoDialog", "False Easting:", 0, QApplication::UnicodeUTF8));
    theFalseNorthingTextLabel->setText(QApplication::translate("ossimQtImageInfoDialog", "False Northing:", 0, QApplication::UnicodeUTF8));
    theTiePointX_TextLabel->setText(QApplication::translate("ossimQtImageInfoDialog", "Tie Point X:", 0, QApplication::UnicodeUTF8));
    theTiePointY_TextLabel->setText(QApplication::translate("ossimQtImageInfoDialog", "Tie Point Y", 0, QApplication::UnicodeUTF8));
    theScaleX_TextLabel->setText(QApplication::translate("ossimQtImageInfoDialog", "Scale X:", 0, QApplication::UnicodeUTF8));
    theScaleY_TextLabel->setText(QApplication::translate("ossimQtImageInfoDialog", "Scale Y:", 0, QApplication::UnicodeUTF8));
    theScaleFactorTextLabel->setText(QApplication::translate("ossimQtImageInfoDialog", "Scale Factor", 0, QApplication::UnicodeUTF8));
    theParallelOneTextLabel->setText(QApplication::translate("ossimQtImageInfoDialog", "STD Parallel 1:", 0, QApplication::UnicodeUTF8));
    theParallelTwoTextLabel->setText(QApplication::translate("ossimQtImageInfoDialog", "STD Parallel 2:", 0, QApplication::UnicodeUTF8));
    theSavePushButton->setText(QApplication::translate("ossimQtImageInfoDialog", "Save", 0, QApplication::UnicodeUTF8));
    theResetPushButton->setText(QApplication::translate("ossimQtImageInfoDialog", "Reset", 0, QApplication::UnicodeUTF8));
    theImageInfoTabWidget->setTabText(theImageInfoTabWidget->indexOf(theProjectionPage), QApplication::translate("ossimQtImageInfoDialog", "Image Projection", 0, QApplication::UnicodeUTF8));
    theValidVeticesGroupBox->setTitle(QApplication::translate("ossimQtImageInfoDialog", "Valid Vertices", 0, QApplication::UnicodeUTF8));
    theVertex0GroupBox->setTitle(QApplication::translate("ossimQtImageInfoDialog", "Vertex[0]", 0, QApplication::UnicodeUTF8));
    theVertex0LineGroupBox->setTitle(QApplication::translate("ossimQtImageInfoDialog", "Line", 0, QApplication::UnicodeUTF8));
    theVertex0SampleGroupBox->setTitle(QApplication::translate("ossimQtImageInfoDialog", "Sample", 0, QApplication::UnicodeUTF8));
    theVertex1GroupBox->setTitle(QApplication::translate("ossimQtImageInfoDialog", "Vertex[1]", 0, QApplication::UnicodeUTF8));
    theVertex1LineGroupBox->setTitle(QApplication::translate("ossimQtImageInfoDialog", "Line", 0, QApplication::UnicodeUTF8));
    theVertex1SampleGroupBox->setTitle(QApplication::translate("ossimQtImageInfoDialog", "Sample", 0, QApplication::UnicodeUTF8));
    theVertex2GroupBox->setTitle(QApplication::translate("ossimQtImageInfoDialog", "Vertex[2]", 0, QApplication::UnicodeUTF8));
    theVertex2LineGroupBox->setTitle(QApplication::translate("ossimQtImageInfoDialog", "Line", 0, QApplication::UnicodeUTF8));
    theVertex2SampleGroupBox->setTitle(QApplication::translate("ossimQtImageInfoDialog", "Sample", 0, QApplication::UnicodeUTF8));
    theVertex3GroupBox->setTitle(QApplication::translate("ossimQtImageInfoDialog", "Vertex[3]", 0, QApplication::UnicodeUTF8));
    theVertex3LineGroupBox->setTitle(QApplication::translate("ossimQtImageInfoDialog", "Line", 0, QApplication::UnicodeUTF8));
    theVertex3SampleGroupBox->setTitle(QApplication::translate("ossimQtImageInfoDialog", "Sample", 0, QApplication::UnicodeUTF8));
    theScanForValidVerticesPushButton->setText(QApplication::translate("ossimQtImageInfoDialog", "Scan for valid vertices", 0, QApplication::UnicodeUTF8));
    theImageInfoTabWidget->setTabText(theImageInfoTabWidget->indexOf(theValidVerticesPage), QApplication::translate("ossimQtImageInfoDialog", "Valid Vertices", 0, QApplication::UnicodeUTF8));
    theHelpPushButton->setText(QApplication::translate("ossimQtImageInfoDialog", "Help", 0, QApplication::UnicodeUTF8));
    theClosePushButton->setText(QApplication::translate("ossimQtImageInfoDialog", "Close", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ossimQtImageInfoDialog);
    } // retranslateUi

};

namespace Ui {
    class ossimQtImageInfoDialog: public Ui_ossimQtImageInfoDialog {};
} // namespace Ui

class ossimQtImageInfoDialog : public QDialog, public Ui::ossimQtImageInfoDialog
{
    Q_OBJECT

public:
    ossimQtImageInfoDialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0);
    ~ossimQtImageInfoDialog();

    virtual void setImageWidget( ossimQtScrollingImageWidget * widget );

public slots:
    virtual void closeButtonClicked();
    virtual void helpButtonClicked();
    virtual void scanForMinMaxButtonClicked();
    virtual void scanForValidVerticesButtonClicked();
    virtual void layerComboBoxActivated( int );
    virtual void bandSpinBoxValueChanged( int );
    virtual void projectionComboBoxActivated( const QString & );
    virtual void datumComboBoxActivated( const QString & );
    virtual void pixelTypeComboBoxActivated( const QString & );
    virtual void zoneComboBoxActivated( const QString & );
    virtual void hemisphereComboBoxActivated( const QString & );
    virtual void originLatitudeLineEditReturnPressed();
    virtual void centralMeridianLineEditReturnPressed();
    virtual void tiePointX_LineEditReturnPressed();
    virtual void tiePointY_LineEditReturnPressed();
    virtual void parallelOneLineEditReturnPressed();
    virtual void parallelTwoLineEditReturnPressed();
    virtual void resetProjectionPushButtonClicked();
    virtual void saveProjectionPushButtonClicked();
    virtual void customDialogHasClosed();
    virtual void imageInfoTabWidgetSelected( const QString & qs );

protected slots:
    virtual void languageChange();

private:
    ossimQtImageInfoController* theController;

    virtual void init();
    virtual void destroy();

};

#endif // OSSIMQTIMAGEINFODIALOG_H
