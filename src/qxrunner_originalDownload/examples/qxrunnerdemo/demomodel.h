#ifndef DEMOMODEL_H
#define DEMOMODEL_H

#include <qxrunner/runnermodel.h>
#include <QStringList>

using namespace QxRunner;

class DemoModel : public RunnerModel
{
	Q_OBJECT

public:  // Operations

	DemoModel(const QStringList& data, QObject* parent = 0);

	~DemoModel();

	QString name() const;
	
private:  // Operations

	void setupModelData(const QStringList& itemList, RunnerItem* parent);
};

#endif // DEMOMODEL_H
