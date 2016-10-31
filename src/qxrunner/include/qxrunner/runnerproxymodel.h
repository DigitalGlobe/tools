/*!
 * \file  runnerproxymodel.h
 *
 * \brief Declares class RunnerProxyModel.
 */

#ifndef RUNNERROXYMODEL_H
#define RUNNERROXYMODEL_H

#include "proxymodelcommon.h"

#include <QSortFilterProxyModel>

namespace QxRunner {

/*!
 * \brief The RunnerProxyModel class provides support for sorting and
 *        filtering data passed between RunnerModel and a view.
 *       
 * When a column of the model is disabled, no data is returned for
 * that column.
 *
 * \sa \ref views
 */

class RunnerProxyModel : public QSortFilterProxyModel,
                         public ProxyModelCommon
{
	Q_OBJECT

public: // Operations

	/*!
	 * Constructs a runner proxy model with the given \a parent.
	 */
    RunnerProxyModel(QObject* parent);

	/*!
	 * Destroys this runner proxy model.
	 */
    ~RunnerProxyModel();

	/*!
	 * Returns the data stored under the given \a role for the item
	 * referred to by \a index.
	 */
	QVariant data(const QModelIndex& index, int role) const;

private: // Operations

	// Copy and assignment not supported.
	RunnerProxyModel(const RunnerProxyModel&);
	RunnerProxyModel& operator=(const RunnerProxyModel&);
};

} // namespace

#endif // RUNNERROXYMODEL_H
