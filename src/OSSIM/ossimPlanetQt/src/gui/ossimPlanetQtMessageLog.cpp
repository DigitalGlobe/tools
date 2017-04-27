
#include <ossimPlanetQt/ossimPlanetQtMessageLog.h>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QIcon>

ossimPlanetQtMessageLog::ossimPlanetQtMessageLog(QWidget* parent)
:QDialog(parent)
{
	setupUi(this);
	theMaxNumberOfItems = 1024;
	theNumberOfItems = 0;
	connect(this->theCloseButton, SIGNAL(clicked(bool)), this, SLOT(closeButtonClicked(bool)));
	connect(this->theClearButton, SIGNAL(clicked(bool)), this, SLOT(clearButtonClicked(bool)));
}

void ossimPlanetQtMessageLog::addMessage(const QString& message,
													  ossimPlanetQtMessageLog::Type type)
{
	++theNumberOfItems;
	QTreeWidgetItem* item = new QTreeWidgetItem(theMessageLog);
	switch(type)
	{
		case ossimPlanetQtMessageLog::LOG_ERROR:
		{
			item->setIcon(0, QIcon(":error"));
			break;
		}
		case ossimPlanetQtMessageLog::LOG_WARNING:
		{
			item->setIcon(0, QIcon(":warning"));
			break;
		}
		default:
		{
			item->setIcon(0, QIcon(":info"));
			break;
		}
	}
	item->setText(0, message);
	
	adjustItemCount();
}

void ossimPlanetQtMessageLog::showLastMessage()
{
	if(theMessageLog->topLevelItemCount() > 0)
	{
		theMessageLog->scrollToItem(theMessageLog->topLevelItem(theMessageLog->topLevelItemCount() -1));
	}
}

void ossimPlanetQtMessageLog::closeButtonClicked(bool checked)
{
	hide();
}

void ossimPlanetQtMessageLog::clearButtonClicked(bool checked)
{
	theMessageLog->clear();
}

void ossimPlanetQtMessageLog::adjustItemCount()
{
		while(theNumberOfItems > theMaxNumberOfItems)
		{
			QTreeWidgetItem* item  = theMessageLog->takeTopLevelItem(0);
			if(item)
			{
				delete item;
			}
			--theNumberOfItems;
		}
}
