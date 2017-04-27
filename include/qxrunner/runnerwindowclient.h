/*!
 * \file  runnerwindowclient.h
 *
 * \brief Declares class RunnerWindowClient.
 */

#ifndef RUNNERWINDOWCLIENT_H
#define RUNNERWINDOWCLIENT_H

#include <QTreeView>

namespace QxRunner {

class RunnerWindow;
class RunnerModel;
class RunnerProxyModel;
class ResultsModel;
class ResultsProxyModel;

/*!
 * \brief The RunnerWindowClient class simplifies access to RunnerWindow.
 *
 * The RunnerWindowClient class defines a set of methods for accessing
 * objects and attributes exposed by the main window. Most of the
 * methods delegate requests to RunnerWindow. This class is intended
 * to be subclassed or used directly.
 */

class RunnerWindowClient
{
public: // Operations

	/*!
	 * Constructs a runner window client for the given \a window.
	 */
	RunnerWindowClient(RunnerWindow* window);

	/*!
	 * Destroys this runner window client.
	 */
	virtual ~RunnerWindowClient();

	/*!
	 * Returns the main window.
	 */
	RunnerWindow* window() const;

	/*!
	 * Returns the runner view from the main window.
	 */
	QTreeView* runnerView() const;

	/*!
	 * Returns the results view from the main window.
	 */
	QTreeView* resultsView() const;

	/*!
	 * Returns the runner model from the main window.
	 */
	RunnerModel* runnerModel() const;

	/*!
	 * Returns the runner proxy model from the main window.
	 */
	RunnerProxyModel* runnerProxyModel() const;

	/*!
	 * Returns the results model from the main window.
	 */
	ResultsModel* resultsModel() const;

	/*!
	 * Returns the results proxy model from the main window.
	 */
	ResultsProxyModel* resultsProxyModel() const;

	/*!
	 * Returns true if the results view in the main window is
	 * visible, otherwise false.
	 */
	bool isResultsViewVisible() const;

private: // Operations

	// Copy and assignment not supported.
	RunnerWindowClient(const RunnerWindowClient&);
	RunnerWindowClient& operator=(const RunnerWindowClient&);

private: // Attributes

	RunnerWindow* m_window;
};

} // namespace

#endif // RUNNERWINDOWCLIENT_H
