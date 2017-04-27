#ifndef OSSIMQTSTRINGLISTPROPERTYDIALOG_H
#define OSSIMQTSTRINGLISTPROPERTYDIALOG_H

#include <QtCore/QVariant>
//Added by qt3to4:
#include <Qt3Support/Q3HBoxLayout>
#include <Qt3Support/Q3VBoxLayout>

class ossimQtStringListPropertyController;

#include <Qt3Support/Q3ListBox>
#include <Qt3Support/Q3TextEdit>
#include <Qt3Support/Q3WidgetStack>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
// #include <QtGui/Q3ButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
// #include <QtGui/Q3HBoxLayout>
#include <QtGui/QPushButton>
// #include <QtGui/Q3VBoxLayout>
#include <QtGui/QWidget>
#include <Qt3Support/Q3MimeSourceFactory>

class Ui_ossimQtStringListPropertyDialog
{
public:
    Q3VBoxLayout *vboxLayout;
    Q3WidgetStack *theWidgetStack;
    QWidget *WStackPage;
    Q3VBoxLayout *vboxLayout1;
    Q3TextEdit *theTextEdit;
    QWidget *WStackPage1;
    Q3VBoxLayout *vboxLayout2;
    Q3HBoxLayout *hboxLayout;
    Q3ListBox *theConstraintListbox;
    Q3VBoxLayout *vboxLayout3;
    QPushButton *theAddButton;
    QPushButton *theRemoveButton;
    Q3VBoxLayout *vboxLayout4;
    Q3ListBox *theValueListbox;
    Q3HBoxLayout *hboxLayout1;
    QPushButton *theUpButton;
    QPushButton *theDownButton;
    Q3HBoxLayout *hboxLayout2;
    QCheckBox *theAutoApplyCheckBox;
    QPushButton *theApplyButton;
    QPushButton *theCloseButton;

    void setupUi(QDialog *ossimQtStringListPropertyDialog)
    {
    ossimQtStringListPropertyDialog->setObjectName(QString::fromUtf8("ossimQtStringListPropertyDialog"));
    ossimQtStringListPropertyDialog->resize(QSize(437, 309).expandedTo(ossimQtStringListPropertyDialog->minimumSizeHint()));
    vboxLayout = new Q3VBoxLayout(ossimQtStringListPropertyDialog);
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(11);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    theWidgetStack = new Q3WidgetStack(ossimQtStringListPropertyDialog);
    theWidgetStack->setObjectName(QString::fromUtf8("theWidgetStack"));
    WStackPage = new QWidget(theWidgetStack);
    WStackPage->setObjectName(QString::fromUtf8("WStackPage"));
    vboxLayout1 = new Q3VBoxLayout(WStackPage);
    vboxLayout1->setSpacing(6);
    vboxLayout1->setMargin(0);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    theTextEdit = new Q3TextEdit(WStackPage);
    theTextEdit->setObjectName(QString::fromUtf8("theTextEdit"));

    vboxLayout1->addWidget(theTextEdit);

    theWidgetStack->addWidget(WStackPage, 0);
    WStackPage1 = new QWidget(theWidgetStack);
    WStackPage1->setObjectName(QString::fromUtf8("WStackPage1"));
    vboxLayout2 = new Q3VBoxLayout(WStackPage1);
    vboxLayout2->setSpacing(6);
    vboxLayout2->setMargin(0);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    hboxLayout = new Q3HBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    theConstraintListbox = new Q3ListBox(WStackPage1);
    theConstraintListbox->setObjectName(QString::fromUtf8("theConstraintListbox"));

    hboxLayout->addWidget(theConstraintListbox);

    vboxLayout3 = new Q3VBoxLayout();
    vboxLayout3->setSpacing(6);
    vboxLayout3->setMargin(0);
    vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
    theAddButton = new QPushButton(WStackPage1);
    theAddButton->setObjectName(QString::fromUtf8("theAddButton"));

    vboxLayout3->addWidget(theAddButton);

    theRemoveButton = new QPushButton(WStackPage1);
    theRemoveButton->setObjectName(QString::fromUtf8("theRemoveButton"));

    vboxLayout3->addWidget(theRemoveButton);


    hboxLayout->addLayout(vboxLayout3);

    vboxLayout4 = new Q3VBoxLayout();
    vboxLayout4->setSpacing(6);
    vboxLayout4->setMargin(0);
    vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
    theValueListbox = new Q3ListBox(WStackPage1);
    theValueListbox->setObjectName(QString::fromUtf8("theValueListbox"));

    vboxLayout4->addWidget(theValueListbox);

    hboxLayout1 = new Q3HBoxLayout();
    hboxLayout1->setSpacing(6);
    hboxLayout1->setMargin(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    theUpButton = new QPushButton(WStackPage1);
    theUpButton->setObjectName(QString::fromUtf8("theUpButton"));

    hboxLayout1->addWidget(theUpButton);

    theDownButton = new QPushButton(WStackPage1);
    theDownButton->setObjectName(QString::fromUtf8("theDownButton"));

    hboxLayout1->addWidget(theDownButton);


    vboxLayout4->addLayout(hboxLayout1);


    hboxLayout->addLayout(vboxLayout4);


    vboxLayout2->addLayout(hboxLayout);

    theWidgetStack->addWidget(WStackPage1, 1);

    vboxLayout->addWidget(theWidgetStack);

    hboxLayout2 = new Q3HBoxLayout();
    hboxLayout2->setSpacing(6);
    hboxLayout2->setMargin(0);
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    theAutoApplyCheckBox = new QCheckBox(ossimQtStringListPropertyDialog);
    theAutoApplyCheckBox->setObjectName(QString::fromUtf8("theAutoApplyCheckBox"));
    theAutoApplyCheckBox->setChecked(true);

    hboxLayout2->addWidget(theAutoApplyCheckBox);

    theApplyButton = new QPushButton(ossimQtStringListPropertyDialog);
    theApplyButton->setObjectName(QString::fromUtf8("theApplyButton"));

    hboxLayout2->addWidget(theApplyButton);

    theCloseButton = new QPushButton(ossimQtStringListPropertyDialog);
    theCloseButton->setObjectName(QString::fromUtf8("theCloseButton"));

    hboxLayout2->addWidget(theCloseButton);


    vboxLayout->addLayout(hboxLayout2);

    retranslateUi(ossimQtStringListPropertyDialog);

    QMetaObject::connectSlotsByName(ossimQtStringListPropertyDialog);
    } // setupUi

    void retranslateUi(QDialog *ossimQtStringListPropertyDialog)
    {
    ossimQtStringListPropertyDialog->setWindowTitle(QApplication::translate("ossimQtStringListPropertyDialog", "String List Property", 0, QApplication::UnicodeUTF8));
    theAddButton->setText(QApplication::translate("ossimQtStringListPropertyDialog", "Add", 0, QApplication::UnicodeUTF8));
    theRemoveButton->setText(QApplication::translate("ossimQtStringListPropertyDialog", "Remove", 0, QApplication::UnicodeUTF8));
    theUpButton->setText(QApplication::translate("ossimQtStringListPropertyDialog", "Up", 0, QApplication::UnicodeUTF8));
    theDownButton->setText(QApplication::translate("ossimQtStringListPropertyDialog", "Down", 0, QApplication::UnicodeUTF8));
    theAutoApplyCheckBox->setText(QApplication::translate("ossimQtStringListPropertyDialog", "Auto apply", 0, QApplication::UnicodeUTF8));
    theApplyButton->setText(QApplication::translate("ossimQtStringListPropertyDialog", "Apply", 0, QApplication::UnicodeUTF8));
    theCloseButton->setText(QApplication::translate("ossimQtStringListPropertyDialog", "Close", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ossimQtStringListPropertyDialog);
    } // retranslateUi

};

namespace Ui {
    class ossimQtStringListPropertyDialog: public Ui_ossimQtStringListPropertyDialog {};
} // namespace Ui

class ossimQtStringListPropertyDialog : public QDialog, public Ui::ossimQtStringListPropertyDialog
{
    Q_OBJECT

public:
    ossimQtStringListPropertyDialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0);
    ~ossimQtStringListPropertyDialog();

    virtual ossimQtStringListPropertyController * controller();

public slots:
    virtual void theCloseButton_clicked();
    virtual void theApplyButton_clicked();

protected:
    ossimQtStringListPropertyController *theController;

protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

};

#endif // OSSIMQTSTRINGLISTPROPERTYDIALOG_H
