/********************************************************************************
** Form generated from reading UI file 'statuswidget.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATUSWIDGET_H
#define UI_STATUSWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StatusWidget
{
public:
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QLabel *labelNumTotalText;
    QLabel *labelNumTotal;
    QLabel *labelNumSelectedText;
    QLabel *labelNumSelected;
    QLabel *labelNumRunText;
    QLabel *labelNumRun;
    QHBoxLayout *hboxLayout1;
    QLabel *labelNumSuccessPic;
    QLabel *labelNumSuccess;
    QHBoxLayout *hboxLayout2;
    QLabel *labelNumExceptionsPic;
    QLabel *labelNumExceptions;
    QHBoxLayout *hboxLayout3;
    QLabel *labelNumFatalsPic;
    QLabel *labelNumFatals;
    QHBoxLayout *hboxLayout4;
    QLabel *labelNumErrorsPic;
    QLabel *labelNumErrors;
    QHBoxLayout *hboxLayout5;
    QLabel *labelNumWarningsPic;
    QLabel *labelNumWarnings;
    QHBoxLayout *hboxLayout6;
    QLabel *labelNumInfosPic;
    QLabel *labelNumInfos;
    QSpacerItem *spacerItem1;
    QProgressBar *progressRun;

    void setupUi(QWidget *StatusWidget)
    {
        if (StatusWidget->objectName().isEmpty())
            StatusWidget->setObjectName(QStringLiteral("StatusWidget"));
        StatusWidget->resize(510, 20);
        hboxLayout = new QHBoxLayout(StatusWidget);
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        spacerItem = new QSpacerItem(6, 12, QSizePolicy::Fixed, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        labelNumTotalText = new QLabel(StatusWidget);
        labelNumTotalText->setObjectName(QStringLiteral("labelNumTotalText"));

        hboxLayout->addWidget(labelNumTotalText);

        labelNumTotal = new QLabel(StatusWidget);
        labelNumTotal->setObjectName(QStringLiteral("labelNumTotal"));

        hboxLayout->addWidget(labelNumTotal);

        labelNumSelectedText = new QLabel(StatusWidget);
        labelNumSelectedText->setObjectName(QStringLiteral("labelNumSelectedText"));

        hboxLayout->addWidget(labelNumSelectedText);

        labelNumSelected = new QLabel(StatusWidget);
        labelNumSelected->setObjectName(QStringLiteral("labelNumSelected"));

        hboxLayout->addWidget(labelNumSelected);

        labelNumRunText = new QLabel(StatusWidget);
        labelNumRunText->setObjectName(QStringLiteral("labelNumRunText"));

        hboxLayout->addWidget(labelNumRunText);

        labelNumRun = new QLabel(StatusWidget);
        labelNumRun->setObjectName(QStringLiteral("labelNumRun"));

        hboxLayout->addWidget(labelNumRun);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(3);
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        hboxLayout1->setObjectName(QStringLiteral("hboxLayout1"));
        labelNumSuccessPic = new QLabel(StatusWidget);
        labelNumSuccessPic->setObjectName(QStringLiteral("labelNumSuccessPic"));
        labelNumSuccessPic->setPixmap(QPixmap(QString::fromUtf8(":/icons/success.png")));

        hboxLayout1->addWidget(labelNumSuccessPic);

        labelNumSuccess = new QLabel(StatusWidget);
        labelNumSuccess->setObjectName(QStringLiteral("labelNumSuccess"));

        hboxLayout1->addWidget(labelNumSuccess);


        hboxLayout->addLayout(hboxLayout1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(3);
        hboxLayout2->setContentsMargins(0, 0, 0, 0);
        hboxLayout2->setObjectName(QStringLiteral("hboxLayout2"));
        labelNumExceptionsPic = new QLabel(StatusWidget);
        labelNumExceptionsPic->setObjectName(QStringLiteral("labelNumExceptionsPic"));
        labelNumExceptionsPic->setPixmap(QPixmap(QString::fromUtf8(":/icons/exception.png")));

        hboxLayout2->addWidget(labelNumExceptionsPic);

        labelNumExceptions = new QLabel(StatusWidget);
        labelNumExceptions->setObjectName(QStringLiteral("labelNumExceptions"));

        hboxLayout2->addWidget(labelNumExceptions);


        hboxLayout->addLayout(hboxLayout2);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setSpacing(3);
        hboxLayout3->setContentsMargins(0, 0, 0, 0);
        hboxLayout3->setObjectName(QStringLiteral("hboxLayout3"));
        labelNumFatalsPic = new QLabel(StatusWidget);
        labelNumFatalsPic->setObjectName(QStringLiteral("labelNumFatalsPic"));
        labelNumFatalsPic->setPixmap(QPixmap(QString::fromUtf8(":/icons/fatal.png")));

        hboxLayout3->addWidget(labelNumFatalsPic);

        labelNumFatals = new QLabel(StatusWidget);
        labelNumFatals->setObjectName(QStringLiteral("labelNumFatals"));

        hboxLayout3->addWidget(labelNumFatals);


        hboxLayout->addLayout(hboxLayout3);

        hboxLayout4 = new QHBoxLayout();
        hboxLayout4->setSpacing(3);
        hboxLayout4->setContentsMargins(0, 0, 0, 0);
        hboxLayout4->setObjectName(QStringLiteral("hboxLayout4"));
        labelNumErrorsPic = new QLabel(StatusWidget);
        labelNumErrorsPic->setObjectName(QStringLiteral("labelNumErrorsPic"));
        labelNumErrorsPic->setPixmap(QPixmap(QString::fromUtf8(":/icons/error.png")));

        hboxLayout4->addWidget(labelNumErrorsPic);

        labelNumErrors = new QLabel(StatusWidget);
        labelNumErrors->setObjectName(QStringLiteral("labelNumErrors"));

        hboxLayout4->addWidget(labelNumErrors);


        hboxLayout->addLayout(hboxLayout4);

        hboxLayout5 = new QHBoxLayout();
        hboxLayout5->setSpacing(3);
        hboxLayout5->setContentsMargins(0, 0, 0, 0);
        hboxLayout5->setObjectName(QStringLiteral("hboxLayout5"));
        labelNumWarningsPic = new QLabel(StatusWidget);
        labelNumWarningsPic->setObjectName(QStringLiteral("labelNumWarningsPic"));
        labelNumWarningsPic->setPixmap(QPixmap(QString::fromUtf8(":/icons/warning.png")));

        hboxLayout5->addWidget(labelNumWarningsPic);

        labelNumWarnings = new QLabel(StatusWidget);
        labelNumWarnings->setObjectName(QStringLiteral("labelNumWarnings"));

        hboxLayout5->addWidget(labelNumWarnings);


        hboxLayout->addLayout(hboxLayout5);

        hboxLayout6 = new QHBoxLayout();
        hboxLayout6->setSpacing(3);
        hboxLayout6->setContentsMargins(0, 0, 0, 0);
        hboxLayout6->setObjectName(QStringLiteral("hboxLayout6"));
        labelNumInfosPic = new QLabel(StatusWidget);
        labelNumInfosPic->setObjectName(QStringLiteral("labelNumInfosPic"));
        labelNumInfosPic->setPixmap(QPixmap(QString::fromUtf8(":/icons/info.png")));

        hboxLayout6->addWidget(labelNumInfosPic);

        labelNumInfos = new QLabel(StatusWidget);
        labelNumInfos->setObjectName(QStringLiteral("labelNumInfos"));

        hboxLayout6->addWidget(labelNumInfos);


        hboxLayout->addLayout(hboxLayout6);

        spacerItem1 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem1);

        progressRun = new QProgressBar(StatusWidget);
        progressRun->setObjectName(QStringLiteral("progressRun"));
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(7), static_cast<QSizePolicy::Policy>(0));
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(progressRun->sizePolicy().hasHeightForWidth());
        progressRun->setSizePolicy(sizePolicy);
        progressRun->setMaximumSize(QSize(16777215, 16));
        progressRun->setValue(0);
        progressRun->setOrientation(Qt::Horizontal);

        hboxLayout->addWidget(progressRun);


        retranslateUi(StatusWidget);

        QMetaObject::connectSlotsByName(StatusWidget);
    } // setupUi

    void retranslateUi(QWidget *StatusWidget)
    {
        StatusWidget->setWindowTitle(QApplication::translate("StatusWidget", "StatusWidget", Q_NULLPTR));
        labelNumTotalText->setText(QApplication::translate("StatusWidget", "Total:", Q_NULLPTR));
        labelNumTotal->setText(QApplication::translate("StatusWidget", "0", Q_NULLPTR));
        labelNumSelectedText->setText(QApplication::translate("StatusWidget", "Selected:", Q_NULLPTR));
        labelNumSelected->setText(QApplication::translate("StatusWidget", "0", Q_NULLPTR));
        labelNumRunText->setText(QApplication::translate("StatusWidget", "Run:", Q_NULLPTR));
        labelNumRun->setText(QApplication::translate("StatusWidget", "0", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        labelNumSuccessPic->setToolTip(QApplication::translate("StatusWidget", "Success", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        labelNumSuccessPic->setText(QString());
        labelNumSuccess->setText(QApplication::translate("StatusWidget", "0", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        labelNumExceptionsPic->setToolTip(QApplication::translate("StatusWidget", "Exceptions", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        labelNumExceptionsPic->setText(QString());
        labelNumExceptions->setText(QApplication::translate("StatusWidget", "0", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        labelNumFatalsPic->setToolTip(QApplication::translate("StatusWidget", "Fatals", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        labelNumFatalsPic->setText(QString());
        labelNumFatals->setText(QApplication::translate("StatusWidget", "0", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        labelNumErrorsPic->setToolTip(QApplication::translate("StatusWidget", "Errors", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        labelNumErrorsPic->setText(QString());
        labelNumErrors->setText(QApplication::translate("StatusWidget", "0", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        labelNumWarningsPic->setToolTip(QApplication::translate("StatusWidget", "Warnings", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        labelNumWarningsPic->setText(QString());
        labelNumWarnings->setText(QApplication::translate("StatusWidget", "0", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        labelNumInfosPic->setToolTip(QApplication::translate("StatusWidget", "Infos", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        labelNumInfosPic->setText(QString());
        labelNumInfos->setText(QApplication::translate("StatusWidget", "0", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class StatusWidget: public Ui_StatusWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATUSWIDGET_H
