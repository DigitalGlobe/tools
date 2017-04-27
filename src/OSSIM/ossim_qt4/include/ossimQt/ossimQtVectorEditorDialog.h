#ifndef OSSIMQTVECTOREDITORDIALOG_H
#define OSSIMQTVECTOREDITORDIALOG_H

#include <QtCore/QVariant>
//Added by qt3to4:
#include <Qt3Support/Q3HBoxLayout>
#include <Qt3Support/Q3GridLayout>
#include <Qt3Support/Q3Frame>
#include <QtGui/QLabel>
#include <Qt3Support/Q3VBoxLayout>

class ossimQtVectorEditorDialogController;

#include <Qt3Support/Q3Frame>
#include <Qt3Support/Q3GroupBox>
#include <Qt3Support/Q3ListBox>
#include <Qt3Support/Q3WidgetStack>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <Qt3Support/Q3MimeSourceFactory>

class Ui_ossimQtVectorEditorDialog
{
public:
    Q3VBoxLayout *vboxLayout;
    Q3GroupBox *theFeatureGroupBox;
    Q3HBoxLayout *hboxLayout;
    Q3VBoxLayout *vboxLayout1;
    Q3ListBox *theFeatureListbox;
    QGridLayout *gridLayout;
    QPushButton *theDisableAllButton;
    QPushButton *theEnableAllButton;
    QPushButton *theMoveUpButton;
    QPushButton *theMoveDownButton;
    Q3GroupBox *theFeaturePropertiesGroupBox;
    Q3VBoxLayout *vboxLayout2;
    Q3WidgetStack *thePropertyWidgetStack;
    QWidget *page;
    Q3VBoxLayout *vboxLayout3;
    QCheckBox *thePointEnabledCheckBox;
    QCheckBox *thePointFilledCheckBox;
    Q3HBoxLayout *hboxLayout1;
    QLabel *thePointRadiusLabel;
    QLineEdit *thePointRadiusInput;
    Q3HBoxLayout *hboxLayout2;
    QLabel *thePointColorLabel;
    Q3Frame *thePointColorFrame;
    QPushButton *thePointColorButton;
    QWidget *page1;
    Q3VBoxLayout *vboxLayout4;
    QCheckBox *theLineEnabledCheckBox;
    Q3HBoxLayout *hboxLayout3;
    QLabel *theLineThicknessLabel;
    QSpinBox *theLineThicknessInput;
    Q3HBoxLayout *hboxLayout4;
    QLabel *theLineColorLabel;
    Q3Frame *theLineColorFrame;
    QPushButton *theLineColorPushButton;
    QWidget *page2;
    Q3VBoxLayout *vboxLayout5;
    QCheckBox *thePolygonEnabledCheckBox;
    Q3HBoxLayout *hboxLayout5;
    QLabel *thePolygonThicknessLabel;
    QSpinBox *thePolygonThicknessInput;
    QCheckBox *thePolygonFilledCheckBox;
    Q3HBoxLayout *hboxLayout6;
    QLabel *thePolygonColorLabel;
    Q3Frame *thePolygonColorFrame;
    QPushButton *thePolygonColorPushButton;
    QWidget *page3;
    Q3VBoxLayout *vboxLayout6;
    QCheckBox *theFontEnabledCheckBox;
    Q3HBoxLayout *hboxLayout7;
    QLabel *theFontFamilyName;
    QComboBox *theFontFamilyNameComboBox;
    Q3HBoxLayout *hboxLayout8;
    QLabel *theFontStyleLabel;
    QComboBox *theFontStyleNameComboBox;
    Q3HBoxLayout *hboxLayout9;
    QLabel *thePointSizeLabel;
    QSpinBox *theFontPointSizeSpinBox;
    Q3HBoxLayout *hboxLayout10;
    QLabel *theFontScaleLabel;
    QLineEdit *theFontScaleInput;
    Q3HBoxLayout *hboxLayout11;
    QLabel *theFontShearLabel;
    QLineEdit *theFontShearInput;
    Q3HBoxLayout *hboxLayout12;
    QLabel *theFontRotationLabel;
    QLineEdit *theFontRotationInput;
    Q3HBoxLayout *hboxLayout13;
    QLabel *theFontColorLabel;
    Q3Frame *theFontColorFrame;
    QPushButton *theFontColorPushButton;
    Q3HBoxLayout *hboxLayout14;
    QCheckBox *theAutoApplyCheckBox;
    QSpacerItem *spacerItem;
    QPushButton *theApplyButton;
    QPushButton *theCloseButton;

    void setupUi(QDialog *ossimQtVectorEditorDialog)
    {
    ossimQtVectorEditorDialog->setObjectName(QString::fromUtf8("ossimQtVectorEditorDialog"));
    ossimQtVectorEditorDialog->resize(QSize(472, 386).expandedTo(ossimQtVectorEditorDialog->minimumSizeHint()));
    vboxLayout = new Q3VBoxLayout(ossimQtVectorEditorDialog);
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(11);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    theFeatureGroupBox = new Q3GroupBox(ossimQtVectorEditorDialog);
    theFeatureGroupBox->setObjectName(QString::fromUtf8("theFeatureGroupBox"));
    theFeatureGroupBox->setColumnLayout(0, Qt::Vertical);
    theFeatureGroupBox->layout()->setSpacing(6);
    theFeatureGroupBox->layout()->setMargin(11);
    hboxLayout = new Q3HBoxLayout(theFeatureGroupBox->layout());
    hboxLayout->setAlignment(Qt::AlignTop);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    vboxLayout1 = new Q3VBoxLayout();
    vboxLayout1->setSpacing(6);
    vboxLayout1->setMargin(0);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    theFeatureListbox = new Q3ListBox(theFeatureGroupBox);
    theFeatureListbox->setObjectName(QString::fromUtf8("theFeatureListbox"));

    vboxLayout1->addWidget(theFeatureListbox);

    gridLayout = new QGridLayout();
    gridLayout->setSpacing(6);
    gridLayout->setMargin(0);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    theDisableAllButton = new QPushButton(theFeatureGroupBox);
    theDisableAllButton->setObjectName(QString::fromUtf8("theDisableAllButton"));

    gridLayout->addWidget(theDisableAllButton, 0, 0, 1, 1);

    theEnableAllButton = new QPushButton(theFeatureGroupBox);
    theEnableAllButton->setObjectName(QString::fromUtf8("theEnableAllButton"));

    gridLayout->addWidget(theEnableAllButton, 1, 0, 1, 1);

    theMoveUpButton = new QPushButton(theFeatureGroupBox);
    theMoveUpButton->setObjectName(QString::fromUtf8("theMoveUpButton"));
    theMoveUpButton->setEnabled(false);

    gridLayout->addWidget(theMoveUpButton, 0, 1, 1, 1);

    theMoveDownButton = new QPushButton(theFeatureGroupBox);
    theMoveDownButton->setObjectName(QString::fromUtf8("theMoveDownButton"));
    theMoveDownButton->setEnabled(false);

    gridLayout->addWidget(theMoveDownButton, 1, 1, 1, 1);


    vboxLayout1->addLayout(gridLayout);


    hboxLayout->addLayout(vboxLayout1);

    theFeaturePropertiesGroupBox = new Q3GroupBox(theFeatureGroupBox);
    theFeaturePropertiesGroupBox->setObjectName(QString::fromUtf8("theFeaturePropertiesGroupBox"));
    theFeaturePropertiesGroupBox->setColumnLayout(0, Qt::Vertical);
    theFeaturePropertiesGroupBox->layout()->setSpacing(6);
    theFeaturePropertiesGroupBox->layout()->setMargin(11);
    vboxLayout2 = new Q3VBoxLayout(theFeaturePropertiesGroupBox->layout());
    vboxLayout2->setAlignment(Qt::AlignTop);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    thePropertyWidgetStack = new Q3WidgetStack(theFeaturePropertiesGroupBox);
    thePropertyWidgetStack->setObjectName(QString::fromUtf8("thePropertyWidgetStack"));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(7), static_cast<QSizePolicy::Policy>(7));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(thePropertyWidgetStack->sizePolicy().hasHeightForWidth());
    thePropertyWidgetStack->setSizePolicy(sizePolicy);
    page = new QWidget(thePropertyWidgetStack);
    page->setObjectName(QString::fromUtf8("page"));
    vboxLayout3 = new Q3VBoxLayout(page);
    vboxLayout3->setSpacing(6);
    vboxLayout3->setMargin(0);
    vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
    thePointEnabledCheckBox = new QCheckBox(page);
    thePointEnabledCheckBox->setObjectName(QString::fromUtf8("thePointEnabledCheckBox"));

    vboxLayout3->addWidget(thePointEnabledCheckBox);

    thePointFilledCheckBox = new QCheckBox(page);
    thePointFilledCheckBox->setObjectName(QString::fromUtf8("thePointFilledCheckBox"));

    vboxLayout3->addWidget(thePointFilledCheckBox);

    hboxLayout1 = new Q3HBoxLayout();
    hboxLayout1->setSpacing(6);
    hboxLayout1->setMargin(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    thePointRadiusLabel = new QLabel(page);
    thePointRadiusLabel->setObjectName(QString::fromUtf8("thePointRadiusLabel"));

    hboxLayout1->addWidget(thePointRadiusLabel);

    thePointRadiusInput = new QLineEdit(page);
    thePointRadiusInput->setObjectName(QString::fromUtf8("thePointRadiusInput"));

    hboxLayout1->addWidget(thePointRadiusInput);


    vboxLayout3->addLayout(hboxLayout1);

    hboxLayout2 = new Q3HBoxLayout();
    hboxLayout2->setSpacing(6);
    hboxLayout2->setMargin(0);
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    thePointColorLabel = new QLabel(page);
    thePointColorLabel->setObjectName(QString::fromUtf8("thePointColorLabel"));

    hboxLayout2->addWidget(thePointColorLabel);

    thePointColorFrame = new Q3Frame(page);
    thePointColorFrame->setObjectName(QString::fromUtf8("thePointColorFrame"));
    QPalette palette;
    palette.setColor(QPalette::Active, static_cast<QPalette::ColorRole>(0), QColor(0, 0, 0));
    palette.setColor(QPalette::Active, static_cast<QPalette::ColorRole>(1), QColor(192, 192, 192));
    palette.setColor(QPalette::Active, static_cast<QPalette::ColorRole>(2), QColor(255, 255, 255));
    palette.setColor(QPalette::Active, static_cast<QPalette::ColorRole>(3), QColor(223, 223, 223));
    palette.setColor(QPalette::Active, static_cast<QPalette::ColorRole>(4), QColor(96, 96, 96));
    palette.setColor(QPalette::Active, static_cast<QPalette::ColorRole>(5), QColor(128, 128, 128));
    palette.setColor(QPalette::Active, static_cast<QPalette::ColorRole>(6), QColor(0, 0, 0));
    palette.setColor(QPalette::Active, static_cast<QPalette::ColorRole>(7), QColor(255, 255, 255));
    palette.setColor(QPalette::Active, static_cast<QPalette::ColorRole>(8), QColor(0, 0, 0));
    palette.setColor(QPalette::Active, static_cast<QPalette::ColorRole>(9), QColor(255, 255, 255));
    palette.setColor(QPalette::Active, static_cast<QPalette::ColorRole>(10), QColor(255, 255, 255));
    palette.setColor(QPalette::Active, static_cast<QPalette::ColorRole>(11), QColor(0, 0, 0));
    palette.setColor(QPalette::Active, static_cast<QPalette::ColorRole>(12), QColor(0, 0, 128));
    palette.setColor(QPalette::Active, static_cast<QPalette::ColorRole>(13), QColor(255, 255, 255));
    palette.setColor(QPalette::Active, static_cast<QPalette::ColorRole>(14), QColor(0, 0, 0));
    palette.setColor(QPalette::Active, static_cast<QPalette::ColorRole>(15), QColor(0, 0, 0));
    palette.setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(0), QColor(0, 0, 0));
    palette.setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(1), QColor(192, 192, 192));
    palette.setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(2), QColor(255, 255, 255));
    palette.setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(3), QColor(220, 220, 220));
    palette.setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(4), QColor(96, 96, 96));
    palette.setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(5), QColor(128, 128, 128));
    palette.setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(6), QColor(0, 0, 0));
    palette.setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(7), QColor(255, 255, 255));
    palette.setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(8), QColor(0, 0, 0));
    palette.setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(9), QColor(255, 255, 255));
    palette.setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(10), QColor(255, 255, 255));
    palette.setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(11), QColor(0, 0, 0));
    palette.setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(12), QColor(0, 0, 128));
    palette.setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(13), QColor(255, 255, 255));
    palette.setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(14), QColor(0, 0, 255));
    palette.setColor(QPalette::Inactive, static_cast<QPalette::ColorRole>(15), QColor(255, 0, 255));
    palette.setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(0), QColor(128, 128, 128));
    palette.setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(1), QColor(192, 192, 192));
    palette.setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(2), QColor(255, 255, 255));
    palette.setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(3), QColor(220, 220, 220));
    palette.setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(4), QColor(96, 96, 96));
    palette.setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(5), QColor(128, 128, 128));
    palette.setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(6), QColor(128, 128, 128));
    palette.setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(7), QColor(255, 255, 255));
    palette.setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(8), QColor(128, 128, 128));
    palette.setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(9), QColor(255, 255, 255));
    palette.setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(10), QColor(255, 255, 255));
    palette.setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(11), QColor(0, 0, 0));
    palette.setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(12), QColor(0, 0, 128));
    palette.setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(13), QColor(255, 255, 255));
    palette.setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(14), QColor(0, 0, 255));
    palette.setColor(QPalette::Disabled, static_cast<QPalette::ColorRole>(15), QColor(255, 0, 255));
    thePointColorFrame->setPalette(palette);
    thePointColorFrame->setFrameShape(Q3Frame::NoFrame);
    thePointColorFrame->setFrameShadow(Q3Frame::Raised);

    hboxLayout2->addWidget(thePointColorFrame);

    thePointColorButton = new QPushButton(page);
    thePointColorButton->setObjectName(QString::fromUtf8("thePointColorButton"));
    QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(thePointColorButton->sizePolicy().hasHeightForWidth());
    thePointColorButton->setSizePolicy(sizePolicy1);
    thePointColorButton->setMaximumSize(QSize(32, 32767));

    hboxLayout2->addWidget(thePointColorButton);


    vboxLayout3->addLayout(hboxLayout2);

    thePropertyWidgetStack->addWidget(page, 0);
    page1 = new QWidget(thePropertyWidgetStack);
    page1->setObjectName(QString::fromUtf8("page1"));
    vboxLayout4 = new Q3VBoxLayout(page1);
    vboxLayout4->setSpacing(6);
    vboxLayout4->setMargin(0);
    vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
    theLineEnabledCheckBox = new QCheckBox(page1);
    theLineEnabledCheckBox->setObjectName(QString::fromUtf8("theLineEnabledCheckBox"));

    vboxLayout4->addWidget(theLineEnabledCheckBox);

    hboxLayout3 = new Q3HBoxLayout();
    hboxLayout3->setSpacing(6);
    hboxLayout3->setMargin(0);
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
    theLineThicknessLabel = new QLabel(page1);
    theLineThicknessLabel->setObjectName(QString::fromUtf8("theLineThicknessLabel"));

    hboxLayout3->addWidget(theLineThicknessLabel);

    theLineThicknessInput = new QSpinBox(page1);
    theLineThicknessInput->setObjectName(QString::fromUtf8("theLineThicknessInput"));
    theLineThicknessInput->setMinimum(1);

    hboxLayout3->addWidget(theLineThicknessInput);


    vboxLayout4->addLayout(hboxLayout3);

    hboxLayout4 = new Q3HBoxLayout();
    hboxLayout4->setSpacing(6);
    hboxLayout4->setMargin(0);
    hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
    theLineColorLabel = new QLabel(page1);
    theLineColorLabel->setObjectName(QString::fromUtf8("theLineColorLabel"));

    hboxLayout4->addWidget(theLineColorLabel);

    theLineColorFrame = new Q3Frame(page1);
    theLineColorFrame->setObjectName(QString::fromUtf8("theLineColorFrame"));
    theLineColorFrame->setFrameShape(Q3Frame::NoFrame);
    theLineColorFrame->setFrameShadow(Q3Frame::Raised);

    hboxLayout4->addWidget(theLineColorFrame);

    theLineColorPushButton = new QPushButton(page1);
    theLineColorPushButton->setObjectName(QString::fromUtf8("theLineColorPushButton"));
    QSizePolicy sizePolicy2(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(theLineColorPushButton->sizePolicy().hasHeightForWidth());
    theLineColorPushButton->setSizePolicy(sizePolicy2);
    theLineColorPushButton->setMaximumSize(QSize(32, 32767));

    hboxLayout4->addWidget(theLineColorPushButton);


    vboxLayout4->addLayout(hboxLayout4);

    thePropertyWidgetStack->addWidget(page1, 1);
    page2 = new QWidget(thePropertyWidgetStack);
    page2->setObjectName(QString::fromUtf8("page2"));
    vboxLayout5 = new Q3VBoxLayout(page2);
    vboxLayout5->setSpacing(6);
    vboxLayout5->setMargin(0);
    vboxLayout5->setObjectName(QString::fromUtf8("vboxLayout5"));
    thePolygonEnabledCheckBox = new QCheckBox(page2);
    thePolygonEnabledCheckBox->setObjectName(QString::fromUtf8("thePolygonEnabledCheckBox"));

    vboxLayout5->addWidget(thePolygonEnabledCheckBox);

    hboxLayout5 = new Q3HBoxLayout();
    hboxLayout5->setSpacing(6);
    hboxLayout5->setMargin(0);
    hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
    thePolygonThicknessLabel = new QLabel(page2);
    thePolygonThicknessLabel->setObjectName(QString::fromUtf8("thePolygonThicknessLabel"));

    hboxLayout5->addWidget(thePolygonThicknessLabel);

    thePolygonThicknessInput = new QSpinBox(page2);
    thePolygonThicknessInput->setObjectName(QString::fromUtf8("thePolygonThicknessInput"));
    thePolygonThicknessInput->setMinimum(1);

    hboxLayout5->addWidget(thePolygonThicknessInput);


    vboxLayout5->addLayout(hboxLayout5);

    thePolygonFilledCheckBox = new QCheckBox(page2);
    thePolygonFilledCheckBox->setObjectName(QString::fromUtf8("thePolygonFilledCheckBox"));

    vboxLayout5->addWidget(thePolygonFilledCheckBox);

    hboxLayout6 = new Q3HBoxLayout();
    hboxLayout6->setSpacing(6);
    hboxLayout6->setMargin(0);
    hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
    thePolygonColorLabel = new QLabel(page2);
    thePolygonColorLabel->setObjectName(QString::fromUtf8("thePolygonColorLabel"));

    hboxLayout6->addWidget(thePolygonColorLabel);

    thePolygonColorFrame = new Q3Frame(page2);
    thePolygonColorFrame->setObjectName(QString::fromUtf8("thePolygonColorFrame"));
    thePolygonColorFrame->setFrameShape(Q3Frame::NoFrame);
    thePolygonColorFrame->setFrameShadow(Q3Frame::Raised);

    hboxLayout6->addWidget(thePolygonColorFrame);

    thePolygonColorPushButton = new QPushButton(page2);
    thePolygonColorPushButton->setObjectName(QString::fromUtf8("thePolygonColorPushButton"));
    QSizePolicy sizePolicy3(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(thePolygonColorPushButton->sizePolicy().hasHeightForWidth());
    thePolygonColorPushButton->setSizePolicy(sizePolicy3);
    thePolygonColorPushButton->setMaximumSize(QSize(32, 32767));

    hboxLayout6->addWidget(thePolygonColorPushButton);


    vboxLayout5->addLayout(hboxLayout6);

    thePropertyWidgetStack->addWidget(page2, 2);
    page3 = new QWidget(thePropertyWidgetStack);
    page3->setObjectName(QString::fromUtf8("page3"));
    vboxLayout6 = new Q3VBoxLayout(page3);
    vboxLayout6->setSpacing(6);
    vboxLayout6->setMargin(0);
    vboxLayout6->setObjectName(QString::fromUtf8("vboxLayout6"));
    theFontEnabledCheckBox = new QCheckBox(page3);
    theFontEnabledCheckBox->setObjectName(QString::fromUtf8("theFontEnabledCheckBox"));

    vboxLayout6->addWidget(theFontEnabledCheckBox);

    hboxLayout7 = new Q3HBoxLayout();
    hboxLayout7->setSpacing(6);
    hboxLayout7->setMargin(0);
    hboxLayout7->setObjectName(QString::fromUtf8("hboxLayout7"));
    theFontFamilyName = new QLabel(page3);
    theFontFamilyName->setObjectName(QString::fromUtf8("theFontFamilyName"));

    hboxLayout7->addWidget(theFontFamilyName);

    theFontFamilyNameComboBox = new QComboBox(page3);
    theFontFamilyNameComboBox->setObjectName(QString::fromUtf8("theFontFamilyNameComboBox"));

    hboxLayout7->addWidget(theFontFamilyNameComboBox);


    vboxLayout6->addLayout(hboxLayout7);

    hboxLayout8 = new Q3HBoxLayout();
    hboxLayout8->setSpacing(6);
    hboxLayout8->setMargin(0);
    hboxLayout8->setObjectName(QString::fromUtf8("hboxLayout8"));
    theFontStyleLabel = new QLabel(page3);
    theFontStyleLabel->setObjectName(QString::fromUtf8("theFontStyleLabel"));

    hboxLayout8->addWidget(theFontStyleLabel);

    theFontStyleNameComboBox = new QComboBox(page3);
    theFontStyleNameComboBox->setObjectName(QString::fromUtf8("theFontStyleNameComboBox"));

    hboxLayout8->addWidget(theFontStyleNameComboBox);


    vboxLayout6->addLayout(hboxLayout8);

    hboxLayout9 = new Q3HBoxLayout();
    hboxLayout9->setSpacing(6);
    hboxLayout9->setMargin(0);
    hboxLayout9->setObjectName(QString::fromUtf8("hboxLayout9"));
    thePointSizeLabel = new QLabel(page3);
    thePointSizeLabel->setObjectName(QString::fromUtf8("thePointSizeLabel"));

    hboxLayout9->addWidget(thePointSizeLabel);

    theFontPointSizeSpinBox = new QSpinBox(page3);
    theFontPointSizeSpinBox->setObjectName(QString::fromUtf8("theFontPointSizeSpinBox"));
    theFontPointSizeSpinBox->setMaximum(200);
    theFontPointSizeSpinBox->setMinimum(1);

    hboxLayout9->addWidget(theFontPointSizeSpinBox);


    vboxLayout6->addLayout(hboxLayout9);

    hboxLayout10 = new Q3HBoxLayout();
    hboxLayout10->setSpacing(6);
    hboxLayout10->setMargin(0);
    hboxLayout10->setObjectName(QString::fromUtf8("hboxLayout10"));
    theFontScaleLabel = new QLabel(page3);
    theFontScaleLabel->setObjectName(QString::fromUtf8("theFontScaleLabel"));

    hboxLayout10->addWidget(theFontScaleLabel);

    theFontScaleInput = new QLineEdit(page3);
    theFontScaleInput->setObjectName(QString::fromUtf8("theFontScaleInput"));

    hboxLayout10->addWidget(theFontScaleInput);


    vboxLayout6->addLayout(hboxLayout10);

    hboxLayout11 = new Q3HBoxLayout();
    hboxLayout11->setSpacing(6);
    hboxLayout11->setMargin(0);
    hboxLayout11->setObjectName(QString::fromUtf8("hboxLayout11"));
    theFontShearLabel = new QLabel(page3);
    theFontShearLabel->setObjectName(QString::fromUtf8("theFontShearLabel"));

    hboxLayout11->addWidget(theFontShearLabel);

    theFontShearInput = new QLineEdit(page3);
    theFontShearInput->setObjectName(QString::fromUtf8("theFontShearInput"));

    hboxLayout11->addWidget(theFontShearInput);


    vboxLayout6->addLayout(hboxLayout11);

    hboxLayout12 = new Q3HBoxLayout();
    hboxLayout12->setSpacing(6);
    hboxLayout12->setMargin(0);
    hboxLayout12->setObjectName(QString::fromUtf8("hboxLayout12"));
    theFontRotationLabel = new QLabel(page3);
    theFontRotationLabel->setObjectName(QString::fromUtf8("theFontRotationLabel"));

    hboxLayout12->addWidget(theFontRotationLabel);

    theFontRotationInput = new QLineEdit(page3);
    theFontRotationInput->setObjectName(QString::fromUtf8("theFontRotationInput"));

    hboxLayout12->addWidget(theFontRotationInput);


    vboxLayout6->addLayout(hboxLayout12);

    hboxLayout13 = new Q3HBoxLayout();
    hboxLayout13->setSpacing(6);
    hboxLayout13->setMargin(0);
    hboxLayout13->setObjectName(QString::fromUtf8("hboxLayout13"));
    theFontColorLabel = new QLabel(page3);
    theFontColorLabel->setObjectName(QString::fromUtf8("theFontColorLabel"));

    hboxLayout13->addWidget(theFontColorLabel);

    theFontColorFrame = new Q3Frame(page3);
    theFontColorFrame->setObjectName(QString::fromUtf8("theFontColorFrame"));
    theFontColorFrame->setFrameShape(Q3Frame::NoFrame);
    theFontColorFrame->setFrameShadow(Q3Frame::Raised);

    hboxLayout13->addWidget(theFontColorFrame);

    theFontColorPushButton = new QPushButton(page3);
    theFontColorPushButton->setObjectName(QString::fromUtf8("theFontColorPushButton"));
    QSizePolicy sizePolicy4(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
    sizePolicy4.setHorizontalStretch(0);
    sizePolicy4.setVerticalStretch(0);
    sizePolicy4.setHeightForWidth(theFontColorPushButton->sizePolicy().hasHeightForWidth());
    theFontColorPushButton->setSizePolicy(sizePolicy4);
    theFontColorPushButton->setMaximumSize(QSize(32, 32767));

    hboxLayout13->addWidget(theFontColorPushButton);


    vboxLayout6->addLayout(hboxLayout13);

    thePropertyWidgetStack->addWidget(page3, 3);

    vboxLayout2->addWidget(thePropertyWidgetStack);


    hboxLayout->addWidget(theFeaturePropertiesGroupBox);


    vboxLayout->addWidget(theFeatureGroupBox);

    hboxLayout14 = new Q3HBoxLayout();
    hboxLayout14->setSpacing(6);
    hboxLayout14->setMargin(0);
    hboxLayout14->setObjectName(QString::fromUtf8("hboxLayout14"));
    theAutoApplyCheckBox = new QCheckBox(ossimQtVectorEditorDialog);
    theAutoApplyCheckBox->setObjectName(QString::fromUtf8("theAutoApplyCheckBox"));
    theAutoApplyCheckBox->setChecked(true);

    hboxLayout14->addWidget(theAutoApplyCheckBox);

    spacerItem = new QSpacerItem(31, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout14->addItem(spacerItem);

    theApplyButton = new QPushButton(ossimQtVectorEditorDialog);
    theApplyButton->setObjectName(QString::fromUtf8("theApplyButton"));

    hboxLayout14->addWidget(theApplyButton);

    theCloseButton = new QPushButton(ossimQtVectorEditorDialog);
    theCloseButton->setObjectName(QString::fromUtf8("theCloseButton"));

    hboxLayout14->addWidget(theCloseButton);


    vboxLayout->addLayout(hboxLayout14);

    retranslateUi(ossimQtVectorEditorDialog);

    QMetaObject::connectSlotsByName(ossimQtVectorEditorDialog);
    } // setupUi

    void retranslateUi(QDialog *ossimQtVectorEditorDialog)
    {
    ossimQtVectorEditorDialog->setWindowTitle(QApplication::translate("ossimQtVectorEditorDialog", "Vpf Editor", 0, QApplication::UnicodeUTF8));
    theFeatureGroupBox->setTitle(QApplication::translate("ossimQtVectorEditorDialog", "Feature", 0, QApplication::UnicodeUTF8));
    theDisableAllButton->setText(QApplication::translate("ossimQtVectorEditorDialog", "Disable all", 0, QApplication::UnicodeUTF8));
    theEnableAllButton->setText(QApplication::translate("ossimQtVectorEditorDialog", "Enable all", 0, QApplication::UnicodeUTF8));
    theMoveUpButton->setText(QApplication::translate("ossimQtVectorEditorDialog", "Move up", 0, QApplication::UnicodeUTF8));
    theMoveDownButton->setText(QApplication::translate("ossimQtVectorEditorDialog", "Move down", 0, QApplication::UnicodeUTF8));
    theFeaturePropertiesGroupBox->setTitle(QApplication::translate("ossimQtVectorEditorDialog", "Properties", 0, QApplication::UnicodeUTF8));
    thePointEnabledCheckBox->setText(QApplication::translate("ossimQtVectorEditorDialog", "Enabled", 0, QApplication::UnicodeUTF8));
    thePointFilledCheckBox->setText(QApplication::translate("ossimQtVectorEditorDialog", "Filled", 0, QApplication::UnicodeUTF8));
    thePointRadiusLabel->setText(QApplication::translate("ossimQtVectorEditorDialog", "Radius", 0, QApplication::UnicodeUTF8));
    thePointColorLabel->setText(QApplication::translate("ossimQtVectorEditorDialog", "Color", 0, QApplication::UnicodeUTF8));
    thePointColorButton->setText(QApplication::translate("ossimQtVectorEditorDialog", "...", 0, QApplication::UnicodeUTF8));
    theLineEnabledCheckBox->setText(QApplication::translate("ossimQtVectorEditorDialog", "Enabled", 0, QApplication::UnicodeUTF8));
    theLineThicknessLabel->setText(QApplication::translate("ossimQtVectorEditorDialog", "Thickness:", 0, QApplication::UnicodeUTF8));
    theLineColorLabel->setText(QApplication::translate("ossimQtVectorEditorDialog", "Color", 0, QApplication::UnicodeUTF8));
    theLineColorPushButton->setText(QApplication::translate("ossimQtVectorEditorDialog", "...", 0, QApplication::UnicodeUTF8));
    thePolygonEnabledCheckBox->setText(QApplication::translate("ossimQtVectorEditorDialog", "Enabled", 0, QApplication::UnicodeUTF8));
    thePolygonThicknessLabel->setText(QApplication::translate("ossimQtVectorEditorDialog", "Thickness:", 0, QApplication::UnicodeUTF8));
    thePolygonFilledCheckBox->setText(QApplication::translate("ossimQtVectorEditorDialog", "Filled", 0, QApplication::UnicodeUTF8));
    thePolygonColorLabel->setText(QApplication::translate("ossimQtVectorEditorDialog", "Color", 0, QApplication::UnicodeUTF8));
    thePolygonColorPushButton->setText(QApplication::translate("ossimQtVectorEditorDialog", "...", 0, QApplication::UnicodeUTF8));
    theFontEnabledCheckBox->setText(QApplication::translate("ossimQtVectorEditorDialog", "Enabled", 0, QApplication::UnicodeUTF8));
    theFontFamilyName->setText(QApplication::translate("ossimQtVectorEditorDialog", "Family:", 0, QApplication::UnicodeUTF8));
    theFontStyleLabel->setText(QApplication::translate("ossimQtVectorEditorDialog", "Style:", 0, QApplication::UnicodeUTF8));
    thePointSizeLabel->setText(QApplication::translate("ossimQtVectorEditorDialog", "Point size:", 0, QApplication::UnicodeUTF8));
    theFontScaleLabel->setText(QApplication::translate("ossimQtVectorEditorDialog", "Scale (x y)", 0, QApplication::UnicodeUTF8));
    theFontShearLabel->setText(QApplication::translate("ossimQtVectorEditorDialog", "Shear (x y)", 0, QApplication::UnicodeUTF8));
    theFontRotationLabel->setText(QApplication::translate("ossimQtVectorEditorDialog", "Rotation:", 0, QApplication::UnicodeUTF8));
    theFontColorLabel->setText(QApplication::translate("ossimQtVectorEditorDialog", "Color:", 0, QApplication::UnicodeUTF8));
    theFontColorPushButton->setText(QApplication::translate("ossimQtVectorEditorDialog", "...", 0, QApplication::UnicodeUTF8));
    theAutoApplyCheckBox->setText(QApplication::translate("ossimQtVectorEditorDialog", "Auto apply", 0, QApplication::UnicodeUTF8));
    theApplyButton->setText(QApplication::translate("ossimQtVectorEditorDialog", "Apply", 0, QApplication::UnicodeUTF8));
    theCloseButton->setText(QApplication::translate("ossimQtVectorEditorDialog", "Close", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ossimQtVectorEditorDialog);
    } // retranslateUi

};

namespace Ui {
    class ossimQtVectorEditorDialog: public Ui_ossimQtVectorEditorDialog {};
} // namespace Ui

class ossimQtVectorEditorDialog : public QDialog, public Ui::ossimQtVectorEditorDialog
{
    Q_OBJECT

public:
    ossimQtVectorEditorDialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0);
    ~ossimQtVectorEditorDialog();

    virtual ossimQtVectorEditorDialogController * controller();

public slots:
    virtual void theApplyButton_clicked();
    virtual void theFeatureListbox_selectionChanged( Q3ListBoxItem * li );
    virtual void theCloseButton_clicked();
    virtual void thePolygonColorPushButton_clicked();
    virtual void theLineColorPushButton_clicked();
    virtual void thePointColorButton_clicked();
    virtual void theLineEnabledCheckBox_toggled( bool );
    virtual void thePointEnabledCheckBox_toggled( bool );
    virtual void theFontEnabledCheckBox_toggled( bool );
    virtual void thePolygonEnabledCheckBox_clicked();
    virtual void theLineThicknessInput_valueChanged( int value );
    virtual void thePolygonThicknessInput_valueChanged( int );
    virtual void thePolygonFilledCheckBox_toggled( bool );
    virtual void thePointFilledCheckbox_toggled( bool );
    virtual void theFontPointSizeSpinBox_valueChanged( int );
    virtual void theFontColorPushButton_clicked();
    virtual void theFontScaleInput_textChanged( const QString & );
    virtual void theDisableAllButton_clicked();
    virtual void theEnableAllButton_clicked();
    virtual void theFontFamilyNameComboBox_activated( const QString & );
    virtual void theFontStyleNameComboBox_activated( const QString & );
    virtual void theFontShearInput_textChanged( const QString & );
    virtual void theFontRotationInput_textChanged( const QString & );

protected:
    ossimQtVectorEditorDialogController* theController;

protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // OSSIMQTVECTOREDITORDIALOG_H
