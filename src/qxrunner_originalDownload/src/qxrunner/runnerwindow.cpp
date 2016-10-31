/*!
 * \file  runnerwindow.cpp
 *
 * \brief Implements class RunnerWindow.
 */

#include "runnerwindow.h"
#include "runnerviewcontroller.h"
#include "resultsviewcontroller.h"
#include "runnermodel.h"
#include "resultsmodel.h"
#include "runnerproxymodel.h"
#include "resultsproxymodel.h"
#include "statuswidget.h"
#include "stoppingdialog.h"
#include "columnsdialog.h"
#include "settingsdialog.h"
#include "aboutdialog.h"
#include "appsettings.h"
#include "utils.h"

#include <QMessageBox>
#include <QCloseEvent>

// Helper function needed to expand Q_INIT_RESOURCE outside the namespace.
static void initQxRunnerResource()
{
	Q_INIT_RESOURCE(qxrunner);
}

namespace QxRunner {

RunnerWindow::RunnerWindow(QWidget* parent, Qt::WFlags flags)
            : QMainWindow(parent, flags)
{
	initQxRunnerResource();

	ui.setupUi(this);

	// Controllers for the tree views, it's best to create them at the
	// very beginning.
	m_runnerViewController = new RunnerViewController(this, runnerView());
	m_resultsViewController = new ResultsViewController(this, resultsView());

	// Adjust GUI.
	ui.actionMinimalUpdate->setCheckable(true);
	resultsView()->header()->setResizeMode(QHeaderView::Stretch);

	// Replace results menu item which serves as a placeholder
	// for the action from the dock widget.
	QAction* actionResults = ui.dockResults->toggleViewAction();
	actionResults->setText(ui.actionResults->text());

	ui.menuView->insertAction(ui.actionResults, actionResults);
	ui.menuView->removeAction(ui.actionResults);

	qSwap(ui.actionResults, actionResults);
	delete actionResults;

	connect(ui.actionResults, SIGNAL(toggled(bool)), SLOT(showResults(bool)));

	// Insert menu items for toolbar selection submenu.
	QMenu* toolbarsMenu = new QMenu(this);
	toolbarsMenu->insertAction(0, ui.runnerToolBar->toggleViewAction());
	toolbarsMenu->insertAction(0, ui.viewToolBar->toggleViewAction());
	ui.actionToolbars->setMenu(toolbarsMenu);

	// Apply settings.
	AppSettings settings(this);
	settings.applyBaseSettings();

	// Fine tuning of the focus rect handling because there should
	// always be a focus rect visible in the views.
	connect(runnerView(),  SIGNAL(clicked(const QModelIndex&)),
                           SLOT(ensureFocusRect(const QModelIndex&)));
	connect(resultsView(), SIGNAL(clicked(const QModelIndex&)),
                           SLOT(ensureFocusRect(const QModelIndex&)));

	// To keep the views synchronized when there are highlighted rows
	// which get clicked again..
	connect(runnerView(),  SIGNAL(pressed(const QModelIndex&)),
                           SLOT(scrollToHighlightedRows()));
	connect(resultsView(), SIGNAL(pressed(const QModelIndex&)),
                           SLOT(scrollToHighlightedRows()));

	// Set-up statusbar.
	m_statusWidget = new StatusWidget(0);
	statusBar()->addPermanentWidget(m_statusWidget, 1);
	statusWidget()->progressRun->hide();

	// Disable user interaction while there is no data.
	enableItemActions(false);
	enableResultsFilter(false);

	// Initial results.
	displayNumInfos(0);
	displayNumWarnings(0);
	displayNumErrors(0);
	displayNumFatals(0);
	displayNumExceptions(0);

	// Results filter commands.
	connect(ui.buttonInfos,    SIGNAL(toggled(bool)),
                               SLOT(setResultsFilter()));
	connect(ui.buttonWarnings, SIGNAL(toggled(bool)),
                               SLOT(setResultsFilter()));
	connect(ui.buttonErrors,   SIGNAL(toggled(bool)),
                               SLOT(setResultsFilter()));
	connect(ui.buttonFatals,   SIGNAL(toggled(bool)),
                               SLOT(setResultsFilter()));

	// File commands.
	connect(ui.actionExit, SIGNAL(triggered(bool)), SLOT(close()));

	// Item commands.
	ui.actionStart->setShortcut(QKeySequence(tr("Ctrl+R")));
	connect(ui.actionStart, SIGNAL(triggered(bool)), SLOT(runItems()));

	ui.actionStop->setShortcut(QKeySequence(tr("Ctrl+K")));
	connect(ui.actionStop, SIGNAL(triggered(bool)), SLOT(stopItems()));

	// View commands
	ui.actionSelectAll->setShortcut(QKeySequence(tr("Ctrl+A")));
	connect(ui.actionSelectAll, SIGNAL(triggered(bool)),
                                runnerController(),
                                SLOT(selectAll()));

	ui.actionUnselectAll->setShortcut(QKeySequence(tr("Ctrl+U")));
	connect(ui.actionUnselectAll, SIGNAL(triggered(bool)),
                                  runnerController(),
                                  SLOT(unselectAll()));

	ui.actionExpandAll->setShortcut(QKeySequence(tr("Ctrl++")));
	connect(ui.actionExpandAll, SIGNAL(triggered(bool)),
                                runnerController(),
                                SLOT(expandAll()));

	ui.actionCollapseAll->setShortcut(QKeySequence(tr("Ctrl+-")));
	connect(ui.actionCollapseAll, SIGNAL(triggered(bool)),
                                  runnerController(),
                                  SLOT(collapseAll()));

	connect(ui.actionColumns, SIGNAL(triggered(bool)),
                              SLOT(showColumnsSelection()));

	connect(ui.actionSettings, SIGNAL(triggered(bool)),
                               SLOT(showSettings()));

	// Help commands
	connect(ui.actionAbout, SIGNAL(triggered(bool)),
                            SLOT(showAbout()));

	// Set initially available.
	m_sema.release();
}

RunnerWindow::~RunnerWindow()
{
	// Deleting the model is left to the owner of the model instance.
}

void RunnerWindow::show()
{
	setUpdatesEnabled(false);			// Reduce flickering

	AppSettings settings(this);
	settings.applyWindowSettings();		// Could enable updates again

	QMainWindow::show();

	// Workaround: The results tree view has a maximum height defined in
	// Designer which now is reset. Without the height restriction the
	// dock window would occupy half the main window after construcution
	// instead of sitting "nicely" at the bottom.
	// This is only for the situation when the main window geometry isn't
	// restored from the settings.
	resultsView()->setMaximumSize(QSize(16777215, 16777215));

	setUpdatesEnabled(true);

	// Change the results tree view to manual adjustment after the columns
	// have been stretched over the tree view at inital display.
	// This is only for the situation when the column sizes aren't restored
	// from the settings.
	resultsView()->header()->setResizeMode(QHeaderView::Interactive);

	// Some menus depend on current state.
	adjustMenus();

	QApplication::setActiveWindow(this);
}

void RunnerWindow::setModel(RunnerModel* model)
{
	// It's not expected to set another model at the moment but if done so then
	// at least remove the previous model from the views to prevent the runner
	// from crashing. Deleting a previous model is left to the owner of the model
	// instance. The fact that a previous model could have a running thread is
	// taken into account by simply asking the model to stop it without further
	// verification. A better solution must consider this in a later version.
	RunnerModel* prevModel = runnerModel();

	if (prevModel)
	{
		// Hope it stops the thread if it is running.
		prevModel->stopItems();

		// As long as the proxy models can't be set from outside they
		// get deleted.
		RunnerProxyModel* m1 = runnerProxyModel();
		ResultsProxyModel* m2 = resultsProxyModel();

		runnerView()->setModel(0);
		resultsView()->setModel(0);
		runnerView()->reset();
		resultsView()->reset();

		delete m1;
		delete m2;

		prevModel->disconnect();
	}

	// Set initial title.
	QStringList tokens = windowTitle().split(" ");
	setWindowTitle(tokens[0].trimmed());

	// No user interaction without a model or a model without columns.
	bool valid = true;

	if (!model)
	{
		valid = false;
	}
	else if (model->columnCount() < 1)
	{
		valid = false;
	}

	if (!valid)
	{
		enableItemActions(false);
		enableResultsFilter(false);
		return;
	}

	// Set title with model name.
	if (!model->name().trimmed().isEmpty())
	{
		setWindowTitle(windowTitle() + " - " + model->name());
	}

	// Hide filter buttons for results that are not expected.
	int results = model->expectedResults();

	bool expected;

	expected = results & QxRunner::RunInfo;
	ui.buttonInfos->setVisible(expected);
	expected = results & QxRunner::RunWarning;
	ui.buttonWarnings->setVisible(expected);
	expected = results & QxRunner::RunError;
	ui.buttonErrors->setVisible(expected);
	expected = results & QxRunner::RunFatal;
	ui.buttonFatals->setVisible(expected);

	// Proxy models for the views with the source model as their parent
	// to have the proxies deleted when the model gets deleted.
	RunnerProxyModel* runnerProxyModel = new RunnerProxyModel(model);
	runnerProxyModel->setSourceModel(model);

	runnerView()->setModel(runnerProxyModel);

	// Results model is contained in the runner model.
	ResultsModel* resultsModel = model->resultsModel();
	ResultsProxyModel* resultsProxyModel = new ResultsProxyModel(model);
	resultsProxyModel->setSourceModel(resultsModel);

	resultsView()->setModel(resultsProxyModel);

	// Filter actions enabled, item actions only when there is data.
	enableResultsFilter(true);

	// Get the relevant column count.
	int columnCount = model->columnCount();

	// Set the defaults for enabled and thus visible columns.
	QBitArray enabledRunnerColumns(columnCount);
	QBitArray enabledResultsColumns(columnCount, true);

	for (int i = 0; i < 2; i++)
	{
		if (i < columnCount)
		{
			runnerView()->showColumn(i);
			enabledRunnerColumns[i] = true;
		}
	}

	for (int i = 2; i < columnCount; i++)
	{
		runnerView()->hideColumn(i);
	}


	for (int i = 0; i < columnCount; i++)
	{
		resultsView()->showColumn(i);
	}

	// Set the defaults in the proxy models.
	runnerProxyModel->setEnabledColumns(enabledRunnerColumns);
	resultsProxyModel->setEnabledColumns(enabledResultsColumns);

	// Suppress results data display if view isn't shown.
	showResults(isResultsViewVisible());

	// Very limited user interaction without data.
	if (model->rowCount() < 1)
	{
		enableItemActions(false);
		ui.actionColumns->setEnabled(true);
		ui.actionSettings->setEnabled(true);
		AppSettings settings(this);
		settings.applyColumnSettings();		// Have no data in columns
		return;
	}

	// Item statistics.
	connect(model, SIGNAL(numTotalChanged(int)),
                   SLOT(displayNumTotal(int)));
	connect(model, SIGNAL(numSelectedChanged(int)),
                   SLOT(displayNumSelected(int)));
	connect(model, SIGNAL(numSuccessChanged(int)),
                   SLOT(displayNumSuccess(int)));
	connect(model, SIGNAL(numInfosChanged(int)),
                   SLOT(displayNumInfos(int)));
	connect(model, SIGNAL(numWarningsChanged(int)),
                   SLOT(displayNumWarnings(int)));
	connect(model, SIGNAL(numErrorsChanged(int)),
                   SLOT(displayNumErrors(int)));
	connect(model, SIGNAL(numFatalsChanged(int)),
                   SLOT(displayNumFatals(int)));
	connect(model, SIGNAL(numExceptionsChanged(int)),
                   SLOT(displayNumExceptions(int)));

	// This will fire the signals connected above.
	model->countItems();

	// Expand the branches, do it in the brackground to reduce flickering.
	runnerView()->setUpdatesEnabled(false);

	runnerController()->expandAll();
	AppSettings settings(this);
	settings.applyColumnSettings();		// Have data in columns

	runnerView()->setUpdatesEnabled(true);

	// How much data is wanted when running the items.
	connect(ui.actionMinimalUpdate, SIGNAL(triggered(bool)),
                                    model,
                                    SLOT(setMinimalUpdate(bool)));
	
	model->setMinimalUpdate(isMinimalUpdate());

	// Get notified of items run.
	connect(model, SIGNAL(numStartedChanged(int)),
                   SLOT(displayProgress(int)));
	connect(model, SIGNAL(itemStarted(const QModelIndex&)),
                   SLOT(highlightRunningItem(const QModelIndex&)));
	connect(model, SIGNAL(numCompletedChanged(int)),
                   SLOT(displayNumCompleted(int)));
	connect(model, SIGNAL(allItemsCompleted()),
                   SLOT(displayCompleted()));
	connect(model, SIGNAL(itemCompleted(const QModelIndex&)),
                   resultsModel,
                   SLOT(addResult(const QModelIndex&)));

	enableItemActions(true);
	ui.actionStop->setDisabled(true);

	// Set the filter in the results model.
	setResultsFilter();

	// Start with top row highlighted.
	QModelIndex index = runnerView()->indexBelow(runnerView()->rootIndex());
	runnerView()->setCurrentIndex(index);
}

QTreeView* RunnerWindow::runnerView() const
{
	return ui.treeRunner;
}

QTreeView* RunnerWindow::resultsView() const
{
	return ui.treeResults;
}

RunnerModel* RunnerWindow::runnerModel() const
{
	return runnerController()->runnerModel();
}

RunnerProxyModel* RunnerWindow::runnerProxyModel() const
{
	return runnerController()->runnerProxyModel();
}

ResultsModel* RunnerWindow::resultsModel() const
{
	// The results model is contained in the runner model
	// (but could also be retrieved from the results view controller).
	return runnerModel()->resultsModel();
}

ResultsProxyModel* RunnerWindow::resultsProxyModel() const
{
	return resultsController()->resultsProxyModel();
}

void RunnerWindow::displayProgress(int numItems) const
{
	// Display only when there are selected items
	if (statusWidget()->progressRun->maximum() > 0)
	{
		statusWidget()->progressRun->setValue(numItems);
		statusWidget()->progressRun->show();
	}
}

void RunnerWindow::displayCompleted() const
{
	statusWidget()->progressRun->hide();
	enableControlsAfterRunning();
}

bool RunnerWindow::isResultsViewVisible() const
{
	return ui.actionResults->isChecked();
}

void RunnerWindow::displayNumTotal(int numItems) const
{
	statusWidget()->labelNumTotal->setText(QString().setNum(numItems));
}

void RunnerWindow::displayNumSelected(int numItems) const
{
	statusWidget()->labelNumSelected->setText(QString().setNum(numItems));

	// During item selection the progress bar shouldn't be visible.
	statusWidget()->progressRun->hide();
	statusWidget()->progressRun->setMaximum(numItems);
}

void RunnerWindow::displayNumCompleted(int numItems) const
{
	statusWidget()->labelNumRun->setText(QString().setNum(numItems));
}

void RunnerWindow::displayNumSuccess(int numItems) const
{
	displayStatusNum(statusWidget()->labelNumSuccess,
                     statusWidget()->labelNumSuccess, numItems);

	// Num success always visible.
	statusWidget()->labelNumSuccess->show();
}

void RunnerWindow::displayNumInfos(int numItems) const
{
	if (numItems != 1)
	{
		ui.buttonInfos->setText(QString().setNum(numItems) +
                                tr(" Infos"));
	}
	else
	{
		ui.buttonInfos->setText(tr("1 Info"));
	}

	displayStatusNum(statusWidget()->labelNumInfos,
                     statusWidget()->labelNumInfosPic, numItems);
}

void RunnerWindow::displayNumWarnings(int numItems) const
{
	if (numItems != 1)
	{
		ui.buttonWarnings->setText(QString().setNum(numItems) +
                                   tr(" Warnings"));
	}
	else
	{
		ui.buttonWarnings->setText(tr("1 Warning"));
	}

	displayStatusNum(statusWidget()->labelNumWarnings,
                     statusWidget()->labelNumWarningsPic, numItems);
}

void RunnerWindow::displayNumErrors(int numItems) const
{
	if (numItems != 1)
	{
		ui.buttonErrors->setText(QString().setNum(numItems) +
                                 tr(" Errors"));
	}
	else
	{
		ui.buttonErrors->setText(tr("1 Error"));
	}

	displayStatusNum(statusWidget()->labelNumErrors,
                     statusWidget()->labelNumErrorsPic, numItems);
}

void RunnerWindow::displayNumFatals(int numItems) const
{
	if (numItems != 1)
	{
		ui.buttonFatals->setText(QString().setNum(numItems) +
                                 tr(" Fatals"));
	}
	else
	{
		ui.buttonFatals->setText(tr("1 Fatal"));
	}

	displayStatusNum(statusWidget()->labelNumFatals,
                     statusWidget()->labelNumFatalsPic, numItems);
}

void RunnerWindow::displayNumExceptions(int numItems) const
{
	
	displayStatusNum(statusWidget()->labelNumExceptions,
                     statusWidget()->labelNumExceptionsPic, numItems);
}

void RunnerWindow::highlightRunningItem(const QModelIndex& runnerItemIndex) const
{
	if (isMinimalUpdate())
	{
		return;
	}

	// Determine runner model proxy index and highlight related row.
	QModelIndex index;
	index = Utils::proxyIndexFromModel(runnerProxyModel(), runnerItemIndex);
	runnerController()->setHighlightedRow(index);
}

void RunnerWindow::setResultsFilter() const
{
	// Remember currently highlighted result.
	QModelIndex resultIndex;
	QModelIndexList indexes;
	indexes = resultsView()->selectionModel()->selectedIndexes();

	if (indexes.count() > 0)
	{
		resultIndex = Utils::modelIndexFromProxy(resultsProxyModel(),
                                                 indexes.first());
	}

	// Determine filter settings.
	int filter = 0;

	if (ui.buttonInfos->isEnabled() && ui.buttonInfos->isChecked())
	{
		filter = QxRunner::RunInfo;
	}

	if (ui.buttonWarnings->isEnabled() && ui.buttonWarnings->isChecked())
	{
		filter = filter | QxRunner::RunWarning;
	}

	if (ui.buttonErrors->isEnabled() && ui.buttonErrors->isChecked())
	{
		filter = filter | QxRunner::RunError;
	}

	if (ui.buttonFatals->isEnabled() && ui.buttonFatals->isChecked())
	{
		filter = filter | QxRunner::RunFatal;
	}
	
	// Setting the filter updates the view.
	resultsProxyModel()->setFilter(filter);

	// When no result was highlighted before than try to highlight the
	// one that corresponds to the currently highlighted runner item.
	if (!resultIndex.isValid())
	{
		syncResultWithRunnerItem(runnerView()->selectionModel()->selection(),
                                 runnerView()->selectionModel()->selection());
		return;
	}

	// At least there should be a current but not necessarily highlighted result
	// in order to see the focus rect when the results view gets the focus.
	ensureCurrentResult();

	// Try to highlight same result as was highlighted before.
	QModelIndex currentIndex;
	currentIndex = Utils::proxyIndexFromModel(resultsProxyModel(), resultIndex);

	if (!currentIndex.isValid())
	{
		// Previously highlighted result is filtered out now.
		return;
	}

	// Highlight result without affecting the runner view.
	enableResultSync(false);
	resultsView()->setCurrentIndex(currentIndex);
	enableResultSync(true);

	// Make the row in every tree view visible and expand corresponding parents.
	scrollToHighlightedRows();
}

void RunnerWindow::syncResultWithRunnerItem(const QItemSelection& selected,
                                            const QItemSelection& deselected) const
{
	// Do nothing when there are no results or no runner item is selected.
	if (!resultsView()->indexBelow(resultsView()->rootIndex()).isValid())
	{
		return;
	}

	QModelIndexList indexes = selected.indexes();

	if (indexes.count() < 1 )
	{
		return;
	}

	// Prevent from circular dependencies.
	enableResultSync(false);

	// Try to highlight the corresponding result.
	resultsView()->clearSelection();

	// Get the results model index that corresponds to the runner item index.
	QModelIndex runnerItemIndex;
	runnerItemIndex = Utils::modelIndexFromProxy(runnerProxyModel(), indexes.first());
	QModelIndex resultIndex = resultsModel()->mapFromRunnerItemIndex(runnerItemIndex);

	// At least there should be a current but not necessarily highlighted result
	// in order to see the focus rect when the results view gets the focus.
	ensureCurrentResult();

	// If not found then there is no result for this runner item.
	if (!resultIndex.isValid())
	{
		// Enable selection handler again.
		enableResultSync(true);
		return;
	}

	// Prepare row highlighting.
	QModelIndex currentIndex = Utils::proxyIndexFromModel(resultsProxyModel(),
                                                          resultIndex);

	// When results proxy model index not exists it is filtered out.
	if (!currentIndex.isValid())
	{
		// Enable selection handler again.
		enableResultSync(true);
		return;
	}

	// Highlight corresponding result now.
	resultsView()->setCurrentIndex(currentIndex);

	// Make the row in every tree view visible and expand corresponding parents.
	scrollToHighlightedRows();

	// Enable selection handler again.
	enableResultSync(true);
}

void RunnerWindow::syncRunnerItemWithResult(const QItemSelection& selected,
                                            const QItemSelection& deselected) const
{
	// Do nothing when no result is selected.
	QModelIndexList indexes = selected.indexes();

	if (indexes.count() < 1 )
	{
		return;
	}

	// Determine the results model index.
	QModelIndex resultIndex;
	resultIndex = Utils::modelIndexFromProxy(resultsProxyModel(), indexes.first());

	// Get the corresponding runner item index contained in the results model.
	QModelIndex runnerItemIndex;
	runnerItemIndex = resultsModel()->mapToRunnerItemIndex(resultIndex);

	// Prevent from circular dependencies.
	enableRunnerItemSync(false);

	// Determine the proxy model index and highlight it.
	QModelIndex currentIndex;
	currentIndex = Utils::proxyIndexFromModel(runnerProxyModel(), runnerItemIndex);
	runnerView()->setCurrentIndex(currentIndex);

	// Make the row in every tree view visible and expand corresponding parents.
	scrollToHighlightedRows();

	// Enable selection handler again.
	enableRunnerItemSync(true);
}

void RunnerWindow::ensureFocusRect(const QModelIndex&  index)
{
	QTreeView* treeView;

	if (runnerView()->hasFocus())
	{
		treeView = runnerView();
	}
	else
	{
		treeView = resultsView();
	}

	// No relevance when selections not allowed.
	if (treeView->selectionMode() == QAbstractItemView::NoSelection)
	{
		return;
	}

	// Focus rect is there when column entry not empty.
	QString data = index.data().toString();

	if (!data.trimmed().isEmpty())
	{
		return;
	}

	// No relevance when column 0 is empty.
	if (index.column() == 0)
	{
		return;
	}

	// Tree views are already synchronized.
	enableRunnerItemSync(false);
	enableResultSync(false);

	// This ensures that there is a focus rect.
	treeView->clearSelection();

	QItemSelectionModel* selectionModel = treeView->selectionModel();
	selectionModel->setCurrentIndex(index.sibling(index.row(), 0),
                                    QItemSelectionModel::Select |
                                    QItemSelectionModel::Rows);

	// Enable selection handler again.
	enableRunnerItemSync(true);
	enableResultSync(true);
}

void RunnerWindow::scrollToHighlightedRows() const
{
	// No relevance when selections not allowed.
	if (runnerView()->selectionMode() == QAbstractItemView::NoSelection ||
        resultsView()->selectionMode() == QAbstractItemView::NoSelection)
	{
		return;
	}

	// Note: It's important not to use the current index but work with the
	// selection instead due to the fact that these indexes might not be the same.

	QModelIndex index;
	QModelIndexList indexes;

	indexes = runnerView()->selectionModel()->selectedIndexes();

	if (indexes.count() > 0)
	{
		index = indexes.first();
	}

	if (index.isValid())
	{
		runnerView()->scrollTo(index);
	}

	index = QModelIndex();
	indexes = resultsView()->selectionModel()->selectedIndexes();

	if (indexes.count() > 0)
	{
		index = indexes.first();
	}

	if (index.isValid())
	{
		resultsView()->scrollTo(index);
	}
	else
	{
		// Try to highlight a result.
		syncResultWithRunnerItem(runnerView()->selectionModel()->selection(),
                                 runnerView()->selectionModel()->selection());
	}
}

void RunnerWindow::runItems()
{
	// Do not interfere with stopping the items. Could happen because Qt
	// input processing could be faster than executing event handlers.
	if (!m_sema.tryAcquire())
	{
		return;
	}

	disableControlsBeforeRunning();
	runnerModel()->runItems();
	m_sema.release();
}

void RunnerWindow::stopItems()
{
	// Do not interfere with running the items. Could happen because Qt
	// input processing could be faster than executing event handlers.
	if (!m_sema.tryAcquire())
	{
		return;
	}

	ui.actionStop->setDisabled(true);
	QCoreApplication::processEvents();		// Disable command immediately

	// Stopping is done in a dialog which shows itself only when
	// it takes several attempts to succeed (if ever). 
	StoppingDialog dlg(this, runnerModel());

	int r = dlg.exec();

	if (r == QDialog::Accepted)
	{
		enableControlsAfterRunning();
		m_sema.release();
		return;
	}

	// Give a chance for another stop request.
	ui.actionStop->setEnabled(true);
	m_sema.release();
}

void RunnerWindow::showResults(bool show)
{
	if (!runnerModel())
	{
		return;
	}

	resultsProxyModel()->setActive(show);

	// An invisible results view means that the dock widget was closed
	// and is shown now. In this case the data is retrieved again.
	bool visible = ui.treeResults->isVisible();

	if (show && !visible)
	{
		// Show data according to current filtering.
		resultsProxyModel()->clear();
		setResultsFilter();

		// Make sure that highlighted row sync works as expected.
		ui.dockResults->show();
	}

	if (!show || runnerModel()->isRunning())
	{
		return;
	}
	// Let the dock widget get its position and size before
	// synchronizing the highlighted rows display.
	QCoreApplication::processEvents();
	scrollToHighlightedRows();
}

void RunnerWindow::showColumnsSelection()
{
	ColumnsDialog dlg(this, this);
	dlg.exec();
}

void RunnerWindow::showSettings()
{
	SettingsDialog dlg(this, this);
	dlg.exec();
}

void RunnerWindow::showAbout()
{
	AboutDialog dlg(this, runnerModel());
	dlg.exec();
}

void RunnerWindow::disableControlsBeforeRunning()
{
	enableItemActions(false);
	resultsController()->enableSorting(false);

	ui.actionStop->setEnabled(true);
	runnerView()->setCursor(QCursor(Qt::BusyCursor));
	runnerView()->setFocus();
	runnerView()->setSelectionMode(QAbstractItemView::NoSelection);
	resultsView()->setSelectionMode(QAbstractItemView::NoSelection);
	enableRunnerItemSync(false);
	enableResultSync(false);

	// Change color for highlighted rows to orange. If a similar color is
	// defined for the background then green is used. Determining a color
	// could be more sophisticated but must suffice for now.
	QPalette palette(runnerView()->palette());

	// Save current highlighting color for restoring it later.
	m_highlightBrush = palette.brush(QPalette::Active, QPalette::Highlight);

	// Create kind of orange ('pure' orange is QColor(255, 165, 0, 255)).
	QBrush orange(QColor(255, 153, 51, 255));

	QBrush newBrush;

	// Look at RGB values of background (base).
	QBrush baseBrush = palette.brush(QPalette::Active, QPalette::Base);

	bool rOk = (baseBrush.color().red() == 255) || (baseBrush.color().red() < 205);
	bool gOk = (baseBrush.color().green() > orange.color().green() + 50) ||
               (baseBrush.color().green() < orange.color().green() - 50);
	bool bOk = (baseBrush.color().blue() > orange.color().blue() + 50) ||
               (baseBrush.color().blue() < orange.color().blue() - 50);

	if (rOk && gOk && bOk)
	{
		newBrush = orange;
	}
	else
	{
		newBrush = QBrush(QColor(Qt::green));
	}

	palette.setBrush(QPalette::Active, QPalette::Highlight, newBrush);
	runnerView()->setPalette(palette);
}

void RunnerWindow::enableControlsAfterRunning() const
{
	// Wait until all items stopped.
	while (runnerModel()->isRunning(100))
	{
		// Prevent GUI from freezing.
		QCoreApplication::processEvents();
	}

	// Show results now if minimal update was active.
	if (isMinimalUpdate())
	{
		runnerModel()->emitItemResults();
	}

	// Give the GUI a chance to update.
	QCoreApplication::processEvents();

	// Enable user interaction.
	enableItemActions(true);

	ui.actionStop->setDisabled(true);
	runnerView()->setCursor(QCursor());
	runnerView()->setFocus();
	runnerView()->setSelectionMode(QAbstractItemView::SingleSelection);
	resultsView()->setSelectionMode(QAbstractItemView::SingleSelection);
	enableRunnerItemSync(true);
	enableResultSync(true);
	ensureCurrentResult();

	// Reset color for highlighted rows.
	QPalette palette(runnerView()->palette());
	palette.setBrush(QPalette::Active, QPalette::Highlight, m_highlightBrush);
	runnerView()->setPalette(palette);

	if (!isMinimalUpdate())
	{
		return;
	}

	// Scroll to the last processed item when in minimal update mode.
	// Determine the results model index first.
	int row = resultsModel()->rowCount() - 1;

	if (row < 1)
	{
		return;
	}

	QModelIndex resultIndex = resultsModel()->index(row, 0);
	QModelIndex runnerItemIndex;
	runnerItemIndex = resultsModel()->mapToRunnerItemIndex(resultIndex);
	QModelIndex currentIndex = Utils::proxyIndexFromModel(runnerProxyModel(),
                                                          runnerItemIndex);

	// Suppress synchronisation of results view.
	enableRunnerItemSync(false);

	// Highlight row of runner item.
	runnerController()->setHighlightedRow(currentIndex);

	// Enable selection handler again.
	enableRunnerItemSync(true);
}

void RunnerWindow::enableItemActions(bool enable) const
{
	ui.actionStart->setEnabled(enable);
	ui.actionStop->setEnabled(enable);
	ui.actionSelectAll->setEnabled(enable);
	ui.actionUnselectAll->setEnabled(enable);
	ui.actionExpandAll->setEnabled(enable);
	ui.actionCollapseAll->setEnabled(enable);
	ui.actionMinimalUpdate->setEnabled(enable);
	ui.actionColumns->setEnabled(enable);
	ui.actionSettings->setEnabled(enable);
}

void RunnerWindow::enableResultsFilter(bool enable) const
{
	ui.buttonInfos->setEnabled(enable);
	ui.buttonWarnings->setEnabled(enable);
	ui.buttonErrors->setEnabled(enable);
	ui.buttonFatals->setEnabled(enable);
}

void RunnerWindow::enableRunnerItemSync(bool enable) const
{
	if (enable)
	{
		connect(runnerView()->selectionModel(),
                   SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
                   SLOT(syncResultWithRunnerItem(const QItemSelection&, const QItemSelection&)));
	}
	else
	{
		disconnect(runnerView()->selectionModel(),
                   SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
				   this,
				   SLOT(syncResultWithRunnerItem(const QItemSelection&, const QItemSelection&)));
	}
}

void RunnerWindow::enableResultSync(bool enable) const
{
	if (enable)
	{
		connect(resultsView()->selectionModel(),
                   SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
                   SLOT(syncRunnerItemWithResult(const QItemSelection&, const QItemSelection&)));
	}
	else
	{
		disconnect(resultsView()->selectionModel(),
                   SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
				   this,
				   SLOT(syncRunnerItemWithResult(const QItemSelection&, const QItemSelection&)));
	}
}

void RunnerWindow::ensureCurrentResult() const
{
	// Do nothing if there is a current index or no data at all.
	if (resultsView()->currentIndex().isValid())
	{
		return;
	}

	// Try to make first visible index the current one.
	QModelIndex currentIndex = resultsView()->indexAt(QPoint(1, 1));

	if (!currentIndex.isValid())
	{
		return;
	}

	// Set current index without highlighting.
	resultsView()->selectionModel()->setCurrentIndex(currentIndex,
                                                     QItemSelectionModel::NoUpdate);
}

void RunnerWindow::adjustMenus() const
{
	bool haveChildren = false;

	// Look for an item with children.
	QModelIndex index;
	index = runnerView()->indexBelow(runnerView()->rootIndex());

	while (index.isValid())
	{
		if (index.child(0, 0).isValid())
		{
			haveChildren = true;
			break;
		}

		index = index.sibling(index.row() + 1, 0);
	}

	QList<QAction*> viewActions = ui.menuView->actions();

	for (int i = 3; i < 6; i++)
	{
		viewActions[i]->setVisible(haveChildren);
	}
}

void RunnerWindow::displayStatusNum(QLabel* labelForText,
                                    QLabel* labelForPic, int numItems) const
{
	labelForText->setText(": " + QString().setNum(numItems));

	bool visible;

	if (numItems > 0)
	{
		visible = true;
	}
	else
	{
		visible =false;
	}

	labelForText->setVisible(visible);
	labelForPic->setVisible(visible);
}

bool RunnerWindow::isMinimalUpdate() const
{
	return ui.actionMinimalUpdate->isChecked();
}

Ui::StatusWidget* RunnerWindow::statusWidget() const
{
	return &(m_statusWidget->ui);
}

RunnerViewController* RunnerWindow::runnerController() const
{
	return m_runnerViewController;
}

ResultsViewController* RunnerWindow::resultsController() const
{
	return m_resultsViewController;
}

void RunnerWindow::closeEvent(QCloseEvent* event)
{
	AppSettings settings(this);
	settings.writeSettings();

	if (!runnerModel())
	{
		return;
	}

	// Stopping is done in a dialog which shows itself only when
	// it takes several attempts to succeed (if ever). 
	StoppingDialog dlg(this, runnerModel());

	int r = dlg.exec();

	if (r == QDialog::Accepted)
	{
		return;
	}

	// Items not stoppable.
	QString msg;
	msg = tr("There are items running which can't be stopped immediately.\n"
             "Should the program exit anyway which could result in inconsistent data?");

	r = QMessageBox::warning(this, tr("QxRunner"), msg,
                             tr("&Yes"), tr("&No"), 0, 0, 1);
	if (r)
	{
		event->ignore();
		return;
	}

	// Exit the hard way.
	QApplication::quit();
	exit(1);
}

} // namespace
