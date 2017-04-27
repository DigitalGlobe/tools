/********************************************************************************
** Form generated from reading UI file 'columnsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COLUMNSDIALOG_H
#define UI_COLUMNSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_ColumnsDialog
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;
    QPushButton *buttonApply;
    QSpacerItem *spacerItem;
    QTreeWidget *treeColumns;

    void setupUi(QDialog *ColumnsDialog)
    {
        if (ColumnsDialog->objectName().isEmpty())
            ColumnsDialog->setObjectName(QStringLiteral("ColumnsDialog"));
        ColumnsDialog->resize(294, 195);
        ColumnsDialog->setSizeGripEnabled(true);
        gridLayout = new QGridLayout(ColumnsDialog);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(9, 9, 9, 9);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        buttonOk = new QPushButton(ColumnsDialog);
        buttonOk->setObjectName(QStringLiteral("buttonOk"));

        hboxLayout->addWidget(buttonOk);

        buttonCancel = new QPushButton(ColumnsDialog);
        buttonCancel->setObjectName(QStringLiteral("buttonCancel"));

        hboxLayout->addWidget(buttonCancel);

        buttonApply = new QPushButton(ColumnsDialog);
        buttonApply->setObjectName(QStringLiteral("buttonApply"));

        hboxLayout->addWidget(buttonApply);


        gridLayout->addLayout(hboxLayout, 1, 1, 1, 1);

        spacerItem = new QSpacerItem(25, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacerItem, 1, 0, 1, 1);

        treeColumns = new QTreeWidget(ColumnsDialog);
        treeColumns->setObjectName(QStringLiteral("treeColumns"));
        treeColumns->setColumnCount(1);

        gridLayout->addWidget(treeColumns, 0, 0, 1, 2);

        QWidget::setTabOrder(treeColumns, buttonOk);
        QWidget::setTabOrder(buttonOk, buttonCancel);
        QWidget::setTabOrder(buttonCancel, buttonApply);

        retranslateUi(ColumnsDialog);

        buttonOk->setDefault(true);


        QMetaObject::connectSlotsByName(ColumnsDialog);
    } // setupUi

    void retranslateUi(QDialog *ColumnsDialog)
    {
        ColumnsDialog->setWindowTitle(QApplication::translate("ColumnsDialog", "Columns", Q_NULLPTR));
        buttonOk->setText(QApplication::translate("ColumnsDialog", "OK", Q_NULLPTR));
        buttonCancel->setText(QApplication::translate("ColumnsDialog", "Cancel", Q_NULLPTR));
        buttonApply->setText(QApplication::translate("ColumnsDialog", "&Apply", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem = treeColumns->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("ColumnsDialog", "Visible Columns for Views", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ColumnsDialog: public Ui_ColumnsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COLUMNSDIALOG_H
