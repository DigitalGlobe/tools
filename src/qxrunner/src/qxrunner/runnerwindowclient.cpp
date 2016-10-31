/*!
 * \file  runnerwindowclient.cpp
 *
 * \brief Implements class RunnerWindowClient.
 */

#include "runnerwindowclient.h"
#include "runnerwindow.h"

namespace QxRunner {

RunnerWindowClient::RunnerWindowClient(RunnerWindow* runnerWindow)
                  : m_window(runnerWindow)
{

}

RunnerWindowClient::~RunnerWindowClient()
{

}

RunnerWindow* RunnerWindowClient::window() const
{
	return m_window;
}

QTreeView* RunnerWindowClient::runnerView() const
{
	return window()->runnerView();
}

QTreeView* RunnerWindowClient::resultsView() const
{
	return window()->resultsView();
}

RunnerModel* RunnerWindowClient::runnerModel() const
{
	return window()->runnerModel();
}

RunnerProxyModel* RunnerWindowClient::runnerProxyModel() const
{
	return window()->runnerProxyModel();
}

ResultsModel* RunnerWindowClient::resultsModel() const
{
	return window()->resultsModel();
}

ResultsProxyModel* RunnerWindowClient::resultsProxyModel() const
{
	return window()->resultsProxyModel();
}

bool RunnerWindowClient::isResultsViewVisible() const
{
	return window()->isResultsViewVisible();
}

} // namespace

