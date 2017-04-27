/*!
 * \file  appsettings.h
 *
 * \brief Declares class AppSettings.
 */

#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include "runnerwindowclient.h"

namespace Ui {
	class RunnerWindow;
}

namespace QxRunner {

class RunnerWindow;

/*!
 * \brief The AppSettings class manages persistent platform-independent
 *        application settings.
 *
 * This helper class is introduced to avoid bloating the main window
 * class with rudimentary code. It is a friend of RunnerWindow because
 * it not only reads or writes the settings file but also applies the
 * settings to the main window by restoring window geometry and the like
 * at program start-up. For missing or invalid settings reasonable defaults
 * are used.
 *
 * \sa \ref settings
 */

class AppSettings : public RunnerWindowClient
{
public: // Operations

	/*!
	 * Constructs an application settings object with the
	 * given \a window.
	 */
	AppSettings(RunnerWindow* window);

	/*!
	 * Destroys this application settings.
	 */
	virtual ~AppSettings();

	/*!
	 * Writes all settings into the INI file.
	 */
	void writeSettings();

	/*!
	 * Reads and applies settings which have nothing to do with
	 * window geometry or the layout of the view columns.
	 */
	void applyBaseSettings() const;

	/*!
	 * Reads and applies settings which define window geometry.
	 * Restores the main window's toolbars and dock widgets.
	 */
	void applyWindowSettings() const;

	/*!
	 * Shows the columns in the views which have been visible at last
	 * program exit and resizes them.
	 */
	void applyColumnSettings() const;

private: // Operations

	/*!
	 * Returns the pointer to the internal structure of the main window.
	 */
	Ui::RunnerWindow* windowUi() const;

	// Copy and assignment not supported.
	AppSettings(const AppSettings&);
	AppSettings& operator=(const AppSettings&);

private: // Attributes

	QString m_organization;
	QString m_application;
};

} // namespace

#endif // APPSETTINGS_H
