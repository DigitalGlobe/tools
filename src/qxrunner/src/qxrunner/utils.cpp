/*!
 * \file  utils.cpp
 *
 * \brief Implements class Utils.
 */

#include "utils.h"
#include "runneritem.h"

#include <QTreeView>
#include <QHeaderView>
#include <QIcon>
#include <QSortFilterProxyModel>

namespace QxRunner {

QList<int> Utils::columnSizes(QTreeView* view)
{
	QList<int> sizes;

	for (int i = 0; i < view->header()->count(); i++)
	{
		sizes.append(view->columnWidth(i));
	}

	return sizes;
}

QVariant Utils::resultIcon(int result)
{
	switch (result)
	{
		case QxRunner::RunSuccess:
			return QIcon(":/icons/success.png");
			break;

		case QxRunner::RunInfo:
			return QIcon(":/icons/info.png");
			break;

		case QxRunner::RunWarning:
			return QIcon(":/icons/warning.png");
			break;

		case QxRunner::RunError:
			return QIcon(":/icons/error.png");
			break;

		case QxRunner::RunFatal:
			return QIcon(":/icons/fatal.png");
			break;

		case QxRunner::RunException:
			return QIcon(":/icons/exception.png");
			break;

		default:
			return QIcon(":/icons/item.png");
			break;
	}
}

QAbstractItemModel* Utils::modelFromProxy(QAbstractItemModel* model)
{
	QSortFilterProxyModel* proxyModel;
	proxyModel = static_cast<QSortFilterProxyModel*>(model);

	if (proxyModel)
	{
		return proxyModel->sourceModel();
	}
	else
	{
		return 0;
	}
}

QModelIndex Utils::modelIndexFromProxy(QAbstractItemModel* model,
                                       const QModelIndex& index)
{
	if (!index.isValid())
	{
		return QModelIndex();
	}

	QSortFilterProxyModel* proxyModel;
	proxyModel = static_cast<QSortFilterProxyModel*>(model);

	return proxyModel->mapToSource(index);
}

QModelIndex Utils::proxyIndexFromModel(QAbstractItemModel* model,
                                       const QModelIndex& index)
{
	if (!index.isValid())
	{
		return QModelIndex();
	}

	QSortFilterProxyModel* proxyModel;
	proxyModel = static_cast<QSortFilterProxyModel*>(model);
	
	return proxyModel->mapFromSource(index);
}

} // namespace
