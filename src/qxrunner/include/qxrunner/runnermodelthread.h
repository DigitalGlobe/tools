/*!
 * \file  runnermodelthread.h
 *
 * \brief Declares class RunnerModelThread.
 */

#ifndef RUNNERMODELTHREAD_H
#define RUNNERMODELTHREAD_H

#include <QThread>

namespace QxRunner {

class RunnerModel;

/*!
 * \brief The RunnerModelThread class enables asynchronous execution
 *        of runner items.
 *
 * The RunnerModelThread class enables RunnerModel to asynchronously
 * execute RunnerItem objects. When the thread is started it calls the
 * private \c threadCode() method of RunnerModel where the 'real'
 * threaded code is executed. Therefore RunnerModelThread is a friend
 * of RunnerModel.
 */

class RunnerModelThread : public QThread
{
	Q_OBJECT

public: // Operations

	/*!
	 * Constructs a runner model thread with the given \a parent.
	 */
    RunnerModelThread(RunnerModel* parent);

	/*!
	 * Destroys this runner model thread.
	 */
    ~RunnerModelThread();

	/*!
	 * Causes the current thread to sleep for \a msecs milliseconds.
	 */
	void msleep(unsigned long msecs) const;

private: // Operations

	/*!
	 * Reimplemented from QThread. Starts the thread.
	 */
	void run();

	// Copy and assignment not supported.
	RunnerModelThread(const RunnerModelThread&);
	RunnerModelThread& operator=(const RunnerModelThread&);
};

} // namespace

#endif // RUNNERMODELTHREAD_H
