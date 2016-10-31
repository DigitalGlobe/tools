/*!
 * \file  resultsproxymodel.cpp
 *
 * \brief Implements class ResultsProxyModel.
 */

#include "resultsproxymodel.h"
#include "resultsmodel.h"

namespace QxRunner {

ResultsProxyModel::ResultsProxyModel(QObject* parent,  int filter)
                 : QSortFilterProxyModel(parent), m_filter(filter)
{

}

ResultsProxyModel::~ResultsProxyModel()
{

}

QVariant ResultsProxyModel::data(const QModelIndex& index, int role) const
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

int ResultsProxyModel::filter() const
{
	return m_filter;
}

void ResultsProxyModel::setFilter(int filter)
{
	if (m_filter != filter)
	{
		// Update only when not same filter.
		m_filter = filter;
		reset();
	}
}

bool ResultsProxyModel::filterAcceptsRow(int source_row,
                                         const QModelIndex& source_parent) const
{
	// No data when proxy model is inactive.
	if (!isActive())
	{
		return false;
	}

	int result = model()->result(source_row);

	if (result & m_filter || result == QxRunner::RunException)
	{
		return true;
	}
	else
	{
		return false;
	}
}

ResultsModel* ResultsProxyModel::model() const
{
	return static_cast<ResultsModel*>(sourceModel());
}

} // namespace

