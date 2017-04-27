#ifndef OSSIMQTUNITCONVERTERDIALOG_H
#define OSSIMQTUNITCONVERTERDIALOG_H

#include <QtCore/QVariant>


#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include "ossim/base/ossimUnitConversionTool.h"

class Ui_ossimQtUnitConverterDialog
{
public:
    QVBoxLayout *vboxLayout;
    QGridLayout *gridLayout;
    QLabel *textLabel2;
    QLineEdit *theSecondsField;
    QLineEdit *theMilesField;
    QLabel *textLabel2_4;
    QLineEdit *theMinutesField;
    QLabel *textLabel2_5_2;
    QLabel *textLabel2_5_2_2;
    QLabel *textLabel2_2;
    QLabel *textLabel2_5;
    QLabel *textLabel2_5_2_2_2;
    QLineEdit *theRadiansField;
    QLabel *textLabel2_3;
    QLabel *textLabel2_5_2_2_3;
    QLineEdit *theNauticalMilesField;
    QLineEdit *theDegreesField;
    QLabel *textLabel2_5_2_2_3_2;
    QLineEdit *theMetersField;
    QLineEdit *theDmsField;
    QLineEdit *theUsSurveyFeetField;
    QLineEdit *theFeetField;
    QFrame *line4;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *theCloseButton;
    QSpacerItem *spacerItem1;

    void setupUi(QDialog *ossimQtUnitConverterDialog)
    {
    ossimQtUnitConverterDialog->setObjectName(QString::fromUtf8("ossimQtUnitConverterDialog"));
    ossimQtUnitConverterDialog->resize(QSize(318, 366).expandedTo(ossimQtUnitConverterDialog->minimumSizeHint()));
    vboxLayout = new QVBoxLayout(ossimQtUnitConverterDialog);
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(11);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    gridLayout = new QGridLayout();
    gridLayout->setSpacing(6);
    gridLayout->setMargin(0);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    textLabel2 = new QLabel(ossimQtUnitConverterDialog);
    textLabel2->setObjectName(QString::fromUtf8("textLabel2"));

    gridLayout->addWidget(textLabel2, 0, 0, 1, 1);

    theSecondsField = new QLineEdit(ossimQtUnitConverterDialog);
    theSecondsField->setObjectName(QString::fromUtf8("theSecondsField"));

    gridLayout->addWidget(theSecondsField, 8, 1, 1, 1);

    theMilesField = new QLineEdit(ossimQtUnitConverterDialog);
    theMilesField->setObjectName(QString::fromUtf8("theMilesField"));

    gridLayout->addWidget(theMilesField, 3, 1, 1, 1);

    textLabel2_4 = new QLabel(ossimQtUnitConverterDialog);
    textLabel2_4->setObjectName(QString::fromUtf8("textLabel2_4"));

    gridLayout->addWidget(textLabel2_4, 3, 0, 1, 1);

    theMinutesField = new QLineEdit(ossimQtUnitConverterDialog);
    theMinutesField->setObjectName(QString::fromUtf8("theMinutesField"));

    gridLayout->addWidget(theMinutesField, 7, 1, 1, 1);

    textLabel2_5_2 = new QLabel(ossimQtUnitConverterDialog);
    textLabel2_5_2->setObjectName(QString::fromUtf8("textLabel2_5_2"));

    gridLayout->addWidget(textLabel2_5_2, 5, 0, 1, 1);

    textLabel2_5_2_2 = new QLabel(ossimQtUnitConverterDialog);
    textLabel2_5_2_2->setObjectName(QString::fromUtf8("textLabel2_5_2_2"));

    gridLayout->addWidget(textLabel2_5_2_2, 6, 0, 1, 1);

    textLabel2_2 = new QLabel(ossimQtUnitConverterDialog);
    textLabel2_2->setObjectName(QString::fromUtf8("textLabel2_2"));

    gridLayout->addWidget(textLabel2_2, 1, 0, 1, 1);

    textLabel2_5 = new QLabel(ossimQtUnitConverterDialog);
    textLabel2_5->setObjectName(QString::fromUtf8("textLabel2_5"));

    gridLayout->addWidget(textLabel2_5, 4, 0, 1, 1);

    textLabel2_5_2_2_2 = new QLabel(ossimQtUnitConverterDialog);
    textLabel2_5_2_2_2->setObjectName(QString::fromUtf8("textLabel2_5_2_2_2"));

    gridLayout->addWidget(textLabel2_5_2_2_2, 7, 0, 1, 1);

    theRadiansField = new QLineEdit(ossimQtUnitConverterDialog);
    theRadiansField->setObjectName(QString::fromUtf8("theRadiansField"));

    gridLayout->addWidget(theRadiansField, 5, 1, 1, 1);

    textLabel2_3 = new QLabel(ossimQtUnitConverterDialog);
    textLabel2_3->setObjectName(QString::fromUtf8("textLabel2_3"));

    gridLayout->addWidget(textLabel2_3, 2, 0, 1, 1);

    textLabel2_5_2_2_3 = new QLabel(ossimQtUnitConverterDialog);
    textLabel2_5_2_2_3->setObjectName(QString::fromUtf8("textLabel2_5_2_2_3"));

    gridLayout->addWidget(textLabel2_5_2_2_3, 8, 0, 1, 1);

    theNauticalMilesField = new QLineEdit(ossimQtUnitConverterDialog);
    theNauticalMilesField->setObjectName(QString::fromUtf8("theNauticalMilesField"));

    gridLayout->addWidget(theNauticalMilesField, 4, 1, 1, 1);

    theDegreesField = new QLineEdit(ossimQtUnitConverterDialog);
    theDegreesField->setObjectName(QString::fromUtf8("theDegreesField"));

    gridLayout->addWidget(theDegreesField, 6, 1, 1, 1);

    textLabel2_5_2_2_3_2 = new QLabel(ossimQtUnitConverterDialog);
    textLabel2_5_2_2_3_2->setObjectName(QString::fromUtf8("textLabel2_5_2_2_3_2"));

    gridLayout->addWidget(textLabel2_5_2_2_3_2, 9, 0, 1, 1);

    theMetersField = new QLineEdit(ossimQtUnitConverterDialog);
    theMetersField->setObjectName(QString::fromUtf8("theMetersField"));

    gridLayout->addWidget(theMetersField, 0, 1, 1, 1);

    theDmsField = new QLineEdit(ossimQtUnitConverterDialog);
    theDmsField->setObjectName(QString::fromUtf8("theDmsField"));

    gridLayout->addWidget(theDmsField, 9, 1, 1, 1);

    theUsSurveyFeetField = new QLineEdit(ossimQtUnitConverterDialog);
    theUsSurveyFeetField->setObjectName(QString::fromUtf8("theUsSurveyFeetField"));

    gridLayout->addWidget(theUsSurveyFeetField, 2, 1, 1, 1);

    theFeetField = new QLineEdit(ossimQtUnitConverterDialog);
    theFeetField->setObjectName(QString::fromUtf8("theFeetField"));

    gridLayout->addWidget(theFeetField, 1, 1, 1, 1);


    vboxLayout->addLayout(gridLayout);

    line4 = new QFrame(ossimQtUnitConverterDialog);
    line4->setObjectName(QString::fromUtf8("line4"));
    line4->setFrameShape(QFrame::HLine);
    line4->setFrameShadow(QFrame::Sunken);

    vboxLayout->addWidget(line4);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    theCloseButton = new QPushButton(ossimQtUnitConverterDialog);
    theCloseButton->setObjectName(QString::fromUtf8("theCloseButton"));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(theCloseButton->sizePolicy().hasHeightForWidth());
    theCloseButton->setSizePolicy(sizePolicy);
    theCloseButton->setFocusPolicy(Qt::TabFocus);
    theCloseButton->setAutoDefault(false);

    hboxLayout->addWidget(theCloseButton);

    spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem1);


    vboxLayout->addLayout(hboxLayout);

    QWidget::setTabOrder(theMetersField, theFeetField);
    QWidget::setTabOrder(theFeetField, theUsSurveyFeetField);
    QWidget::setTabOrder(theUsSurveyFeetField, theMilesField);
    QWidget::setTabOrder(theMilesField, theNauticalMilesField);
    QWidget::setTabOrder(theNauticalMilesField, theRadiansField);
    QWidget::setTabOrder(theRadiansField, theDegreesField);
    QWidget::setTabOrder(theDegreesField, theMinutesField);
    QWidget::setTabOrder(theMinutesField, theSecondsField);
    QWidget::setTabOrder(theSecondsField, theDmsField);
    QWidget::setTabOrder(theDmsField, theCloseButton);
    retranslateUi(ossimQtUnitConverterDialog);

    QMetaObject::connectSlotsByName(ossimQtUnitConverterDialog);
    } // setupUi

    void retranslateUi(QDialog *ossimQtUnitConverterDialog)
    {
    ossimQtUnitConverterDialog->setWindowTitle(QApplication::translate("ossimQtUnitConverterDialog", "Unit Converter", 0, QApplication::UnicodeUTF8));
    textLabel2->setText(QApplication::translate("ossimQtUnitConverterDialog", "Meters:", 0, QApplication::UnicodeUTF8));
    textLabel2_4->setText(QApplication::translate("ossimQtUnitConverterDialog", "Miles:", 0, QApplication::UnicodeUTF8));
    textLabel2_5_2->setText(QApplication::translate("ossimQtUnitConverterDialog", "Radians:", 0, QApplication::UnicodeUTF8));
    textLabel2_5_2_2->setText(QApplication::translate("ossimQtUnitConverterDialog", "Degrees:", 0, QApplication::UnicodeUTF8));
    textLabel2_2->setText(QApplication::translate("ossimQtUnitConverterDialog", "Feet:", 0, QApplication::UnicodeUTF8));
    textLabel2_5->setText(QApplication::translate("ossimQtUnitConverterDialog", "Nautical miles:", 0, QApplication::UnicodeUTF8));
    textLabel2_5_2_2_2->setText(QApplication::translate("ossimQtUnitConverterDialog", "Minutes:", 0, QApplication::UnicodeUTF8));
    textLabel2_3->setText(QApplication::translate("ossimQtUnitConverterDialog", "U.S. survey feet:", 0, QApplication::UnicodeUTF8));
    textLabel2_5_2_2_3->setText(QApplication::translate("ossimQtUnitConverterDialog", "Seconds:", 0, QApplication::UnicodeUTF8));
    textLabel2_5_2_2_3_2->setText(QApplication::translate("ossimQtUnitConverterDialog", "Dms:", 0, QApplication::UnicodeUTF8));
    theCloseButton->setText(QApplication::translate("ossimQtUnitConverterDialog", "Close", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ossimQtUnitConverterDialog);
    } // retranslateUi

};

namespace Ui {
    class ossimQtUnitConverterDialog: public Ui_ossimQtUnitConverterDialog {};
} // namespace Ui

class ossimQtUnitConverterDialog : public QDialog, public Ui::ossimQtUnitConverterDialog
{
    Q_OBJECT

public:
    ossimQtUnitConverterDialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0);
    ~ossimQtUnitConverterDialog();

public slots:
    virtual void updateFields();
    virtual void theCloseButton_clicked();
    virtual void theDmsField_returnPressed();
    virtual void theMetersField_returnPressed();
    virtual void theFeetField_returnPressed();
    virtual void theUsSurveyFeetField_returnPressed();
    virtual void theNauticalMilesField_returnPressed();
    virtual void theRadiansField_returnPressed();
    virtual void theDegreesField_returnPressed();
    virtual void theMinutesField_returnPressed();
    virtual void theSecondsField_returnPressed();
    virtual void theMilesField_returnPressed();

protected:
    ossimUnitConversionTool theConversionTool;
    mutable bool theUpdateFlag;

protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

};

#endif // OSSIMQTUNITCONVERTERDIALOG_H
