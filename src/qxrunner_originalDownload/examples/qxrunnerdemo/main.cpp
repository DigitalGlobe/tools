#include "demomodel.h"

#include <QApplication>
#include <qxrunner/runner.h>

QStringList demoItems();			// Helper function

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

	DemoModel model(demoItems());	// Setup the data

	QxRunner::Runner runner(&model);
	runner.run();

	return 0;
}

// Creates a list of items for the model.
QStringList demoItems()
{
	QStringList itemList;

	itemList << "L0" << "Group 1";
	itemList << "CH" << "Demo item 1a";
	itemList << "CH" << "Demo item 1b";
	itemList << "CH" << "Demo item 1c";
	itemList << "CH" << "Demo item 1d";

	itemList << "L0" << "Group 2";
	itemList << "CH" << "Demo item 2a";
	itemList << "CH" << "Demo item 2b";
	itemList << "CH" << "Demo item 2c";
	itemList << "CH" << "Demo item 2d";

	itemList << "L0" << "Group 3";
	itemList << "CH" << "Demo item 3a";
	itemList << "CH" << "Demo item 3b";
	itemList << "CH" << "Demo item 3c";
	itemList << "CH" << "Demo item 3d";

	itemList << "L1" << "Group 3.1";
	itemList << "CH" << "Demo item 3.1a";
	itemList << "CH" << "Demo item 3.1b";
	itemList << "CH" << "Demo item 3.1c";
	itemList << "CH" << "Demo item 3.1d";

	itemList << "L2" << "Group 3.1.1";
	itemList << "CH" << "Demo item 3.1.1a";
	itemList << "CH" << "Demo item 3.1.1b";
	itemList << "CH" << "Demo item 3.1.1c";
	itemList << "CH" << "Demo item 3.1.1d";

	itemList << "L2" << "Group 3.2";
	itemList << "CH" << "Demo item 3.2a";
	itemList << "CH" << "Demo item 3.2b";
	itemList << "CH" << "Demo item 3.2c";
	itemList << "CH" << "Demo item 3.2d";

	itemList << "L3" << "Group 3.2.1";
	itemList << "CH" << "Demo item 3.2.1a";
	itemList << "CH" << "Demo item 3.2.1b";
	itemList << "CH" << "Demo item 3.2.1c";
	itemList << "CH" << "Demo item 3.2.1d";

	itemList << "L0" << "Demo item 4";

	itemList << "L0" << "Group 5";
	itemList << "CH" << "Demo item 5a";
	itemList << "CH" << "Demo item 5b";
	itemList << "CH" << "Demo item 5c";
	itemList << "CH" << "Demo item 5d";

	itemList << "L0" << "Demo item 6";
	itemList << "L0" << "Demo item 7";

	return itemList;
}
