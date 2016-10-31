/*!
 * \file  runnermodelthread.cpp
 *
 * \brief Implements class RunnerModelThread.
 */

#include "runnermodelthread.h"
#include "runnermodel.h"

namespace QxRunner {

RunnerModelThread::RunnerModelThread(RunnerModel* parent)
                 : QThread(parent)
{

}

RunnerModelThread::~RunnerModelThread()
{

}

void RunnerModelThread::msleep(unsigned long msecs) const
{
	QThread::msleep(msecs);
}

void RunnerModelThread::run()
{
	setTerminationEnabled();

	RunnerModel* model = static_cast<RunnerModel*>(parent());

	if (model)
	{
		model->threadCode();
	}
}

} // namespace
