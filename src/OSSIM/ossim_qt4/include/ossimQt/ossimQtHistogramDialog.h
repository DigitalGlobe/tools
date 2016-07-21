#ifndef OSSIMQTHISTOGRAMDIALOG_H
#define OSSIMQTHISTOGRAMDIALOG_H

#include <QtCore/QVariant>
//Added by qt3to4:
#include <Qt3Support/Q3HBoxLayout>
#include <Qt3Support/Q3GridLayout>
#include <Qt3Support/Q3Frame>
#include <QtGui/QPixmap>
#include <QtGui/QLabel>

class ossimQtHistogramController;
class ossimHistogramRemapper;

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
// #include <QtGui/Q3ButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
// #include <QtGui/Q3Frame>
// #include <QtGui/Q3GridLayout>
// #include <QtGui/Q3HBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include "ossimQtHistogramWidget.h"

class Ui_ossimQtHistogramDialog
{
public:
    Q3GridLayout *gridLayout;
    Q3HBoxLayout *hboxLayout;
    QLineEdit *theLowClipPercentLineEdit;
    QLabel *textLabel4;
    QSpacerItem *spacerItem;
    Q3HBoxLayout *hboxLayout1;
    QSpacerItem *spacerItem1;
    QLabel *textLabel3;
    QComboBox *theStretchModeComboBox;
    Q3HBoxLayout *hboxLayout2;
    QLineEdit *theHighClipPercentLineEdit;
    QLabel *textLabel5;
    QSpacerItem *spacerItem2;
    Q3HBoxLayout *hboxLayout3;
    QSpacerItem *spacerItem3;
    QLabel *theChannelTextLabel;
    QComboBox *theBandComboBox;
    Q3HBoxLayout *hboxLayout4;
    QLineEdit *theLowClipValueLineEdit;
    QLabel *textLabel6;
    QSpacerItem *spacerItem4;
    ossimQtHistogramWidget *theHistogramWidget;
    Q3HBoxLayout *hboxLayout5;
    QLineEdit *theHighClipValueLineEdit;
    QLabel *textLabel7;
    QSpacerItem *spacerItem5;
    Q3HBoxLayout *hboxLayout6;
    QLineEdit *theMidPointLineEdit;
    QLabel *textLabel8;
    QSpacerItem *spacerItem6;
    Q3HBoxLayout *hboxLayout7;
    QLineEdit *theOutputMinValue;
    QLabel *textLabel9;
    QSpacerItem *spacerItem7;
    Q3HBoxLayout *hboxLayout8;
    QLineEdit *theOutputMaxValue;
    QLabel *textLabel10;
    QSpacerItem *spacerItem8;
    Q3Frame *theClipPointLine;
    Q3HBoxLayout *hboxLayout9;
    QLabel *theHistogramFileLabel;
    QLineEdit *theHistogramFileLineEdit;
    QPushButton *theHistogramFilePushButton;
    Q3HBoxLayout *hboxLayout10;
    QSpacerItem *spacerItem9;
    QCheckBox *theEnableCheckBox;
    QPushButton *theApplyButton;
    QPushButton *theResetButton;
    QPushButton *theCloseButton;

    void setupUi(QDialog *ossimQtHistogramDialog)
    {
    ossimQtHistogramDialog->setObjectName(QString::fromUtf8("ossimQtHistogramDialog"));
    ossimQtHistogramDialog->resize(QSize(463, 314).expandedTo(ossimQtHistogramDialog->minimumSizeHint()));
    gridLayout = new Q3GridLayout(ossimQtHistogramDialog);
    gridLayout->setSpacing(6);
    gridLayout->setMargin(11);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    hboxLayout = new Q3HBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    theLowClipPercentLineEdit = new QLineEdit(ossimQtHistogramDialog);
    theLowClipPercentLineEdit->setObjectName(QString::fromUtf8("theLowClipPercentLineEdit"));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(theLowClipPercentLineEdit->sizePolicy().hasHeightForWidth());
    theLowClipPercentLineEdit->setSizePolicy(sizePolicy);
    theLowClipPercentLineEdit->setMinimumSize(QSize(60, 0));
    theLowClipPercentLineEdit->setMaximumSize(QSize(60, 32767));

    hboxLayout->addWidget(theLowClipPercentLineEdit);

    textLabel4 = new QLabel(ossimQtHistogramDialog);
    textLabel4->setObjectName(QString::fromUtf8("textLabel4"));

    hboxLayout->addWidget(textLabel4);

    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);


    gridLayout->addLayout(hboxLayout, 0, 1, 1, 1);

    hboxLayout1 = new Q3HBoxLayout();
    hboxLayout1->setSpacing(6);
    hboxLayout1->setMargin(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    spacerItem1 = new QSpacerItem(30, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem1);

    textLabel3 = new QLabel(ossimQtHistogramDialog);
    textLabel3->setObjectName(QString::fromUtf8("textLabel3"));

    hboxLayout1->addWidget(textLabel3);

    theStretchModeComboBox = new QComboBox(ossimQtHistogramDialog);
    theStretchModeComboBox->setObjectName(QString::fromUtf8("theStretchModeComboBox"));
    theStretchModeComboBox->setMinimumSize(QSize(130, 0));

    hboxLayout1->addWidget(theStretchModeComboBox);


    gridLayout->addLayout(hboxLayout1, 0, 0, 1, 1);

    hboxLayout2 = new Q3HBoxLayout();
    hboxLayout2->setSpacing(6);
    hboxLayout2->setMargin(0);
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    theHighClipPercentLineEdit = new QLineEdit(ossimQtHistogramDialog);
    theHighClipPercentLineEdit->setObjectName(QString::fromUtf8("theHighClipPercentLineEdit"));
    QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(theHighClipPercentLineEdit->sizePolicy().hasHeightForWidth());
    theHighClipPercentLineEdit->setSizePolicy(sizePolicy1);
    theHighClipPercentLineEdit->setMinimumSize(QSize(60, 0));
    theHighClipPercentLineEdit->setMaximumSize(QSize(60, 32767));

    hboxLayout2->addWidget(theHighClipPercentLineEdit);

    textLabel5 = new QLabel(ossimQtHistogramDialog);
    textLabel5->setObjectName(QString::fromUtf8("textLabel5"));

    hboxLayout2->addWidget(textLabel5);

    spacerItem2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout2->addItem(spacerItem2);


    gridLayout->addLayout(hboxLayout2, 1, 1, 1, 1);

    hboxLayout3 = new Q3HBoxLayout();
    hboxLayout3->setSpacing(6);
    hboxLayout3->setMargin(0);
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
    spacerItem3 = new QSpacerItem(60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout3->addItem(spacerItem3);

    theChannelTextLabel = new QLabel(ossimQtHistogramDialog);
    theChannelTextLabel->setObjectName(QString::fromUtf8("theChannelTextLabel"));

    hboxLayout3->addWidget(theChannelTextLabel);

    theBandComboBox = new QComboBox(ossimQtHistogramDialog);
    theBandComboBox->setObjectName(QString::fromUtf8("theBandComboBox"));
    theBandComboBox->setMinimumSize(QSize(130, 0));

    hboxLayout3->addWidget(theBandComboBox);


    gridLayout->addLayout(hboxLayout3, 1, 0, 1, 1);

    hboxLayout4 = new Q3HBoxLayout();
    hboxLayout4->setSpacing(6);
    hboxLayout4->setMargin(0);
    hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
    theLowClipValueLineEdit = new QLineEdit(ossimQtHistogramDialog);
    theLowClipValueLineEdit->setObjectName(QString::fromUtf8("theLowClipValueLineEdit"));
    theLowClipValueLineEdit->setMinimumSize(QSize(60, 0));
    theLowClipValueLineEdit->setMaximumSize(QSize(60, 32767));

    hboxLayout4->addWidget(theLowClipValueLineEdit);

    textLabel6 = new QLabel(ossimQtHistogramDialog);
    textLabel6->setObjectName(QString::fromUtf8("textLabel6"));

    hboxLayout4->addWidget(textLabel6);

    spacerItem4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout4->addItem(spacerItem4);


    gridLayout->addLayout(hboxLayout4, 2, 1, 1, 1);

    theHistogramWidget = new ossimQtHistogramWidget(ossimQtHistogramDialog);
    theHistogramWidget->setObjectName(QString::fromUtf8("theHistogramWidget"));
    theHistogramWidget->setMinimumSize(QSize(256, 128));

    gridLayout->addWidget(theHistogramWidget, 2, 0, 5, 1);

    hboxLayout5 = new Q3HBoxLayout();
    hboxLayout5->setSpacing(6);
    hboxLayout5->setMargin(0);
    hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
    theHighClipValueLineEdit = new QLineEdit(ossimQtHistogramDialog);
    theHighClipValueLineEdit->setObjectName(QString::fromUtf8("theHighClipValueLineEdit"));
    theHighClipValueLineEdit->setMinimumSize(QSize(60, 0));
    theHighClipValueLineEdit->setMaximumSize(QSize(60, 32767));

    hboxLayout5->addWidget(theHighClipValueLineEdit);

    textLabel7 = new QLabel(ossimQtHistogramDialog);
    textLabel7->setObjectName(QString::fromUtf8("textLabel7"));

    hboxLayout5->addWidget(textLabel7);

    spacerItem5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout5->addItem(spacerItem5);


    gridLayout->addLayout(hboxLayout5, 3, 1, 1, 1);

    hboxLayout6 = new Q3HBoxLayout();
    hboxLayout6->setSpacing(6);
    hboxLayout6->setMargin(0);
    hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
    theMidPointLineEdit = new QLineEdit(ossimQtHistogramDialog);
    theMidPointLineEdit->setObjectName(QString::fromUtf8("theMidPointLineEdit"));
    QSizePolicy sizePolicy2(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(0));
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(theMidPointLineEdit->sizePolicy().hasHeightForWidth());
    theMidPointLineEdit->setSizePolicy(sizePolicy2);
    theMidPointLineEdit->setMinimumSize(QSize(60, 0));
    theMidPointLineEdit->setMaximumSize(QSize(60, 32767));

    hboxLayout6->addWidget(theMidPointLineEdit);

    textLabel8 = new QLabel(ossimQtHistogramDialog);
    textLabel8->setObjectName(QString::fromUtf8("textLabel8"));

    hboxLayout6->addWidget(textLabel8);

    spacerItem6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout6->addItem(spacerItem6);


    gridLayout->addLayout(hboxLayout6, 4, 1, 1, 1);

    hboxLayout7 = new Q3HBoxLayout();
    hboxLayout7->setSpacing(6);
    hboxLayout7->setMargin(0);
    hboxLayout7->setObjectName(QString::fromUtf8("hboxLayout7"));
    theOutputMinValue = new QLineEdit(ossimQtHistogramDialog);
    theOutputMinValue->setObjectName(QString::fromUtf8("theOutputMinValue"));
    theOutputMinValue->setMinimumSize(QSize(60, 0));
    theOutputMinValue->setMaximumSize(QSize(60, 32767));

    hboxLayout7->addWidget(theOutputMinValue);

    textLabel9 = new QLabel(ossimQtHistogramDialog);
    textLabel9->setObjectName(QString::fromUtf8("textLabel9"));

    hboxLayout7->addWidget(textLabel9);

    spacerItem7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout7->addItem(spacerItem7);


    gridLayout->addLayout(hboxLayout7, 5, 1, 1, 1);

    hboxLayout8 = new Q3HBoxLayout();
    hboxLayout8->setSpacing(6);
    hboxLayout8->setMargin(0);
    hboxLayout8->setObjectName(QString::fromUtf8("hboxLayout8"));
    theOutputMaxValue = new QLineEdit(ossimQtHistogramDialog);
    theOutputMaxValue->setObjectName(QString::fromUtf8("theOutputMaxValue"));
    theOutputMaxValue->setMinimumSize(QSize(60, 0));
    theOutputMaxValue->setMaximumSize(QSize(60, 32767));

    hboxLayout8->addWidget(theOutputMaxValue);

    textLabel10 = new QLabel(ossimQtHistogramDialog);
    textLabel10->setObjectName(QString::fromUtf8("textLabel10"));

    hboxLayout8->addWidget(textLabel10);

    spacerItem8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout8->addItem(spacerItem8);


    gridLayout->addLayout(hboxLayout8, 6, 1, 1, 1);

    theClipPointLine = new Q3Frame(ossimQtHistogramDialog);
    theClipPointLine->setObjectName(QString::fromUtf8("theClipPointLine"));
    theClipPointLine->setMinimumSize(QSize(256, 0));
    theClipPointLine->setFrameShape(Q3Frame::HLine);
    theClipPointLine->setFrameShadow(Q3Frame::Sunken);
    theClipPointLine->setLineWidth(2);

    gridLayout->addWidget(theClipPointLine, 7, 0, 1, 1);

    hboxLayout9 = new Q3HBoxLayout();
    hboxLayout9->setSpacing(6);
    hboxLayout9->setMargin(0);
    hboxLayout9->setObjectName(QString::fromUtf8("hboxLayout9"));
    theHistogramFileLabel = new QLabel(ossimQtHistogramDialog);
    theHistogramFileLabel->setObjectName(QString::fromUtf8("theHistogramFileLabel"));
    theHistogramFileLabel->setMinimumSize(QSize(90, 0));

    hboxLayout9->addWidget(theHistogramFileLabel);

    theHistogramFileLineEdit = new QLineEdit(ossimQtHistogramDialog);
    theHistogramFileLineEdit->setObjectName(QString::fromUtf8("theHistogramFileLineEdit"));
    theHistogramFileLineEdit->setMinimumSize(QSize(220, 0));

    hboxLayout9->addWidget(theHistogramFileLineEdit);

    theHistogramFilePushButton = new QPushButton(ossimQtHistogramDialog);
    theHistogramFilePushButton->setObjectName(QString::fromUtf8("theHistogramFilePushButton"));
    theHistogramFilePushButton->setAutoDefault(false);

    hboxLayout9->addWidget(theHistogramFilePushButton);


    gridLayout->addLayout(hboxLayout9, 8, 0, 1, 2);

    hboxLayout10 = new Q3HBoxLayout();
    hboxLayout10->setSpacing(6);
    hboxLayout10->setMargin(0);
    hboxLayout10->setObjectName(QString::fromUtf8("hboxLayout10"));
    spacerItem9 = new QSpacerItem(120, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout10->addItem(spacerItem9);

    theEnableCheckBox = new QCheckBox(ossimQtHistogramDialog);
    theEnableCheckBox->setObjectName(QString::fromUtf8("theEnableCheckBox"));

    hboxLayout10->addWidget(theEnableCheckBox);

    theApplyButton = new QPushButton(ossimQtHistogramDialog);
    theApplyButton->setObjectName(QString::fromUtf8("theApplyButton"));

    hboxLayout10->addWidget(theApplyButton);

    theResetButton = new QPushButton(ossimQtHistogramDialog);
    theResetButton->setObjectName(QString::fromUtf8("theResetButton"));

    hboxLayout10->addWidget(theResetButton);

    theCloseButton = new QPushButton(ossimQtHistogramDialog);
    theCloseButton->setObjectName(QString::fromUtf8("theCloseButton"));

    hboxLayout10->addWidget(theCloseButton);


    gridLayout->addLayout(hboxLayout10, 9, 0, 1, 2);

    retranslateUi(ossimQtHistogramDialog);

    QMetaObject::connectSlotsByName(ossimQtHistogramDialog);
    } // setupUi

    void retranslateUi(QDialog *ossimQtHistogramDialog)
    {
    ossimQtHistogramDialog->setWindowTitle(QApplication::translate("ossimQtHistogramDialog", "histogram", 0, QApplication::UnicodeUTF8));
    textLabel4->setText(QApplication::translate("ossimQtHistogramDialog", "low clip percent", 0, QApplication::UnicodeUTF8));
    textLabel3->setText(QApplication::translate("ossimQtHistogramDialog", "stretch mode:", 0, QApplication::UnicodeUTF8));
    textLabel5->setText(QApplication::translate("ossimQtHistogramDialog", "high clip percent", 0, QApplication::UnicodeUTF8));
    theChannelTextLabel->setText(QApplication::translate("ossimQtHistogramDialog", "display channel:", 0, QApplication::UnicodeUTF8));
    theBandComboBox->clear();
    theBandComboBox->addItem(QApplication::translate("ossimQtHistogramDialog", "master", 0, QApplication::UnicodeUTF8));
    textLabel6->setText(QApplication::translate("ossimQtHistogramDialog", "low clip value", 0, QApplication::UnicodeUTF8));
    textLabel7->setText(QApplication::translate("ossimQtHistogramDialog", "high clip value", 0, QApplication::UnicodeUTF8));
    textLabel8->setText(QApplication::translate("ossimQtHistogramDialog", "mid point", 0, QApplication::UnicodeUTF8));
    textLabel9->setText(QApplication::translate("ossimQtHistogramDialog", "output min value", 0, QApplication::UnicodeUTF8));
    textLabel10->setText(QApplication::translate("ossimQtHistogramDialog", "output max value", 0, QApplication::UnicodeUTF8));
    theHistogramFileLabel->setText(QApplication::translate("ossimQtHistogramDialog", "histogram file:", 0, QApplication::UnicodeUTF8));
    theHistogramFileLineEdit->setText(QApplication::translate("ossimQtHistogramDialog", "", 0, QApplication::UnicodeUTF8));
    theHistogramFilePushButton->setText(QApplication::translate("ossimQtHistogramDialog", "set histogram file", 0, QApplication::UnicodeUTF8));
    theEnableCheckBox->setText(QApplication::translate("ossimQtHistogramDialog", "enabled", 0, QApplication::UnicodeUTF8));
    theApplyButton->setText(QApplication::translate("ossimQtHistogramDialog", "apply", 0, QApplication::UnicodeUTF8));
    theResetButton->setText(QApplication::translate("ossimQtHistogramDialog", "reset", 0, QApplication::UnicodeUTF8));
    theCloseButton->setText(QApplication::translate("ossimQtHistogramDialog", "close", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ossimQtHistogramDialog);
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
"256 160 1 1",
". c #ffffff",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................",
"................................................................................................................................................................................................................................................................"};


    switch (id) {
        case image0_ID: return QPixmap((const char**)image0_data);
        default: return QPixmap();
    } // switch
    } // icon

};

namespace Ui {
    class ossimQtHistogramDialog: public Ui_ossimQtHistogramDialog {};
} // namespace Ui

class ossimQtHistogramDialog : public QDialog, public Ui::ossimQtHistogramDialog
{
    Q_OBJECT

public:
    ossimQtHistogramDialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0);
    ~ossimQtHistogramDialog();

    virtual void setHistogramRemapper( ossimHistogramRemapper * remapper );

public slots:
    virtual void closeButtonClicked();
    virtual void applyButtonClicked();
    virtual void resetButtonClicked();
    virtual void enableCheckBoxClicked();
    virtual void lowClipPercentReturnPressed();
    virtual void highClipPercentReturnPressed();
    virtual void lowClipValueReturnPressed();
    virtual void highClipValueReturnPressed();
    virtual void midPointReturnPressed();
    virtual void outputMinValueReturnPressed();
    virtual void outputMaxValueReturnPressed();
    virtual void stretchModeComboBoxActivated( int item );
    virtual void bandComboBoxActivated( int item );
    virtual void histogramFileLineEditReturnPressed();
    virtual void histogramFilePushButtonClicked();

protected slots:
    virtual void languageChange();

private:
    ossimQtHistogramController* theController;

    virtual void init();
    virtual void destroy();

};

#endif // OSSIMQTHISTOGRAMDIALOG_H
