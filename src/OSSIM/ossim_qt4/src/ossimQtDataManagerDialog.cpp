#include <ossimQt/ossimQtDataManagerDialog.h>

#include <QtCore/QVariant>
#include <QtGui/QImage>
#include <QtGui/QPixmap>

#include <ossimQt/ossimQtDataManagerListBox.h>
#include <ossimQt/ossimQtMainWindow.h>
#include <ossimQt/ossimQtEvent.h>
#include <ossimQt/ossimQtApplicationUtility.h>
#include <ossim/base/ossimConnectableObject.h>

/*
 *  Constructs a ossimQtDataManagerDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
ossimQtDataManagerDialog::ossimQtDataManagerDialog(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
ossimQtDataManagerDialog::~ossimQtDataManagerDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ossimQtDataManagerDialog::languageChange()
{
    retranslateUi(this);
}

void ossimQtDataManagerDialog::setDataManager(ossimDataManager* dataManager)
{
    theDataManagerListBox->setDataManager(dataManager);
}


void ossimQtDataManagerDialog::theCloseButton_clicked()
{
    close(true);
}


void ossimQtDataManagerDialog::theDisplayButton_clicked()
{
  ossimConnectableObject::ConnectableObjectList objList = theDataManagerListBox->getSelectedObjects();

  for(int i = 0; i < (int)objList.size(); ++i)
    {
      ossimQtDisplayChainEvent* evt = new ossimQtDisplayChainEvent(objList[i]->getId().getId());
      ossimQtApplicationUtility::postEventToRoot(this, evt);
    }
}

void Ui_ossimQtDataManagerDialog::setupUi(QDialog *ossimQtDataManagerDialog)
{
   if (ossimQtDataManagerDialog->objectName().isEmpty())
      ossimQtDataManagerDialog->setObjectName(QString::fromUtf8("ossimQtDataManagerDialog"));
   ossimQtDataManagerDialog->resize(565, 360);
   hboxLayout = new QHBoxLayout(ossimQtDataManagerDialog);
   hboxLayout->setSpacing(6);
   hboxLayout->setMargin(11);
   hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
   theDataManagerListBox = new ossimQtDataManagerListBox(ossimQtDataManagerDialog);
   theDataManagerListBox->setObjectName(QString::fromUtf8("theDataManagerListBox"));

   hboxLayout->addWidget(theDataManagerListBox);

   vboxLayout = new QVBoxLayout();
   vboxLayout->setSpacing(6);
   vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
   theDisplayButton = new QPushButton(ossimQtDataManagerDialog);
   theDisplayButton->setObjectName(QString::fromUtf8("theDisplayButton"));
   QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
   sizePolicy.setHorizontalStretch(0);
   sizePolicy.setVerticalStretch(0);
   sizePolicy.setHeightForWidth(theDisplayButton->sizePolicy().hasHeightForWidth());
   theDisplayButton->setSizePolicy(sizePolicy);

   vboxLayout->addWidget(theDisplayButton);

   theDeleteButton = new QPushButton(ossimQtDataManagerDialog);
   theDeleteButton->setObjectName(QString::fromUtf8("theDeleteButton"));
   sizePolicy.setHeightForWidth(theDeleteButton->sizePolicy().hasHeightForWidth());
   theDeleteButton->setSizePolicy(sizePolicy);

   vboxLayout->addWidget(theDeleteButton);

   theDuplicatButton = new QPushButton(ossimQtDataManagerDialog);
   theDuplicatButton->setObjectName(QString::fromUtf8("theDuplicatButton"));
   sizePolicy.setHeightForWidth(theDuplicatButton->sizePolicy().hasHeightForWidth());
   theDuplicatButton->setSizePolicy(sizePolicy);

   vboxLayout->addWidget(theDuplicatButton);

   theMosaicButton = new QPushButton(ossimQtDataManagerDialog);
   theMosaicButton->setObjectName(QString::fromUtf8("theMosaicButton"));
   theMosaicButton->setEnabled(true);

   vboxLayout->addWidget(theMosaicButton);

   theBlendButton = new QPushButton(ossimQtDataManagerDialog);
   theBlendButton->setObjectName(QString::fromUtf8("theBlendButton"));
   theBlendButton->setEnabled(true);

   vboxLayout->addWidget(theBlendButton);

   theFeatherButton = new QPushButton(ossimQtDataManagerDialog);
   theFeatherButton->setObjectName(QString::fromUtf8("theFeatherButton"));
   theFeatherButton->setEnabled(true);

   vboxLayout->addWidget(theFeatherButton);

   spacerItem = new QSpacerItem(20, 70, QSizePolicy::Minimum, QSizePolicy::Expanding);

   vboxLayout->addItem(spacerItem);

   theCloseButton = new QPushButton(ossimQtDataManagerDialog);
   theCloseButton->setObjectName(QString::fromUtf8("theCloseButton"));
   sizePolicy.setHeightForWidth(theCloseButton->sizePolicy().hasHeightForWidth());
   theCloseButton->setSizePolicy(sizePolicy);

   vboxLayout->addWidget(theCloseButton);


   hboxLayout->addLayout(vboxLayout);


   retranslateUi(ossimQtDataManagerDialog);
   QObject::connect(theCloseButton, SIGNAL(clicked()), ossimQtDataManagerDialog, SLOT(theCloseButton_clicked()));
   QObject::connect(theDisplayButton, SIGNAL(clicked()), ossimQtDataManagerDialog, SLOT(theDisplayButton_clicked()));
   QObject::connect(theDeleteButton, SIGNAL(clicked()), theDataManagerListBox, SLOT(deleteSelectedObjects()));
   QObject::connect(theDuplicatButton, SIGNAL(clicked()), theDataManagerListBox, SLOT(duplicateSelectedObjects()));
   QObject::connect(theMosaicButton, SIGNAL(clicked()), theDataManagerListBox, SLOT(mosaicSelectedObjects()));
   QObject::connect(theBlendButton, SIGNAL(clicked()), theDataManagerListBox, SLOT(blendSelectedObjects()));
   QObject::connect(theFeatherButton, SIGNAL(clicked()), theDataManagerListBox, SLOT(featherSelectedObjects()));

   QMetaObject::connectSlotsByName(ossimQtDataManagerDialog);
} // setupUi

void Ui_ossimQtDataManagerDialog::retranslateUi(QDialog *ossimQtDataManagerDialog)
{
   ossimQtDataManagerDialog->setWindowTitle(QApplication::translate("ossimQtDataManagerDialog", "Data Manager", 0, QApplication::UnicodeUTF8));
   theDisplayButton->setText(QApplication::translate("ossimQtDataManagerDialog", "Display", 0, QApplication::UnicodeUTF8));
   theDeleteButton->setText(QApplication::translate("ossimQtDataManagerDialog", "Delete", 0, QApplication::UnicodeUTF8));
   theDuplicatButton->setText(QApplication::translate("ossimQtDataManagerDialog", "Duplicate", 0, QApplication::UnicodeUTF8));
   theMosaicButton->setText(QApplication::translate("ossimQtDataManagerDialog", "Mosaic", 0, QApplication::UnicodeUTF8));
   theBlendButton->setText(QApplication::translate("ossimQtDataManagerDialog", "Blend", 0, QApplication::UnicodeUTF8));
   theFeatherButton->setText(QApplication::translate("ossimQtDataManagerDialog", "Feather", 0, QApplication::UnicodeUTF8));
   theCloseButton->setText(QApplication::translate("ossimQtDataManagerDialog", "Close", 0, QApplication::UnicodeUTF8));
   Q_UNUSED(ossimQtDataManagerDialog);
} // retranslateUi
