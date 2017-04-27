/*!
 * \file  viewcontrollercommon.cpp
 *
 * \brief Implements class ViewControllerCommon.
 */

#include "viewcontrollercommon.h"
#include "utils.h"

namespace QxRunner {

ViewControllerCommon::ViewControllerCommon(QTreeView* view) : m_view(view)
{
	header()->setDefaultAlignment(Qt::AlignLeft);
	header()->setSectionsMovable(false);
}

ViewControllerCommon::~ViewControllerCommon()
{

}

QModelIndex ViewControllerCommon::highlightedRow() const
{
	QItemSelectionModel* selectionModel = view()->selectionModel();
	QModelIndexList indexes = selectionModel->selectedIndexes();

	QModelIndex index;

	if (indexes.count() > 0)
	{
		index = indexes.first();
	}

	return index;
}

void ViewControllerCommon::setHighlightedRow(const QModelIndex& index) const
{
	// Row gets highlighted anyway, independent of current selection mode.
	view()->clearSelection();

	QItemSelectionModel* selectionModel = view()->selectionModel();
	selectionModel->setCurrentIndex(index, QItemSelectionModel::Select |
                                           QItemSelectionModel::Rows);

	// This will expand any parents.
	view()->scrollTo(index);
}

QTreeView* ViewControllerCommon::view() const
{
	return m_view;
}

QHeaderView* ViewControllerCommon::header() const
{
	return view()->header();
}

QAbstractItemModel* ViewControllerCommon::model() const
{
	return static_cast<QAbstractItemModel*>(Utils::modelFromProxy(view()->model()));
}

QSortFilterProxyModel* ViewControllerCommon::proxyModel() const
{
	return static_cast<QSortFilterProxyModel*>(view()->model());
}

} // namespace

