/*!
 * \file  resultsviewcontroller.h
 *
 * \brief Declares class ResultsViewController.
 */

#ifndef RESULTSVIEWCONTROLLER_H
#define RESULTSVIEWCONTROLLER_H

#include "viewcontrollercommon.h"

#include <QObject>

namespace QxRunner {

class ResultsModel;
class ResultsProxyModel;

/*!
 * \brief The ResultsViewController class provides functionality for
 *        the results view.
 *
 * This helper class is introduced to avoid bloating the main window
 * class with code specific for views representing data from ResultsModel.
 *
 * Sorting of data isn't available as long as there is no data to
 * represent. When the model contains data then clicking on a column
 * header sorts the data according to that column.
 *
 * \sa \ref views
 */

class ResultsViewController : public QObject,
                              public ViewControllerCommon
{
	Q_OBJECT

public: // Operations

	/*!
	 * Constructs a results view controller with the given \a parent
	 * and \a view.
	 */
    ResultsViewController(QObject* parent, QTreeView* view);

	/*!
	 * Destroys this results view controller.
	 */
    ~ResultsViewController();

	/*!
	 * Enables sorting if \a enable is true, otherwise disables
	 * sorting.
	 */
	void enableSorting(bool enable) const;

	/*!
	 * Returns the model that contains the data that is available
	 * through the proxy model.
	 */
	ResultsModel* resultsModel() const;

	/*!
	 * Returns the proxy model that the view is presenting.
	 */
	ResultsProxyModel* resultsProxyModel() const;

private slots:

	/*!
	 * Enables sortign when the model has data, otherwise disables
	 * sorting.
	 */
	void setupSorting() const;

private: // Operations

	// Copy and assignment not supported.
	ResultsViewController(const ResultsViewController&);
	ResultsViewController& operator=(const ResultsViewController&);
};

} // namespace

#endif // RESULTSVIEWCONTROLLER_H
