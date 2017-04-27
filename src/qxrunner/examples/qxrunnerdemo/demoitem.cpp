#include "demoitem.h"

#include <QCoreApplication>
#include <stdlib.h>

DemoItem::DemoItem(const QList<QVariant>& data, RunnerItem* parent)
        : RunnerItem(data, parent)
{

}

DemoItem::~DemoItem()
{
	// Remove destructor if it doesn't class specific cleanup.
}

int DemoItem::run()
{
	if (child(0))
	{
		return QxRunner::NoResult;	// Have nothing to do as a parent
	}

	//Mimick some processing.
	//for (int i = 0; i < 30000; i++)
	//{
	//	QCoreApplication::processEvents();
	//}

	QString msg;
	QString line;

	// Randomly generated result.
	switch (rand() % 6)
	{
		case 0:
			msg = "Run completed successfully.";
			line.setNum(__LINE__);
			setResult(QxRunner::RunSuccess);
			break;

		case 1:
			msg = "Run completed with an information.";
			line.setNum(__LINE__);
			setResult(QxRunner::RunInfo);
			break;

		case 2:
			msg = "Run completed with a warning.";
			line.setNum(__LINE__);
			setResult(QxRunner::RunWarning);
			break;

		case 3:
			msg = "Run completed with an error.";
			line.setNum(__LINE__);
			setResult(QxRunner::RunError);
			break;

		case 4:
			msg = "Run completed with a fatal error.";
			line.setNum(__LINE__);
			setResult(QxRunner::RunFatal);
			break;

		case 5:
			// This gets caught by the caller as a fallback, but actually
			// all exceptions should get handled in the run() method.
			throw 0;
			break;
	}

	// Fill item with resulting data.
	setData(2, msg);
	setData(3, __FILE__);
	setData(4, line);

	return result();
}
