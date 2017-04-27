#include <ossimQt/ossimQtUnitConverterDialog.h>

#include <ossim/base/ossimDms.h>

/*
 *  Constructs a ossimQtUnitConverterDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
ossimQtUnitConverterDialog::ossimQtUnitConverterDialog(QWidget* parent, const char* name, bool modal, Qt::WFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);


    // signals and slots connections
    connect(theCloseButton, SIGNAL(clicked()), this, SLOT(theCloseButton_clicked()));
    connect(theDmsField, SIGNAL(returnPressed()), this, SLOT(theDmsField_returnPressed()));
    connect(theMetersField, SIGNAL(returnPressed()), this, SLOT(theMetersField_returnPressed()));
    connect(theFeetField, SIGNAL(returnPressed()), this, SLOT(theFeetField_returnPressed()));
    connect(theUsSurveyFeetField, SIGNAL(returnPressed()), this, SLOT(theUsSurveyFeetField_returnPressed()));
    connect(theNauticalMilesField, SIGNAL(returnPressed()), this, SLOT(theNauticalMilesField_returnPressed()));
    connect(theRadiansField, SIGNAL(returnPressed()), this, SLOT(theRadiansField_returnPressed()));
    connect(theDegreesField, SIGNAL(returnPressed()), this, SLOT(theDegreesField_returnPressed()));
    connect(theMinutesField, SIGNAL(returnPressed()), this, SLOT(theMinutesField_returnPressed()));
    connect(theSecondsField, SIGNAL(returnPressed()), this, SLOT(theSecondsField_returnPressed()));
    connect(theMilesField, SIGNAL(returnPressed()), this, SLOT(theMilesField_returnPressed()));
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ossimQtUnitConverterDialog::~ossimQtUnitConverterDialog()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ossimQtUnitConverterDialog::languageChange()
{
    retranslateUi(this);
}

void ossimQtUnitConverterDialog::init()
{
    updateFields();
}

void ossimQtUnitConverterDialog::destroy()
{
}

void ossimQtUnitConverterDialog::updateFields()
{
    theMetersField->blockSignals(true);
    theFeetField->blockSignals(true);
    theUsSurveyFeetField->blockSignals(true);
    theMilesField->blockSignals(true);
    theNauticalMilesField->blockSignals(true);
    theRadiansField->blockSignals(true);
    theDegreesField->blockSignals(true);
    theMinutesField->blockSignals(true);
    theSecondsField->blockSignals(true);
    theDmsField->blockSignals(true);
	    
    ossimDms  dms(theConversionTool.getDegrees(),false);
    theMetersField->setText(QString::number(theConversionTool.getMeters(),'g',12));
    theFeetField->setText(QString::number(theConversionTool.getFeet(),'g',12));
    theUsSurveyFeetField->setText(QString::number(theConversionTool.getUsSurveyFeet(),'g',12));
    theMilesField->setText(QString::number(theConversionTool.getMiles(),'g',12));
    theNauticalMilesField->setText(QString::number(theConversionTool.getNauticalMiles(),'g',12));
    theRadiansField->setText(QString::number(theConversionTool.getRadians(),'g',12));
    theDegreesField->setText(QString::number(theConversionTool.getDegrees(),'g',12));
    theMinutesField->setText(QString::number(theConversionTool.getMinutes(),'g',12));
    theSecondsField->setText(QString::number(theConversionTool.getSeconds(),'g',12));
    theDmsField->setText(dms.toString("ddd@mm'ss.ssss\"").c_str());
    
    theMetersField->blockSignals(false);
    theFeetField->blockSignals(false);
    theUsSurveyFeetField->blockSignals(false);
    theMilesField->blockSignals(false);
    theNauticalMilesField->blockSignals(false);
    theRadiansField->blockSignals(false);
    theDegreesField->blockSignals(false);
    theMinutesField->blockSignals(false);
    theSecondsField->blockSignals(false);
    theDmsField->blockSignals(false);
}

void ossimQtUnitConverterDialog::theCloseButton_clicked()
{
    close(true);
}

void ossimQtUnitConverterDialog::theDmsField_returnPressed()
{
    ossimDms dms(0.0,false);
    
    dms.setDegrees(theDmsField->text().ascii());
 
    theConversionTool.setDegrees(dms.getDegrees());
    
    updateFields();
}


void ossimQtUnitConverterDialog::theMetersField_returnPressed()
{
    theConversionTool.setMeters(theMetersField->text().toDouble());
    updateFields();

}


void ossimQtUnitConverterDialog::theFeetField_returnPressed()
{
    theConversionTool.setFeet(theFeetField->text().toDouble());
    updateFields();

}


void ossimQtUnitConverterDialog::theUsSurveyFeetField_returnPressed()
{
    theConversionTool.setUsSurveyFeet(theUsSurveyFeetField->text().toDouble());
    updateFields();

}

void ossimQtUnitConverterDialog::theNauticalMilesField_returnPressed()
{
    theConversionTool.setNauticalMiles(theNauticalMilesField->text().toDouble());
    updateFields();

}


void ossimQtUnitConverterDialog::theRadiansField_returnPressed()
{
    theConversionTool.setRadians(theRadiansField->text().toDouble());
    updateFields();

}


void ossimQtUnitConverterDialog::theDegreesField_returnPressed()
{
    theConversionTool.setDegrees(theDegreesField->text().toDouble());
    updateFields();

}


void ossimQtUnitConverterDialog::theMinutesField_returnPressed()
{
    theConversionTool.setMinutes(theMinutesField->text().toDouble());
    updateFields();

}


void ossimQtUnitConverterDialog::theSecondsField_returnPressed()
{
    theConversionTool.setSeconds(theSecondsField->text().toDouble());
    updateFields();

}

void ossimQtUnitConverterDialog::theMilesField_returnPressed()
{
    theConversionTool.setMiles(theMilesField->text().toDouble());
    updateFields();

}
