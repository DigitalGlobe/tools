/********************************************************************************
** Form generated from reading UI file 'aboutdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTDIALOG_H
#define UI_ABOUTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_AboutDialog
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *spacerItem;
    QLabel *labelIcon;
    QLabel *labelQxRunner;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem1;
    QPushButton *buttonOk;
    QGridLayout *gridLayout1;
    QLabel *labelModelNameText;
    QLabel *labelVersionText;
    QLabel *labelQtVersion;
    QLabel *labelQtVersionText;
    QLabel *labelModelName;
    QLabel *labelVersion;
    QSpacerItem *spacerItem2;

    void setupUi(QDialog *AboutDialog)
    {
        if (AboutDialog->objectName().isEmpty())
            AboutDialog->setObjectName(QStringLiteral("AboutDialog"));
        AboutDialog->resize(214, 158);
        gridLayout = new QGridLayout(AboutDialog);
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout->setContentsMargins(9, 9, 9, 9);
#endif
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        spacerItem = new QSpacerItem(41, 91, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(spacerItem, 2, 0, 2, 1);

        labelIcon = new QLabel(AboutDialog);
        labelIcon->setObjectName(QStringLiteral("labelIcon"));
        labelIcon->setPixmap(QPixmap(QString::fromUtf8(":/icons/qxrunner_64x64.png")));

        gridLayout->addWidget(labelIcon, 0, 0, 2, 1);

        labelQxRunner = new QLabel(AboutDialog);
        labelQxRunner->setObjectName(QStringLiteral("labelQxRunner"));
        labelQxRunner->setFrameShape(QFrame::NoFrame);
        labelQxRunner->setFrameShadow(QFrame::Plain);

        gridLayout->addWidget(labelQxRunner, 0, 1, 1, 1);

        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        spacerItem1 = new QSpacerItem(51, 25, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem1);

        buttonOk = new QPushButton(AboutDialog);
        buttonOk->setObjectName(QStringLiteral("buttonOk"));

        hboxLayout->addWidget(buttonOk);


        gridLayout->addLayout(hboxLayout, 3, 1, 1, 1);

        gridLayout1 = new QGridLayout();
#ifndef Q_OS_MAC
        gridLayout1->setSpacing(6);
#endif
        gridLayout1->setContentsMargins(0, 0, 0, 0);
        gridLayout1->setObjectName(QStringLiteral("gridLayout1"));
        labelModelNameText = new QLabel(AboutDialog);
        labelModelNameText->setObjectName(QStringLiteral("labelModelNameText"));
        labelModelNameText->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout1->addWidget(labelModelNameText, 1, 0, 1, 1);

        labelVersionText = new QLabel(AboutDialog);
        labelVersionText->setObjectName(QStringLiteral("labelVersionText"));

        gridLayout1->addWidget(labelVersionText, 0, 0, 1, 1);

        labelQtVersion = new QLabel(AboutDialog);
        labelQtVersion->setObjectName(QStringLiteral("labelQtVersion"));
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(5));
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(labelQtVersion->sizePolicy().hasHeightForWidth());
        labelQtVersion->setSizePolicy(sizePolicy);

        gridLayout1->addWidget(labelQtVersion, 2, 1, 1, 1);

        labelQtVersionText = new QLabel(AboutDialog);
        labelQtVersionText->setObjectName(QStringLiteral("labelQtVersionText"));

        gridLayout1->addWidget(labelQtVersionText, 2, 0, 1, 1);

        labelModelName = new QLabel(AboutDialog);
        labelModelName->setObjectName(QStringLiteral("labelModelName"));
        sizePolicy.setHeightForWidth(labelModelName->sizePolicy().hasHeightForWidth());
        labelModelName->setSizePolicy(sizePolicy);

        gridLayout1->addWidget(labelModelName, 1, 1, 1, 1);

        labelVersion = new QLabel(AboutDialog);
        labelVersion->setObjectName(QStringLiteral("labelVersion"));
        sizePolicy.setHeightForWidth(labelVersion->sizePolicy().hasHeightForWidth());
        labelVersion->setSizePolicy(sizePolicy);

        gridLayout1->addWidget(labelVersion, 0, 1, 1, 1);


        gridLayout->addLayout(gridLayout1, 1, 1, 2, 1);

        spacerItem2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(spacerItem2, 4, 0, 1, 2);


        retranslateUi(AboutDialog);

        buttonOk->setDefault(true);


        QMetaObject::connectSlotsByName(AboutDialog);
    } // setupUi

    void retranslateUi(QDialog *AboutDialog)
    {
        AboutDialog->setWindowTitle(QApplication::translate("AboutDialog", "About QxRunner", Q_NULLPTR));
        labelIcon->setText(QString());
        labelQxRunner->setText(QApplication::translate("AboutDialog", "QxRunner", Q_NULLPTR));
        buttonOk->setText(QApplication::translate("AboutDialog", "OK", Q_NULLPTR));
        labelModelNameText->setText(QApplication::translate("AboutDialog", "Model:", Q_NULLPTR));
        labelVersionText->setText(QApplication::translate("AboutDialog", "Version:", Q_NULLPTR));
        labelQtVersion->setText(QApplication::translate("AboutDialog", "0", Q_NULLPTR));
        labelQtVersionText->setText(QApplication::translate("AboutDialog", "Qt:", Q_NULLPTR));
        labelModelName->setText(QApplication::translate("AboutDialog", "dummy", Q_NULLPTR));
        labelVersion->setText(QApplication::translate("AboutDialog", "0", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AboutDialog: public Ui_AboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTDIALOG_H
