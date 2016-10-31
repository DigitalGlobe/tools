/*!
 * \file  aboutdialog.cpp
 *
 * \brief Implements class AboutDialog.
 */

#include "aboutdialog.h"
#include "runnermodel.h"
#include "qxrunner_global.h"

namespace QxRunner {

AboutDialog::AboutDialog(QWidget* parent, RunnerModel* model)
           : QDialog(parent, Qt::WindowTitleHint      |
                             Qt::WindowSystemMenuHint |
                             Qt::MSWindowsFixedSizeDialogHint)
{
	ui.setupUi(this);

	connect(ui.buttonOk, SIGNAL(clicked()), SLOT(accept()));

	// QxRunner version.
	ui.labelVersion->setText(version());

	// Model info.
	QString modelInfo;

	if (model)
	{
		modelInfo = model->name().trimmed();
		QString modelAbout = model->about().trimmed();

		if (!modelAbout.isEmpty())
		{
			if (!modelInfo.isEmpty())
			{
				modelInfo += "\n";
			}

			modelInfo += modelAbout;
		}
	}

	ui.labelModelName->setText(modelInfo);

	// Qt version.
	ui.labelQtVersion->setText(qVersion());

	resize(QSize(1, 1));	// Works better than adjustSize()
}

AboutDialog::~AboutDialog()
{

}

} // namespace
