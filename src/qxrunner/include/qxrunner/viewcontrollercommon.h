/*!
 * \file  viewcontrollercommon.h
 *
 * \brief Declares class ViewControllerCommon.
 */

#ifndef VIEWCONTROLLERCOMMON_H
#define VIEWCONTROLLERCOMMON_H

#include <QTreeView>
#include <QHeaderView>
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>

namespace QxRunner {

/*!
 * \brief The ViewControllerCommon class defines a set of common
 *        methods for view controllers.
 *
 * This class should be seen as a 'static decorator' for the view
 * controller classes. It is not intended to be used directly, but
 * must be subclassed.
 *
 * \sa \ref views
 */

class ViewControllerCommon
{
public: // Operations

	/*!
	 * Constructs a view controller common for the given \a view. Sets
	 * view default properties.
	 */
	ViewControllerCommon(QTreeView* view);

	/*!
	 * Destroys this view controller common.
	 */
	virtual ~ViewControllerCommon();

	/*!
	 * Returns the index of column 0 of the highlighted row. The index
	 * isn't necessarily the current view index. If no row is highlighted
	 * the returned index is invalid.
	 */
	QModelIndex highlightedRow() const;
	
	/*!
	 * Highlights the row of the \a index independent of the selection
	 * mode and ensures that it is visible. The given index becomes the
	 * new current index.
	 */
	void setHighlightedRow(const QModelIndex& index) const;

protected: // Operations

	/*!
	 * Returns the view widget.
	 */
	QTreeView* view() const;

	/*!
	 * Returns the header of the view widget.
	 */
	QHeaderView* header() const;

	/*!
	 * Returns the model that contains the data that is available
	 * through the proxy model.
	 */
	QAbstractItemModel* model() const;

	/*!
	 * Returns the proxy model that the view is presenting.
	 */
	QSortFilterProxyModel* proxyModel() const;

private: // Operations

	// Copy and assignment not supported.
	ViewControllerCommon(const ViewControllerCommon&);
	ViewControllerCommon& operator=(const ViewControllerCommon&);

private: // Attributes

	QTreeView* m_view;
};

} // namespace

#endif // VIEWCONTROLLERCOMMON_H
