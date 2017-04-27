/*!
 * \file  proxymodelcommon.cpp
 *
 * \brief Implements class ProxyModelCommon.
 */

#include "proxymodelcommon.h"

namespace QxRunner {

ProxyModelCommon::ProxyModelCommon()
{
	m_active = true;
}

ProxyModelCommon::~ProxyModelCommon()
{

}

bool ProxyModelCommon::isActive() const
{
	return m_active;
}

void ProxyModelCommon::setActive(bool active)
{
	m_active = active;
}

QBitArray ProxyModelCommon::enabledColumns() const
{
	return m_enabledColumns;
}

void ProxyModelCommon::setEnabledColumns(const QBitArray& enabledColumns)
{
	m_enabledColumns = enabledColumns;
}

bool ProxyModelCommon::isColumnEnabled(int column) const
{
	if (column >= 0 && column < m_enabledColumns.size())
	{
		return m_enabledColumns[column];
	}
	else
	{
		return false;
	}
}

} // namespace

