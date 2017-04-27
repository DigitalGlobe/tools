#include "demomodel.h"
#include "demoitem.h"

DemoModel::DemoModel(const QStringList& data, QObject* parent)
         : RunnerModel(parent)
{
	// Data for column headers is stored in the root item.
	QList<QVariant> rootData;
	rootData << tr("Item Name") << tr("Result") << tr("Message")
             << tr("File Name") << tr("Line Number");

	setRootItem(new DemoItem(rootData));

	// Fill the model items.
	setupModelData(data, rootItem());
}

DemoModel::~DemoModel()
{
	// Remove destructor if it doesn't class specific cleanup.
}

QString DemoModel::name() const
{
	return tr("Demo");
}

void DemoModel::setupModelData(const QStringList& itemList, RunnerItem* parent)
{
    QList<DemoItem*> parents;

    DemoItem* item;
	QString token;
	QList<QVariant> columnData;

	QList<QString>::const_iterator it = itemList.constBegin();

    for (; it != itemList.constEnd(); ++it)
	{
		token = *it;

		// First column has the item name, remaining columns are empty.
		it++;
		columnData.clear();
		columnData << *it;

		// Note: Appending empty columns is not needed since the runner
		// item constructor takes care of it.
		// -> columnData << "" << "" << "" << "";		

		if (token == "L0")
		{
			item = new DemoItem(columnData, parent);
			parent->appendChild(item);

			parents.clear();
			parents << item;
		}
		else if (token == "CH")
		{
			parents.last()->appendChild(new DemoItem(columnData, parents.last()));
		}
		else if (token == "L1")
		{
			item = parents.first();
			parents.clear();
			parents << item;

			item = new DemoItem(columnData, parents.last());
			parents.last()->appendChild(item);

			parents << item;
		}
		else
		{
			item = new DemoItem(columnData, parents.last());
			parents.last()->appendChild(item);

			parents << item;
		}
	}
}
