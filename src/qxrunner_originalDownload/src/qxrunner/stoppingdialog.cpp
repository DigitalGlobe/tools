/*!
 * \file  stoppingdialog.cpp
 *
 * \brief Implements class StoppingDialog.
 */

#include "stoppingdialog.h"
#include "runnermodel.h"

namespace QxRunner {

StoppingDialog::StoppingDialog(QWidget* parent, RunnerModel* model)
              : QDialog(parent, Qt::WindowTitleHint      |
                                Qt::WindowSystemMenuHint |
                                Qt::MSWindowsFixedSizeDialogHint),
                m_model(model)
{
	ui.setupUi(this);

	connect(ui.buttonCancel, SIGNAL(clicked()), SLOT(reject()));

	m_shouldClose = false;
}

StoppingDialog::~StoppingDialog()
{

}

int StoppingDialog::exec()
{
	bool stopped = m_model->stopItems();

	if (stopped)
	{
		return QDialog::Accepted;
	}

	// If not successfull at first attempt then show the dialog.
	adjustSize();
	show();

	while (!(stopped || m_shouldClose))
	{
		// Prevent GUI from freezing.
		QCoreApplication::processEvents();

		ui.progressBar->setValue(ui.progressBar->value() + 1);

		stopped = m_model->stopItems();

		if (ui.progressBar->value() >= ui.progressBar->maximum())
		{
			ui.progressBar->setValue(ui.progressBar->minimum());
		}
	}

	if (stopped)
	{
		return QDialog::Accepted;
	}
	else
	{
		return QDialog::Rejected;
	}
}

void StoppingDialog::reject()
{
	// Notify the exec() loop to end because the dialog gets closed.
	m_shouldClose = true;

	QDialog::reject();
}

} // namespace
