/*!
 * \file  resultsmodel.cpp
 *
 * \brief Implements class ResultsModel.
 */

#include "resultsmodel.h"
#include "runneritem.h"
#include "utils.h"
#include "qxrunner_global.h"

namespace QxRunner {

ResultsModel::ResultsModel(const QStringList& headerData, QObject* parent)
            :  QAbstractListModel(parent), m_headerData(headerData)
{

}

ResultsModel::~ResultsModel()
{

}

QVariant ResultsModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
	{
        return QVariant();
	}

	if (role == Qt::CheckStateRole)
	{
		// Results have no items with checked state.
		return QVariant();
	}

	if (role == Qt::TextAlignmentRole)
	{
		return int(Qt::AlignLeft | Qt::AlignTop);
	}

	RunnerItem* item = itemFromIndex(m_runnerItemIndexes.at(index.row()));

	if (role == Qt::DisplayRole)
	{
		 return item->data(index.column());
	}

	if (role != Qt::DecorationRole || index.column() != 0)
	{
		// First column only has a decoration.
		return QVariant();
	}

	// Icon corresponding to the item's result code.
	return Utils::resultIcon(item->result());
}

QVariant ResultsModel::headerData(int section, Qt::Orientation orientation,
                                  int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
        return m_headerData.at(section);
	}

	return QVariant();
}

int ResultsModel::rowCount(const QModelIndex& parent) const
{
	return m_runnerItemIndexes.count();
}

int ResultsModel::columnCount(const QModelIndex& parent) const
{
	return m_headerData.count();
}

int ResultsModel::result(int row) const
{
	QModelIndex runnerItemIndex = mapToRunnerItemIndex(index(row, 0));

	if (!runnerItemIndex.isValid())
	{
		return QxRunner::NoResult;
	}

	RunnerItem* item = itemFromIndex(runnerItemIndex);

	return item->result();
}

QModelIndex ResultsModel::mapToRunnerItemIndex(const QModelIndex& index) const
{
	if (!index.isValid())
	{
		return QModelIndex();
	}

	return m_runnerItemIndexes.value(index.row());

	// Note: QList provides sensible default values if the row
	// number is out of range.
}

QModelIndex ResultsModel::mapFromRunnerItemIndex(const QModelIndex& runnerItemIndex) const
{
	if (!runnerItemIndex.isValid())
	{
		return QModelIndex();
	}

	QModelIndex modelIndex;

	qint64 id = runnerItemIndex.internalId();

	if (m_runnerItemMap.contains(id))
	{
		modelIndex = index(m_runnerItemMap[id], 0);
	}

	return modelIndex;
}

void ResultsModel::addResult(const QModelIndex& runnerItemIndex)
{
	if (!runnerItemIndex.isValid())
	{
		return;
	}

	beginInsertRows(QModelIndex(), rowCount(), rowCount() + 1);

	m_runnerItemIndexes.append(QPersistentModelIndex(runnerItemIndex));
	m_runnerItemMap[runnerItemIndex.internalId()] = rowCount() - 1;

	endInsertRows();
}

void ResultsModel::clear()
{
	m_runnerItemIndexes.clear();
	m_runnerItemMap.clear();
    //reset();
    endResetModel();
}

RunnerItem* ResultsModel::itemFromIndex(const QModelIndex& runnerItemIndex) const
{
	return static_cast<RunnerItem*>(runnerItemIndex.internalPointer());
}

} // namespace
