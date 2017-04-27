#include <ossimQt/ossimQtDatumConverterDialog.h>

#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QLabel>

/*
 *  Constructs a ossimQtDatumConverterDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
ossimQtDatumConverterDialog::ossimQtDatumConverterDialog(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

void ossimQtDatumConverterDialog::setupUi(QDialog *ossimQtDatumConverterDialog)
{
   ossimQtDatumConverterDialog->setObjectName(QString::fromUtf8("ossimQtDatumConverterDialog"));
   vboxLayout = new QVBoxLayout(ossimQtDatumConverterDialog);
   vboxLayout->setSpacing(6);
   vboxLayout->setMargin(11);
   vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
   hboxLayout = new QHBoxLayout();
   hboxLayout->setSpacing(6);
   hboxLayout->setMargin(0);
   hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
   gridLayout = new QGridLayout();
   gridLayout->setSpacing(6);
   gridLayout->setMargin(0);
   gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
   groupBox2 = new QGroupBox(ossimQtDatumConverterDialog);
   groupBox2->setObjectName(QString::fromUtf8("groupBox2"));
   // groupBox2->setColumnLayout(0, Qt::Vertical);
//   groupBox2->layout()->setSpacing(6);
//   groupBox2->layout()->setMargin(0);
   vboxLayout1 = new QVBoxLayout(groupBox2);//->layout());
   vboxLayout1->setAlignment(Qt::AlignTop);
   vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
   vboxLayout1->setMargin(0);
   textEdit1 = new QTextEdit(groupBox2);
   textEdit1->setObjectName(QString::fromUtf8("textEdit1"));

   vboxLayout1->addWidget(textEdit1);


   gridLayout->addWidget(groupBox2, 1, 0, 1, 4);

   textLabel1_2_2_2_2_2_2_2 = new QLabel(ossimQtDatumConverterDialog);
   textLabel1_2_2_2_2_2_2_2->setObjectName(QString::fromUtf8("textLabel1_2_2_2_2_2_2_2"));
   textLabel1_2_2_2_2_2_2_2->setWordWrap(false);

   gridLayout->addWidget(textLabel1_2_2_2_2_2_2_2, 8, 0, 1, 2);

   lineEdit1_2_2_2_2_2_2 = new QLineEdit(ossimQtDatumConverterDialog);
   lineEdit1_2_2_2_2_2_2->setObjectName(QString::fromUtf8("lineEdit1_2_2_2_2_2_2"));

   gridLayout->addWidget(lineEdit1_2_2_2_2_2_2, 3, 2, 1, 2);

   textLabel1_2_2_2_2_2_2_2_2 = new QLabel(ossimQtDatumConverterDialog);
   textLabel1_2_2_2_2_2_2_2_2->setObjectName(QString::fromUtf8("textLabel1_2_2_2_2_2_2_2_2"));
   textLabel1_2_2_2_2_2_2_2_2->setWordWrap(false);

   gridLayout->addWidget(textLabel1_2_2_2_2_2_2_2_2, 9, 0, 1, 2);

   textLabel1_2_2_2_2 = new QLabel(ossimQtDatumConverterDialog);
   textLabel1_2_2_2_2->setObjectName(QString::fromUtf8("textLabel1_2_2_2_2"));
   textLabel1_2_2_2_2->setWordWrap(false);

   gridLayout->addWidget(textLabel1_2_2_2_2, 5, 0, 1, 2);

   lineEdit1_2_2_2_2_2_2_2 = new QLineEdit(ossimQtDatumConverterDialog);
   lineEdit1_2_2_2_2_2_2_2->setObjectName(QString::fromUtf8("lineEdit1_2_2_2_2_2_2_2"));

   gridLayout->addWidget(lineEdit1_2_2_2_2_2_2_2, 2, 2, 1, 2);

   lineEdit1_2_2 = new QLineEdit(ossimQtDatumConverterDialog);
   lineEdit1_2_2->setObjectName(QString::fromUtf8("lineEdit1_2_2"));

   gridLayout->addWidget(lineEdit1_2_2, 7, 2, 1, 2);

   lineEdit1_2_2_2_2_2 = new QLineEdit(ossimQtDatumConverterDialog);
   lineEdit1_2_2_2_2_2->setObjectName(QString::fromUtf8("lineEdit1_2_2_2_2_2"));

   gridLayout->addWidget(lineEdit1_2_2_2_2_2, 4, 2, 1, 1);

   theDatumLeftCombo = new QComboBox(ossimQtDatumConverterDialog);
   theDatumLeftCombo->setObjectName(QString::fromUtf8("theDatumLeftCombo"));

   gridLayout->addWidget(theDatumLeftCombo, 0, 1, 1, 3);

   textLabel1 = new QLabel(ossimQtDatumConverterDialog);
   textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
   textLabel1->setWordWrap(false);

   gridLayout->addWidget(textLabel1, 0, 0, 1, 1);

   lineEdit1_2_2_2 = new QLineEdit(ossimQtDatumConverterDialog);
   lineEdit1_2_2_2->setObjectName(QString::fromUtf8("lineEdit1_2_2_2"));

   gridLayout->addWidget(lineEdit1_2_2_2, 6, 2, 1, 2);

   lineEdit1 = new QLineEdit(ossimQtDatumConverterDialog);
   lineEdit1->setObjectName(QString::fromUtf8("lineEdit1"));

   gridLayout->addWidget(lineEdit1, 9, 2, 1, 2);

   checkBox4 = new QCheckBox(ossimQtDatumConverterDialog);
   checkBox4->setObjectName(QString::fromUtf8("checkBox4"));

   gridLayout->addWidget(checkBox4, 4, 3, 1, 1);

   textLabel1_2_2_2_2_2_2 = new QLabel(ossimQtDatumConverterDialog);
   textLabel1_2_2_2_2_2_2->setObjectName(QString::fromUtf8("textLabel1_2_2_2_2_2_2"));
   textLabel1_2_2_2_2_2_2->setWordWrap(false);

   gridLayout->addWidget(textLabel1_2_2_2_2_2_2, 7, 0, 1, 2);

   textLabel1_2 = new QLabel(ossimQtDatumConverterDialog);
   textLabel1_2->setObjectName(QString::fromUtf8("textLabel1_2"));
   textLabel1_2->setWordWrap(false);

   gridLayout->addWidget(textLabel1_2, 2, 0, 1, 2);

   textLabel1_2_2_2_2_2 = new QLabel(ossimQtDatumConverterDialog);
   textLabel1_2_2_2_2_2->setObjectName(QString::fromUtf8("textLabel1_2_2_2_2_2"));
   textLabel1_2_2_2_2_2->setWordWrap(false);

   gridLayout->addWidget(textLabel1_2_2_2_2_2, 6, 0, 1, 2);

   textLabel1_2_2 = new QLabel(ossimQtDatumConverterDialog);
   textLabel1_2_2->setObjectName(QString::fromUtf8("textLabel1_2_2"));
   textLabel1_2_2->setWordWrap(false);

   gridLayout->addWidget(textLabel1_2_2, 3, 0, 1, 2);

   lineEdit1_2 = new QLineEdit(ossimQtDatumConverterDialog);
   lineEdit1_2->setObjectName(QString::fromUtf8("lineEdit1_2"));

   gridLayout->addWidget(lineEdit1_2, 8, 2, 1, 2);

   lineEdit1_2_2_2_2 = new QLineEdit(ossimQtDatumConverterDialog);
   lineEdit1_2_2_2_2->setObjectName(QString::fromUtf8("lineEdit1_2_2_2_2"));

   gridLayout->addWidget(lineEdit1_2_2_2_2, 5, 2, 1, 2);

   textLabel1_2_2_2 = new QLabel(ossimQtDatumConverterDialog);
   textLabel1_2_2_2->setObjectName(QString::fromUtf8("textLabel1_2_2_2"));
   textLabel1_2_2_2->setWordWrap(false);

   gridLayout->addWidget(textLabel1_2_2_2, 4, 0, 1, 2);


   hboxLayout->addLayout(gridLayout);

   line2 = new QFrame(ossimQtDatumConverterDialog);
   line2->setObjectName(QString::fromUtf8("line2"));
   line2->setFrameShape(QFrame::VLine);
   line2->setFrameShadow(QFrame::Sunken);

   hboxLayout->addWidget(line2);

   gridLayout1 = new QGridLayout();
   gridLayout1->setSpacing(6);
   gridLayout1->setMargin(0);
   gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
   textLabel1_2_3 = new QLabel(ossimQtDatumConverterDialog);
   textLabel1_2_3->setObjectName(QString::fromUtf8("textLabel1_2_3"));
   textLabel1_2_3->setWordWrap(false);

   gridLayout1->addWidget(textLabel1_2_3, 2, 0, 1, 2);

   lineEdit1_2_2_2_2_2_2_3 = new QLineEdit(ossimQtDatumConverterDialog);
   lineEdit1_2_2_2_2_2_2_3->setObjectName(QString::fromUtf8("lineEdit1_2_2_2_2_2_2_3"));

   gridLayout1->addWidget(lineEdit1_2_2_2_2_2_2_3, 3, 2, 1, 2);

   textLabel1_2_2_2_2_2_2_3 = new QLabel(ossimQtDatumConverterDialog);
   textLabel1_2_2_2_2_2_2_3->setObjectName(QString::fromUtf8("textLabel1_2_2_2_2_2_2_3"));
   textLabel1_2_2_2_2_2_2_3->setWordWrap(false);

   gridLayout1->addWidget(textLabel1_2_2_2_2_2_2_3, 7, 0, 1, 2);

   theDatumLeftCombo_2 = new QComboBox(ossimQtDatumConverterDialog);
   theDatumLeftCombo_2->setObjectName(QString::fromUtf8("theDatumLeftCombo_2"));

   gridLayout1->addWidget(theDatumLeftCombo_2, 0, 1, 1, 3);

   textLabel1_2_2_2_2_2_3 = new QLabel(ossimQtDatumConverterDialog);
   textLabel1_2_2_2_2_2_3->setObjectName(QString::fromUtf8("textLabel1_2_2_2_2_2_3"));
   textLabel1_2_2_2_2_2_3->setWordWrap(false);

   gridLayout1->addWidget(textLabel1_2_2_2_2_2_3, 6, 0, 1, 2);

   lineEdit1_2_2_2_3 = new QLineEdit(ossimQtDatumConverterDialog);
   lineEdit1_2_2_2_3->setObjectName(QString::fromUtf8("lineEdit1_2_2_2_3"));

   gridLayout1->addWidget(lineEdit1_2_2_2_3, 6, 2, 1, 2);

   lineEdit1_2_2_2_2_3 = new QLineEdit(ossimQtDatumConverterDialog);
   lineEdit1_2_2_2_2_3->setObjectName(QString::fromUtf8("lineEdit1_2_2_2_2_3"));

   gridLayout1->addWidget(lineEdit1_2_2_2_2_3, 5, 2, 1, 2);

   textLabel1_2_2_3 = new QLabel(ossimQtDatumConverterDialog);
   textLabel1_2_2_3->setObjectName(QString::fromUtf8("textLabel1_2_2_3"));
   textLabel1_2_2_3->setWordWrap(false);

   gridLayout1->addWidget(textLabel1_2_2_3, 3, 0, 1, 2);

   lineEdit1_2_2_2_2_2_3 = new QLineEdit(ossimQtDatumConverterDialog);
   lineEdit1_2_2_2_2_2_3->setObjectName(QString::fromUtf8("lineEdit1_2_2_2_2_2_3"));

   gridLayout1->addWidget(lineEdit1_2_2_2_2_2_3, 4, 2, 1, 1);

   lineEdit1_2_3 = new QLineEdit(ossimQtDatumConverterDialog);
   lineEdit1_2_3->setObjectName(QString::fromUtf8("lineEdit1_2_3"));

   gridLayout1->addWidget(lineEdit1_2_3, 8, 2, 1, 2);

   lineEdit1_2_2_2_2_2_2_2_2 = new QLineEdit(ossimQtDatumConverterDialog);
   lineEdit1_2_2_2_2_2_2_2_2->setObjectName(QString::fromUtf8("lineEdit1_2_2_2_2_2_2_2_2"));

   gridLayout1->addWidget(lineEdit1_2_2_2_2_2_2_2_2, 2, 2, 1, 2);

   textLabel1_2_2_2_2_3 = new QLabel(ossimQtDatumConverterDialog);
   textLabel1_2_2_2_2_3->setObjectName(QString::fromUtf8("textLabel1_2_2_2_2_3"));
   textLabel1_2_2_2_2_3->setWordWrap(false);

   gridLayout1->addWidget(textLabel1_2_2_2_2_3, 5, 0, 1, 2);

   lineEdit1_2_2_3 = new QLineEdit(ossimQtDatumConverterDialog);
   lineEdit1_2_2_3->setObjectName(QString::fromUtf8("lineEdit1_2_2_3"));

   gridLayout1->addWidget(lineEdit1_2_2_3, 7, 2, 1, 2);

   textLabel1_2_2_2_2_2_2_2_2_2 = new QLabel(ossimQtDatumConverterDialog);
   textLabel1_2_2_2_2_2_2_2_2_2->setObjectName(QString::fromUtf8("textLabel1_2_2_2_2_2_2_2_2_2"));
   textLabel1_2_2_2_2_2_2_2_2_2->setWordWrap(false);

   gridLayout1->addWidget(textLabel1_2_2_2_2_2_2_2_2_2, 9, 0, 1, 2);

   textLabel1_2_2_2_2_2_2_2_3 = new QLabel(ossimQtDatumConverterDialog);
   textLabel1_2_2_2_2_2_2_2_3->setObjectName(QString::fromUtf8("textLabel1_2_2_2_2_2_2_2_3"));
   textLabel1_2_2_2_2_2_2_2_3->setWordWrap(false);

   gridLayout1->addWidget(textLabel1_2_2_2_2_2_2_2_3, 8, 0, 1, 2);

   textLabel1_3 = new QLabel(ossimQtDatumConverterDialog);
   textLabel1_3->setObjectName(QString::fromUtf8("textLabel1_3"));
   textLabel1_3->setWordWrap(false);

   gridLayout1->addWidget(textLabel1_3, 0, 0, 1, 1);

   textLabel1_2_2_2_3 = new QLabel(ossimQtDatumConverterDialog);
   textLabel1_2_2_2_3->setObjectName(QString::fromUtf8("textLabel1_2_2_2_3"));
   textLabel1_2_2_2_3->setWordWrap(false);

   gridLayout1->addWidget(textLabel1_2_2_2_3, 4, 0, 1, 2);

   groupBox2_2 = new QGroupBox(ossimQtDatumConverterDialog);
   groupBox2_2->setObjectName(QString::fromUtf8("groupBox2_2"));
   // groupBox2_2->setColumnLayout(0, Qt::Vertical);
   //groupBox2_2->layout()->setSpacing(6);
   //groupBox2_2->layout()->setMargin(0);
   vboxLayout2 = new QVBoxLayout(groupBox2_2);//->layout());
   vboxLayout2->setAlignment(Qt::AlignTop);
   vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
   vboxLayout2->setMargin(0);
   textEdit1_2 = new QTextEdit(groupBox2_2);
   textEdit1_2->setObjectName(QString::fromUtf8("textEdit1_2"));

   vboxLayout2->addWidget(textEdit1_2);


   gridLayout1->addWidget(groupBox2_2, 1, 0, 1, 4);

   checkBox4_2 = new QCheckBox(ossimQtDatumConverterDialog);
   checkBox4_2->setObjectName(QString::fromUtf8("checkBox4_2"));

   gridLayout1->addWidget(checkBox4_2, 4, 3, 1, 1);

   lineEdit1_3 = new QLineEdit(ossimQtDatumConverterDialog);
   lineEdit1_3->setObjectName(QString::fromUtf8("lineEdit1_3"));

   gridLayout1->addWidget(lineEdit1_3, 9, 2, 1, 2);


   hboxLayout->addLayout(gridLayout1);


   vboxLayout->addLayout(hboxLayout);

   line2_2 = new QFrame(ossimQtDatumConverterDialog);
   line2_2->setObjectName(QString::fromUtf8("line2_2"));
   line2_2->setFrameShape(QFrame::HLine);
   line2_2->setFrameShadow(QFrame::Sunken);

   vboxLayout->addWidget(line2_2);

   hboxLayout1 = new QHBoxLayout();
   hboxLayout1->setSpacing(6);
   hboxLayout1->setMargin(0);
   hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
   spacerItem = new QSpacerItem(342, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

   hboxLayout1->addItem(spacerItem);

   pushButton12 = new QPushButton(ossimQtDatumConverterDialog);
   pushButton12->setObjectName(QString::fromUtf8("pushButton12"));
   QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
   sizePolicy.setHorizontalStretch(0);
   sizePolicy.setVerticalStretch(0);
   sizePolicy.setHeightForWidth(pushButton12->sizePolicy().hasHeightForWidth());
   pushButton12->setSizePolicy(sizePolicy);

   hboxLayout1->addWidget(pushButton12);

   spacerItem1 = new QSpacerItem(341, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

   hboxLayout1->addItem(spacerItem1);


   vboxLayout->addLayout(hboxLayout1);


   retranslateUi(ossimQtDatumConverterDialog);

   QSize size(799, 471);
   size = size.expandedTo(ossimQtDatumConverterDialog->minimumSizeHint());
   ossimQtDatumConverterDialog->resize(size);

   QObject::connect(pushButton12, SIGNAL(clicked()), ossimQtDatumConverterDialog, SLOT(pushButton12_clicked()));

   QMetaObject::connectSlotsByName(ossimQtDatumConverterDialog);
} // setupUi

void ossimQtDatumConverterDialog::retranslateUi(QDialog *ossimQtDatumConverterDialog)
{
   ossimQtDatumConverterDialog->setWindowTitle(QApplication::translate("ossimQtDatumConverterDialog", "Datum converter", 0, QApplication::UnicodeUTF8));
   groupBox2->setTitle(QApplication::translate("ossimQtDatumConverterDialog", "Datum Description", 0, QApplication::UnicodeUTF8));
   textLabel1_2_2_2_2_2_2_2->setText(QApplication::translate("ossimQtDatumConverterDialog", "Height above mean sea level", 0, QApplication::UnicodeUTF8));
   textLabel1_2_2_2_2_2_2_2_2->setText(QApplication::translate("ossimQtDatumConverterDialog", "Geocentric <X, Y, Z>", 0, QApplication::UnicodeUTF8));
   textLabel1_2_2_2_2->setText(QApplication::translate("ossimQtDatumConverterDialog", "UTM Easting", 0, QApplication::UnicodeUTF8));
   textLabel1->setText(QApplication::translate("ossimQtDatumConverterDialog", "Datum:", 0, QApplication::UnicodeUTF8));
   checkBox4->setText(QApplication::translate("ossimQtDatumConverterDialog", "Fix zone", 0, QApplication::UnicodeUTF8));
   textLabel1_2_2_2_2_2_2->setText(QApplication::translate("ossimQtDatumConverterDialog", "Height above ellipsoid", 0, QApplication::UnicodeUTF8));
   textLabel1_2->setText(QApplication::translate("ossimQtDatumConverterDialog", "Lat", 0, QApplication::UnicodeUTF8));
   textLabel1_2_2_2_2_2->setText(QApplication::translate("ossimQtDatumConverterDialog", "UTM Northing", 0, QApplication::UnicodeUTF8));
   textLabel1_2_2->setText(QApplication::translate("ossimQtDatumConverterDialog", "Lon:", 0, QApplication::UnicodeUTF8));
   textLabel1_2_2_2->setText(QApplication::translate("ossimQtDatumConverterDialog", "UTM Zone", 0, QApplication::UnicodeUTF8));
   textLabel1_2_3->setText(QApplication::translate("ossimQtDatumConverterDialog", "Lat", 0, QApplication::UnicodeUTF8));
   textLabel1_2_2_2_2_2_2_3->setText(QApplication::translate("ossimQtDatumConverterDialog", "Height above ellipsoid", 0, QApplication::UnicodeUTF8));
   textLabel1_2_2_2_2_2_3->setText(QApplication::translate("ossimQtDatumConverterDialog", "UTM Northing", 0, QApplication::UnicodeUTF8));
   textLabel1_2_2_3->setText(QApplication::translate("ossimQtDatumConverterDialog", "Lon:", 0, QApplication::UnicodeUTF8));
   textLabel1_2_2_2_2_3->setText(QApplication::translate("ossimQtDatumConverterDialog", "UTM Easting", 0, QApplication::UnicodeUTF8));
   textLabel1_2_2_2_2_2_2_2_2_2->setText(QApplication::translate("ossimQtDatumConverterDialog", "Geocentric <X, Y, Z>", 0, QApplication::UnicodeUTF8));
   textLabel1_2_2_2_2_2_2_2_3->setText(QApplication::translate("ossimQtDatumConverterDialog", "Height above mean sea level", 0, QApplication::UnicodeUTF8));
   textLabel1_3->setText(QApplication::translate("ossimQtDatumConverterDialog", "Datum:", 0, QApplication::UnicodeUTF8));
   textLabel1_2_2_2_3->setText(QApplication::translate("ossimQtDatumConverterDialog", "UTM Zone", 0, QApplication::UnicodeUTF8));
   groupBox2_2->setTitle(QApplication::translate("ossimQtDatumConverterDialog", "Datum Description", 0, QApplication::UnicodeUTF8));
   checkBox4_2->setText(QApplication::translate("ossimQtDatumConverterDialog", "Fix zone", 0, QApplication::UnicodeUTF8));
   pushButton12->setText(QApplication::translate("ossimQtDatumConverterDialog", "Close", 0, QApplication::UnicodeUTF8));
   Q_UNUSED(ossimQtDatumConverterDialog);
} // retranslateUi

/*
 *  Destroys the object and frees any allocated resources
 */
ossimQtDatumConverterDialog::~ossimQtDatumConverterDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ossimQtDatumConverterDialog::languageChange()
{
    retranslateUi(this);
}

void ossimQtDatumConverterDialog::pushButton12_clicked()
{
    close(true);
}
