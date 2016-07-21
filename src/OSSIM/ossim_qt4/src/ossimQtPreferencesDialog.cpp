// $Id: ossimQtPreferencesDialog.cpp 12141 2007-12-07 18:05:13Z gpotts $

#include <ossimQt/ossimQtPreferencesDialog.h>
#include <ossimQt/ossimQtPreferencesDialogController.h>

// #include <qvariant.h>
// #include <qimage.h>
// #include <qpixmap.h>

#include <QtGui/QMessageBox>
#include <QtGui/QGroupBox>
#include <QtGui/QListWidget>
#include <QtGui/QListWidgetItem>
#include <QtGui/QTextEdit>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

/*
 *  Constructs a ossimQtPreferencesDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
ossimQtPreferencesDialog::ossimQtPreferencesDialog(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
	theController = 0;
    setupUi(this);

    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ossimQtPreferencesDialog::~ossimQtPreferencesDialog()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ossimQtPreferencesDialog::languageChange()
{
    retranslateUi(this);
}

void ossimQtPreferencesDialog::setupUi(QDialog *ossimQtPreferencesDialog)
{
   ossimQtPreferencesDialog->setObjectName(QString::fromUtf8("ossimQtPreferencesDialog"));
   ossimQtPreferencesDialog->setSizeGripEnabled(true);
   vboxLayout = new QVBoxLayout(ossimQtPreferencesDialog);
   vboxLayout->setSpacing(6);
   vboxLayout->setMargin(11);
   vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
   theTabWidget = new QTabWidget(ossimQtPreferencesDialog);
    theTabWidget->setObjectName(QString::fromUtf8("theTabWidget"));
    TabPage = new QWidget();
    TabPage->setObjectName(QString::fromUtf8("TabPage"));
    hboxLayout = new QHBoxLayout(TabPage);
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    groupBox1 = new QGroupBox(TabPage);
    groupBox1->setObjectName(QString::fromUtf8("groupBox1"));
    //groupBox1->setColumnLayout(0, Qt::Vertical);
    //groupBox1->layout()->setSpacing(6);
    //groupBox1->layout()->setMargin(0);
    vboxLayout1 = new QVBoxLayout(groupBox1);//->layout());
//    vboxLayout1 = new QVBoxLayout(groupBox1);
    vboxLayout1->setAlignment(Qt::AlignTop);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    vboxLayout1->setMargin(0);
    vboxLayout2 = new QVBoxLayout();
    vboxLayout2->setSpacing(6);
    vboxLayout2->setMargin(0);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    thePluginList = new QListWidget(groupBox1);
    thePluginList->setObjectName(QString::fromUtf8("thePluginList"));
    thePluginList->setSelectionMode(QAbstractItemView::MultiSelection);

   vboxLayout2->addWidget(thePluginList);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setSpacing(6);
    hboxLayout1->setMargin(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    theAddPluginButton = new QPushButton(groupBox1);
    theAddPluginButton->setObjectName(QString::fromUtf8("theAddPluginButton"));

    hboxLayout1->addWidget(theAddPluginButton);

    theSelectAllPluginButton = new QPushButton(groupBox1);
    theSelectAllPluginButton->setObjectName(QString::fromUtf8("theSelectAllPluginButton"));

    hboxLayout1->addWidget(theSelectAllPluginButton);

    theReloadPluginButton = new QPushButton(groupBox1);
    theReloadPluginButton->setObjectName(QString::fromUtf8("theReloadPluginButton"));

    hboxLayout1->addWidget(theReloadPluginButton);

    theRemovePluginButton = new QPushButton(groupBox1);
    theRemovePluginButton->setObjectName(QString::fromUtf8("theRemovePluginButton"));
    theRemovePluginButton->setEnabled(true);

    hboxLayout1->addWidget(theRemovePluginButton);


    vboxLayout2->addLayout(hboxLayout1);


    vboxLayout1->addLayout(vboxLayout2);


    hboxLayout->addWidget(groupBox1);

    groupBox2 = new QGroupBox(TabPage);
    groupBox2->setObjectName(QString::fromUtf8("groupBox2"));
    // groupBox2->setColumnLayout(0, Qt::Vertical);
//    groupBox2->layout()->setSpacing(6);
//    groupBox2->layout()->setMargin(0);
    vboxLayout3 = new QVBoxLayout(groupBox2);//->layout());
    vboxLayout3->setAlignment(Qt::AlignTop);
    vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
    vboxLayout3->setMargin(0);
    thePluginDescription = new QTextEdit(groupBox2);
    thePluginDescription->setObjectName(QString::fromUtf8("thePluginDescription"));
    thePluginDescription->setEnabled(true);
    thePluginDescription->setReadOnly(true);

    vboxLayout3->addWidget(thePluginDescription);


    hboxLayout->addWidget(groupBox2);

    theTabWidget->addTab(TabPage, QApplication::translate("ossimQtPreferencesDialog", "Plugins", 0, QApplication::UnicodeUTF8));

    vboxLayout->addWidget(theTabWidget);

    hboxLayout2 = new QHBoxLayout();
    hboxLayout2->setSpacing(6);
    hboxLayout2->setMargin(0);
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    spacerItem = new QSpacerItem(284, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout2->addItem(spacerItem);

    theCloseButton = new QPushButton(ossimQtPreferencesDialog);
    theCloseButton->setObjectName(QString::fromUtf8("theCloseButton"));

    hboxLayout2->addWidget(theCloseButton);

    spacerItem1 = new QSpacerItem(284, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout2->addItem(spacerItem1);


    vboxLayout->addLayout(hboxLayout2);

    hboxLayout3 = new QHBoxLayout();
    hboxLayout3->setSpacing(6);
    hboxLayout3->setMargin(0);
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));

    vboxLayout->addLayout(hboxLayout3);


    retranslateUi(ossimQtPreferencesDialog);

    QSize size(684, 409);
    size = size.expandedTo(ossimQtPreferencesDialog->minimumSizeHint());
    ossimQtPreferencesDialog->resize(size);

    QObject::connect(theAddPluginButton, SIGNAL(clicked()), ossimQtPreferencesDialog, SLOT(theAddPluginButton_clicked()));
    QObject::connect(theRemovePluginButton, SIGNAL(clicked()), ossimQtPreferencesDialog, SLOT(theRemovePluginButton_clicked()));
    QObject::connect(theCloseButton, SIGNAL(clicked()), ossimQtPreferencesDialog, SLOT(theCloseButton_clicked()));
    QObject::connect(thePluginList, SIGNAL(itemClicked(QListWidgetItem*)), ossimQtPreferencesDialog, SLOT(thePluginList_currentChanged(QListWidgetItem*)));
    QObject::connect(theSelectAllPluginButton, SIGNAL(clicked()), ossimQtPreferencesDialog, SLOT(theSelectAllPluginButton_clicked()));
    QObject::connect(theReloadPluginButton, SIGNAL(clicked()), ossimQtPreferencesDialog, SLOT(theReloadPluginButton_clicked()));

    QMetaObject::connectSlotsByName(ossimQtPreferencesDialog);
} // setupUi

void ossimQtPreferencesDialog::retranslateUi(QDialog *ossimQtPreferencesDialog)
{
    ossimQtPreferencesDialog->setWindowTitle(QApplication::translate("ossimQtPreferencesDialog", "Preferences", 0, QApplication::UnicodeUTF8));
    groupBox1->setTitle(QApplication::translate("ossimQtPreferencesDialog", "Plugins", 0, QApplication::UnicodeUTF8));
    theAddPluginButton->setText(QApplication::translate("ossimQtPreferencesDialog", "Add", 0, QApplication::UnicodeUTF8));
    theSelectAllPluginButton->setText(QApplication::translate("ossimQtPreferencesDialog", "Select all", 0, QApplication::UnicodeUTF8));
    theReloadPluginButton->setText(QApplication::translate("ossimQtPreferencesDialog", "Reload", 0, QApplication::UnicodeUTF8));
    theRemovePluginButton->setText(QApplication::translate("ossimQtPreferencesDialog", "Remove", 0, QApplication::UnicodeUTF8));
    groupBox2->setTitle(QApplication::translate("ossimQtPreferencesDialog", "Description", 0, QApplication::UnicodeUTF8));
    theTabWidget->setTabText(theTabWidget->indexOf(TabPage), QApplication::translate("ossimQtPreferencesDialog", "Plugins", 0, QApplication::UnicodeUTF8));
    theCloseButton->setText(QApplication::translate("ossimQtPreferencesDialog", "Close", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ossimQtPreferencesDialog);
} // retranslateUi

void ossimQtPreferencesDialog::init()
{
    theController = new ossimQtPreferencesDialogController(this);
}

void ossimQtPreferencesDialog::destroy()
{
    if(theController)
    {
	delete theController;
	theController = 0;
    }
}

ossimQtPreferencesDialogController* ossimQtPreferencesDialog::controller()
{
    return theController;
}


void ossimQtPreferencesDialog::theAddPluginButton_clicked()
{
    if(theController)
    {
	theController->addPlugin();
    }
}

void ossimQtPreferencesDialog::theRemovePluginButton_clicked()
{
    if(theController)
    {
	theController->removePlugin();
    }
}


void ossimQtPreferencesDialog::theCloseButton_clicked()
{
    reject();
}


void ossimQtPreferencesDialog::thePluginList_currentChanged( QListWidgetItem *  item)
{
    if(theController)
    {
	theController->currentPluginSelectionChanged(item);
    }
}


void ossimQtPreferencesDialog::theSelectAllPluginButton_clicked()
{
    if(theController)
    {
	theController->selectAllPlugin();
    }
}


void ossimQtPreferencesDialog::theReloadPluginButton_clicked()
{
    if(theController)
    {
	theController->reloadPlugin();
    }

}
