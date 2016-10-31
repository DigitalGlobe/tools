/*!
 * \file  settingsdialog.h
 *
 * \brief Declares class SettingsDialog.
 */

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "ui_settingsdialog.h"
#include "runnerwindowclient.h"

namespace QxRunner {

/*!
 * \brief The SettingsDialog class allows selection of program
 *        settings.
 *
 * The settings can be applied immediately and will be stored in the
 * settings file at program exit. Currently the only choice presented
 * in the dialog is whether the views have alternating row colors or not.
 */

class SettingsDialog : public QDialog,
                       public RunnerWindowClient
{
    Q_OBJECT

public: // Operations

	/*!
	 * Constructs a settings dialog with the given \a parent
	 * and \a window.
	 */
    SettingsDialog(QWidget* parent, RunnerWindow* window);

	/*!
	 * Destroys this settings dialog.
	 */
    ~SettingsDialog();

private slots:

	/*!
	 * All settings are applied but the dialog remains open.
	 */
	void apply();

	/*!
	 * All settings are applied and the dialog gets closed.
	 */
	void accept();

	/*!
	 * The dialog gets closed without applying any settings.
	 */
	void reject();

private: // Operations

	// Copy and assignment not supported.
	SettingsDialog(const SettingsDialog&);
	SettingsDialog& operator=(const SettingsDialog&);

private: // Attributes

    Ui::SettingsDialog ui;

	bool m_alternatingRowColors;
};

} // namespace

#endif // SETTINGSDIALOG_H
