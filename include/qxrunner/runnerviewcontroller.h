/*!
 * \file  runnerviewcontroller.h
 *
 * \brief Declares class RunnerViewController.
 */

#ifndef RUNNERVIEWCONTROLLER_H
#define RUNNERVIEWCONTROLLER_H

#include "viewcontrollercommon.h"

#include <QObject>

namespace QxRunner {

class RunnerModel;
class RunnerProxyModel;

/*!
 * \brief The RunnerViewController class provides functionality for
 *        the runner view.
 *
 * This helper class is introduced to avoid bloating the main window
 * class with code specific for views representing data from RunnerModel.
 * 
 * Most methods of the class facilitate user interaction with the view.
 * Sorting of data by clicking on a column header isn't supported.
 *
 * \sa \ref views and \ref selected_item
 */

class RunnerViewController : public QObject,
                             public ViewControllerCommon
{
	Q_OBJECT

public: // Operations

	/*!
	 * Constructs a runner view controller with the given \a parent
	 * and \a view.
	 */
    RunnerViewController(QObject* parent, QTreeView* view);

	/*!
	 * Destroys this runner view controller.
	 */
    ~RunnerViewController();

	/*!
	 * Returns the model that contains the data that is available
	 * through the proxy model.
	 */
	RunnerModel* runnerModel() const;

	/*!
	 * Returns the proxy model that the view is presenting.
	 */
	RunnerProxyModel* runnerProxyModel() const;

public slots:

	/*!
	 * Selects all items in the view.
	 */
	void selectAll() const;

	/*!
	 * Unselects all items in the view.
	 */
	void unselectAll() const;

	/*!
	 * Expands all branches in the view.
	 */
	void expandAll() const;

	/*!
	 * Collapses all branches in the view.
	 */
	void collapseAll() const;

private: // Operations

	/*!
	 * Helper method to recursively expand all branches. Starts with
	 * the item referred to by \a index.
	 */
	void expand(const QModelIndex& index) const;

	/*!
	 * Helper method to recursively collapse all branches. Starts with
	 * the item referred to by \a index.
	 */
	void collapse(const QModelIndex& index) const;

	/*!
	 * If \a select is true then all items in the view selected,
	 * otherwise unselected.
	 */
	void selectAllItems(bool select) const;

	/*!
	 * Handles key press events for the view. A space mimicks a mouse
	 * click. +/- expand/collapse branches.
	 */
	bool eventFilter(QObject* obj, QEvent* event);

	// Copy and assignment not supported.
	RunnerViewController(const RunnerViewController&);
	RunnerViewController& operator=(const RunnerViewController&);
};

} // namespace

#endif // RUNNERVIEWCONTROLLER_H
