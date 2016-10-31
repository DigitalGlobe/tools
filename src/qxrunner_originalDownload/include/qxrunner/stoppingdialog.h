/*!
 * \file  stoppingdialog.h
 *
 * \brief Declares class StoppingDialog.
 */

#ifndef STOPPINGDIALOG_H
#define STOPPINGDIALOG_H

#include "ui_stoppingdialog.h"

namespace QxRunner {

class RunnerModel;

/*!
 * \brief The StoppingDialog class tries to stop item execution.
 *       
 * The stopping dialog instructs RunnerModel to stop item execution
 * without the dialog being visible on the screen. If not successful
 * at first attempt the dialog is shown to the user to give a visual
 * hint as well as a chance to interrupt the stopping.
 */

class StoppingDialog : public QDialog
{
    Q_OBJECT

public: // Operations

	/*!
	 * Constructs a stopping dialog with the given \a parent and
	 * \a model.
	 */
    StoppingDialog(QWidget* parent, RunnerModel* model);

	/*!
	 * Destroys this stopping dialog.
	 */
    ~StoppingDialog();

	/*!
	 * Reimplemented from QDialog. Returns \c QDialog::Accepted when
	 * stopping was successful, otherwise \c QDialog::Rejected.
	 */
	int exec();

private slots:

	/*!
	 * Reimplemented from QDialog. Ensures that the dialog gets
	 * closed properly.
	 */
	void reject();

private: // Operations

	// Copy and assignment not supported.
	StoppingDialog(const StoppingDialog&);
	StoppingDialog& operator=(const StoppingDialog&);

private: // Attributes

    Ui::StoppingDialog ui;

	RunnerModel* m_model;

	bool m_shouldClose;
};

} // namespace

#endif // STOPPINGDIALOG_H
