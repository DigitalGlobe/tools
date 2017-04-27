// $Id: ossimQtGeometryBuilderDialog.cpp 12141 2007-12-07 18:05:13Z gpotts $

#include <ossimQt/ossimQtGeometryBuilderDialog.h>

#include <QtCore/QVariant>
#include <ossimQt/ossimQtGeometryBuilderDialogController.h>

#include <QtGui/QGroupBox>
#include <QtGui/QTextEdit>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>


/*
 *  Constructs a ossimQtGeometryBuilderDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
ossimQtGeometryBuilderDialog::ossimQtGeometryBuilderDialog(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ossimQtGeometryBuilderDialog::~ossimQtGeometryBuilderDialog()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ossimQtGeometryBuilderDialog::languageChange()
{
    retranslateUi(this);
}

void ossimQtGeometryBuilderDialog::setupUi(QDialog *ossimQtGeometryBuilderDialog)
{
    ossimQtGeometryBuilderDialog->setObjectName(QString::fromUtf8("ossimQtGeometryBuilderDialog"));
    vboxLayout = new QVBoxLayout(ossimQtGeometryBuilderDialog);
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(11);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    theTab = new QTabWidget(ossimQtGeometryBuilderDialog);
    theTab->setObjectName(QString::fromUtf8("theTab"));
    TabPage = new QWidget();
    TabPage->setObjectName(QString::fromUtf8("TabPage"));
    vboxLayout1 = new QVBoxLayout(TabPage);
    vboxLayout1->setSpacing(6);
    vboxLayout1->setMargin(0);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    textLabel1 = new QLabel(TabPage);
    textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(5));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(textLabel1->sizePolicy().hasHeightForWidth());
    textLabel1->setSizePolicy(sizePolicy);
    textLabel1->setWordWrap(false);

    hboxLayout->addWidget(textLabel1);

    theOssimProjectionList = new QComboBox(TabPage);
    theOssimProjectionList->setObjectName(QString::fromUtf8("theOssimProjectionList"));
    QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(0));
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(theOssimProjectionList->sizePolicy().hasHeightForWidth());
    theOssimProjectionList->setSizePolicy(sizePolicy1);

    hboxLayout->addWidget(theOssimProjectionList);


    vboxLayout1->addLayout(hboxLayout);

    theOssimGeometryString = new QTextEdit(TabPage);
    theOssimGeometryString->setObjectName(QString::fromUtf8("theOssimGeometryString"));

    vboxLayout1->addWidget(theOssimGeometryString);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setSpacing(6);
    hboxLayout1->setMargin(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    spacerItem = new QSpacerItem(110, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem);

    theOssimSaveGeomButton = new QPushButton(TabPage);
    theOssimSaveGeomButton->setObjectName(QString::fromUtf8("theOssimSaveGeomButton"));
    QSizePolicy sizePolicy2(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(theOssimSaveGeomButton->sizePolicy().hasHeightForWidth());
    theOssimSaveGeomButton->setSizePolicy(sizePolicy2);

    hboxLayout1->addWidget(theOssimSaveGeomButton);

    theMakeAffineAdjustableButton = new QPushButton(TabPage);
    theMakeAffineAdjustableButton->setObjectName(QString::fromUtf8("theMakeAffineAdjustableButton"));
    theMakeAffineAdjustableButton->setEnabled(false);

    hboxLayout1->addWidget(theMakeAffineAdjustableButton);

    spacerItem1 = new QSpacerItem(130, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem1);


    vboxLayout1->addLayout(hboxLayout1);

    theTab->addTab(TabPage, QApplication::translate("ossimQtGeometryBuilderDialog", "OSSIM Geometry", 0, QApplication::UnicodeUTF8));
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    vboxLayout2 = new QVBoxLayout(tab);
    vboxLayout2->setSpacing(6);
    vboxLayout2->setMargin(0);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    groupBox1 = new QGroupBox(tab);
    groupBox1->setObjectName(QString::fromUtf8("groupBox1"));
    // groupBox1->setColumnLayout(0, Qt::Vertical);
//    groupBox1->layout()->setSpacing(6);
//    groupBox1->layout()->setMargin(0);
    vboxLayout3 = new QVBoxLayout(groupBox1);//->layout());
    vboxLayout3->setAlignment(Qt::AlignTop);
    vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
    vboxLayout3->setMargin(0);
    theWktProjectionString = new QTextEdit(groupBox1);
    theWktProjectionString->setObjectName(QString::fromUtf8("theWktProjectionString"));

    vboxLayout3->addWidget(theWktProjectionString);


    vboxLayout2->addWidget(groupBox1);

    hboxLayout2 = new QHBoxLayout();
    hboxLayout2->setSpacing(6);
    hboxLayout2->setMargin(0);
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    spacerItem2 = new QSpacerItem(240, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout2->addItem(spacerItem2);

    theGdalWktLoadFromFileButton = new QPushButton(tab);
    theGdalWktLoadFromFileButton->setObjectName(QString::fromUtf8("theGdalWktLoadFromFileButton"));

    hboxLayout2->addWidget(theGdalWktLoadFromFileButton);

    theGdalWktConvertButton = new QPushButton(tab);
    theGdalWktConvertButton->setObjectName(QString::fromUtf8("theGdalWktConvertButton"));
    QSizePolicy sizePolicy3(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(theGdalWktConvertButton->sizePolicy().hasHeightForWidth());
    theGdalWktConvertButton->setSizePolicy(sizePolicy3);

    hboxLayout2->addWidget(theGdalWktConvertButton);

    spacerItem3 = new QSpacerItem(250, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout2->addItem(spacerItem3);


    vboxLayout2->addLayout(hboxLayout2);

    theTab->addTab(tab, QApplication::translate("ossimQtGeometryBuilderDialog", "GDAL Wkt String", 0, QApplication::UnicodeUTF8));

    vboxLayout->addWidget(theTab);

    hboxLayout3 = new QHBoxLayout();
    hboxLayout3->setSpacing(6);
    hboxLayout3->setMargin(0);
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
    spacerItem4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout3->addItem(spacerItem4);

    theCancelButton = new QPushButton(ossimQtGeometryBuilderDialog);
    theCancelButton->setObjectName(QString::fromUtf8("theCancelButton"));
    QSizePolicy sizePolicy4(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
    sizePolicy4.setHorizontalStretch(0);
    sizePolicy4.setVerticalStretch(0);
    sizePolicy4.setHeightForWidth(theCancelButton->sizePolicy().hasHeightForWidth());
    theCancelButton->setSizePolicy(sizePolicy4);

    hboxLayout3->addWidget(theCancelButton);

    spacerItem5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout3->addItem(spacerItem5);


    vboxLayout->addLayout(hboxLayout3);

    QWidget::setTabOrder(theWktProjectionString, theGdalWktConvertButton);
    QWidget::setTabOrder(theGdalWktConvertButton, theTab);
    QWidget::setTabOrder(theTab, theOssimGeometryString);
    QWidget::setTabOrder(theOssimGeometryString, theOssimSaveGeomButton);
    QWidget::setTabOrder(theOssimSaveGeomButton, theCancelButton);

    retranslateUi(ossimQtGeometryBuilderDialog);

    QSize size(776, 670);
    size = size.expandedTo(ossimQtGeometryBuilderDialog->minimumSizeHint());
    ossimQtGeometryBuilderDialog->resize(size);

    QObject::connect(theGdalWktConvertButton, SIGNAL(clicked()), ossimQtGeometryBuilderDialog, SLOT(theGdalWktConvertButton_clicked()));
    QObject::connect(theCancelButton, SIGNAL(clicked()), ossimQtGeometryBuilderDialog, SLOT(theCancelButton_clicked()));
    QObject::connect(theOssimSaveGeomButton, SIGNAL(clicked()), ossimQtGeometryBuilderDialog, SLOT(theOssimSaveGeomButton_clicked()));
    QObject::connect(theGdalWktLoadFromFileButton, SIGNAL(clicked()), ossimQtGeometryBuilderDialog, SLOT(theGdalWktLoadFromFileButton_clicked()));
    QObject::connect(theOssimProjectionList, SIGNAL(activated(QString)), ossimQtGeometryBuilderDialog, SLOT(theOssimProjectionList_activated(QString)));

    QMetaObject::connectSlotsByName(ossimQtGeometryBuilderDialog);
} // setupUi

void ossimQtGeometryBuilderDialog::retranslateUi(QDialog *ossimQtGeometryBuilderDialog)
{
    ossimQtGeometryBuilderDialog->setWindowTitle(QApplication::translate("ossimQtGeometryBuilderDialog", "Geometry Builder", 0, QApplication::UnicodeUTF8));
    textLabel1->setText(QApplication::translate("ossimQtGeometryBuilderDialog", "Projections:", 0, QApplication::UnicodeUTF8));
    theOssimSaveGeomButton->setText(QApplication::translate("ossimQtGeometryBuilderDialog", "Save geom", 0, QApplication::UnicodeUTF8));
    theMakeAffineAdjustableButton->setText(QApplication::translate("ossimQtGeometryBuilderDialog", "Make Affine Adjustable", 0, QApplication::UnicodeUTF8));
    theTab->setTabText(theTab->indexOf(TabPage), QApplication::translate("ossimQtGeometryBuilderDialog", "OSSIM Geometry", 0, QApplication::UnicodeUTF8));
    groupBox1->setTitle(QApplication::translate("ossimQtGeometryBuilderDialog", "Wkt PROJCS Projection String", 0, QApplication::UnicodeUTF8));
    theGdalWktLoadFromFileButton->setText(QApplication::translate("ossimQtGeometryBuilderDialog", "Load file", 0, QApplication::UnicodeUTF8));
    theGdalWktConvertButton->setText(QApplication::translate("ossimQtGeometryBuilderDialog", "Convert", 0, QApplication::UnicodeUTF8));
    theTab->setTabText(theTab->indexOf(tab), QApplication::translate("ossimQtGeometryBuilderDialog", "GDAL Wkt String", 0, QApplication::UnicodeUTF8));
    theCancelButton->setText(QApplication::translate("ossimQtGeometryBuilderDialog", "Cancel", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ossimQtGeometryBuilderDialog);
} // retranslateUi

void ossimQtGeometryBuilderDialog::init()
{
    theController = new ossimQtGeometryBuilderDialogController(this);
}

void ossimQtGeometryBuilderDialog::destroy()
{
    if(theController)
    {
	delete theController;
	theController = 0;
    }
}


void ossimQtGeometryBuilderDialog::theGdalWktConvertButton_clicked()
{
    if(theController)
    {
	theController->convertFromWkt();
    }
}

void ossimQtGeometryBuilderDialog::theCancelButton_clicked()
{
    close(true);
}


void ossimQtGeometryBuilderDialog::theOssimSaveGeomButton_clicked()
{
    if(theController)
    {
	theController->saveOssimGeometry();
    }
}


void ossimQtGeometryBuilderDialog::theGdalWktLoadFromFileButton_clicked()
{
    if(theController)
    {
	theController->loadWktStringFromFile();
    }
}


ossimQtGeometryBuilderDialogController* ossimQtGeometryBuilderDialog::controller()
{
    return theController;
}

void ossimQtGeometryBuilderDialog::theOssimProjectionList_activated( const QString & )
{
    if(theController)
    {
	theController->generateDefaultOssimGeometry(theOssimProjectionList->currentText().ascii());
    }

}
