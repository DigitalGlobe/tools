#ifndef DEMOITEM_H
#define DEMOITEM_H

#include <qxrunner/runneritem.h>

using namespace QxRunner;

class DemoItem : public RunnerItem
{
public: // Operations

	DemoItem(const QList<QVariant>& data, RunnerItem* parent = 0);
	
	~DemoItem();

	int run();
};

#endif // DEMOITEM_H
