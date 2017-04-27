/*!
 * \file  runner.h
 *
 * \brief Declares class Runner.
 */

#ifndef RUNNER_H
#define RUNNER_H

#include "qxrunner_global.h"

class QIcon;

namespace QxRunner {

class RunnerModel;

/*!
 * \brief The Runner class starts a QxRunner application.
 *
 * This class creates the main window and shows it on screen. When
 * the main window closes QApplication also quits.
 *
 * The runner can be given an application icon, typically displayed in
 * the top-left corner of windows. This icon overrides the QxRunner
 * default icon. Not to be confused with the icon of the executable
 * application file itself as presented on the desktop. To change this
 * it is necessary to employ a platform-dependent technique as
 * described in the Qt documentation.
 */

class QXRUNNER_EXPORT Runner
{
public: // Operations

	/*!
	 * Constructs a runner with the given \a model.
	 */
	Runner(RunnerModel* model);

	/*!
	 * Destroys this runner.
	 *
	 * \note
	 * Deleting the model provided at construction time is left to the
	 * owner of the model instance.
	 */
	virtual ~Runner();

	/*!
	 * Shows the main window.
	 */
	void run();

	/*!
	 * Sets the application \a icon.
	 */
	void setWindowIcon(const QIcon& icon);

private: // Operations

	// Copy and assignment not supported.
	Runner(const Runner&);
	Runner& operator=(const Runner&);

private: // Attributes

	RunnerModel* m_model;

	QIcon* m_icon;
};

} // namespace

#endif // RUNNER_H
