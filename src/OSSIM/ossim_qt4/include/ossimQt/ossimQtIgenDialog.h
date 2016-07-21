#ifndef OSSIMQTIGENDIALOG_H
#define OSSIMQTIGENDIALOG_H

#include <QtCore/QVariant>

class ossimQtIgenController;
class ossimQtScrollingImageWidget;
class ossimObject;

#include <Qt3Support/Q3ButtonGroup>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <Qt3Support/Q3MimeSourceFactory>

class Ui_ossimQtIgenDialog
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    Q3ButtonGroup *theOutputImageButtonGroup;
    QHBoxLayout *hboxLayout1;
    QLineEdit *theOutputFileLineEdit;
    QPushButton *theOutputFilePushButton;
    Q3ButtonGroup *theOutputImageTypeButtonGroup;
    QHBoxLayout *hboxLayout2;
    QComboBox *theOutputTypeComboBox;
    QPushButton *theEditWriterPushButton;
    Q3ButtonGroup *buttonGroup7;
    QHBoxLayout *hboxLayout3;
    Q3ButtonGroup *theScaleButtonGroup;
    QHBoxLayout *hboxLayout4;
    QLineEdit *theGsdLineEdit;
    Q3ButtonGroup *theLinesButtonGroup;
    QHBoxLayout *hboxLayout5;
    QLineEdit *theLinesLineEdit;
    Q3ButtonGroup *theSamplesButtonGroup;
    QHBoxLayout *hboxLayout6;
    QLineEdit *theSamplesLineEdit;
    QPushButton *theSceneRectPushButton;
    QHBoxLayout *hboxLayout7;
    QSpacerItem *spacerItem;
    QPushButton *theSaveSpecFilePushButton;
    QPushButton *theRunIgenPushButton;
    QPushButton *theCloseButton;
    QSpacerItem *spacerItem1;

    void setupUi(QDialog *ossimQtIgenDialog)
    {
    ossimQtIgenDialog->setObjectName(QString::fromUtf8("ossimQtIgenDialog"));
    ossimQtIgenDialog->resize(QSize(602, 229).expandedTo(ossimQtIgenDialog->minimumSizeHint()));
    vboxLayout = new QVBoxLayout(ossimQtIgenDialog);
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(11);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    theOutputImageButtonGroup = new Q3ButtonGroup(ossimQtIgenDialog);
    theOutputImageButtonGroup->setObjectName(QString::fromUtf8("theOutputImageButtonGroup"));
    theOutputImageButtonGroup->setAlignment(Qt::AlignHCenter);
    theOutputImageButtonGroup->setColumnLayout(0, Qt::Vertical);
    theOutputImageButtonGroup->layout()->setSpacing(6);
    theOutputImageButtonGroup->layout()->setMargin(11);
    hboxLayout1 = new QHBoxLayout(theOutputImageButtonGroup->layout());
    hboxLayout1->setAlignment(Qt::AlignTop);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    theOutputFileLineEdit = new QLineEdit(theOutputImageButtonGroup);
    theOutputFileLineEdit->setObjectName(QString::fromUtf8("theOutputFileLineEdit"));
    theOutputFileLineEdit->setMinimumSize(QSize(180, 0));
    theOutputFileLineEdit->setReadOnly(false);

    hboxLayout1->addWidget(theOutputFileLineEdit);

    theOutputFilePushButton = new QPushButton(theOutputImageButtonGroup);
    theOutputFilePushButton->setObjectName(QString::fromUtf8("theOutputFilePushButton"));
    theOutputFilePushButton->setAutoDefault(false);

    hboxLayout1->addWidget(theOutputFilePushButton);


    hboxLayout->addWidget(theOutputImageButtonGroup);

    theOutputImageTypeButtonGroup = new Q3ButtonGroup(ossimQtIgenDialog);
    theOutputImageTypeButtonGroup->setObjectName(QString::fromUtf8("theOutputImageTypeButtonGroup"));
    theOutputImageTypeButtonGroup->setAlignment(Qt::AlignHCenter);
    theOutputImageTypeButtonGroup->setColumnLayout(0, Qt::Vertical);
    theOutputImageTypeButtonGroup->layout()->setSpacing(6);
    theOutputImageTypeButtonGroup->layout()->setMargin(11);
    hboxLayout2 = new QHBoxLayout(theOutputImageTypeButtonGroup->layout());
    hboxLayout2->setAlignment(Qt::AlignTop);
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    theOutputTypeComboBox = new QComboBox(theOutputImageTypeButtonGroup);
    theOutputTypeComboBox->setObjectName(QString::fromUtf8("theOutputTypeComboBox"));
    theOutputTypeComboBox->setMinimumSize(QSize(180, 0));

    hboxLayout2->addWidget(theOutputTypeComboBox);

    theEditWriterPushButton = new QPushButton(theOutputImageTypeButtonGroup);
    theEditWriterPushButton->setObjectName(QString::fromUtf8("theEditWriterPushButton"));
    theEditWriterPushButton->setAutoDefault(false);

    hboxLayout2->addWidget(theEditWriterPushButton);


    hboxLayout->addWidget(theOutputImageTypeButtonGroup);


    vboxLayout->addLayout(hboxLayout);

    buttonGroup7 = new Q3ButtonGroup(ossimQtIgenDialog);
    buttonGroup7->setObjectName(QString::fromUtf8("buttonGroup7"));
    buttonGroup7->setAlignment(Qt::AlignHCenter);
    buttonGroup7->setColumnLayout(0, Qt::Vertical);
    buttonGroup7->layout()->setSpacing(6);
    buttonGroup7->layout()->setMargin(11);
    hboxLayout3 = new QHBoxLayout(buttonGroup7->layout());
    hboxLayout3->setAlignment(Qt::AlignTop);
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
    theScaleButtonGroup = new Q3ButtonGroup(buttonGroup7);
    theScaleButtonGroup->setObjectName(QString::fromUtf8("theScaleButtonGroup"));
    theScaleButtonGroup->setAlignment(Qt::AlignHCenter);
    theScaleButtonGroup->setColumnLayout(0, Qt::Vertical);
    theScaleButtonGroup->layout()->setSpacing(6);
    theScaleButtonGroup->layout()->setMargin(11);
    hboxLayout4 = new QHBoxLayout(theScaleButtonGroup->layout());
    hboxLayout4->setAlignment(Qt::AlignTop);
    hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
    theGsdLineEdit = new QLineEdit(theScaleButtonGroup);
    theGsdLineEdit->setObjectName(QString::fromUtf8("theGsdLineEdit"));
    theGsdLineEdit->setMinimumSize(QSize(90, 0));

    hboxLayout4->addWidget(theGsdLineEdit);


    hboxLayout3->addWidget(theScaleButtonGroup);

    theLinesButtonGroup = new Q3ButtonGroup(buttonGroup7);
    theLinesButtonGroup->setObjectName(QString::fromUtf8("theLinesButtonGroup"));
    theLinesButtonGroup->setAlignment(Qt::AlignHCenter);
    theLinesButtonGroup->setColumnLayout(0, Qt::Vertical);
    theLinesButtonGroup->layout()->setSpacing(6);
    theLinesButtonGroup->layout()->setMargin(11);
    hboxLayout5 = new QHBoxLayout(theLinesButtonGroup->layout());
    hboxLayout5->setAlignment(Qt::AlignTop);
    hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
    theLinesLineEdit = new QLineEdit(theLinesButtonGroup);
    theLinesLineEdit->setObjectName(QString::fromUtf8("theLinesLineEdit"));
    theLinesLineEdit->setMinimumSize(QSize(90, 0));

    hboxLayout5->addWidget(theLinesLineEdit);


    hboxLayout3->addWidget(theLinesButtonGroup);

    theSamplesButtonGroup = new Q3ButtonGroup(buttonGroup7);
    theSamplesButtonGroup->setObjectName(QString::fromUtf8("theSamplesButtonGroup"));
    theSamplesButtonGroup->setAlignment(Qt::AlignHCenter);
    theSamplesButtonGroup->setColumnLayout(0, Qt::Vertical);
    theSamplesButtonGroup->layout()->setSpacing(6);
    theSamplesButtonGroup->layout()->setMargin(11);
    hboxLayout6 = new QHBoxLayout(theSamplesButtonGroup->layout());
    hboxLayout6->setAlignment(Qt::AlignTop);
    hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
    theSamplesLineEdit = new QLineEdit(theSamplesButtonGroup);
    theSamplesLineEdit->setObjectName(QString::fromUtf8("theSamplesLineEdit"));
    theSamplesLineEdit->setMinimumSize(QSize(90, 0));

    hboxLayout6->addWidget(theSamplesLineEdit);


    hboxLayout3->addWidget(theSamplesButtonGroup);

    theSceneRectPushButton = new QPushButton(buttonGroup7);
    theSceneRectPushButton->setObjectName(QString::fromUtf8("theSceneRectPushButton"));
    theSceneRectPushButton->setAutoDefault(false);

    hboxLayout3->addWidget(theSceneRectPushButton);


    vboxLayout->addWidget(buttonGroup7);

    hboxLayout7 = new QHBoxLayout();
    hboxLayout7->setSpacing(6);
    hboxLayout7->setMargin(0);
    hboxLayout7->setObjectName(QString::fromUtf8("hboxLayout7"));
    spacerItem = new QSpacerItem(143, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout7->addItem(spacerItem);

    theSaveSpecFilePushButton = new QPushButton(ossimQtIgenDialog);
    theSaveSpecFilePushButton->setObjectName(QString::fromUtf8("theSaveSpecFilePushButton"));
    theSaveSpecFilePushButton->setAutoDefault(false);

    hboxLayout7->addWidget(theSaveSpecFilePushButton);

    theRunIgenPushButton = new QPushButton(ossimQtIgenDialog);
    theRunIgenPushButton->setObjectName(QString::fromUtf8("theRunIgenPushButton"));
    theRunIgenPushButton->setAutoDefault(false);

    hboxLayout7->addWidget(theRunIgenPushButton);

    theCloseButton = new QPushButton(ossimQtIgenDialog);
    theCloseButton->setObjectName(QString::fromUtf8("theCloseButton"));
    theCloseButton->setAutoDefault(false);

    hboxLayout7->addWidget(theCloseButton);

    spacerItem1 = new QSpacerItem(143, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout7->addItem(spacerItem1);


    vboxLayout->addLayout(hboxLayout7);

    retranslateUi(ossimQtIgenDialog);

    QMetaObject::connectSlotsByName(ossimQtIgenDialog);
    } // setupUi

    void retranslateUi(QDialog *ossimQtIgenDialog)
    {
    ossimQtIgenDialog->setWindowTitle(QApplication::translate("ossimQtIgenDialog", "image generator", 0, QApplication::UnicodeUTF8));
    theOutputImageButtonGroup->setTitle(QApplication::translate("ossimQtIgenDialog", "output image", 0, QApplication::UnicodeUTF8));
    theOutputFilePushButton->setText(QApplication::translate("ossimQtIgenDialog", "...", 0, QApplication::UnicodeUTF8));
    theOutputImageTypeButtonGroup->setTitle(QApplication::translate("ossimQtIgenDialog", "output image type", 0, QApplication::UnicodeUTF8));
    theEditWriterPushButton->setText(QApplication::translate("ossimQtIgenDialog", "edit writer", 0, QApplication::UnicodeUTF8));
    buttonGroup7->setTitle(QApplication::translate("ossimQtIgenDialog", "geometry", 0, QApplication::UnicodeUTF8));
    theScaleButtonGroup->setTitle(QApplication::translate("ossimQtIgenDialog", "scale", 0, QApplication::UnicodeUTF8));
    theLinesButtonGroup->setTitle(QApplication::translate("ossimQtIgenDialog", "lines", 0, QApplication::UnicodeUTF8));
    theSamplesButtonGroup->setTitle(QApplication::translate("ossimQtIgenDialog", "samples", 0, QApplication::UnicodeUTF8));
    theSceneRectPushButton->setText(QApplication::translate("ossimQtIgenDialog", "use scene rect", 0, QApplication::UnicodeUTF8));
    theSaveSpecFilePushButton->setText(QApplication::translate("ossimQtIgenDialog", "save spec file only", 0, QApplication::UnicodeUTF8));
    theRunIgenPushButton->setText(QApplication::translate("ossimQtIgenDialog", "save image", 0, QApplication::UnicodeUTF8));
    theCloseButton->setText(QApplication::translate("ossimQtIgenDialog", "close", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ossimQtIgenDialog);
    } // retranslateUi

};

namespace Ui {
    class ossimQtIgenDialog: public Ui_ossimQtIgenDialog {};
} // namespace Ui

class ossimQtIgenDialog : public QDialog, public Ui::ossimQtIgenDialog
{
    Q_OBJECT

public:
    ossimQtIgenDialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0);
    ~ossimQtIgenDialog();

    ossimQtScrollingImageWidget* theImageWidget;

    virtual void setImageWidget( ossimQtScrollingImageWidget * widget );
    virtual void setView( const ossimObject * view );
    virtual ossimQtIgenController * controller();

public slots:
    virtual void closeButtonClicked();
    virtual void outputFilePushButtonClicked();
    virtual void saveSpecFilePushButtonClicked();
    virtual void runIgenPushButtonClicked();
    virtual void gsdLineEditReturnPressed();
    virtual void linesLineEditReturnPressed();
    virtual void samplesLineEditReturnPressed();
    virtual void outputFileLineEditReturnPressed();
    virtual void sceneRectPushButtonClicked();
    virtual void imageWidgetDestroyed();
    virtual void editWriterPushButtonClicked();
    virtual void outputTypeComboBoxActivated( const QString & type );

protected slots:
    virtual void languageChange();

private:
    ossimQtIgenController* theController;

    void init();
    void destroy();

};

#endif // OSSIMQTIGENDIALOG_H
