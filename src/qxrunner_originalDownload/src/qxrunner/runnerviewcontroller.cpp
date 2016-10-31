/*!
 * \file  runnerviewcontroller.cpp
 *
 * \brief Implements class RunnerViewController.
 */

#include "runnerviewcontroller.h"
#include "runnermodel.h"
#include "runnerproxymodel.h"
#include "utils.h"

#include <QCoreApplication>
#include <QKeyEvent>

namespace QxRunner {

RunnerViewController::RunnerViewController(QObject* parent, QTreeView* view)
                    : QObject(parent), ViewControllerCommon(view)
{
	// Intercept tree view events for key press handling.
	view->installEventFilter(static_cast<RunnerViewController*>(this));
}

RunnerViewController::~RunnerViewController()
{

}

RunnerModel* RunnerViewController::runnerModel() const
{
	return static_cast<RunnerModel*>(model());
}

RunnerProxyModel* RunnerViewController::runnerProxyModel() const
{
	return static_cast<RunnerProxyModel*>(proxyModel());
}

void RunnerViewController::selectAll() const
{
	selectAllItems(true);
}

void RunnerViewController::unselectAll() const
{
	selectAllItems(false);
}

void RunnerViewController::expandAll() const
{
	// Recursively expand all branches.
	QModelIndex currentIndex;
	currentIndex = view()->indexBelow(view()->rootIndex());
	expand(currentIndex);

	// Make highlighted row visible.
	currentIndex = highlightedRow();

	if (currentIndex.isValid())
	{
		// This will expand any parents.
		view()->scrollTo(currentIndex);
	}
}

void RunnerViewController::collapseAll() const
{
	// Recursively collapse all branches.
	QModelIndex currentIndex;
	currentIndex = view()->indexBelow(view()->rootIndex());
	collapse(currentIndex);

	// Highlight and show top level parent of current branch.
	currentIndex = highlightedRow();

	if (!currentIndex.isValid())
	{
		return;		// No selection at all
	}

	QModelIndex parentIndex = currentIndex.parent();

	// Search top level parent.
	while (parentIndex.isValid())
	{
		currentIndex = parentIndex;
		parentIndex = parentIndex.parent();
	}

	setHighlightedRow(currentIndex);
}

void RunnerViewController::expand(const QModelIndex& index) const
{
	if (!index.isValid())
	{
		return;
	}

	QModelIndex currentIndex = index;

	// Recursively expand branches.
	while (currentIndex.isValid())
	{
		if (currentIndex.child(0, 0).isValid())
		{
			// First proceed the levels down.
			expand(currentIndex.child(0, 0));

			// Expand this level.
			if (!view()->isExpanded(currentIndex))
			{
				view()->expand(currentIndex);
			}
		}

		currentIndex = currentIndex.sibling(currentIndex.row() + 1, 0);
	}
}

void RunnerViewController::collapse(const QModelIndex& index) const
{
	if (!index.isValid())
	{
		return;
	}

	QModelIndex currentIndex = index;

	// Recursively collapse branches.
	while (currentIndex.isValid())
	{
		if (currentIndex.child(0, 0).isValid())
		{
			// First collapse this level.
			if (view()->isExpanded(currentIndex))
			{
				view()->collapse(currentIndex);
			}

			// Proceed one level down.
			collapse(currentIndex.child(0, 0));
		}

		currentIndex = currentIndex.sibling(currentIndex.row() + 1, 0);
	}
}

void RunnerViewController::selectAllItems(bool select) const
{
	QModelIndex runnerItemIndex;

	QModelIndex currentIndex = view()->indexBelow(view()->rootIndex());

	while (currentIndex.isValid())
	{
		runnerItemIndex = Utils::modelIndexFromProxy(proxyModel(), currentIndex);
		runnerModel()->setItemChecked(runnerItemIndex, select);

		currentIndex = currentIndex.sibling(currentIndex.row() + 1, 0);
	}

	// Update counters.
	runnerModel()->countItems();
}

bool RunnerViewController::eventFilter(QObject* obj, QEvent* event)
{
	if (event->type() != QEvent::KeyPress)
	{
		// Pass the event on to the parent class.
		return QObject::eventFilter(obj, event);
	}

	QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
	int key = keyEvent->key();

	// Tree view relevant keys make the currently highlighted row visible.
	if (key == Qt::Key_Minus || key == Qt::Key_Plus  ||
        key == Qt::Key_Left  || key == Qt::Key_Right ||
        key == Qt::Key_Up    || key == Qt::Key_Down  ||
        key == Qt::Key_Space)
	{
		if (highlightedRow().isValid())
		{
			view()->scrollTo(highlightedRow());
		}
	}

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
		QCoreApplication::postEvent(view(), newKeyEvent);

		return true;
	}

	if (key != Qt::Key_Space)
	{
		return QObject::eventFilter(obj, event);
	}

	// Mimick a click of the user.
	QModelIndex runnerItemIndex;
	runnerItemIndex = Utils::modelIndexFromProxy(proxyModel(), highlightedRow());
	bool checked = runnerModel()->data(runnerItemIndex, Qt::CheckStateRole).toBool();
	runnerModel()->setItemChecked(runnerItemIndex, !checked);

	return true;
}

} // namespace
