#ifndef OSSIMQTCOMBINEREDITORDIALOG_H
#define OSSIMQTCOMBINEREDITORDIALOG_H

#include <QtCore/QVariant>
//Added by qt3to4:
#include <Qt3Support/Q3HBoxLayout>
#include <Qt3Support/Q3Frame>
#include <QtGui/QPixmap>
#include <QtGui/QLabel>
#include <Qt3Support/Q3VBoxLayout>

class ossimQtCombinerEditorDialogController;

#include <Qt3Support/Q3ListBox>
#include <Qt3Support/Q3WidgetStack>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "ossimQtLayerEditor.h"

class Ui_ossimQtCombinerEditorDialog
{
public:
    Q3VBoxLayout *vboxLayout;
    ossimQtLayerEditor *theLayerEditor;
    Q3WidgetStack *theWidgetStack;
    QWidget *blend;
    Q3VBoxLayout *vboxLayout1;
    Q3HBoxLayout *hboxLayout;
    Q3ListBox *theInputWeightListBox;
    Q3VBoxLayout *vboxLayout2;
    QSpacerItem *spacerItem;
    QSlider *theBlendWeightSlider;
    QSpacerItem *spacerItem1;
    QWidget *hillshade;
    Q3VBoxLayout *vboxLayout3;
    Q3HBoxLayout *hboxLayout1;
    QLabel *theElevAngleLabel;
    QLineEdit *theElevationAngleInput;
    Q3HBoxLayout *hboxLayout2;
    QLabel *theAzimuthAngleLabel;
    QLineEdit *theAzimuthAngleInput;
    Q3HBoxLayout *hboxLayout3;
    QLabel *theSmoothnessFactor;
    QLineEdit *theSmoothnessFactorInput;
    Q3HBoxLayout *hboxLayout4;
    QSpacerItem *spacerItem2;
    QCheckBox *theAutoApplyCheckBox;
    QPushButton *theApplyButton;
    QPushButton *theResetButton;
    QPushButton *theCloseButton;
    QSpacerItem *spacerItem3;
    Q3Frame *line1;

    void setupUi(QDialog *ossimQtCombinerEditorDialog)
    {
    ossimQtCombinerEditorDialog->setObjectName(QString::fromUtf8("ossimQtCombinerEditorDialog"));
    ossimQtCombinerEditorDialog->resize(QSize(583, 381).expandedTo(ossimQtCombinerEditorDialog->minimumSizeHint()));
    vboxLayout = new Q3VBoxLayout(ossimQtCombinerEditorDialog);
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(11);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    theLayerEditor = new ossimQtLayerEditor(ossimQtCombinerEditorDialog);
    theLayerEditor->setObjectName(QString::fromUtf8("theLayerEditor"));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(1));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(theLayerEditor->sizePolicy().hasHeightForWidth());
    theLayerEditor->setSizePolicy(sizePolicy);
    theLayerEditor->setMinimumSize(QSize(0, 0));
    theLayerEditor->setMaximumSize(QSize(32767, 32767));

    vboxLayout->addWidget(theLayerEditor);

    theWidgetStack = new Q3WidgetStack(ossimQtCombinerEditorDialog);
    theWidgetStack->setObjectName(QString::fromUtf8("theWidgetStack"));
    blend = new QWidget(theWidgetStack);
    blend->setObjectName(QString::fromUtf8("blend"));
    vboxLayout1 = new Q3VBoxLayout(blend);
    vboxLayout1->setSpacing(6);
    vboxLayout1->setMargin(0);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    hboxLayout = new Q3HBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    theInputWeightListBox = new Q3ListBox(blend);
    theInputWeightListBox->setObjectName(QString::fromUtf8("theInputWeightListBox"));

    hboxLayout->addWidget(theInputWeightListBox);

    vboxLayout2 = new Q3VBoxLayout();
    vboxLayout2->setSpacing(6);
    vboxLayout2->setMargin(0);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    spacerItem = new QSpacerItem(20, 21, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout2->addItem(spacerItem);

    theBlendWeightSlider = new QSlider(blend);
    theBlendWeightSlider->setObjectName(QString::fromUtf8("theBlendWeightSlider"));
    theBlendWeightSlider->setMaximum(100);
    theBlendWeightSlider->setOrientation(Qt::Horizontal);

    vboxLayout2->addWidget(theBlendWeightSlider);

    spacerItem1 = new QSpacerItem(20, 21, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout2->addItem(spacerItem1);


    hboxLayout->addLayout(vboxLayout2);


    vboxLayout1->addLayout(hboxLayout);

    theWidgetStack->addWidget(blend, 0);
    hillshade = new QWidget(theWidgetStack);
    hillshade->setObjectName(QString::fromUtf8("hillshade"));
    vboxLayout3 = new Q3VBoxLayout(hillshade);
    vboxLayout3->setSpacing(6);
    vboxLayout3->setMargin(0);
    vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
    hboxLayout1 = new Q3HBoxLayout();
    hboxLayout1->setSpacing(6);
    hboxLayout1->setMargin(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    theElevAngleLabel = new QLabel(hillshade);
    theElevAngleLabel->setObjectName(QString::fromUtf8("theElevAngleLabel"));

    hboxLayout1->addWidget(theElevAngleLabel);

    theElevationAngleInput = new QLineEdit(hillshade);
    theElevationAngleInput->setObjectName(QString::fromUtf8("theElevationAngleInput"));
    QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(0));
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(theElevationAngleInput->sizePolicy().hasHeightForWidth());
    theElevationAngleInput->setSizePolicy(sizePolicy1);

    hboxLayout1->addWidget(theElevationAngleInput);


    vboxLayout3->addLayout(hboxLayout1);

    hboxLayout2 = new Q3HBoxLayout();
    hboxLayout2->setSpacing(6);
    hboxLayout2->setMargin(0);
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    theAzimuthAngleLabel = new QLabel(hillshade);
    theAzimuthAngleLabel->setObjectName(QString::fromUtf8("theAzimuthAngleLabel"));

    hboxLayout2->addWidget(theAzimuthAngleLabel);

    theAzimuthAngleInput = new QLineEdit(hillshade);
    theAzimuthAngleInput->setObjectName(QString::fromUtf8("theAzimuthAngleInput"));
    QSizePolicy sizePolicy2(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(0));
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(theAzimuthAngleInput->sizePolicy().hasHeightForWidth());
    theAzimuthAngleInput->setSizePolicy(sizePolicy2);

    hboxLayout2->addWidget(theAzimuthAngleInput);


    vboxLayout3->addLayout(hboxLayout2);

    hboxLayout3 = new Q3HBoxLayout();
    hboxLayout3->setSpacing(6);
    hboxLayout3->setMargin(0);
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
    theSmoothnessFactor = new QLabel(hillshade);
    theSmoothnessFactor->setObjectName(QString::fromUtf8("theSmoothnessFactor"));

    hboxLayout3->addWidget(theSmoothnessFactor);

    theSmoothnessFactorInput = new QLineEdit(hillshade);
    theSmoothnessFactorInput->setObjectName(QString::fromUtf8("theSmoothnessFactorInput"));
    QSizePolicy sizePolicy3(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(0));
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(theSmoothnessFactorInput->sizePolicy().hasHeightForWidth());
    theSmoothnessFactorInput->setSizePolicy(sizePolicy3);

    hboxLayout3->addWidget(theSmoothnessFactorInput);


    vboxLayout3->addLayout(hboxLayout3);

    theWidgetStack->addWidget(hillshade, 1);

    vboxLayout->addWidget(theWidgetStack);

    hboxLayout4 = new Q3HBoxLayout();
    hboxLayout4->setSpacing(6);
    hboxLayout4->setMargin(0);
    hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
    spacerItem2 = new QSpacerItem(60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout4->addItem(spacerItem2);

    theAutoApplyCheckBox = new QCheckBox(ossimQtCombinerEditorDialog);
    theAutoApplyCheckBox->setObjectName(QString::fromUtf8("theAutoApplyCheckBox"));
    theAutoApplyCheckBox->setChecked(true);

    hboxLayout4->addWidget(theAutoApplyCheckBox);

    theApplyButton = new QPushButton(ossimQtCombinerEditorDialog);
    theApplyButton->setObjectName(QString::fromUtf8("theApplyButton"));

    hboxLayout4->addWidget(theApplyButton);

    theResetButton = new QPushButton(ossimQtCombinerEditorDialog);
    theResetButton->setObjectName(QString::fromUtf8("theResetButton"));
    theResetButton->setEnabled(false);

    hboxLayout4->addWidget(theResetButton);

    theCloseButton = new QPushButton(ossimQtCombinerEditorDialog);
    theCloseButton->setObjectName(QString::fromUtf8("theCloseButton"));

    hboxLayout4->addWidget(theCloseButton);

    spacerItem3 = new QSpacerItem(60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout4->addItem(spacerItem3);


    vboxLayout->addLayout(hboxLayout4);

    line1 = new Q3Frame(ossimQtCombinerEditorDialog);
    line1->setObjectName(QString::fromUtf8("line1"));
    line1->setFrameShape(Q3Frame::HLine);
    line1->setFrameShadow(Q3Frame::Sunken);

    vboxLayout->addWidget(line1);

    retranslateUi(ossimQtCombinerEditorDialog);

    QMetaObject::connectSlotsByName(ossimQtCombinerEditorDialog);
    } // setupUi

    void retranslateUi(QDialog *ossimQtCombinerEditorDialog)
    {
    ossimQtCombinerEditorDialog->setWindowTitle(QApplication::translate("ossimQtCombinerEditorDialog", "Combiner Editor", 0, QApplication::UnicodeUTF8));
    theElevAngleLabel->setText(QApplication::translate("ossimQtCombinerEditorDialog", "Elevation angle:", 0, QApplication::UnicodeUTF8));
    theAzimuthAngleLabel->setText(QApplication::translate("ossimQtCombinerEditorDialog", "Azimuth angle", 0, QApplication::UnicodeUTF8));
    theSmoothnessFactor->setText(QApplication::translate("ossimQtCombinerEditorDialog", "Smoothness factor:", 0, QApplication::UnicodeUTF8));
    theAutoApplyCheckBox->setText(QApplication::translate("ossimQtCombinerEditorDialog", "Auto apply", 0, QApplication::UnicodeUTF8));
    theApplyButton->setText(QApplication::translate("ossimQtCombinerEditorDialog", "&Apply", 0, QApplication::UnicodeUTF8));
    theResetButton->setText(QApplication::translate("ossimQtCombinerEditorDialog", "&Reset", 0, QApplication::UnicodeUTF8));
    theCloseButton->setText(QApplication::translate("ossimQtCombinerEditorDialog", "&Close", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ossimQtCombinerEditorDialog);
    } // retranslateUi


protected:
    enum IconID
    {
        image0_ID,
        unknown_ID
    };
    static QPixmap icon(IconID id)
    {
    static const char* const image0_data[] = { 
"0 0 0 1"};


    switch (id) {
        case image0_ID: return QPixmap((const char**)image0_data);
        default: return QPixmap();
    } // switch
    } // icon

};

namespace Ui {
    class ossimQtCombinerEditorDialog: public Ui_ossimQtCombinerEditorDialog {};
} // namespace Ui

class ossimQtCombinerEditorDialog : public QDialog, public Ui::ossimQtCombinerEditorDialog
{
    Q_OBJECT

public:
    ossimQtCombinerEditorDialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0);
    ~ossimQtCombinerEditorDialog();

    virtual ossimQtCombinerEditorDialogController * controller();

public slots:
    virtual void theCloseButton_clicked();
    virtual void theApplyButton_clicked();
    virtual void theResetButton_clicked();
    virtual void theAutoApplyCheckBox_toggled( bool value );
    virtual void propertyValuesChanged();

signals:
    void apply();
    void reset();

protected:
    ossimQtCombinerEditorDialogController* theController;

protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

};

#endif // OSSIMQTCOMBINEREDITORDIALOG_H
