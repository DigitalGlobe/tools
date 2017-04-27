/********************************************************************************
** Form generated from reading UI file 'settingsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSDIALOG_H
#define UI_SETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsDialog
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;
    QPushButton *buttonApply;
    QTabWidget *tabWidget;
    QWidget *tabView;
    QVBoxLayout *vboxLayout;
    QCheckBox *checkBoxRowColors;
    QSpacerItem *spacerItem1;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName(QStringLiteral("SettingsDialog"));
        SettingsDialog->resize(286, 164);
        gridLayout = new QGridLayout(SettingsDialog);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(9, 9, 9, 9);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        spacerItem = new QSpacerItem(17, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        buttonOk = new QPushButton(SettingsDialog);
        buttonOk->setObjectName(QStringLiteral("buttonOk"));

        hboxLayout->addWidget(buttonOk);

        buttonCancel = new QPushButton(SettingsDialog);
        buttonCancel->setObjectName(QStringLiteral("buttonCancel"));

        hboxLayout->addWidget(buttonCancel);

        buttonApply = new QPushButton(SettingsDialog);
        buttonApply->setObjectName(QStringLiteral("buttonApply"));

        hboxLayout->addWidget(buttonApply);


        gridLayout->addLayout(hboxLayout, 1, 0, 1, 1);

        tabWidget = new QTabWidget(SettingsDialog);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabView = new QWidget();
        tabView->setObjectName(QStringLiteral("tabView"));
        vboxLayout = new QVBoxLayout(tabView);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(9, 9, 9, 9);
        vboxLayout->setObjectName(QStringLiteral("vboxLayout"));
        checkBoxRowColors = new QCheckBox(tabView);
        checkBoxRowColors->setObjectName(QStringLiteral("checkBoxRowColors"));

        vboxLayout->addWidget(checkBoxRowColors);

        spacerItem1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacerItem1);

        tabWidget->addTab(tabView, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

        QWidget::setTabOrder(tabWidget, checkBoxRowColors);
        QWidget::setTabOrder(checkBoxRowColors, buttonOk);
        QWidget::setTabOrder(buttonOk, buttonCancel);
        QWidget::setTabOrder(buttonCancel, buttonApply);

        retranslateUi(SettingsDialog);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QApplication::translate("SettingsDialog", "Settings", Q_NULLPTR));
        buttonOk->setText(QApplication::translate("SettingsDialog", "OK", Q_NULLPTR));
        buttonCancel->setText(QApplication::translate("SettingsDialog", "Cancel", Q_NULLPTR));
        buttonApply->setText(QApplication::translate("SettingsDialog", "&Apply", Q_NULLPTR));
        checkBoxRowColors->setText(QApplication::translate("SettingsDialog", "A&lternating row colors", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabView), QApplication::translate("SettingsDialog", "&View", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H
