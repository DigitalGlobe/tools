#include <ossimQt/ossimQtViewDialog.h>


#include <ossimQt/ossimQtViewDialogController.h>

/*
 *  Constructs a ossimQtViewDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
ossimQtViewDialog::ossimQtViewDialog(QWidget* parent, const char* name, bool modal, Qt::WFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);


    // signals and slots connections
    connect(theCloseButton, SIGNAL(clicked()), this, SLOT(closeButton_clicked()));
    connect(theApplyButton, SIGNAL(clicked()), this, SLOT(applyButton_clicked()));
    connect(thePropagateButton, SIGNAL(clicked()), this, SLOT(propagateButton_clicked()));
    connect(theMapRadioButton, SIGNAL(clicked()), this, SLOT(mapRadioButton_clicked()));
    connect(theIdentityRadioButton, SIGNAL(clicked()), this, SLOT(identityButton_clicked()));
    connect(theStatePlaneRadioButton, SIGNAL(clicked()), this, SLOT(statePlaneRadioButton_clicked()));
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ossimQtViewDialog::~ossimQtViewDialog()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ossimQtViewDialog::languageChange()
{
    retranslateUi(this);
}

void ossimQtViewDialog::init()
{
    theController = new ossimQtViewDialogController(this);
}

void ossimQtViewDialog::destroy()
{
   if(theController)
   {
      delete theController;
      theController = NULL;
   }
}

void ossimQtViewDialog::setDefaultView()
{
   theController->setDefaultView();
}

void ossimQtViewDialog::closeButton_clicked()
{
   close(true);
}

void ossimQtViewDialog::applyButton_clicked()
{
   theController->apply();
}
ossimQtViewDialogController* ossimQtViewDialog::controller()
{
   return theController;
}

void ossimQtViewDialog::statePlaneRadioButton_clicked()
{
   theController->transformationTypeChanged();
}

void ossimQtViewDialog::mapRadioButton_clicked()
{
   theController->transformationTypeChanged();
}

void ossimQtViewDialog::identityButton_clicked()
{
   theController->transformationTypeChanged();
}

void ossimQtViewDialog::projectionComboBox_textChanged( const QString & value)
{
   theController->currentProjectionChanged(value);
}

void ossimQtViewDialog::projectionComboBox_activated( const QString & value )
{
   theController->currentProjectionChanged(value);
}

void ossimQtViewDialog::propagateButton_clicked()
{
   theController->propagate();
}

void Ui_ossimQtViewDialog::setupUi(QDialog *ossimQtViewDialog)
{
   ossimQtViewDialog->setObjectName(QString::fromUtf8("ossimQtViewDialog"));
   ossimQtViewDialog->resize(QSize(433, 351).expandedTo(ossimQtViewDialog->minimumSizeHint()));
   ossimQtViewDialog->setSizeGripEnabled(true);
   vboxLayout = new Q3VBoxLayout(ossimQtViewDialog);
   vboxLayout->setSpacing(6);
   vboxLayout->setMargin(11);
   vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
   theTransformationGroup = new Q3ButtonGroup(ossimQtViewDialog);
   theTransformationGroup->setObjectName(QString::fromUtf8("theTransformationGroup"));
   QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(1));
   sizePolicy.setHorizontalStretch(0);
   sizePolicy.setVerticalStretch(0);
   sizePolicy.setHeightForWidth(theTransformationGroup->sizePolicy().hasHeightForWidth());
   theTransformationGroup->setSizePolicy(sizePolicy);
   theTransformationGroup->setColumnLayout(0, Qt::Vertical);
   theTransformationGroup->layout()->setSpacing(6);
   theTransformationGroup->layout()->setMargin(11);
   hboxLayout = new Q3HBoxLayout(theTransformationGroup->layout());
   hboxLayout->setAlignment(Qt::AlignTop);
   hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));

   theMapRadioButton = new QRadioButton(theTransformationGroup);
   theMapRadioButton->setObjectName(QString::fromUtf8("theMapRadioButton"));
   theMapRadioButton->setChecked(true);
   hboxLayout->addWidget(theMapRadioButton);

   theIdentityRadioButton = new QRadioButton(theTransformationGroup);
   theIdentityRadioButton->setObjectName(QString::fromUtf8("theIdentityRadioButton"));
   hboxLayout->addWidget(theIdentityRadioButton);

   theStatePlaneRadioButton = new QRadioButton(theTransformationGroup);
   theStatePlaneRadioButton->setObjectName(QString::fromUtf8("theStatePlaneRadioButton"));
   hboxLayout->addWidget(theStatePlaneRadioButton);

   vboxLayout->addWidget(theTransformationGroup);

   gridLayout = new QGridLayout();
   gridLayout->setSpacing(6);
   gridLayout->setMargin(0);
   gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
   theHemisphereComboBox = new QComboBox(ossimQtViewDialog);
   theHemisphereComboBox->setObjectName(QString::fromUtf8("theHemisphereComboBox"));
   theHemisphereComboBox->setEditable(false);
   theHemisphereComboBox->setAutoCompletion(false);
   theHemisphereComboBox->setDuplicatesEnabled(false);

   gridLayout->addWidget(theHemisphereComboBox, 5, 1, 1, 1);

   theScaleFactorInput = new QLineEdit(ossimQtViewDialog);
   theScaleFactorInput->setObjectName(QString::fromUtf8("theScaleFactorInput"));
   theScaleFactorInput->setEnabled(true);

   gridLayout->addWidget(theScaleFactorInput, 7, 1, 1, 1);

   theGsdInput = new QLineEdit(ossimQtViewDialog);
   theGsdInput->setObjectName(QString::fromUtf8("theGsdInput"));

   gridLayout->addWidget(theGsdInput, 2, 1, 1, 1);

   theGsdLabel = new QLabel(ossimQtViewDialog);
   theGsdLabel->setObjectName(QString::fromUtf8("theGsdLabel"));
   QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(5));
   sizePolicy1.setHorizontalStretch(0);
   sizePolicy1.setVerticalStretch(0);
   sizePolicy1.setHeightForWidth(theGsdLabel->sizePolicy().hasHeightForWidth());
   theGsdLabel->setSizePolicy(sizePolicy1);

   gridLayout->addWidget(theGsdLabel, 2, 0, 1, 1);

   theDatumComboBox = new QComboBox(ossimQtViewDialog);
   theDatumComboBox->setObjectName(QString::fromUtf8("theDatumComboBox"));
   theDatumComboBox->setEditable(false);
   theDatumComboBox->setAutoCompletion(false);
   theDatumComboBox->setDuplicatesEnabled(false);

   gridLayout->addWidget(theDatumComboBox, 0, 1, 1, 1);

   theZoneLabel = new QLabel(ossimQtViewDialog);
   theZoneLabel->setObjectName(QString::fromUtf8("theZoneLabel"));

   gridLayout->addWidget(theZoneLabel, 4, 0, 1, 1);

   theScaleFactorLabel = new QLabel(ossimQtViewDialog);
   theScaleFactorLabel->setObjectName(QString::fromUtf8("theScaleFactorLabel"));

   gridLayout->addWidget(theScaleFactorLabel, 7, 0, 1, 1);

   theStandardParallelInput = new QLineEdit(ossimQtViewDialog);
   theStandardParallelInput->setObjectName(QString::fromUtf8("theStandardParallelInput"));
   theStandardParallelInput->setEnabled(true);

   gridLayout->addWidget(theStandardParallelInput, 6, 1, 1, 1);

   theDatumLabel = new QLabel(ossimQtViewDialog);
   theDatumLabel->setObjectName(QString::fromUtf8("theDatumLabel"));

   gridLayout->addWidget(theDatumLabel, 0, 0, 1, 1);

   theHemisphereLabel = new QLabel(ossimQtViewDialog);
   theHemisphereLabel->setObjectName(QString::fromUtf8("theHemisphereLabel"));

   gridLayout->addWidget(theHemisphereLabel, 5, 0, 1, 1);

   theZoneInput = new QLineEdit(ossimQtViewDialog);
   theZoneInput->setObjectName(QString::fromUtf8("theZoneInput"));

   gridLayout->addWidget(theZoneInput, 4, 1, 1, 1);

   theStandardParallelLabel = new QLabel(ossimQtViewDialog);
   theStandardParallelLabel->setObjectName(QString::fromUtf8("theStandardParallelLabel"));

   gridLayout->addWidget(theStandardParallelLabel, 6, 0, 1, 1);

   theMapProjectionLabel = new QLabel(ossimQtViewDialog);
   theMapProjectionLabel->setObjectName(QString::fromUtf8("theMapProjectionLabel"));
   QSizePolicy sizePolicy2(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(5));
   sizePolicy2.setHorizontalStretch(0);
   sizePolicy2.setVerticalStretch(0);
   sizePolicy2.setHeightForWidth(theMapProjectionLabel->sizePolicy().hasHeightForWidth());
   theMapProjectionLabel->setSizePolicy(sizePolicy2);

   gridLayout->addWidget(theMapProjectionLabel, 1, 0, 1, 1);

   theProjectionComboBox = new QComboBox(ossimQtViewDialog);
   theProjectionComboBox->setObjectName(QString::fromUtf8("theProjectionComboBox"));
   theProjectionComboBox->setEditable(false);
   theProjectionComboBox->setAutoCompletion(false);
   theProjectionComboBox->setDuplicatesEnabled(false);

   gridLayout->addWidget(theProjectionComboBox, 1, 1, 1, 1);

   theOriginLabel = new QLabel(ossimQtViewDialog);
   theOriginLabel->setObjectName(QString::fromUtf8("theOriginLabel"));

   gridLayout->addWidget(theOriginLabel, 3, 0, 1, 1);

   theOriginInput = new QLineEdit(ossimQtViewDialog);
   theOriginInput->setObjectName(QString::fromUtf8("theOriginInput"));

   gridLayout->addWidget(theOriginInput, 3, 1, 1, 1);


   vboxLayout->addLayout(gridLayout);

   hboxLayout1 = new Q3HBoxLayout();
   hboxLayout1->setSpacing(6);
   hboxLayout1->setMargin(0);
   hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
   theHelpButton = new QPushButton(ossimQtViewDialog);
   theHelpButton->setObjectName(QString::fromUtf8("theHelpButton"));
   theHelpButton->setEnabled(false);
   theHelpButton->setAutoDefault(true);

   hboxLayout1->addWidget(theHelpButton);

   spacerItem = new QSpacerItem(140, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

   hboxLayout1->addItem(spacerItem);

   thePropagateButton = new QPushButton(ossimQtViewDialog);
   thePropagateButton->setObjectName(QString::fromUtf8("thePropagateButton"));

   hboxLayout1->addWidget(thePropagateButton);

   theApplyButton = new QPushButton(ossimQtViewDialog);
   theApplyButton->setObjectName(QString::fromUtf8("heApplyButton"));
   theApplyButton->setAutoDefault(true);
   theApplyButton->setDefault(true);

   hboxLayout1->addWidget(theApplyButton);

   theCloseButton = new QPushButton(ossimQtViewDialog);
   theCloseButton->setObjectName(QString::fromUtf8("theCloseButton"));
   theCloseButton->setAutoDefault(true);

   hboxLayout1->addWidget(theCloseButton);


   vboxLayout->addLayout(hboxLayout1);

   retranslateUi(ossimQtViewDialog);

   QMetaObject::connectSlotsByName(ossimQtViewDialog);
} // setupUi

void Ui_ossimQtViewDialog::retranslateUi(QDialog *ossimQtViewDialog)
{
   ossimQtViewDialog->setWindowTitle(QApplication::translate("ossimQtViewDialog", "View Geometry", 0, QApplication::UnicodeUTF8));
   theTransformationGroup->setTitle(QApplication::translate("ossimQtViewDialog", "Transformation", 0, QApplication::UnicodeUTF8));
   theMapRadioButton->setText(QApplication::translate("ossimQtViewDialog", "Map", 0, QApplication::UnicodeUTF8));
   theIdentityRadioButton->setText(QApplication::translate("ossimQtViewDialog", "Identity", 0, QApplication::UnicodeUTF8));
   theStatePlaneRadioButton->setText(QApplication::translate("ossimQtViewDialog", "State Plane", 0, QApplication::UnicodeUTF8));
   theHemisphereComboBox->clear();
   theHemisphereComboBox->addItem(QApplication::translate("ossimQtViewDialog", "North", 0, QApplication::UnicodeUTF8));
   theHemisphereComboBox->addItem(QApplication::translate("ossimQtViewDialog", "South", 0, QApplication::UnicodeUTF8));
   theGsdLabel->setText(QApplication::translate("ossimQtViewDialog", "Meters per pixel", 0, QApplication::UnicodeUTF8));
   theZoneLabel->setText(QApplication::translate("ossimQtViewDialog", "Zone:", 0, QApplication::UnicodeUTF8));
   theScaleFactorLabel->setText(QApplication::translate("ossimQtViewDialog", "Scale factor:", 0, QApplication::UnicodeUTF8));
   theDatumLabel->setText(QApplication::translate("ossimQtViewDialog", "Datum:", 0, QApplication::UnicodeUTF8));
   theHemisphereLabel->setText(QApplication::translate("ossimQtViewDialog", "Hemisphere:", 0, QApplication::UnicodeUTF8));
   theStandardParallelLabel->setText(QApplication::translate("ossimQtViewDialog", "Parallels 1 & 2", 0, QApplication::UnicodeUTF8));
   theMapProjectionLabel->setText(QApplication::translate("ossimQtViewDialog", "Projection:", 0, QApplication::UnicodeUTF8));
   theOriginLabel->setText(QApplication::translate("ossimQtViewDialog", "Origin Lat lon", 0, QApplication::UnicodeUTF8));
   theHelpButton->setText(QApplication::translate("ossimQtViewDialog", "&Help", 0, QApplication::UnicodeUTF8));
   theHelpButton->setShortcut(QApplication::translate("ossimQtViewDialog", "F1", 0, QApplication::UnicodeUTF8));
   thePropagateButton->setText(QApplication::translate("ossimQtViewDialog", "Propagate", 0, QApplication::UnicodeUTF8));
   theApplyButton->setText(QApplication::translate("ossimQtViewDialog", "&Apply", 0, QApplication::UnicodeUTF8));
   theApplyButton->setShortcut(QApplication::translate("ossimQtViewDialog", "Alt+A", 0, QApplication::UnicodeUTF8));
   theCloseButton->setText(QApplication::translate("ossimQtViewDialog", "&Close", 0, QApplication::UnicodeUTF8));
   theCloseButton->setShortcut(QApplication::translate("ossimQtViewDialog", "Alt+C", 0, QApplication::UnicodeUTF8));
   Q_UNUSED(ossimQtViewDialog);
} // retranslateUi
