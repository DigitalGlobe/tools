/*!
 * \file  appsettings.cpp
 *
 * \brief Implements class AppSettings.
 */

#include "appsettings.h"
#include "runnerwindow.h"
#include "runnermodel.h"
#include "runnerproxymodel.h"
#include "resultsproxymodel.h"
#include "utils.h"

#include <QSettings>
#include <QFileInfo>
#include <QHeaderView>

namespace QxRunner {

AppSettings::AppSettings(RunnerWindow* window)
           : RunnerWindowClient(window)
{
	m_organization = "qxrunner";

	// Since the application name can vary (this is a library) get the
	// application name from the executable.
	QFileInfo fileInfo(QCoreApplication::applicationFilePath());
	m_application = fileInfo.baseName();

	// Look for the model name entry.
	QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       m_organization, m_application);

	settings.beginGroup("model");

	QString modelName = settings.value("name", "").toString();

	if (!modelName.trimmed().isEmpty())
	{
		m_application = modelName;	// Override executable name
	}
	
	// If there is a model then use its name for the 'real' INI file and
	// write it into the application INI file. An existing model name entry
	// gets overwritten.
	if (runnerModel())
	{
		modelName = runnerModel()->name().toLower();
		QFileInfo fileInfo(modelName);
		modelName = fileInfo.baseName();	// Should be a valid filename

		// Validate settings filename by writing a test entry and checking
		// the status of the sync operation.
		QSettings testSettings(QSettings::IniFormat, QSettings::UserScope,
                               m_organization, modelName);

		testSettings.setValue("test", "test_only");
		testSettings.sync();

		QSettings::Status stat = testSettings.status();

		if (stat == QSettings::NoError)
		{
			testSettings.remove("test");
		}
		else
		{
			modelName = "";
		}
	}

	if (!modelName.trimmed().isEmpty())
	{
		// Remove all settings and write 'pointer' to 'real' settings file.
		settings.clear();
		settings.setValue("name", modelName);
		m_application = modelName;
	}

	settings.endGroup();
}

AppSettings::~AppSettings()
{

}

void AppSettings::writeSettings()
{
	QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       m_organization, m_application);

	//--------------------

	settings.beginGroup("mainwindow");

	settings.setValue("maximized", window()->isMaximized());
	settings.setValue("size", window()->size());
	settings.setValue("pos", window()->pos());
	settings.setValue("state", window()->saveState());

	settings.endGroup();

	//--------------------

	// Get the relevant column count.
	int columnCount = 0;
	
	if (runnerModel())
	{
		columnCount = runnerModel()->columnCount();
	}

	settings.beginGroup("runnerview");

	QList<int> runnerColumnSizes = Utils::columnSizes(runnerView());

	// Data conversions needed for storing the settings.
	if (columnCount > 0)
	{
		QByteArray bArray(columnCount, '0');

		for (int i = 0; i < columnCount; i++)
		{
			if (runnerProxyModel()->isColumnEnabled(i))
			{
				bArray[i] = '1';
			}
		}

		settings.setValue("columns", bArray);

		QList<QVariant> vList;

		for (int i = 0; i < columnCount; i++)
		{
			vList.append(runnerColumnSizes[i]);
		}

		settings.setValue("column_sizes", vList);
	}
	else
	{
		settings.remove("columns");
		settings.remove("column_sizes");
	}

	settings.endGroup();

	//--------------------

	settings.beginGroup("resultsview");

	settings.setValue("floating", windowUi()->dockResults->isFloating());
	settings.setValue("fatals", windowUi()->buttonFatals->isChecked());
	settings.setValue("errors", windowUi()->buttonErrors->isChecked());
	settings.setValue("warnings", windowUi()->buttonWarnings->isChecked());
	settings.setValue("infos", windowUi()->buttonInfos->isChecked());

	QList<int> resultsColumnSizes = Utils::columnSizes(resultsView());

	if (columnCount > 0)
	{
		QByteArray bArray(columnCount, '0');

		for (int i = 0; i < columnCount; i++)
		{
			if (resultsProxyModel()->isColumnEnabled(i))
			{
				bArray[i] = '1';
			}
		}

		settings.setValue("columns", bArray);

		QList<QVariant> vList;

		for (int i = 0; i < columnCount; i++)
		{
			vList.append(resultsColumnSizes[i]);
		}

		settings.setValue("column_sizes", vList);
	}
	else
	{
		settings.remove("columns");
		settings.remove("column_sizes");
	}

	settings.endGroup();

	//--------------------

	settings.beginGroup("options");
	settings.setValue("minimal_update", window()->isMinimalUpdate());
	settings.setValue("alternating_row_colors",
                      runnerView()->alternatingRowColors());
	settings.endGroup();
}

void AppSettings::applyBaseSettings() const
{
	QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       m_organization, m_application);

	bool checked;

	settings.beginGroup("resultsview");

	checked = settings.value("fatals", true).toBool();
	windowUi()->buttonFatals->setChecked(checked);
	checked = settings.value("errors", true).toBool();
	windowUi()->buttonErrors->setChecked(checked);
	checked = settings.value("warnings", true).toBool();
	windowUi()->buttonWarnings->setChecked(checked);
	checked = settings.value("infos", true).toBool();
	windowUi()->buttonInfos->setChecked(checked);

	settings.endGroup();

	settings.beginGroup("options");

	checked = settings.value("minimal_update", false).toBool();
	windowUi()->actionMinimalUpdate->setChecked(checked);
	checked = settings.value("alternating_row_colors", true).toBool();
	runnerView()->setAlternatingRowColors(checked);
	resultsView()->setAlternatingRowColors(checked);

	settings.endGroup();
}

void AppSettings::applyWindowSettings() const
{
	QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       m_organization, m_application);

	settings.beginGroup("resultsview");

	bool floating = settings.value("floating", false).toBool();

	settings.endGroup();

	settings.beginGroup("mainwindow");

	if (settings.value("maximized", false).toBool())
	{
		window()->showMaximized();
	}
	else
	{
		QSize defaultSize(window()->size());
		window()->resize(settings.value("size", defaultSize).toSize());
		QPoint defaultPos(window()->pos());
		window()->move(settings.value("pos", defaultPos).toPoint());
	}

	if (settings.contains("state"))
	{
		// In case of a floating dock widget updates must be enabled.
		if (floating)
		{
			window()->setUpdatesEnabled(true);
		}

		resultsView()->setMaximumSize(QSize(16777215, 16777215));
		window()->restoreState(settings.value("state").toByteArray());
	}

	settings.endGroup();
}

void AppSettings::applyColumnSettings() const
{
	// Get the relevant column count.
	int columnCount = runnerModel()->columnCount();

	// Set the runner tree view columns to default sizes first. Results tree view
	// columns have no default size, they must be adjusted otherwise (manually)
	// since it's not easy to find useful column spans.
	for (int i = 0; i < columnCount; i++)
	{
		runnerView()->resizeColumnToContents(i);
	}

	QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       m_organization, m_application);

	// When there are column sizes in the settings then these are applied.
	QByteArray runnerColumns;
	QByteArray resultsColumns;
	QStringList runnerColumnSizes;
	QStringList resultsColumnSizes;

	// Try to read the settings.
	settings.beginGroup("runnerview");

	if (settings.contains("columns"))
	{
		runnerColumns = settings.value("columns").toByteArray();
		runnerColumnSizes = settings.value("column_sizes").toStringList();
	}

	settings.endGroup();

	settings.beginGroup("resultsview");

	if (settings.contains("columns"))
	{
		resultsColumns = settings.value("columns").toByteArray();
		resultsColumnSizes = settings.value("column_sizes").toStringList();
	}

	settings.endGroup();

	if (runnerColumns.size() < 1 && resultsColumns.size() < 1)
	{
		// No settings.
		return;
	}

	// Some validation to prevent from crashes.
	while (runnerColumns.count() < columnCount)
	{
		runnerColumns.append('0');
	}

	while (resultsColumns.count() < columnCount)
	{
		resultsColumns.append('0');
	}

	while (runnerColumnSizes.count() < columnCount)
	{
		runnerColumnSizes.append("25");
	}

	while (resultsColumnSizes.count() < columnCount)
	{
		resultsColumnSizes.append("25");
	}

	// Minimal column width.
	for (int i = 0; i < columnCount; i++)
	{
		if (runnerColumnSizes[i].toInt() < 25)
		{
			runnerColumnSizes[i] = "25";
		}

		if (resultsColumnSizes[i].toInt() < 25)
		{
			resultsColumnSizes[i] = "25";
		}
	}

	// Enable resizing.
    resultsView()->header()->setSectionResizeMode(QHeaderView::Interactive);

	// Disable stretching first to properly resize the columns.
	bool runnerViewStretched = runnerView()->header()->stretchLastSection();
	bool resultsViewStretched = resultsView()->header()->stretchLastSection();

	runnerView()->header()->setStretchLastSection(false);
	resultsView()->header()->setStretchLastSection(false);

	QBitArray enabledRunnerColumns(columnCount);
	QBitArray enabledResultsColumns(columnCount);

	// Establish columns according to the settings.
	for (int i = 0; i < columnCount; i++)
	{
		runnerView()->setColumnHidden(i, runnerColumns[i] != '1');
		runnerView()->header()->resizeSection(i, runnerColumnSizes[i].toInt());
		enabledRunnerColumns[i] = (runnerColumns[i] == '1');

		resultsView()->setColumnHidden(i, resultsColumns[i] != '1');
		resultsView()->header()->resizeSection(i, resultsColumnSizes[i].toInt());
		enabledResultsColumns[i] = (resultsColumns[i] == '1');
	}

	// First column always visible.
	if (runnerView()->isColumnHidden(0))
	{
		runnerView()->setColumnHidden(0, false);
		runnerView()->resizeColumnToContents(0);
		enabledRunnerColumns[0] = true;
	}

	if (resultsView()->isColumnHidden(0))
	{
		resultsView()->setColumnHidden(0, false);
		resultsView()->resizeColumnToContents(0);
		enabledResultsColumns[0] = true;
	}

	runnerView()->header()->setStretchLastSection(runnerViewStretched);
	resultsView()->header()->setStretchLastSection(resultsViewStretched);

	// Update proxy models with the currently enabled, i.e. visible columns.
	runnerProxyModel()->setEnabledColumns(enabledRunnerColumns);
	resultsProxyModel()->setEnabledColumns(enabledResultsColumns);
}

Ui::RunnerWindow* AppSettings::windowUi() const
{
	return &(window()->ui);
}

} // namespace
