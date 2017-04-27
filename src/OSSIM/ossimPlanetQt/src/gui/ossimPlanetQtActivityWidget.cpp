#include <ossimPlanetQt/ossimPlanetQtActivityWidget.h>
#include <ossimPlanetQt/ossimPlanetQtActivityThread.h>
#include <ossimPlanetQt/ossimPlanetQtOperation.h>
#include <QtGui/QHeaderView>
#include <QtGui/QTreeWidgetItemIterator>
#include <QtGui/QApplication>


void ossimPlanetQtActivityWidget::Callback::started(ossimPlanetOperation* operation)
{
   if(theWidget)
   {
      QApplication::postEvent(theWidget, new ossimPlanetQtActivityWidget::QtOperationEvent(operation, 
                                                                                           ossimPlanetQtActivityWidget::STARTED_TYPE));
   }
}

void ossimPlanetQtActivityWidget::Callback::finished(ossimPlanetOperation* operation)
{
   if(theWidget)
   {
      QApplication::postEvent(theWidget, new ossimPlanetQtActivityWidget::QtOperationEvent(operation, 
                                                                                           ossimPlanetQtActivityWidget::FINISHED_TYPE));
   }
}

void ossimPlanetQtActivityWidget::Callback::canceled(ossimPlanetOperation* operation)
{
   if(theWidget)
   {
      QApplication::postEvent(theWidget, new ossimPlanetQtActivityWidget::QtOperationEvent(operation, 
                                                                                           ossimPlanetQtActivityWidget::CANCELED_TYPE));
   }
}

void ossimPlanetQtActivityWidget::Callback::propertyChanged(const ossimString& name,
                                                            ossimPlanetOperation* operation)
{
   if(name == "status")
   {
      if(theWidget)
      {
         QApplication::postEvent(theWidget, new ossimPlanetQtActivityWidget::QtOperationEvent(operation, 
                                                                                              ossimPlanetQtActivityWidget::STATUS_TYPE));
      }
   }
}

//static const char *STAGER_ACTIVITY_KW = "Image Stager Activities";
//static const char *OPEN_FILES_ACTIVITY_KW = "Open Files";
//static const char *MESSAGE_ACTIVITY_KW = "Message Activities";
//static const char *MISC_ACTIVITY_KW = "Miscellaneous Activities";
ossimPlanetQtActivityWidget::ossimPlanetQtActivityWidget(QWidget* parent)
   :QTreeWidget(parent)
{
	theStagerQueue     = new ossimPlanetOperationThreadQueue();
	theOpenFileQueue   = new ossimPlanetOperationThreadQueue();
	theMiscQueue       = new ossimPlanetOperationThreadQueue();
   theCallback = new ossimPlanetQtActivityWidget::Callback(this);

   header()->setHidden(1);
   setRootIsDecorated(true);
   setAutoScroll(true);
   setSortingEnabled(false);
   
   update();
}

ossimPlanetQtActivityWidget::~ossimPlanetQtActivityWidget()
{
	theStagerQueue->removeAllOperations();
	theStagerQueue->cancelCurrentOperation();
	theOpenFileQueue->removeAllOperations();
	theOpenFileQueue->cancelCurrentOperation();
	theMiscQueue->removeAllOperations();
	theMiscQueue->cancelCurrentOperation();
}

void ossimPlanetQtActivityWidget::addOperation(ossimPlanetOperation* operation)
{
   if(!operation) return;
   ossimPlanetQtOperation* qtOperation = dynamic_cast<ossimPlanetQtOperation*>(operation);
   QTreeWidgetItem* item = 0;
   operation->addCallback(theCallback.get());
   ossimString status;
   if(qtOperation)
   {
      qtOperation->setActivityWidget(this);
      qtOperation->status(status);
   }
	if(dynamic_cast<ossimPlanetQtOssimImageStagerOperation*>(operation))
	{
      item = new ActivityItem(this, operation);
      item->setText(0, status.c_str());
      theItemMap.insert(std::make_pair(operation, item));
		theStagerQueue->add(operation);
	}
	else if(dynamic_cast<ossimPlanetQtOpenImageFileOperation*>(operation))
	{
      item = new ActivityItem(this, operation);
      item->setText(0, status.c_str());
      theItemMap.insert(std::make_pair(operation, item));
		theOpenFileQueue->add(operation);
	}
   else
   {
      item = new ActivityItem(this, operation);
      item->setText(0, status.c_str());
      theItemMap.insert(std::make_pair(operation, item));
      theMiscQueue->add(operation);
   }
	
}
bool ossimPlanetQtActivityWidget::event(QEvent* e)
{
   switch(e->type())
   {
      case ossimPlanetQtActivityWidget::STATUS_TYPE:
      {
         OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theItemMapMutex);
         ossimPlanetQtActivityWidget::QtOperationEvent* event = dynamic_cast<ossimPlanetQtActivityWidget::QtOperationEvent*>(e);
         if(event)
         {
            ossimPlanetQtOperation* qtOperation = event->toQtOperation();
            
            if(qtOperation)
            {
               ossimString status;
               qtOperation->status(status);
               OperationItemMapType::iterator iter = theItemMap.find(qtOperation);
               if(iter!=theItemMap.end())
               {
                  iter->second->setText(0, status.c_str());
               }
            }
         }
         break;
      }
      case ossimPlanetQtActivityWidget::FINISHED_TYPE:
      {
         ossimPlanetQtActivityWidget::QtOperationEvent* event = dynamic_cast<ossimPlanetQtActivityWidget::QtOperationEvent*>(e);
         if(event)
         {
            ossimPlanetOperation* operation = event->operation();
            if(operation)
            {
               removeOperation(operation);
            }
         }
         break;
      }
      default:
      {
         break;
      }
	}
   return QTreeWidget::event(e);
}

void ossimPlanetQtActivityWidget::deleteSelectedActivities()
{
	QTreeWidgetItemIterator iter(this, QTreeWidgetItemIterator::Selected);
	while(*iter)
	{
		ossimPlanetQtActivityWidget::ActivityItem* item = dynamic_cast<ossimPlanetQtActivityWidget::ActivityItem*>(*iter);
		++iter;
		if(item)
		{
			removeOperation(item->theOperation.get());
		}
	}
}
void ossimPlanetQtActivityWidget::deleteAllActivities()
{
	QTreeWidgetItemIterator iter(this, QTreeWidgetItemIterator::All);
	while(*iter)
	{
		ossimPlanetQtActivityWidget::ActivityItem* item = dynamic_cast<ossimPlanetQtActivityWidget::ActivityItem*>(*iter);
		++iter;
		if(item)
		{
			removeOperation(item->theOperation.get());
		}
	}
}

void ossimPlanetQtActivityWidget::removeOperation(ossimPlanetOperation* operation)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theItemMapMutex);
   OperationItemMapType::iterator iter = theItemMap.find(operation);
   
   if(iter!=theItemMap.end())
   {
		operation->cancel();
      //removeItemWidget(iter->second, 0);
      delete iter->second;
      theItemMap.erase(iter);
   }
}
