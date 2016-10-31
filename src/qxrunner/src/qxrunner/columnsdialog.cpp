/*!
 * \file  columnsdialog.cpp
 *
 * \brief Implements class ColumnsDialog.
 */

#include "columnsdialog.h"
#include "runnerwindow.h"
#include "runnermodel.h"
#include "runnerproxymodel.h"
#include "resultsproxymodel.h"
#include "utils.h"

#include <QHeaderView>
#include <QKeyEvent>

namespace QxRunner {

ColumnsDialog::ColumnsDialog(QWidget* parent, RunnerWindow* window)
             : QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
               RunnerWindowClient(window)
{
	ui.setupUi(this);

	connect(ui.buttonOk,     SIGNAL(clicked()), SLOT(accept()));
	connect(ui.buttonApply,  SIGNAL(clicked()), SLOT(apply()));
	connect(ui.buttonCancel, SIGNAL(clicked()), SLOT(reject()));

	// Adjust GUI.
	ui.treeColumns->clear();
	ui.treeColumns->headerItem()->setTextAlignment(0, Qt::AlignLeft);

	// Get the relevant column count.
	int columnCount = runnerModel()->columnCount();

	if (columnCount < 1)	// Should not happen
	{
		return;
	}

	// Collect attributes of views in case "Cancel" is pressed.
	m_prevRunnerColumns = runnerProxyModel()->enabledColumns();
	m_prevRunnerColumnSizes = Utils::columnSizes(runnerView());
	m_prevResultsColumns = resultsProxyModel()->enabledColumns();
	m_prevResultsColumnSizes = Utils::columnSizes(resultsView());

	// Show the columns in a branch for each view.
	QTreeWidgetItem* item;
	QStringList itemText;

	itemText << "Runner View";
	m_runnerTopItem = new QTreeWidgetItem(ui.treeColumns, itemText);

	itemText.clear();
	itemText << "Results View";
	m_resultsTopItem = new QTreeWidgetItem(ui.treeColumns, itemText);

	// Column 0 is always visible in both views, therefore it's entry is
	// shown disabled.
	itemText.clear();
	itemText << runnerModel()->headerData(0, Qt::Horizontal).toString();

	item = new QTreeWidgetItem(m_runnerTopItem, itemText);
	item->setCheckState(0, Qt::Checked);
	setItemDisabled(item);

	item = new QTreeWidgetItem(m_resultsTopItem, itemText);
	item->setCheckState(0, Qt::Checked);
	setItemDisabled(item);

	// Add one row for each other column in the model.
	for (int i = 1; i < columnCount; i++)
	{
		itemText.clear();
		itemText << runnerModel()->headerData(i, Qt::Horizontal).toString();

		item = new QTreeWidgetItem(m_runnerTopItem, itemText);

		if (runnerProxyModel()->isColumnEnabled(i))
		{
			item->setCheckState(0, Qt::Checked);
		}
		else
		{
			item->setCheckState(0, Qt::Unchecked);
		}

		item = new QTreeWidgetItem(m_resultsTopItem, itemText);

		if (resultsProxyModel()->isColumnEnabled(i))
		{
			item->setCheckState(0, Qt::Checked);
		}
		else
		{
			item->setCheckState(0, Qt::Unchecked);
		}

		if (!isResultsViewVisible())
		{
			setItemDisabled(item);
		}
	}

	ui.treeColumns->setItemExpanded(m_runnerTopItem, true);
	ui.treeColumns->setItemExpanded(m_resultsTopItem, true);

	// Intercept tree widget events for key press handling.
	ui.treeColumns->installEventFilter(static_cast<ColumnsDialog*>(this));

	adjustSize();
}

ColumnsDialog::~ColumnsDialog()
{

}

void ColumnsDialog::apply()
{
	bool prevState;
	bool newState;
	bool runnerViewChanged = false;
	bool resultsViewChanged = false;

	QTreeWidgetItem* item;

	// Get the relevant column count.
	int columnCount = runnerModel()->columnCount();

	// Apply user selection in case of visibility change.
	for (int i = 0; i < columnCount; i++)
	{
		item = m_runnerTopItem->child(i);

		prevState = runnerProxyModel()->isColumnEnabled(i);
		newState = (item->checkState(0) == Qt::Checked);

		if (prevState != newState)
		{
			runnerView()->setColumnHidden(i, !newState);
			runnerViewChanged = true;
		}

		item = m_resultsTopItem->child(i);

		prevState = resultsProxyModel()->isColumnEnabled(i);
		newState = (item->checkState(0) == Qt::Checked);

		if (prevState != newState)
		{
			resultsView()->setColumnHidden(i, !newState);
			resultsViewChanged = true;
		}
	}

	// If a view has changed then first stretch the columns over the view
	// and then resize the first column if there is data in the view.
	if (runnerViewChanged)
	{
        runnerView()->header()->setSectionResizeMode(QHeaderView::Stretch);
        runnerView()->header()->setSectionResizeMode(QHeaderView::Interactive);

		if (runnerView()->indexBelow(runnerView()->rootIndex()).isValid())
		{
			runnerView()->resizeColumnToContents(0);
		}
	}

	if (resultsViewChanged)
	{
        resultsView()->header()->setSectionResizeMode(QHeaderView::Stretch);
        resultsView()->header()->setSectionResizeMode(QHeaderView::Interactive);

		if (resultsView()->indexBelow(resultsView()->rootIndex()).isValid())
		{
			resultsView()->resizeColumnToContents(0);
		}
	}

	// Let the proxy models know about the newly visible (enabled) columns.
	QBitArray enabledRunnerColumns(columnCount);
	QBitArray enabledResultsColumns(columnCount);

	for (int i = 0; i < columnCount; i++)
	{
		enabledRunnerColumns[i] = !runnerView()->isColumnHidden(i);
		enabledResultsColumns[i] = !resultsView()->isColumnHidden(i);
	}

	runnerProxyModel()->setEnabledColumns(enabledRunnerColumns);
	resultsProxyModel()->setEnabledColumns(enabledResultsColumns);
}

void ColumnsDialog::accept()
{
	// Update the GUI (again).
	apply();

	QDialog::accept();
}

void ColumnsDialog::reject()
{
	// When there is only one column which is also stretched then reestablishing
	// the column width would not work correctly. By disabling stretching of the
	// last column before resizing the columns it works as expected.
	bool runnerViewStretched = runnerView()->header()->stretchLastSection();
	bool resultsViewStretched = resultsView()->header()->stretchLastSection();

	runnerView()->header()->setStretchLastSection(false);
	resultsView()->header()->setStretchLastSection(false);

	// Establish columns as they have been before.
	for (int i = 0; i < runnerModel()->columnCount(); i++)
	{
		runnerView()->setColumnHidden(i, !m_prevRunnerColumns[i]);
		resultsView()->setColumnHidden(i, !m_prevResultsColumns[i]);
		runnerView()->header()->resizeSection(i, m_prevRunnerColumnSizes[i]);
		resultsView()->header()->resizeSection(i, m_prevResultsColumnSizes[i]);
	}

	runnerView()->header()->setStretchLastSection(runnerViewStretched);
	resultsView()->header()->setStretchLastSection(resultsViewStretched);

	// Also reset the proxy models.
	runnerProxyModel()->setEnabledColumns(m_prevRunnerColumns);
	resultsProxyModel()->setEnabledColumns(m_prevResultsColumns);

	QDialog::reject();
}

void ColumnsDialog::setItemDisabled(QTreeWidgetItem* item) const
{
	QColor textColor;
	textColor = QApplication::palette().color(QPalette::Disabled, QPalette::WindowText);

	item->setFlags(Qt::ItemIsEnabled);
	item->setTextColor(0, textColor);
}

bool ColumnsDialog::eventFilter(QObject* obj, QEvent* event)
{
	if (event->type() != QEvent::KeyPress)
	{
		// Pass the event on to the parent class.
		return QObject::eventFilter(obj, event);
	}

	QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
	int key = keyEvent->key();
	
	// Support +/- for expanding/collapsing branches.
	if (key == Qt::Key_Minus || key == Qt::Key_Plus)
	{
		if (key == Qt::Key_Minus)
		{
			key = Qt::Key_Left;
		}
		else
		{
			key = Qt::Key_Right;
		}

		// Create and send key to the tree view.
		QKeyEvent* newKeyEvent = new QKeyEvent(QEvent::KeyPress, key,
                                               keyEvent->modifiers());
		QCoreApplication::postEvent(ui.treeColumns, newKeyEvent);

		return true;
	}

	// Only space bar gets handled further.
	if (key != Qt::Key_Space)
	{
		return QObject::eventFilter(obj, event);
	}

	QTreeWidgetItem* item = ui.treeColumns->currentItem();

	if (!item)
	{
		return QObject::eventFilter(obj, event);
	}

	// Must be an item with a checkbox.
	if (item->childCount() > 0)
	{
		return QObject::eventFilter(obj, event);
	}

	// Disabled items aren't selectable.
	if (!ui.treeColumns->isItemSelected(item))
	{
		return QObject::eventFilter(obj, event);
	}

	if (item->checkState(0) == Qt::Checked)
	{
		item->setCheckState(0, Qt::Unchecked);
	}
	else
	{
		item->setCheckState(0, Qt::Checked);
	}

	return true;
}

} // namespace
