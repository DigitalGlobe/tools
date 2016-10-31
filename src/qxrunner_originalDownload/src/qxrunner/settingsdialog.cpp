/*!
 * \file  settingsdialog.cpp
 *
 * \brief Implements class SettingsDialog.
 */

#include "settingsdialog.h"
#include "runnerwindow.h"

namespace QxRunner {

SettingsDialog::SettingsDialog(QWidget* parent, RunnerWindow* window)
              : QDialog(parent, Qt::WindowTitleHint      |
                                Qt::WindowSystemMenuHint |
                                Qt::MSWindowsFixedSizeDialogHint),
                RunnerWindowClient(window)
{
	ui.setupUi(this);

	connect(ui.buttonOk,     SIGNAL(clicked()), SLOT(accept()));
	connect(ui.buttonApply,  SIGNAL(clicked()), SLOT(apply()));
	connect(ui.buttonCancel, SIGNAL(clicked()), SLOT(reject()));

	m_alternatingRowColors = runnerView()->alternatingRowColors();
	ui.checkBoxRowColors->setChecked(m_alternatingRowColors);
}

SettingsDialog::~SettingsDialog()
{

}

void SettingsDialog::apply()
{
	bool b = ui.checkBoxRowColors->isChecked();
	runnerView()->setAlternatingRowColors(b);
	resultsView()->setAlternatingRowColors(b);
}

void SettingsDialog::accept()
{
	// Update the GUI (again).
	apply();

	QDialog::accept();
}

void SettingsDialog::reject()
{
	runnerView()->setAlternatingRowColors(m_alternatingRowColors);
	resultsView()->setAlternatingRowColors(m_alternatingRowColors);

	QDialog::reject();
}

} // namespace
