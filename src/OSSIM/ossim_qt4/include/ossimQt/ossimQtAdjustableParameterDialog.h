#ifndef OSSIMQTADJUSTABLEPARAMETERDIALOG_H
#define OSSIMQTADJUSTABLEPARAMETERDIALOG_H

#include <QtCore/QVariant>
//Added by qt3to4:
#include <QtGui/QPixmap>
#include <Qt3Support/Q3GridLayout>
#include <Qt3Support/Q3HBoxLayout>
#include <Qt3Support/Q3VBoxLayout>
#include <QtGui/QLabel>
#include <Qt3Support/Q3Frame>

class ossimQtAdjustableParameterDialogController;

#include <Qt3Support/Q3Header>
#include <Qt3Support/Q3Table>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
// #include <QtGui/Q3ButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
// #include <QtGui/Q3Frame>
// #include <QtGui/Q3GridLayout>
// #include <QtGui/Q3HBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
// #include <QtGui/Q3VBoxLayout>
#include <Qt3Support/Q3MimeSourceFactory>

class Ui_ossimQtAdjustableParameterDialog
{
public:
   Q3VBoxLayout *vboxLayout;
   Q3GridLayout *gridLayout;
   QComboBox *theAdjustmentSelectionBox;
   QLabel *theAdjustmentDescriptionLabel;
   QLabel *theAdjustmentSelectionLabel;
   QLineEdit *theAdjustmentDescriptionInput;
   Q3Table *theAdjustableParameterTable;
   Q3Frame *line1;
   Q3HBoxLayout *hboxLayout;
   QCheckBox *theAutoApplyCheckBox;
   QPushButton *theApplyButton;
   QPushButton *theKeepAdjustmentButton;
   QPushButton *theCopyAdjustmentButton;
   QPushButton *theDeleteAdjustmentButto;
   QPushButton *theResetButton;
   QPushButton *theSaveButton;
   QPushButton *theCloseButton;
   
   void setupUi(QDialog *ossimQtAdjustableParameterDialog)
   {
      ossimQtAdjustableParameterDialog->setObjectName(QString::fromUtf8("ossimQtAdjustableParameterDialog"));
      ossimQtAdjustableParameterDialog->resize(QSize(716, 391).expandedTo(ossimQtAdjustableParameterDialog->minimumSizeHint()));
      vboxLayout = new Q3VBoxLayout(ossimQtAdjustableParameterDialog);
      vboxLayout->setSpacing(6);
      vboxLayout->setMargin(11);
      vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
      gridLayout = new Q3GridLayout(vboxLayout);
      // gridLayout = new Q3GridLayout();      
      gridLayout->setSpacing(6);
      gridLayout->setMargin(0);
      gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
      theAdjustmentSelectionBox = new QComboBox(ossimQtAdjustableParameterDialog);
      theAdjustmentSelectionBox->setObjectName(QString::fromUtf8("theAdjustmentSelectionBox"));
      
      gridLayout->addWidget(theAdjustmentSelectionBox, 0, 1, 1, 1);
      
      theAdjustmentDescriptionLabel = new QLabel(ossimQtAdjustableParameterDialog);
      theAdjustmentDescriptionLabel->setObjectName(QString::fromUtf8("theAdjustmentDescriptionLabel"));
      
      gridLayout->addWidget(theAdjustmentDescriptionLabel, 1, 0, 1, 1);
      
      theAdjustmentSelectionLabel = new QLabel(ossimQtAdjustableParameterDialog);
      theAdjustmentSelectionLabel->setObjectName(QString::fromUtf8("theAdjustmentSelectionLabel"));
      
      gridLayout->addWidget(theAdjustmentSelectionLabel, 0, 0, 1, 1);
      
      theAdjustmentDescriptionInput = new QLineEdit(ossimQtAdjustableParameterDialog);
      theAdjustmentDescriptionInput->setObjectName(QString::fromUtf8("theAdjustmentDescriptionInput"));
      
      gridLayout->addWidget(theAdjustmentDescriptionInput, 1, 1, 1, 1);
      
      
      vboxLayout->addLayout(gridLayout);
      
      theAdjustableParameterTable = new Q3Table(ossimQtAdjustableParameterDialog);
      theAdjustableParameterTable->setObjectName(QString::fromUtf8("theAdjustableParameterTable"));
      theAdjustableParameterTable->setNumRows(0);
      theAdjustableParameterTable->setNumCols(5);
      
      vboxLayout->addWidget(theAdjustableParameterTable);
      
      line1 = new Q3Frame(ossimQtAdjustableParameterDialog);
      line1->setObjectName(QString::fromUtf8("line1"));
      line1->setFrameShape(Q3Frame::HLine);
      line1->setFrameShadow(Q3Frame::Sunken);
      
      vboxLayout->addWidget(line1);
      
      hboxLayout = new Q3HBoxLayout();
      hboxLayout->setSpacing(6);
      hboxLayout->setMargin(0);
      hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
      theAutoApplyCheckBox = new QCheckBox(ossimQtAdjustableParameterDialog);
      theAutoApplyCheckBox->setObjectName(QString::fromUtf8("theAutoApplyCheckBox"));
      theAutoApplyCheckBox->setChecked(true);
      
      hboxLayout->addWidget(theAutoApplyCheckBox);
      
      theApplyButton = new QPushButton(ossimQtAdjustableParameterDialog);
      theApplyButton->setObjectName(QString::fromUtf8("theApplyButton"));
      
      hboxLayout->addWidget(theApplyButton);
      
      theKeepAdjustmentButton = new QPushButton(ossimQtAdjustableParameterDialog);
      theKeepAdjustmentButton->setObjectName(QString::fromUtf8("theKeepAdjustmentButton"));
      
      hboxLayout->addWidget(theKeepAdjustmentButton);
      
      theCopyAdjustmentButton = new QPushButton(ossimQtAdjustableParameterDialog);
      theCopyAdjustmentButton->setObjectName(QString::fromUtf8("theCopyAdjustmentButton"));
      
      hboxLayout->addWidget(theCopyAdjustmentButton);
      
      theDeleteAdjustmentButto = new QPushButton(ossimQtAdjustableParameterDialog);
      theDeleteAdjustmentButto->setObjectName(QString::fromUtf8("theDeleteAdjustmentButto"));
      
      hboxLayout->addWidget(theDeleteAdjustmentButto);
      
      theResetButton = new QPushButton(ossimQtAdjustableParameterDialog);
      theResetButton->setObjectName(QString::fromUtf8("theResetButton"));
      
      hboxLayout->addWidget(theResetButton);
      
      theSaveButton = new QPushButton(ossimQtAdjustableParameterDialog);
      theSaveButton->setObjectName(QString::fromUtf8("theSaveButton"));
      
      hboxLayout->addWidget(theSaveButton);
      
      theCloseButton = new QPushButton(ossimQtAdjustableParameterDialog);
      theCloseButton->setObjectName(QString::fromUtf8("theCloseButton"));
      
      hboxLayout->addWidget(theCloseButton);
      
      
      vboxLayout->addLayout(hboxLayout);
      
      retranslateUi(ossimQtAdjustableParameterDialog);
      
      QMetaObject::connectSlotsByName(ossimQtAdjustableParameterDialog);
   } // setupUi
   
   void retranslateUi(QDialog *ossimQtAdjustableParameterDialog)
   {
      ossimQtAdjustableParameterDialog->setWindowTitle(QApplication::translate("ossimQtAdjustableParameterDialog", "Parameter Adjustments", 0, QApplication::UnicodeUTF8));
      theAdjustmentDescriptionLabel->setText(QApplication::translate("ossimQtAdjustableParameterDialog", "Adjustment description:", 0, QApplication::UnicodeUTF8));
      theAdjustmentSelectionLabel->setText(QApplication::translate("ossimQtAdjustableParameterDialog", "Adjustment Selection:", 0, QApplication::UnicodeUTF8));
      theAdjustableParameterTable->horizontalHeader()->setLabel(0, QPixmap(), QApplication::translate("ossimQtAdjustableParameterDialog", "Name", 0, QApplication::UnicodeUTF8));
      theAdjustableParameterTable->horizontalHeader()->setLabel(1, QPixmap(), QApplication::translate("ossimQtAdjustableParameterDialog", "Sigma", 0, QApplication::UnicodeUTF8));
      theAdjustableParameterTable->horizontalHeader()->setLabel(2, QPixmap(), QApplication::translate("ossimQtAdjustableParameterDialog", "Param", 0, QApplication::UnicodeUTF8));
      theAdjustableParameterTable->horizontalHeader()->setLabel(3, QPixmap(), QApplication::translate("ossimQtAdjustableParameterDialog", "Param adjust", 0, QApplication::UnicodeUTF8));
      theAdjustableParameterTable->horizontalHeader()->setLabel(4, QPixmap(), QApplication::translate("ossimQtAdjustableParameterDialog", "Value", 0, QApplication::UnicodeUTF8));
      theAutoApplyCheckBox->setText(QApplication::translate("ossimQtAdjustableParameterDialog", "Auto apply", 0, QApplication::UnicodeUTF8));
      theApplyButton->setText(QApplication::translate("ossimQtAdjustableParameterDialog", "Apply", 0, QApplication::UnicodeUTF8));
      theKeepAdjustmentButton->setText(QApplication::translate("ossimQtAdjustableParameterDialog", "Keep", 0, QApplication::UnicodeUTF8));
      theCopyAdjustmentButton->setText(QApplication::translate("ossimQtAdjustableParameterDialog", "Copy", 0, QApplication::UnicodeUTF8));
      theDeleteAdjustmentButto->setText(QApplication::translate("ossimQtAdjustableParameterDialog", "Delete", 0, QApplication::UnicodeUTF8));
      theResetButton->setText(QApplication::translate("ossimQtAdjustableParameterDialog", "Reset", 0, QApplication::UnicodeUTF8));
      theSaveButton->setText(QApplication::translate("ossimQtAdjustableParameterDialog", "Save...", 0, QApplication::UnicodeUTF8));
      theCloseButton->setText(QApplication::translate("ossimQtAdjustableParameterDialog", "Close", 0, QApplication::UnicodeUTF8));
      Q_UNUSED(ossimQtAdjustableParameterDialog);
   } // retranslateUi
   
};

namespace Ui {
   class ossimQtAdjustableParameterDialog: public Ui_ossimQtAdjustableParameterDialog {};
} // namespace Ui

class ossimQtAdjustableParameterDialog : public QDialog, public Ui::ossimQtAdjustableParameterDialog
{
    Q_OBJECT

public:
    ossimQtAdjustableParameterDialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0);
    ~ossimQtAdjustableParameterDialog();

    virtual ossimQtAdjustableParameterDialogController * controller();

public slots:
    virtual void theCloseButton_clicked();
    virtual void theKeepAdjustmentButton_clicked();
    virtual void theAdjustmentSelectionBox_activated( const QString & );
    virtual void theDeleteAdjustmentButto_clicked();
    virtual void theCopyAdjustmentButton_clicked();
    virtual void theAdjustmentDescriptionInput_textChanged( const QString & );
    virtual void theSaveButton_clicked();

protected:
    ossimQtAdjustableParameterDialogController* theController;

protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

};

#endif // OSSIMQTADJUSTABLEPARAMETERDIALOG_H
