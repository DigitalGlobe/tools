/*!
 * \file  runneritem.cpp
 *
 * \brief Implements class RunnerItem.
 */

#include "runneritem.h"

namespace QxRunner {

RunnerItem::RunnerItem(const QList<QVariant>& data, RunnerItem* parent)
{
    m_parentItem = parent;
    m_itemData = data;

	// Make sure this item has as many columns as the parent.
	if (m_parentItem)
	{
		int parentColumns = m_parentItem->columnCount();
		int itemColumns =  m_itemData.count();

		for (int i = itemColumns; i < parentColumns; i++)
		{
			m_itemData.append("");
		}
	}

	setSelected(true);
	setResult(QxRunner::NoResult);
}

RunnerItem::~RunnerItem()
{
    qDeleteAll(m_childItems);
}

RunnerItem* RunnerItem::parent() const
{
    return m_parentItem;
}

RunnerItem* RunnerItem::child(int row) const
{

	return m_childItems.value(row);

	// Note: QList provides sensible default values if the row
	// number is out of range.
}

void RunnerItem::appendChild(RunnerItem* item)
{
    m_childItems.append(item);
}

int RunnerItem::childCount() const
{
    return m_childItems.count();
}

int RunnerItem::row() const
{
    if (m_parentItem)
	{
        return m_parentItem->m_childItems.indexOf(const_cast<RunnerItem*>(this));
	}

	return 0;
}

int RunnerItem::columnCount() const
{
    return m_itemData.count();
}

QVariant RunnerItem::data(int column) const
{
	return m_itemData.value(column);

	// Note: QList provides sensible default values if the column
	// number is out of range.
}

void RunnerItem::setData(int column, const QVariant& value)
{
	if (column >= 0 && column < columnCount())
	{
		m_itemData.replace(column, value.toString());
	}
}

bool RunnerItem::isSelected() const
{
	return m_selected;
}

void RunnerItem::setSelected(bool select)
{
	m_selected = select;
}

int RunnerItem::result() const
{
	return m_result;
}

void RunnerItem::setResult(int result)
{
	m_result = result;
}

void RunnerItem::clear()
{
	// Initialize columns except column 0 which contains the item name.
	for (int i = 1; i < columnCount(); i++)
	{
		setData(i, "");
	}

	setResult(QxRunner::NoResult);
}

} // namespace
