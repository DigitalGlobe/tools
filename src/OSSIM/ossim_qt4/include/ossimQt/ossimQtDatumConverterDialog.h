#ifndef OSSIMQTDATUMCONVERTERDIALOG_H
#define OSSIMQTDATUMCONVERTERDIALOG_H

#include <QtGui/QDialog>

class QButtonGroup;
class QCheckBox;
class QComboBox;
class QFrame;
class QGridLayout;
class QGroupBox;
class QHBoxLayout;
class QTextEdit;
class QVBoxLayout;
class QLabel;
class QLineEdit;
class QPushButton;
class QSpacerItem;


class ossimQtDatumConverterDialog : public QDialog
{
    Q_OBJECT

public:
    ossimQtDatumConverterDialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~ossimQtDatumConverterDialog();

    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QGridLayout *gridLayout;
    QGroupBox *groupBox2;
    QVBoxLayout *vboxLayout1;
    QTextEdit *textEdit1;
    QLabel *textLabel1_2_2_2_2_2_2_2;
    QLineEdit *lineEdit1_2_2_2_2_2_2;
    QLabel *textLabel1_2_2_2_2_2_2_2_2;
    QLabel *textLabel1_2_2_2_2;
    QLineEdit *lineEdit1_2_2_2_2_2_2_2;
    QLineEdit *lineEdit1_2_2;
    QLineEdit *lineEdit1_2_2_2_2_2;
    QComboBox *theDatumLeftCombo;
    QLabel *textLabel1;
    QLineEdit *lineEdit1_2_2_2;
    QLineEdit *lineEdit1;
    QCheckBox *checkBox4;
    QLabel *textLabel1_2_2_2_2_2_2;
    QLabel *textLabel1_2;
    QLabel *textLabel1_2_2_2_2_2;
    QLabel *textLabel1_2_2;
    QLineEdit *lineEdit1_2;
    QLineEdit *lineEdit1_2_2_2_2;
    QLabel *textLabel1_2_2_2;
    QFrame *line2;
    QGridLayout *gridLayout1;
    QLabel *textLabel1_2_3;
    QLineEdit *lineEdit1_2_2_2_2_2_2_3;
    QLabel *textLabel1_2_2_2_2_2_2_3;
    QComboBox *theDatumLeftCombo_2;
    QLabel *textLabel1_2_2_2_2_2_3;
    QLineEdit *lineEdit1_2_2_2_3;
    QLineEdit *lineEdit1_2_2_2_2_3;
    QLabel *textLabel1_2_2_3;
    QLineEdit *lineEdit1_2_2_2_2_2_3;
    QLineEdit *lineEdit1_2_3;
    QLineEdit *lineEdit1_2_2_2_2_2_2_2_2;
    QLabel *textLabel1_2_2_2_2_3;
    QLineEdit *lineEdit1_2_2_3;
    QLabel *textLabel1_2_2_2_2_2_2_2_2_2;
    QLabel *textLabel1_2_2_2_2_2_2_2_3;
    QLabel *textLabel1_3;
    QLabel *textLabel1_2_2_2_3;
    QGroupBox *groupBox2_2;
    QVBoxLayout *vboxLayout2;
    QTextEdit *textEdit1_2;
    QCheckBox *checkBox4_2;
    QLineEdit *lineEdit1_3;
    QFrame *line2_2;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacerItem;
    QPushButton *pushButton12;
    QSpacerItem *spacerItem1;

    void setupUi(QDialog *ossimQtDatumConverterDialog);
    void retranslateUi(QDialog *ossimQtDatumConverterDialog);

    

public slots:
    virtual void pushButton12_clicked();

protected slots:
    virtual void languageChange();

};

#endif // OSSIMQTDATUMCONVERTERDIALOG_H
