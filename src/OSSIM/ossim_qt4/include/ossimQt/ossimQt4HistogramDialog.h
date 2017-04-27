//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  Marla Bartel
//
// Description:
//
// Histogram dialog.
//
// $Id$
//----------------------------------------------------------------------------
#ifndef ossimQt4HistogramDialog_HEADER
#define ossimQt4HistogramDialog_HEADER

#include <QtGui/QDialog>
#include <QtGui/QComboBox>

// Forward class declaration.
class ossimQtHistogramController;
class ossimHistogramRemapper;
class QLineEdit;
class QWidget;
class QTabWidget;
class QDialogButtonBox;
class QCheckBox;
class QComboBox;
class ossimQtHistogramWidget;

class ossimQt4HistogramDialog : public QDialog
{
   Q_OBJECT
      
public:
   /** public constructor */
   ossimQt4HistogramDialog(QWidget *parent = 0);
   
   /** virtual destructor */
   virtual ~ossimQt4HistogramDialog();
   virtual void setHistogramRemapper( ossimHistogramRemapper* remapper );

public slots:
    virtual void closeButtonClicked();
    virtual void applyButtonClicked();
    virtual void resetButtonClicked();
    virtual void enableCheckBoxClicked();
    virtual void lowClipPercentReturnPressed();
    virtual void highClipPercentReturnPressed();
    virtual void lowClipValueReturnPressed();
    virtual void highClipValueReturnPressed();
    virtual void midPointReturnPressed();
    virtual void stretchModeComboBoxActivated( int item );
    virtual void bandComboBoxActivated( int item );
    virtual void histogramFileLineEditReturnPressed();
    virtual void histogramFilePushButtonClicked();

protected slots:
    // virtual void languageChange();

private:
    
    friend class ossimQtHistogramController;

    ossimQtHistogramController* theController;

    virtual void init();
    virtual void destroy();

    QLineEdit* theLowClipPercentLineEdit;
    QLineEdit* theLowClipValueLineEdit;
    QLineEdit* theHighClipPercentLineEdit;
    QLineEdit* theHighClipValueLineEdit;
    QLineEdit* theOutputMinValue;
    QLineEdit* theOutputMaxValue;
    QLineEdit* theMidPointLineEdit;
    QLineEdit* theHistogramFileLineEdit;
    ossimQtHistogramWidget*   theHistogramWidget;
    QComboBox* theBandComboBox;
    QComboBox* theStretchModeComboBox;
    QCheckBox* theEnableCheckBox;
    QPushButton* theApplyButton;
    QPushButton* theResetButton;
    QPushButton* theCloseButton;
};
    
#endif /* #ifndef ossimQt4HistogramDialog_HEADER */
