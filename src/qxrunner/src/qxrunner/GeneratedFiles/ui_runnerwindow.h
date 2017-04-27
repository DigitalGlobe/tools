/********************************************************************************
** Form generated from reading UI file 'runnerwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RUNNERWINDOW_H
#define UI_RUNNERWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RunnerWindow
{
public:
    QAction *actionStart;
    QAction *actionStop;
    QAction *actionSelectAll;
    QAction *actionUnselectAll;
    QAction *actionAbout;
    QAction *actionExit;
    QAction *actionCollapseAll;
    QAction *actionExpandAll;
    QAction *actionMinimalUpdate;
    QAction *actionColumns;
    QAction *actionResults;
    QAction *actionSettings;
    QAction *actionToolbars;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTreeView *treeRunner;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuRunner;
    QMenu *menuView;
    QToolBar *runnerToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockResults;
    QWidget *dockWidgetContents;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QToolButton *buttonFatals;
    QToolButton *buttonErrors;
    QToolButton *buttonWarnings;
    QToolButton *buttonInfos;
    QSpacerItem *spacerItem;
    QTreeView *treeResults;
    QToolBar *viewToolBar;

    void setupUi(QMainWindow *RunnerWindow)
    {
        if (RunnerWindow->objectName().isEmpty())
            RunnerWindow->setObjectName(QStringLiteral("RunnerWindow"));
        RunnerWindow->resize(602, 390);
        const QIcon icon = QIcon(QString::fromUtf8(":/icons/qxrunner_16x16.png"));
        RunnerWindow->setWindowIcon(icon);
        actionStart = new QAction(RunnerWindow);
        actionStart->setObjectName(QStringLiteral("actionStart"));
        const QIcon icon1 = QIcon(QString::fromUtf8(":/icons/play.png"));
        actionStart->setIcon(icon1);
        actionStop = new QAction(RunnerWindow);
        actionStop->setObjectName(QStringLiteral("actionStop"));
        const QIcon icon2 = QIcon(QString::fromUtf8(":/icons/stop.png"));
        actionStop->setIcon(icon2);
        actionSelectAll = new QAction(RunnerWindow);
        actionSelectAll->setObjectName(QStringLiteral("actionSelectAll"));
        const QIcon icon3 = QIcon(QString::fromUtf8(":/icons/checkedbox.png"));
        actionSelectAll->setIcon(icon3);
        actionUnselectAll = new QAction(RunnerWindow);
        actionUnselectAll->setObjectName(QStringLiteral("actionUnselectAll"));
        const QIcon icon4 = QIcon(QString::fromUtf8(":/icons/uncheckedbox.png"));
        actionUnselectAll->setIcon(icon4);
        actionAbout = new QAction(RunnerWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionExit = new QAction(RunnerWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionCollapseAll = new QAction(RunnerWindow);
        actionCollapseAll->setObjectName(QStringLiteral("actionCollapseAll"));
        const QIcon icon5 = QIcon(QString::fromUtf8(":/icons/expanded.png"));
        actionCollapseAll->setIcon(icon5);
        actionExpandAll = new QAction(RunnerWindow);
        actionExpandAll->setObjectName(QStringLiteral("actionExpandAll"));
        const QIcon icon6 = QIcon(QString::fromUtf8(":/icons/collapsed.png"));
        actionExpandAll->setIcon(icon6);
        actionMinimalUpdate = new QAction(RunnerWindow);
        actionMinimalUpdate->setObjectName(QStringLiteral("actionMinimalUpdate"));
        actionColumns = new QAction(RunnerWindow);
        actionColumns->setObjectName(QStringLiteral("actionColumns"));
        const QIcon icon7 = QIcon(QString::fromUtf8(":/icons/columns.png"));
        actionColumns->setIcon(icon7);
        actionResults = new QAction(RunnerWindow);
        actionResults->setObjectName(QStringLiteral("actionResults"));
        actionSettings = new QAction(RunnerWindow);
        actionSettings->setObjectName(QStringLiteral("actionSettings"));
        const QIcon icon8 = QIcon(QString::fromUtf8(":/icons/properties.png"));
        actionSettings->setIcon(icon8);
        actionToolbars = new QAction(RunnerWindow);
        actionToolbars->setObjectName(QStringLiteral("actionToolbars"));
        centralWidget = new QWidget(RunnerWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(9, 9, 9, 9);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        treeRunner = new QTreeView(centralWidget);
        treeRunner->setObjectName(QStringLiteral("treeRunner"));
        treeRunner->setAlternatingRowColors(true);
        treeRunner->setSelectionMode(QAbstractItemView::SingleSelection);

        gridLayout->addWidget(treeRunner, 0, 0, 1, 1);

        RunnerWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(RunnerWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 602, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuRunner = new QMenu(menuBar);
        menuRunner->setObjectName(QStringLiteral("menuRunner"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        RunnerWindow->setMenuBar(menuBar);
        runnerToolBar = new QToolBar(RunnerWindow);
        runnerToolBar->setObjectName(QStringLiteral("runnerToolBar"));
        runnerToolBar->setOrientation(Qt::Horizontal);
        RunnerWindow->addToolBar(static_cast<Qt::ToolBarArea>(4), runnerToolBar);
        statusBar = new QStatusBar(RunnerWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        RunnerWindow->setStatusBar(statusBar);
        dockResults = new QDockWidget(RunnerWindow);
        dockResults->setObjectName(QStringLiteral("dockResults"));
        dockResults->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        vboxLayout = new QVBoxLayout(dockWidgetContents);
        vboxLayout->setSpacing(3);
        vboxLayout->setContentsMargins(9, 9, 9, 9);
        vboxLayout->setObjectName(QStringLiteral("vboxLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(0);
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        buttonFatals = new QToolButton(dockWidgetContents);
        buttonFatals->setObjectName(QStringLiteral("buttonFatals"));
        buttonFatals->setFocusPolicy(Qt::StrongFocus);
        const QIcon icon9 = QIcon(QString::fromUtf8(":/icons/fatal.png"));
        buttonFatals->setIcon(icon9);
        buttonFatals->setCheckable(true);
        buttonFatals->setChecked(true);
        buttonFatals->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        buttonFatals->setAutoRaise(true);

        hboxLayout->addWidget(buttonFatals);

        buttonErrors = new QToolButton(dockWidgetContents);
        buttonErrors->setObjectName(QStringLiteral("buttonErrors"));
        buttonErrors->setFocusPolicy(Qt::StrongFocus);
        const QIcon icon10 = QIcon(QString::fromUtf8(":/icons/error.png"));
        buttonErrors->setIcon(icon10);
        buttonErrors->setCheckable(true);
        buttonErrors->setChecked(true);
        buttonErrors->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        buttonErrors->setAutoRaise(true);

        hboxLayout->addWidget(buttonErrors);

        buttonWarnings = new QToolButton(dockWidgetContents);
        buttonWarnings->setObjectName(QStringLiteral("buttonWarnings"));
        buttonWarnings->setFocusPolicy(Qt::StrongFocus);
        const QIcon icon11 = QIcon(QString::fromUtf8(":/icons/warning.png"));
        buttonWarnings->setIcon(icon11);
        buttonWarnings->setCheckable(true);
        buttonWarnings->setChecked(true);
        buttonWarnings->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        buttonWarnings->setAutoRaise(true);

        hboxLayout->addWidget(buttonWarnings);

        buttonInfos = new QToolButton(dockWidgetContents);
        buttonInfos->setObjectName(QStringLiteral("buttonInfos"));
        buttonInfos->setFocusPolicy(Qt::StrongFocus);
        const QIcon icon12 = QIcon(QString::fromUtf8(":/icons/info.png"));
        buttonInfos->setIcon(icon12);
        buttonInfos->setCheckable(true);
        buttonInfos->setChecked(true);
        buttonInfos->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        buttonInfos->setAutoRaise(true);

        hboxLayout->addWidget(buttonInfos);

        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);


        vboxLayout->addLayout(hboxLayout);

        treeResults = new QTreeView(dockWidgetContents);
        treeResults->setObjectName(QStringLiteral("treeResults"));
        treeResults->setMaximumSize(QSize(16777215, 80));
        treeResults->setAlternatingRowColors(true);

        vboxLayout->addWidget(treeResults);

        dockResults->setWidget(dockWidgetContents);
        RunnerWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockResults);
        viewToolBar = new QToolBar(RunnerWindow);
        viewToolBar->setObjectName(QStringLiteral("viewToolBar"));
        viewToolBar->setOrientation(Qt::Horizontal);
        RunnerWindow->addToolBar(static_cast<Qt::ToolBarArea>(4), viewToolBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuRunner->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionExit);
        menuHelp->addAction(actionAbout);
        menuRunner->addAction(actionStart);
        menuRunner->addAction(actionStop);
        menuView->addAction(actionSelectAll);
        menuView->addAction(actionUnselectAll);
        menuView->addSeparator();
        menuView->addAction(actionExpandAll);
        menuView->addAction(actionCollapseAll);
        menuView->addSeparator();
        menuView->addAction(actionMinimalUpdate);
        menuView->addAction(actionResults);
        menuView->addAction(actionToolbars);
        menuView->addSeparator();
        menuView->addAction(actionColumns);
        menuView->addAction(actionSettings);
        runnerToolBar->addAction(actionStart);
        runnerToolBar->addAction(actionStop);
        viewToolBar->addAction(actionSelectAll);
        viewToolBar->addAction(actionUnselectAll);
        viewToolBar->addAction(actionExpandAll);
        viewToolBar->addAction(actionCollapseAll);
        viewToolBar->addAction(actionColumns);
        viewToolBar->addAction(actionSettings);

        retranslateUi(RunnerWindow);

        QMetaObject::connectSlotsByName(RunnerWindow);
    } // setupUi

    void retranslateUi(QMainWindow *RunnerWindow)
    {
        RunnerWindow->setWindowTitle(QApplication::translate("RunnerWindow", "QxRunner", Q_NULLPTR));
        actionStart->setText(QApplication::translate("RunnerWindow", "&Start", Q_NULLPTR));
        actionStop->setText(QApplication::translate("RunnerWindow", "S&top", Q_NULLPTR));
        actionSelectAll->setText(QApplication::translate("RunnerWindow", "Select &All", Q_NULLPTR));
        actionUnselectAll->setText(QApplication::translate("RunnerWindow", "&Unselect All", Q_NULLPTR));
        actionAbout->setText(QApplication::translate("RunnerWindow", "&About", Q_NULLPTR));
        actionExit->setText(QApplication::translate("RunnerWindow", "E&xit", Q_NULLPTR));
        actionCollapseAll->setText(QApplication::translate("RunnerWindow", "&Collapse All", Q_NULLPTR));
        actionExpandAll->setText(QApplication::translate("RunnerWindow", "&Expand All", Q_NULLPTR));
        actionMinimalUpdate->setText(QApplication::translate("RunnerWindow", "&Minimal Update", Q_NULLPTR));
        actionColumns->setText(QApplication::translate("RunnerWindow", "C&olumns...", Q_NULLPTR));
        actionResults->setText(QApplication::translate("RunnerWindow", "&Results", Q_NULLPTR));
        actionSettings->setText(QApplication::translate("RunnerWindow", "&Settings...", Q_NULLPTR));
        actionToolbars->setText(QApplication::translate("RunnerWindow", "&Toolbars", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("RunnerWindow", "&File", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("RunnerWindow", "&Help", Q_NULLPTR));
        menuRunner->setTitle(QApplication::translate("RunnerWindow", "&Runner", Q_NULLPTR));
        menuView->setTitle(QApplication::translate("RunnerWindow", "&View", Q_NULLPTR));
        runnerToolBar->setWindowTitle(QApplication::translate("RunnerWindow", "Ru&nner Toolbar", Q_NULLPTR));
        dockResults->setWindowTitle(QApplication::translate("RunnerWindow", "Results", Q_NULLPTR));
        buttonFatals->setText(QApplication::translate("RunnerWindow", "dummy", Q_NULLPTR));
        buttonErrors->setText(QApplication::translate("RunnerWindow", "dummy", Q_NULLPTR));
        buttonWarnings->setText(QApplication::translate("RunnerWindow", "dummy", Q_NULLPTR));
        buttonInfos->setText(QApplication::translate("RunnerWindow", "dummy", Q_NULLPTR));
        viewToolBar->setWindowTitle(QApplication::translate("RunnerWindow", "&View Toolbar", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class RunnerWindow: public Ui_RunnerWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RUNNERWINDOW_H
