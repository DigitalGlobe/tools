/*!
 * \file  runner.cpp
 *
 * \brief Implements class Runner.
 */

#include "runner.h"
#include "runnerwindow.h"

#include <QIcon>

namespace QxRunner {

Runner::Runner(RunnerModel* model) : m_model(model)
{
	m_icon = 0;
}

Runner::~Runner()
{
	delete m_icon;
}

void Runner::run()
{
	RunnerWindow runnerWindow;

	if (m_icon)
	{
		// Application specific icon.
		runnerWindow.setWindowIcon(*m_icon);
	}

	runnerWindow.setModel(m_model);
	runnerWindow.show();

	qApp->connect(qApp, SIGNAL(lastWindowClosed()), qApp, SLOT(quit()));
    qApp->exec();
}

void Runner::setWindowIcon(const QIcon& icon)
{
	m_icon = new QIcon(icon);
}

} // namespace
