
//----------------------------------------------------------------------------
// 
// File: ossimQtBandSelectorDialog.cpp
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description: Band selector dialog box.  Ported from QT3 ui file.
// 
//----------------------------------------------------------------------------

#include <ossimQt/ossimQtBandSelectorDialog.h>

#include <QtGui/QListWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QRadioButton>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QFrame>
#include <QtGui/QPushButton>
#include <QtGui/QApplication>

#include <ossimQt/ossimQtBandSelectorController.h>
#include <ossimQt/ossimQtBandSelectorDialog.h>
#include <ossim/imaging/ossimBandSelector.h>

/*
 *  Constructs a ossimQtBandSelectorDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
ossimQtBandSelectorDialog::ossimQtBandSelectorDialog(QWidget* parent, const char* name, bool modal, Qt::WFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);


    // signals and slots connections
    QObject::connect((QObject*)theClearButton,
                     SIGNAL(clicked()),
                     (QObject*)this,
                     SLOT(clearButtonClicked()));
    
    QObject::connect((QObject*)theAllButton,
                     SIGNAL(clicked()),
                     (QObject*)this,
                     SLOT(allButtonClicked()));
    
    QObject::connect((QObject*)theRightButton,
                     SIGNAL(clicked()),
                     (QObject*)this,
                     SLOT(rightButtonClicked()));
    
    QObject::connect((QObject*)theLeftButton,
                     SIGNAL(clicked()),
                     (QObject*)this,
                     SLOT(leftButtonClicked()));
    
    QObject::connect((QObject*)theUpButton,
                     SIGNAL(clicked()),
                     (QObject*)this,
                     SLOT(upButtonClicked()));
    
    QObject::connect((QObject*)theDownButton,
                     SIGNAL(clicked()),
                     (QObject*)this,
                     SLOT(downButtonClicked()));
    
    QObject::connect((QObject*)theMoveToTopButton,
                     SIGNAL(clicked()),
                     (QObject*)this,
                     SLOT(moveToTopButtonClicked()));
    
    QObject::connect((QObject*)theMoveToBottomButton,
                     SIGNAL(clicked()),
                     (QObject*)this,
                     SLOT(moveToBottomButtonClicked()));
    
    QObject::connect((QObject*)theApplyButton,
                     SIGNAL(clicked()),
                     (QObject*)this,
                     SLOT(applyButtonClicked()));
    
    QObject::connect((QObject*)theCloseButton,
                     SIGNAL(clicked()),
                     (QObject*)this,
                     SLOT(closeButtonClicked()));
    
    QObject::connect((QObject*)theGreyButton,
                     SIGNAL(clicked()),
                     (QObject*)this,
                     SLOT(greyButtonClicked()));
    
    QObject::connect((QObject*)theTrueButton,
                     SIGNAL(clicked()),
                     (QObject*)this,
                     SLOT(trueButtonClicked()));
    
    QObject::connect((QObject*)theNBandButton,
                     SIGNAL(clicked()),
                     (QObject*)this,
                     SLOT(nBandButtonClicked()));
    
    QObject::connect((QObject*)theAvailableBandsListBox,
                     SIGNAL(itemClicked(QListWidgetItem*)),
                     (QObject*)this,
                     SLOT(availableBandClicked(QListWidgetItem*)));
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ossimQtBandSelectorDialog::~ossimQtBandSelectorDialog()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ossimQtBandSelectorDialog::languageChange()
{
    retranslateUi(this);
}

void ossimQtBandSelectorDialog::greyButtonClicked()
{
   if (theController)
   {
      theController->greyButtonClicked();
   }
}

void ossimQtBandSelectorDialog::trueButtonClicked()
{
   if (theController)
   {
      theController->trueButtonClicked();
   }
}

void ossimQtBandSelectorDialog::nBandButtonClicked()
{
   if (theController)
   {
      theController->nBandButtonClicked();
   }
}

void ossimQtBandSelectorDialog::clearButtonClicked()
{
   if (theController)
   {
      theController->clearButtonClicked();
   }

}

void ossimQtBandSelectorDialog::downButtonClicked()
{
   if (theController)
   {
      theController->downButtonClicked();
   }
}

void ossimQtBandSelectorDialog::moveToTopButtonClicked()
{
   if (theController)
   {
      theController->moveToTopButtonClicked();
   }
}

void ossimQtBandSelectorDialog::moveToBottomButtonClicked()
{
   if (theController)
   {
      theController->moveToBottomButtonClicked();
   }
}

void ossimQtBandSelectorDialog::applyButtonClicked()
{
   if (theController)
   {
      theController->applyButtonClicked();
   }
}

void ossimQtBandSelectorDialog::closeButtonClicked()
{
   close(true);
}

void ossimQtBandSelectorDialog::init()
{
   theController = new ossimQtBandSelectorController(this);
   theAvailableBandsListBox->setSelectionMode(QAbstractItemView::SingleSelection);
   theAvailableBandsListBox->setName("available_bands");
   theSelectedBandsListBox->setName("selected_bands");
}

void ossimQtBandSelectorDialog::destroy()
{
   if (theController)
   {
      delete theController;
      theController = NULL;
   }
}


void ossimQtBandSelectorDialog::setBandSelector( ossimBandSelector * bandSelector )
{
   if (theController)
   {
      theController->setBandSelector(bandSelector);
   }
}


void ossimQtBandSelectorDialog::updateDisplay()
{

}



void ossimQtBandSelectorDialog::allButtonClicked()
{
   if (theController)
   {
      theController->allButtonClicked();
   }
}


void ossimQtBandSelectorDialog::rightButtonClicked()
{
   if (theController)
   {
      theController->rightButtonClicked();
   }
}


void ossimQtBandSelectorDialog::leftButtonClicked()
{
   if (theController)
   {
      theController->leftButtonClicked();
   }
}


void ossimQtBandSelectorDialog::upButtonClicked()
{
   if (theController)
   {
      theController->upButtonClicked();
   }
}

void ossimQtBandSelectorDialog::availableBandClicked(QListWidgetItem* item)
{
   if (theController)
   {
      theController->availableBandClicked(item);
   }

}

void Ui_ossimQtBandSelectorDialog::setupUi(QDialog *ossimQtBandSelectorDialog)
{
   ossimQtBandSelectorDialog->setObjectName(QString::fromUtf8("ossimQtBandSelectorDialog"));
   
   ossimQtBandSelectorDialog->resize(QSize(351, 375).expandedTo(ossimQtBandSelectorDialog->minimumSizeHint()));
   
   vboxLayout = new QVBoxLayout(ossimQtBandSelectorDialog);
   // vboxLayout->setSpacing(6);
   // vboxLayout->setMargin(11);
   vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));

   theImageTypeGroupBox = new QGroupBox(ossimQtBandSelectorDialog);
   
   theGreyButton  = new QRadioButton("grey");
   theTrueButton  = new QRadioButton("true");
   theNBandButton = new QRadioButton("nband");

   QHBoxLayout* hbox = new QHBoxLayout;
   hbox->addWidget(theGreyButton);
   hbox->addWidget(theTrueButton);
   hbox->addWidget(theNBandButton);
   theImageTypeGroupBox->setLayout(hbox);
   vboxLayout->addWidget(theImageTypeGroupBox);

   hboxLayout1 = new QHBoxLayout();
   hboxLayout1->setSpacing(6);
   hboxLayout1->setMargin(0);
   hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
   theAvailableBandLabel = new QLabel(ossimQtBandSelectorDialog);
   theAvailableBandLabel->setObjectName(QString::fromUtf8("theAvailableBandLabel"));
   theAvailableBandLabel->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
   
   hboxLayout1->addWidget(theAvailableBandLabel);
   
   spacerItem = new QSpacerItem(120, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
   
   hboxLayout1->addItem(spacerItem);
   
   theSelectedBandsLabel = new QLabel(ossimQtBandSelectorDialog);
   theSelectedBandsLabel->setObjectName(QString::fromUtf8("theSelectedBandsLabel"));
   theSelectedBandsLabel->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
   
   hboxLayout1->addWidget(theSelectedBandsLabel);
   
   
   vboxLayout->addLayout(hboxLayout1);
   
   hboxLayout2 = new QHBoxLayout();
   hboxLayout2->setSpacing(6);
   hboxLayout2->setMargin(0);
   hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
   theAvailableBandsListBox = new QListWidget(ossimQtBandSelectorDialog);
   theAvailableBandsListBox->setObjectName(QString::fromUtf8("theAvailableBandsListBox"));
   
   hboxLayout2->addWidget(theAvailableBandsListBox);
   
   vboxLayout1 = new QVBoxLayout();
   vboxLayout1->setSpacing(0);
   vboxLayout1->setMargin(0);
   vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
   theClearButton = new QPushButton(ossimQtBandSelectorDialog);
   theClearButton->setObjectName(QString::fromUtf8("theClearButton"));

   vboxLayout1->addWidget(theClearButton);

   theAllButton = new QPushButton(ossimQtBandSelectorDialog);
   theAllButton->setObjectName(QString::fromUtf8("theAllButton"));

   vboxLayout1->addWidget(theAllButton);

   theRightButton = new QPushButton(ossimQtBandSelectorDialog);
   theRightButton->setObjectName(QString::fromUtf8("theRightButton"));

   vboxLayout1->addWidget(theRightButton);

   theLeftButton = new QPushButton(ossimQtBandSelectorDialog);
   theLeftButton->setObjectName(QString::fromUtf8("theLeftButton"));

   vboxLayout1->addWidget(theLeftButton);

   theUpButton = new QPushButton(ossimQtBandSelectorDialog);
   theUpButton->setObjectName(QString::fromUtf8("theUpButton"));

   vboxLayout1->addWidget(theUpButton);

   theDownButton = new QPushButton(ossimQtBandSelectorDialog);
   theDownButton->setObjectName(QString::fromUtf8("theDownButton"));

   vboxLayout1->addWidget(theDownButton);

   theMoveToTopButton = new QPushButton(ossimQtBandSelectorDialog);
   theMoveToTopButton->setObjectName(QString::fromUtf8("theMoveToTopButton"));

   vboxLayout1->addWidget(theMoveToTopButton);

   theMoveToBottomButton = new QPushButton(ossimQtBandSelectorDialog);
   theMoveToBottomButton->setObjectName(QString::fromUtf8("theMoveToBottomButton"));

   vboxLayout1->addWidget(theMoveToBottomButton);


   hboxLayout2->addLayout(vboxLayout1);

   theSelectedBandsListBox = new QListWidget(ossimQtBandSelectorDialog);
   theSelectedBandsListBox->setObjectName(QString::fromUtf8("theSelectedBandsListBox"));

   hboxLayout2->addWidget(theSelectedBandsListBox);


   vboxLayout->addLayout(hboxLayout2);

   theLine1 = new QFrame(ossimQtBandSelectorDialog);
   theLine1->setObjectName(QString::fromUtf8("theLine1"));
   theLine1->setFrameShape(QFrame::HLine);
   theLine1->setFrameShadow(QFrame::Sunken);

   vboxLayout->addWidget(theLine1);

   hboxLayout3 = new QHBoxLayout();
   hboxLayout3->setSpacing(6);
   hboxLayout3->setMargin(0);
   hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
   spacerItem1 = new QSpacerItem(30, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

   hboxLayout3->addItem(spacerItem1);

   theApplyButton = new QPushButton(ossimQtBandSelectorDialog);
   theApplyButton->setObjectName(QString::fromUtf8("theApplyButton"));

   hboxLayout3->addWidget(theApplyButton);

   spacerItem2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

   hboxLayout3->addItem(spacerItem2);

   theCloseButton = new QPushButton(ossimQtBandSelectorDialog);
   theCloseButton->setObjectName(QString::fromUtf8("theCloseButton"));

   hboxLayout3->addWidget(theCloseButton);

   spacerItem3 = new QSpacerItem(30, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

   hboxLayout3->addItem(spacerItem3);


   vboxLayout->addLayout(hboxLayout3);

   retranslateUi(ossimQtBandSelectorDialog);

   QMetaObject::connectSlotsByName(ossimQtBandSelectorDialog);
} // setupUi

void Ui_ossimQtBandSelectorDialog::retranslateUi(QDialog *ossimQtBandSelectorDialog)
{
   ossimQtBandSelectorDialog->setWindowTitle(QApplication::translate("ossimQtBandSelectorDialog", "Band Selector", 0, QApplication::UnicodeUTF8));

   // theImageTypeGroupBox->setTitle(QApplication::translate("ossimQtBandSelectorDialog", "", 0, QApplication::UnicodeUTF8));
   theGreyButton->setText(QApplication::translate("ossimQtBandSelectorDialog", "Grey", 0, QApplication::UnicodeUTF8));
   theTrueButton->setText(QApplication::translate("ossimQtBandSelectorDialog", "True", 0, QApplication::UnicodeUTF8));
   theNBandButton->setText(QApplication::translate("ossimQtBandSelectorDialog", "N-Band", 0, QApplication::UnicodeUTF8));
   theAvailableBandLabel->setText(QApplication::translate("ossimQtBandSelectorDialog", "available bands:", 0, QApplication::UnicodeUTF8));
   theSelectedBandsLabel->setText(QApplication::translate("ossimQtBandSelectorDialog", "selected bands:", 0, QApplication::UnicodeUTF8));
   theAvailableBandsListBox->clear();
   // theAvailableBandsListBox->insertItem(QApplication::translate("ossimQtBandSelectorDialog", "New Item", 0, QApplication::UnicodeUTF8));
   theAvailableBandsListBox->setProperty("toolTip", QVariant(QApplication::translate("ossimQtBandSelectorDialog", "\"available bands:\"", 0, QApplication::UnicodeUTF8)));
   theClearButton->setText(QApplication::translate("ossimQtBandSelectorDialog", "Clear", 0, QApplication::UnicodeUTF8));
   theAllButton->setText(QApplication::translate("ossimQtBandSelectorDialog", "All", 0, QApplication::UnicodeUTF8));
   theRightButton->setText(QApplication::translate("ossimQtBandSelectorDialog", "--------->", 0, QApplication::UnicodeUTF8));
   theLeftButton->setText(QApplication::translate("ossimQtBandSelectorDialog", "<---------", 0, QApplication::UnicodeUTF8));
   theUpButton->setText(QApplication::translate("ossimQtBandSelectorDialog", "Up", 0, QApplication::UnicodeUTF8));
   theDownButton->setText(QApplication::translate("ossimQtBandSelectorDialog", "Down", 0, QApplication::UnicodeUTF8));
   theMoveToTopButton->setText(QApplication::translate("ossimQtBandSelectorDialog", "Move to top", 0, QApplication::UnicodeUTF8));
   theMoveToBottomButton->setText(QApplication::translate("ossimQtBandSelectorDialog", "Move To Bottom", 0, QApplication::UnicodeUTF8));
   theSelectedBandsListBox->clear();
   // theSelectedBandsListBox->insertItem(QApplication::translate("ossimQtBandSelectorDialog", "New Item", 0, QApplication::UnicodeUTF8));
   theApplyButton->setText(QApplication::translate("ossimQtBandSelectorDialog", "Apply", 0, QApplication::UnicodeUTF8));
   theCloseButton->setText(QApplication::translate("ossimQtBandSelectorDialog", "Close", 0, QApplication::UnicodeUTF8));
   Q_UNUSED(ossimQtBandSelectorDialog);
} // retranslateUi

