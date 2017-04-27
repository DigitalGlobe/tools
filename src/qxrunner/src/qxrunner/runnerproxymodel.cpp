/*!
 * \file  runnerproxymodel.cpp
 *
 * \brief Implements class RunnerProxyModel.
 */

#include "runnerproxymodel.h"

namespace QxRunner {

RunnerProxyModel::RunnerProxyModel(QObject* parent)
                : QSortFilterProxyModel(parent)
{

}

RunnerProxyModel::~RunnerProxyModel()
{

}

QVariant RunnerProxyModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
	{
		return QVariant();
	}

	if (isColumnEnabled(index.column()))
	{
		return QSortFilterProxyModel::data(index, role);
	}

	return QVariant();
}

} // namespace
