/********************************************************************************
** Form generated from reading UI file 'stoppingdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STOPPINGDIALOG_H
#define UI_STOPPINGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_StoppingDialog
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *buttonCancel;
    QSpacerItem *spacerItem1;
    QLabel *labelPic;
    QProgressBar *progressBar;
    QLabel *labelText;

    void setupUi(QDialog *StoppingDialog)
    {
        if (StoppingDialog->objectName().isEmpty())
            StoppingDialog->setObjectName(QStringLiteral("StoppingDialog"));
        StoppingDialog->resize(200, 73);
        StoppingDialog->setModal(true);
        gridLayout = new QGridLayout(StoppingDialog);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(9, 9, 9, 9);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        spacerItem = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        buttonCancel = new QPushButton(StoppingDialog);
        buttonCancel->setObjectName(QStringLiteral("buttonCancel"));

        hboxLayout->addWidget(buttonCancel);

        spacerItem1 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem1);


        gridLayout->addLayout(hboxLayout, 1, 0, 1, 3);

        labelPic = new QLabel(StoppingDialog);
        labelPic->setObjectName(QStringLiteral("labelPic"));
        labelPic->setPixmap(QPixmap(QString::fromUtf8(":/icons/info_blue_24x24.png")));

        gridLayout->addWidget(labelPic, 0, 0, 1, 1);

        progressBar = new QProgressBar(StoppingDialog);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setMaximumSize(QSize(16777215, 16));
        progressBar->setMinimum(1);
        progressBar->setMaximum(10);
        progressBar->setValue(1);
        progressBar->setTextVisible(false);
        progressBar->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(progressBar, 0, 2, 1, 1);

        labelText = new QLabel(StoppingDialog);
        labelText->setObjectName(QStringLiteral("labelText"));

        gridLayout->addWidget(labelText, 0, 1, 1, 1);


        retranslateUi(StoppingDialog);

        buttonCancel->setDefault(true);


        QMetaObject::connectSlotsByName(StoppingDialog);
    } // setupUi

    void retranslateUi(QDialog *StoppingDialog)
    {
        StoppingDialog->setWindowTitle(QApplication::translate("StoppingDialog", "QxRunner", Q_NULLPTR));
        buttonCancel->setText(QApplication::translate("StoppingDialog", "Cancel", Q_NULLPTR));
        labelPic->setText(QString());
        labelText->setText(QApplication::translate("StoppingDialog", "Stopping...", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class StoppingDialog: public Ui_StoppingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STOPPINGDIALOG_H
