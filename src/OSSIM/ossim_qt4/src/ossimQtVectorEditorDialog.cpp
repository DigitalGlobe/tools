#include <ossimQt/ossimQtVectorEditorDialog.h>

#include <ossimQt/ossimQtVectorEditorDialogController.h>

/*
 *  Constructs a ossimQtVectorEditorDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
ossimQtVectorEditorDialog::ossimQtVectorEditorDialog(QWidget* parent, const char* name, bool modal, Qt::WFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);


    // signals and slots connections
    connect(theApplyButton, SIGNAL(clicked()), this, SLOT(theApplyButton_clicked()));
    connect(theFeatureListbox, SIGNAL(selectionChanged(Q3ListBoxItem*)), this, SLOT(theFeatureListbox_selectionChanged(Q3ListBoxItem*)));
    connect(theCloseButton, SIGNAL(clicked()), this, SLOT(theCloseButton_clicked()));
    connect(thePolygonColorPushButton, SIGNAL(clicked()), this, SLOT(thePolygonColorPushButton_clicked()));
    connect(theLineColorPushButton, SIGNAL(clicked()), this, SLOT(theLineColorPushButton_clicked()));
    connect(thePointColorButton, SIGNAL(clicked()), this, SLOT(thePointColorButton_clicked()));
    connect(theLineEnabledCheckBox, SIGNAL(toggled(bool)), this, SLOT(theLineEnabledCheckBox_toggled(bool)));
    connect(thePointEnabledCheckBox, SIGNAL(toggled(bool)), this, SLOT(thePointEnabledCheckBox_toggled(bool)));
    connect(theFontEnabledCheckBox, SIGNAL(toggled(bool)), this, SLOT(theFontEnabledCheckBox_toggled(bool)));
    connect(thePolygonEnabledCheckBox, SIGNAL(clicked()), this, SLOT(thePolygonEnabledCheckBox_clicked()));
    connect(theLineThicknessInput, SIGNAL(valueChanged(int)), this, SLOT(theLineThicknessInput_valueChanged(int)));
    connect(thePolygonThicknessInput, SIGNAL(valueChanged(int)), this, SLOT(thePolygonThicknessInput_valueChanged(int)));
    connect(thePolygonFilledCheckBox, SIGNAL(toggled(bool)), this, SLOT(thePolygonFilledCheckBox_toggled(bool)));
    connect(thePointFilledCheckBox, SIGNAL(toggled(bool)), this, SLOT(thePointFilledCheckbox_toggled(bool)));
    connect(theFontPointSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(theFontPointSizeSpinBox_valueChanged(int)));
    connect(theFontColorPushButton, SIGNAL(clicked()), this, SLOT(theFontColorPushButton_clicked()));
    connect(theFontScaleInput, SIGNAL(textChanged(const QString&)), this, SLOT(theFontScaleInput_textChanged(const QString&)));
    connect(theDisableAllButton, SIGNAL(clicked()), this, SLOT(theDisableAllButton_clicked()));
    connect(theEnableAllButton, SIGNAL(clicked()), this, SLOT(theEnableAllButton_clicked()));
    connect(theFontFamilyNameComboBox, SIGNAL(activated(const QString&)), this, SLOT(theFontFamilyNameComboBox_activated(const QString&)));
    connect(theFontStyleNameComboBox, SIGNAL(activated(const QString&)), this, SLOT(theFontStyleNameComboBox_activated(const QString&)));
    connect(theFontShearInput, SIGNAL(textChanged(const QString&)), this, SLOT(theFontShearInput_textChanged(const QString&)));
    connect(theFontRotationInput, SIGNAL(textChanged(const QString&)), this, SLOT(theFontRotationInput_textChanged(const QString&)));
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ossimQtVectorEditorDialog::~ossimQtVectorEditorDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ossimQtVectorEditorDialog::languageChange()
{
    retranslateUi(this);
}

void ossimQtVectorEditorDialog::init()
{
    theController = new ossimQtVectorEditorDialogController(this);
}

void ossimQtVectorEditorDialog::theApplyButton_clicked()
{
    theController->apply();
}

void ossimQtVectorEditorDialog::theFeatureListbox_selectionChanged( Q3ListBoxItem * li)
{
    theController->changeToFeature(theFeatureListbox->index(li));
}

ossimQtVectorEditorDialogController* ossimQtVectorEditorDialog::controller()
{
    return theController;
}

void ossimQtVectorEditorDialog::theCloseButton_clicked()
{
    close(true);
}

void ossimQtVectorEditorDialog::thePolygonColorPushButton_clicked()
{
  theController->polygonColor();
}


void ossimQtVectorEditorDialog::theLineColorPushButton_clicked()
{
  theController->lineColor();
}

void ossimQtVectorEditorDialog::thePointColorButton_clicked()
{
  theController->pointColor();
}


void ossimQtVectorEditorDialog::theLineEnabledCheckBox_toggled( bool)
{
    theController->lineEnabledToggled();
}


void ossimQtVectorEditorDialog::thePointEnabledCheckBox_toggled( bool )
{
    theController->pointEnabledToggled();
}


void ossimQtVectorEditorDialog::theFontEnabledCheckBox_toggled( bool )
{
    theController->fontEnabledToggled();
}


void ossimQtVectorEditorDialog::thePolygonEnabledCheckBox_clicked()
{
    theController->polygonEnabledToggled();
}


void ossimQtVectorEditorDialog::theLineThicknessInput_valueChanged( int /*value*/)
{
    theController->lineThicknessChanged();
}


void ossimQtVectorEditorDialog::thePolygonThicknessInput_valueChanged( int )
{
    theController->polygonThicknessChanged();
}


void ossimQtVectorEditorDialog::thePolygonFilledCheckBox_toggled( bool )
{
    theController->polygonFilledToggled();
}


void ossimQtVectorEditorDialog::thePointFilledCheckbox_toggled( bool )
{
    theController->pointFilledToggled();
}


void ossimQtVectorEditorDialog::theFontPointSizeSpinBox_valueChanged( int )
{
   theController->fontPointSize();
}

void ossimQtVectorEditorDialog::theFontColorPushButton_clicked()
{
    theController->fontColor();
}


void ossimQtVectorEditorDialog::theFontScaleInput_textChanged( const QString & )
{
    theController->fontScale();
}

void ossimQtVectorEditorDialog::theDisableAllButton_clicked()
{
    theController->disableAll();
}


void ossimQtVectorEditorDialog::theEnableAllButton_clicked()
{
    theController->enableAll();
}


void ossimQtVectorEditorDialog::theFontFamilyNameComboBox_activated( const QString & )
{
    theController->fontFamilyName();
}


void ossimQtVectorEditorDialog::theFontStyleNameComboBox_activated( const QString & )
{
    theController->fontStyle();
}


void ossimQtVectorEditorDialog::theFontShearInput_textChanged( const QString & )
{
    theController->fontShear();
}


void ossimQtVectorEditorDialog::theFontRotationInput_textChanged( const QString & )
{
    theController->fontRotation();
}
