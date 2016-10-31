/*!
 * \file  resultsproxymodel.h
 *
 * \brief Declares class ResultsProxyModel.
 */

#ifndef RESULTSPROXYMODEL_H
#define RESULTSPROXYMODEL_H

#include "qxrunner_global.h"
#include "proxymodelcommon.h"

#include <QSortFilterProxyModel>

namespace QxRunner {

class ResultsModel;

/*!
 * \brief The ResultsProxyModel class provides support for sorting and
 *        filtering data passed between ResultsModel and a view.
 *
 * When the model is set to inactive no data is returned at all. When a
 * column of the model is disabled, no data is returned for that column.
 *
 * Row filtering is based on the result for a particular row. Rows with
 * a QxRunner::RunException result are never filtered out.
 *
 * \sa \ref views
 */

class ResultsProxyModel : public QSortFilterProxyModel,
                          public ProxyModelCommon
{
	Q_OBJECT

public: // Operations

	/*!
	 * Constructs a results proxy model with the given \a parent and
	 * \a filter.
	 */
	ResultsProxyModel(QObject* parent, int filter = QxRunner::AllResults);

	/*!
	 * Destroys this results proxy model.
	 */
    ~ResultsProxyModel();

	/*!
	 * Returns the data stored under the given \a role for the item
	 * referred to by \a index.
	 */
	QVariant data(const QModelIndex& index, int role) const;

	/*!
	 * Returns the active filter. Is a combination of OR'ed
	 * QxRunner::RunnerResult values.
	 */
	int filter() const;

	/*!
	 * Sets the \a filter for the model. Is a combination of OR'ed
	 * QxRunner::RunnerResult values. Result types defined in the
	 * filter are included in the model.
	 */
	void setFilter(int filter);

protected: // Operations

	/*!
	 * Returns true if the value in the item in the row indicated by
	 * \a source_row should be included in the model. \a source_parent
	 * is ignored.
	 */
	bool filterAcceptsRow(int source_row,
                          const QModelIndex& source_parent) const;

private: // Operations

	/*!
	 * Returns the model that contains the data that is available
	 * through the proxy model.
	 */
	ResultsModel* model() const;

	// Copy and assignment not supported.
	ResultsProxyModel(const ResultsProxyModel&);
	ResultsProxyModel& operator=(const ResultsProxyModel&);

private: // Attributes

	int m_filter;
};

} // namespace

#endif // RESULTSPROXYMODEL_H
