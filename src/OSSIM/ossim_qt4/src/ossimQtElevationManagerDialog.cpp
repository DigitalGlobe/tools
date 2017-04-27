// $Id: ossimQtElevationManagerDialog.cpp 16151 2009-12-19 03:26:33Z okramer $

#include <ossimQt/ossimQtElevationManagerDialog.h>
#include <ossimQt/ossimQtElevationManagerController.h>
#include <ossimQt/ossimQtMainWindowController.h>
#include <ossimQt/ossimQtMainWindow.h>

#include <QtGui/QGroupBox>
#include <QtGui/QFileDialog>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QListWidget>
#include <QtGui/QSlider>
#include <QtGui/QLabel>

/*
 *  Constructs a ossimQtElevationManagerDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
ossimQtElevationManagerDialog::ossimQtElevationManagerDialog(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl),
    theController(0)
{
    setupUi(this);

    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ossimQtElevationManagerDialog::~ossimQtElevationManagerDialog()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

void ossimQtElevationManagerDialog::setupUi(QDialog *ossimQtElevationManagerDialog)
{
   ossimQtElevationManagerDialog->setObjectName(QString::fromUtf8("ossimQtElevationManagerDialog"));
   vboxLayout = new QVBoxLayout(ossimQtElevationManagerDialog);
   vboxLayout->setSpacing(6);
   vboxLayout->setMargin(11);
   vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
   vboxLayout1 = new QVBoxLayout();
   vboxLayout1->setSpacing(6);
   vboxLayout1->setMargin(0);
   vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
   hboxLayout = new QHBoxLayout();
   hboxLayout->setSpacing(6);
   hboxLayout->setMargin(0);
   hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
   theElevationCellGroup = new QGroupBox(ossimQtElevationManagerDialog);
   theElevationCellGroup->setObjectName(QString::fromUtf8("theElevationCellGroup"));
   // theElevationCellGroup->setColumnLayout(0, Qt::Vertical);
//   theElevationCellGroup->layout()->setSpacing(6);
//   theElevationCellGroup->layout()->setMargin(0);
   gridLayout = new QGridLayout(theElevationCellGroup);//->layout());
   gridLayout->setAlignment(Qt::AlignTop);
   gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
   gridLayout->setMargin(0);
   theElevationListBox = new QListWidget(theElevationCellGroup);
   theElevationListBox->setObjectName(QString::fromUtf8("theElevationListBox"));

   gridLayout->addWidget(theElevationListBox, 0, 0, 1, 1);


   hboxLayout->addWidget(theElevationCellGroup);

   vboxLayout2 = new QVBoxLayout();
   vboxLayout2->setSpacing(6);
   vboxLayout2->setMargin(0);
   vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
   theAddButton = new QPushButton(ossimQtElevationManagerDialog);
   theAddButton->setObjectName(QString::fromUtf8("theAddButton"));

   vboxLayout2->addWidget(theAddButton);

   theAddDirectoryButton = new QPushButton(ossimQtElevationManagerDialog);
   theAddDirectoryButton->setObjectName(QString::fromUtf8("theAddDirectoryButton"));

   vboxLayout2->addWidget(theAddDirectoryButton);

   theRemoveButton = new QPushButton(ossimQtElevationManagerDialog);
   theRemoveButton->setObjectName(QString::fromUtf8("theRemoveButton"));

   vboxLayout2->addWidget(theRemoveButton);

   theRemoveAllButton = new QPushButton(ossimQtElevationManagerDialog);
   theRemoveAllButton->setObjectName(QString::fromUtf8("theRemoveAllButton"));

   vboxLayout2->addWidget(theRemoveAllButton);

   theMoveUpButton = new QPushButton(ossimQtElevationManagerDialog);
   theMoveUpButton->setObjectName(QString::fromUtf8("theMoveUpButton"));

   vboxLayout2->addWidget(theMoveUpButton);

   theMoveDownButton = new QPushButton(ossimQtElevationManagerDialog);
   theMoveDownButton->setObjectName(QString::fromUtf8("theMoveDownButton"));

   vboxLayout2->addWidget(theMoveDownButton);

   theMoveToTopButton = new QPushButton(ossimQtElevationManagerDialog);
   theMoveToTopButton->setObjectName(QString::fromUtf8("theMoveToTopButton"));

   vboxLayout2->addWidget(theMoveToTopButton);

   theMoveToBottomButton = new QPushButton(ossimQtElevationManagerDialog);
   theMoveToBottomButton->setObjectName(QString::fromUtf8("theMoveToBottomButton"));

   vboxLayout2->addWidget(theMoveToBottomButton);

   theMosaicButton = new QPushButton(ossimQtElevationManagerDialog);
   theMosaicButton->setObjectName(QString::fromUtf8("theMosaicButton"));

   vboxLayout2->addWidget(theMosaicButton);

   hboxLayout->addLayout(vboxLayout2);

   vboxLayout1->addLayout(hboxLayout);

   hboxLayout1 = new QHBoxLayout();
   hboxLayout1->setSpacing(6);
   hboxLayout1->setMargin(0);
   hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
   theElevationEnabledCheckBox = new QCheckBox(ossimQtElevationManagerDialog);
   theElevationEnabledCheckBox->setObjectName(QString::fromUtf8("theElevationEnabledCheckBox"));

   hboxLayout1->addWidget(theElevationEnabledCheckBox);

   theAutoLoadCheckBox = new QCheckBox(ossimQtElevationManagerDialog);
   theAutoLoadCheckBox->setObjectName(QString::fromUtf8("theAutoLoadCheckBox"));

   hboxLayout1->addWidget(theAutoLoadCheckBox);

   theAutoSortCheckBox = new QCheckBox(ossimQtElevationManagerDialog);
   theAutoSortCheckBox->setObjectName(QString::fromUtf8("theAutoSortCheckBox"));

   hboxLayout1->addWidget(theAutoSortCheckBox);

   vboxLayout1->addLayout(hboxLayout1);

   vboxLayout3 = new QVBoxLayout();
   vboxLayout3->setSpacing(6);
   vboxLayout3->setMargin(0);
   vboxLayout3->addSpacing(10);
   vboxLayout3->setAlignment(Qt::AlignHCenter);
   vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
   hboxLayout3 = new QHBoxLayout();
   hboxLayout3->setSpacing(0);
   hboxLayout3->setMargin(0);
   hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
   theHgtOffsetLabel = new QLabel("                               Height Offset");
   vboxLayout1->addWidget(theHgtOffsetLabel);
   theElevOffsetWidget = new QSlider(Qt::Horizontal, ossimQtElevationManagerDialog);
   theElevOffsetWidget->setObjectName(QString::fromUtf8("theElevOffsetWidget"));
   theElevOffsetWidget->setSingleStep(1);
   theElevOffsetWidget->setSliderPosition(0);
   theElevOffsetWidget->setPageStep(10);
   theElevOffsetWidget->setRange(-400,400);
   theElevOffsetWidget->setFixedWidth(250);
   theElevOffsetWidget->setTracking(false);
   theHgtOffsetValueLabel = new QLabel("0.0");
   theHgtOffsetValueLabel->setFixedWidth(80);
   theHgtOffsetValueLabel->setAlignment(Qt::AlignHCenter);
   hboxLayout3->addWidget(theElevOffsetWidget);
   hboxLayout3->addWidget(theHgtOffsetValueLabel);

   theAutoRefreshCheckBox = new QCheckBox(ossimQtElevationManagerDialog);
   theAutoRefreshCheckBox->setChecked(false);
   theAutoRefreshCheckBox->setObjectName(QString::fromUtf8("theAutoRefreshCheckBox"));
   hboxLayout3->addWidget(theAutoRefreshCheckBox);
   vboxLayout1->addLayout(hboxLayout3);

   //vboxLayout1->addLayout(vboxLayout3);

   vboxLayout->addLayout(vboxLayout1);

   line1 = new QFrame(ossimQtElevationManagerDialog);
   line1->setObjectName(QString::fromUtf8("line1"));
   line1->setFrameShape(QFrame::HLine);
   line1->setFrameShadow(QFrame::Sunken);

   vboxLayout->addWidget(line1);

   hboxLayout2 = new QHBoxLayout();
   hboxLayout2->setSpacing(6);
   hboxLayout2->setMargin(0);
   hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
   spacerItem = new QSpacerItem(215, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);


   hboxLayout2->addItem(spacerItem);

   theCloseButton = new QPushButton(ossimQtElevationManagerDialog);
   theCloseButton->setObjectName(QString::fromUtf8("theCloseButton"));
   QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
   sizePolicy.setHorizontalStretch(0);
   sizePolicy.setVerticalStretch(0);
   sizePolicy.setHeightForWidth(theCloseButton->sizePolicy().hasHeightForWidth());
   theCloseButton->setSizePolicy(sizePolicy);
   theCloseButton->setDefault(true);
   hboxLayout2->addWidget(theCloseButton);

   spacerItem1 = new QSpacerItem(215, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

   hboxLayout2->addItem(spacerItem1);


   vboxLayout->addLayout(hboxLayout2);


    retranslateUi(ossimQtElevationManagerDialog);

    QSize size(320, 393);
    size = size.expandedTo(ossimQtElevationManagerDialog->minimumSizeHint());
    ossimQtElevationManagerDialog->resize(size);

    QObject::connect(theAddButton, SIGNAL(clicked()), ossimQtElevationManagerDialog, SLOT(addButtonClicked()));
    QObject::connect(theRemoveButton, SIGNAL(clicked()), ossimQtElevationManagerDialog, SLOT(removeButtonClicked()));
    QObject::connect(theRemoveAllButton, SIGNAL(clicked()), ossimQtElevationManagerDialog, SLOT(removeAllButtonClicked()));
    QObject::connect(theCloseButton, SIGNAL(clicked()), ossimQtElevationManagerDialog, SLOT(closeButtonClicked()));
    QObject::connect(theAddDirectoryButton, SIGNAL(clicked()), ossimQtElevationManagerDialog, SLOT(addDirectoryClicked()));
    QObject::connect(theElevationEnabledCheckBox, SIGNAL(clicked()), ossimQtElevationManagerDialog, SLOT(elevationEnabledCheckBoxClicked()));
    QObject::connect(theAutoLoadCheckBox, SIGNAL(clicked()), ossimQtElevationManagerDialog, SLOT(autoLoadCheckBoxClicked()));
    QObject::connect(theAutoSortCheckBox, SIGNAL(clicked()), ossimQtElevationManagerDialog, SLOT(autoSortingCheckBoxClicked()));
    QObject::connect(theMosaicButton, SIGNAL(clicked()), ossimQtElevationManagerDialog, SLOT(mosaicButtonClicked()));
    QObject::connect(theMoveUpButton, SIGNAL(clicked()), ossimQtElevationManagerDialog, SLOT(moveUpButtonClicked()));
    QObject::connect(theMoveDownButton, SIGNAL(clicked()), ossimQtElevationManagerDialog, SLOT(moveDownButtonClicked()));
    QObject::connect(theMoveToBottomButton, SIGNAL(clicked()), ossimQtElevationManagerDialog, SLOT(moveToBottomButtonClicked()));
    QObject::connect(theMoveToTopButton, SIGNAL(clicked()), ossimQtElevationManagerDialog, SLOT(moveToTopButtonClicked()));
    QObject::connect(theElevOffsetWidget, SIGNAL(valueChanged(int)), ossimQtElevationManagerDialog, SLOT(elevOffsetChanged(int)));
    QObject::connect(theAutoRefreshCheckBox, SIGNAL(clicked()), ossimQtElevationManagerDialog, SLOT(autoRefreshCheckBoxClicked()));

    QMetaObject::connectSlotsByName(ossimQtElevationManagerDialog);
    } // setupUi

void ossimQtElevationManagerDialog::retranslateUi(QDialog *ossimQtElevationManagerDialog)
{
    ossimQtElevationManagerDialog->setWindowTitle(QApplication::translate("ossimQtElevationManagerDialog", "Elevation Manager", 0, QApplication::UnicodeUTF8));
    theElevationCellGroup->setTitle(QApplication::translate("ossimQtElevationManagerDialog", "Elevation cells", 0, QApplication::UnicodeUTF8));
    theAddButton->setText(QApplication::translate("ossimQtElevationManagerDialog", "Add", 0, QApplication::UnicodeUTF8));
    theAddDirectoryButton->setText(QApplication::translate("ossimQtElevationManagerDialog", "Add directory", 0, QApplication::UnicodeUTF8));
    theRemoveButton->setText(QApplication::translate("ossimQtElevationManagerDialog", "Remove", 0, QApplication::UnicodeUTF8));
    theRemoveAllButton->setText(QApplication::translate("ossimQtElevationManagerDialog", "Remove all", 0, QApplication::UnicodeUTF8));
    theMoveUpButton->setText(QApplication::translate("ossimQtElevationManagerDialog", "Move up", 0, QApplication::UnicodeUTF8));
    theMoveDownButton->setText(QApplication::translate("ossimQtElevationManagerDialog", "Move down", 0, QApplication::UnicodeUTF8));
    theMoveToTopButton->setText(QApplication::translate("ossimQtElevationManagerDialog", "Move to top", 0, QApplication::UnicodeUTF8));
    theMoveToBottomButton->setText(QApplication::translate("ossimQtElevationManagerDialog", "Move to bottom", 0, QApplication::UnicodeUTF8));
    theMosaicButton->setText(QApplication::translate("ossimQtElevationManagerDialog", "Mosaic", 0, QApplication::UnicodeUTF8));
    theElevationEnabledCheckBox->setText(QApplication::translate("ossimQtElevationManagerDialog", "elevation enabled", 0, QApplication::UnicodeUTF8));
    theElevationEnabledCheckBox->setProperty("toolTip", QVariant(QApplication::translate("ossimQtElevationManagerDialog", "elevation enable", 0, QApplication::UnicodeUTF8)));
    theElevationEnabledCheckBox->setProperty("whatsThis", QVariant(QApplication::translate("ossimQtElevationManagerDialog", "Enable elevation lookups", 0, QApplication::UnicodeUTF8)));
    theAutoLoadCheckBox->setText(QApplication::translate("ossimQtElevationManagerDialog", "auto load", 0, QApplication::UnicodeUTF8));
    theAutoLoadCheckBox->setProperty("toolTip", QVariant(QApplication::translate("ossimQtElevationManagerDialog", "elevation enable", 0, QApplication::UnicodeUTF8)));
    theAutoLoadCheckBox->setProperty("whatsThis", QVariant(QApplication::translate("ossimQtElevationManagerDialog", "Enable elevation lookups", 0, QApplication::UnicodeUTF8)));
    theAutoSortCheckBox->setText(QApplication::translate("ossimQtElevationManagerDialog", "auto sorting", 0, QApplication::UnicodeUTF8));
    theAutoSortCheckBox->setProperty("toolTip", QVariant(QApplication::translate("ossimQtElevationManagerDialog", "elevation enable", 0, QApplication::UnicodeUTF8)));
    theAutoSortCheckBox->setProperty("whatsThis", QVariant(QApplication::translate("ossimQtElevationManagerDialog", "Enable elevation lookups", 0, QApplication::UnicodeUTF8)));

    theElevOffsetWidget->setProperty("toolTip", QVariant(QApplication::translate("ossimQtElevationManagerDialog", "Sets elevation offset", 0, QApplication::UnicodeUTF8)));
    theElevOffsetWidget->setProperty("whatsThis", QVariant(QApplication::translate("ossimQtElevationManagerDialog", "Sets elevation offset", 0, QApplication::UnicodeUTF8)));
    theAutoRefreshCheckBox->setProperty("toolTip", QVariant(QApplication::translate("ossimQtElevationManagerDialog", "Auto Refresh", 0, QApplication::UnicodeUTF8)));
    theAutoRefreshCheckBox->setProperty("whatsThis", QVariant(QApplication::translate("ossimQtElevationManagerDialog", "Refreshes all displays after each change", 0, QApplication::UnicodeUTF8)));
    theAutoRefreshCheckBox->setText(QApplication::translate("ossimQtElevationManagerDialog", "Auto Refresh", 0, QApplication::UnicodeUTF8));

    theCloseButton->setText(QApplication::translate("ossimQtElevationManagerDialog", "Close", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ossimQtElevationManagerDialog);
} // retranslateUi


/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ossimQtElevationManagerDialog::languageChange()
{
    retranslateUi(this);
}

void ossimQtElevationManagerDialog::elevationEnabledCheckBoxClicked()
{
   if (theElevationEnabledCheckBox && theController)
   {
      theController->onEnable(theElevationEnabledCheckBox->isChecked());
   }
}


void ossimQtElevationManagerDialog::closeButtonClicked()
{
   close(true);  // true to destroy on close...
}


void ossimQtElevationManagerDialog::addDirectoryClicked()
{
   if (!theController) return;

   QFileDialog* fd = new QFileDialog( this );
   fd->setMode(QFileDialog::DirectoryOnly);
   fd->setDir(ossimQtElevationManagerController::getLatestOpenCellPath().c_str());
   
   QString directory;
   if (fd->exec() == QDialog::Accepted )
   {
      directory = fd->selectedFile();
      theController->onAddDirectory(directory);
   }

   // Delete fd???
   delete fd;
   fd = NULL;
}


void ossimQtElevationManagerDialog::removeButtonClicked()
{
   if (!theController || !theElevationListBox) return;

   QList<QListWidgetItem *> cells = theElevationListBox->selectedItems();

   theController->onRemove(cells);
}

void ossimQtElevationManagerDialog::moveUpButtonClicked()
{
   if (!theController || !theElevationListBox) return;

   QList<QListWidgetItem *> cells = theElevationListBox->selectedItems();
   
   theController->onUp(cells);
}


void ossimQtElevationManagerDialog::removeAllButtonClicked()
{
   if (!theController) return;

   theController->onRemoveAll();
}


void ossimQtElevationManagerDialog::moveDownButtonClicked()
{
   if (!theController || !theElevationListBox) return;

   QList<QListWidgetItem *> cells = theElevationListBox->selectedItems();

   theController->onDown(cells);
}


void ossimQtElevationManagerDialog::moveToTopButtonClicked()
{
   if (!theController || !theElevationListBox) return;

   QList<QListWidgetItem *> cells = theElevationListBox->selectedItems();

   theController->onTop(cells); 
}


void ossimQtElevationManagerDialog::moveToBottomButtonClicked()
{
   if (!theController || !theElevationListBox) return;

   QList<QListWidgetItem *> cells = theElevationListBox->selectedItems();

   theController->onBottom(cells);
}


void ossimQtElevationManagerDialog::mosaicButtonClicked()
{
   if (!theController || !theElevationListBox) return;

   QList<QListWidgetItem *> cells = theElevationListBox->selectedItems();

   theController->onMosaic(cells); 
}


void ossimQtElevationManagerDialog::autoLoadCheckBoxClicked()
{
   if (theAutoLoadCheckBox && theController)
   {
      theController->onAutoLoadEnable(theAutoLoadCheckBox->isChecked());
   }
}


void ossimQtElevationManagerDialog::autoSortingCheckBoxClicked()
{
   if (theAutoSortCheckBox && theController)
   {
      theController->onAutoSortEnable(theAutoSortCheckBox->isChecked());
   }
}

void ossimQtElevationManagerDialog::addButtonClicked()
{
   if (!theController) return;
   
   QStringList cells = QFileDialog::getOpenFileNames("Cells (*.dt0 *.dt1 *.dt2 *.hgt);;All Files(*)",
                                                     ossimQtElevationManagerController::getLatestOpenCellPath().c_str(),
                                                     this,
                                                     "open cell dialog"
                                                     "Select one or more cells" );

   theController->onAdd(cells);
}

void ossimQtElevationManagerDialog::elevOffsetChanged(int)
{
   if (theElevOffsetWidget && theController)
   {
      int pos = theElevOffsetWidget->sliderPosition();
      double offset = 0.5*((double)pos);
      theController->onElevOffsetChanged(offset);
      theHgtOffsetValueLabel->setNum(offset);
   }
}

void ossimQtElevationManagerDialog::autoRefreshCheckBoxClicked()
{
   // Do first refresh now:
   theElevOffsetWidget->setTracking(theAutoRefreshCheckBox->isChecked());
}


void ossimQtElevationManagerDialog::init()
{
   theController = new ossimQtElevationManagerController(this);
}


void ossimQtElevationManagerDialog::destroy()
{
    if (theController)
    {
	delete theController;
	theController = NULL;
    }
}
