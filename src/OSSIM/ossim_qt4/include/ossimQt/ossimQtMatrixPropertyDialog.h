#ifndef OSSIMQTMATRIXPROPERTYDIALOG_H
#define OSSIMQTMATRIXPROPERTYDIALOG_H

#include <QtCore/QVariant>
//Added by qt3to4:
#include <Qt3Support/Q3HBoxLayout>
#include <Qt3Support/Q3VBoxLayout>

class ossimQtMatrixPropertyController;

#include <Qt3Support/Q3Header>
#include <Qt3Support/Q3Table>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
// #include <QtGui/Q3ButtonGroup>
#include <QtGui/QDialog>
// #include <QtGui/Q3HBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
// #include <QtGui/Q3VBoxLayout>
#include <Qt3Support/Q3MimeSourceFactory>

class Ui_ossimQtMatrixPropertyDialog
{
public:
    Q3VBoxLayout *vboxLayout;
    Q3HBoxLayout *hboxLayout;
    Q3Table *theMatrix;
    Q3VBoxLayout *vboxLayout1;
    QPushButton *theNormalizeButton;
    QPushButton *theZeroButton;
    Q3HBoxLayout *hboxLayout1;
    QSpacerItem *spacerItem;
    QPushButton *theApplyButton;
    QPushButton *theCloseButton;
    QSpacerItem *spacerItem1;

    void setupUi(QDialog *ossimQtMatrixPropertyDialog)
    {
    ossimQtMatrixPropertyDialog->setObjectName(QString::fromUtf8("ossimQtMatrixPropertyDialog"));
    ossimQtMatrixPropertyDialog->resize(QSize(399, 348).expandedTo(ossimQtMatrixPropertyDialog->minimumSizeHint()));
    vboxLayout = new Q3VBoxLayout(ossimQtMatrixPropertyDialog);
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(11);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    hboxLayout = new Q3HBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    theMatrix = new Q3Table(ossimQtMatrixPropertyDialog);
    theMatrix->setObjectName(QString::fromUtf8("theMatrix"));
    theMatrix->setNumRows(0);
    theMatrix->setNumCols(0);

    hboxLayout->addWidget(theMatrix);

    vboxLayout1 = new Q3VBoxLayout();
    vboxLayout1->setSpacing(6);
    vboxLayout1->setMargin(0);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    theNormalizeButton = new QPushButton(ossimQtMatrixPropertyDialog);
    theNormalizeButton->setObjectName(QString::fromUtf8("theNormalizeButton"));

    vboxLayout1->addWidget(theNormalizeButton);

    theZeroButton = new QPushButton(ossimQtMatrixPropertyDialog);
    theZeroButton->setObjectName(QString::fromUtf8("theZeroButton"));

    vboxLayout1->addWidget(theZeroButton);


    hboxLayout->addLayout(vboxLayout1);


    vboxLayout->addLayout(hboxLayout);

    hboxLayout1 = new Q3HBoxLayout();
    hboxLayout1->setSpacing(6);
    hboxLayout1->setMargin(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    spacerItem = new QSpacerItem(98, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem);

    theApplyButton = new QPushButton(ossimQtMatrixPropertyDialog);
    theApplyButton->setObjectName(QString::fromUtf8("theApplyButton"));

    hboxLayout1->addWidget(theApplyButton);

    theCloseButton = new QPushButton(ossimQtMatrixPropertyDialog);
    theCloseButton->setObjectName(QString::fromUtf8("theCloseButton"));

    hboxLayout1->addWidget(theCloseButton);

    spacerItem1 = new QSpacerItem(97, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem1);


    vboxLayout->addLayout(hboxLayout1);

    retranslateUi(ossimQtMatrixPropertyDialog);

    QMetaObject::connectSlotsByName(ossimQtMatrixPropertyDialog);
    } // setupUi

    void retranslateUi(QDialog *ossimQtMatrixPropertyDialog)
    {
    ossimQtMatrixPropertyDialog->setWindowTitle(QApplication::translate("ossimQtMatrixPropertyDialog", "Matrix property", 0, QApplication::UnicodeUTF8));
    theNormalizeButton->setText(QApplication::translate("ossimQtMatrixPropertyDialog", "Normalize", 0, QApplication::UnicodeUTF8));
    theZeroButton->setText(QApplication::translate("ossimQtMatrixPropertyDialog", "Zero", 0, QApplication::UnicodeUTF8));
    theApplyButton->setText(QApplication::translate("ossimQtMatrixPropertyDialog", "Apply", 0, QApplication::UnicodeUTF8));
    theCloseButton->setText(QApplication::translate("ossimQtMatrixPropertyDialog", "Close", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ossimQtMatrixPropertyDialog);
    } // retranslateUi

};

namespace Ui {
    class ossimQtMatrixPropertyDialog: public Ui_ossimQtMatrixPropertyDialog {};
} // namespace Ui

class ossimQtMatrixPropertyDialog : public QDialog, public Ui::ossimQtMatrixPropertyDialog
{
    Q_OBJECT

public:
    ossimQtMatrixPropertyDialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0);
    ~ossimQtMatrixPropertyDialog();

    virtual ossimQtMatrixPropertyController * controller();

public slots:
    virtual void theNormalizeButton_clicked();
    virtual void theZeroButton_clicked();
    virtual void theApplyButton_clicked();
    virtual void theCloseButton_clicked();

protected:
    ossimQtMatrixPropertyController *theController;

protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

};

#endif // OSSIMQTMATRIXPROPERTYDIALOG_H
