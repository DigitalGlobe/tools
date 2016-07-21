#ifndef ossimPlanetQtActivityWidget_HEADER
#define ossimPlanetQtActivityWidget_HEADER
#include <QtGui/QTreeWidget>
#include <QtGui/QKeyEvent>
#include <QtGui/QMenu>
#include <ossimPlanetQt/ossimPlanetQtOperation.h>
#include <ossimPlanetQt/ossimPlanetQtActivityThreadQueueItem.h>
#include <ossimPlanetQt/ossimPlanetQtExport.h>
#include <OpenThreads/Mutex>
#include <OpenThreads/ScopedLock>
#include <map>
class OSSIMPLANETQT_DLL ossimPlanetQtActivityWidget : public QTreeWidget
{
   Q_OBJECT
	
public:    
   typedef std::map<ossimPlanetOperation*, QTreeWidgetItem*> OperationItemMapType;
   enum EventId
   {
      STATUS_TYPE          = QEvent::User + 8000,
      FINISHED_TYPE        = STATUS_TYPE + 1,
      CANCELED_TYPE        = FINISHED_TYPE + 1,
      STARTED_TYPE         = CANCELED_TYPE + 1
	};
	class ActivityItem : public QTreeWidgetItem
		{
		public:
			ActivityItem(QTreeWidget* parent,
							 ossimPlanetOperation* operation)
			:QTreeWidgetItem(parent),
			theOperation(operation)
			{
				
			}
			ActivityItem(QTreeWidgetItem* parent,
							 ossimPlanetOperation* operation)
			:QTreeWidgetItem(parent),
			theOperation(operation)
			{
				
			}
			
			osg::ref_ptr<ossimPlanetOperation> theOperation;
		};
   class Callback : public ossimPlanetOperationCallback
      {
      public:
         Callback(ossimPlanetQtActivityWidget* w)
         :theWidget(w)
         {
            
         }
         virtual void started(ossimPlanetOperation* operation);
         virtual void finished(ossimPlanetOperation* operation);
         virtual void canceled(ossimPlanetOperation* operation);
         virtual void propertyChanged(const ossimString& name,
                                      ossimPlanetOperation* operation);
         
      protected:
         ossimPlanetQtActivityWidget* theWidget;
      };
   friend class ossimPlanetQtActivityWidget::Callback;
   class QtOperationEvent : public QEvent
      {
      public:
         QtOperationEvent(ossimPlanetOperation* operation,
                          EventId id)
         :QEvent((QEvent::Type)id),
         theOperation(operation)
         {
            
         }
         ossimPlanetOperation* operation()
         {
            return theOperation.get();
         }
         const ossimPlanetOperation* operation()const
         {
            return theOperation.get();
         }
         ossimPlanetQtOperation* toQtOperation()
         {
            return dynamic_cast<ossimPlanetQtOperation*>(theOperation.get());
         }
         const ossimPlanetQtOperation* toQtOperation()const
         {
            return dynamic_cast<ossimPlanetQtOperation*>(theOperation.get());
         }
      protected:
         osg::ref_ptr<ossimPlanetOperation> theOperation;
      };
	friend class ossimPlanetQtActivityWidgetOperationCallback;
	
   ossimPlanetQtActivityWidget(QWidget* parent = 0);
   virtual ~ossimPlanetQtActivityWidget();
	void addOperation(ossimPlanetOperation* operation);
   //void addActivity(ossimPlanetQtActivityItem* activity);
   virtual bool event(QEvent* e);
   
	void deleteSelectedActivities();
	void deleteAllActivities();
	
protected:
	void removeOperation(ossimPlanetOperation*);
   //void addStagerActivity(ossimPlanetQtActivityItem* activity);
   //void addOpenImageFileActivity(ossimPlanetQtActivityItem* activity);
   //void addMessageActivity(ossimPlanetQtActivityItem* activity);
   //void addMiscActivity(ossimPlanetQtActivityItem* activity);
	//void sendUpdateGuiEvent();
   //virtual void timerEvent(QTimerEvent *event);
   
   //int                                   theTimerId; 
   //ossimPlanetQtActivityThreadQueueItem* theStagerQueue;
   //ossimPlanetQtActivityThreadQueueItem* theOpenFileQueue;
   //ossimPlanetQtActivityThreadQueueItem* theMessageQueue;
   //ossimPlanetQtActivityThreadQueueItem* theMiscQueue;
	
	osg::ref_ptr<ossimPlanetOperationThreadQueue> theStagerQueue;
	osg::ref_ptr<ossimPlanetOperationThreadQueue> theOpenFileQueue;
	osg::ref_ptr<ossimPlanetOperationThreadQueue> theMiscQueue;
   
   OpenThreads::Mutex   theItemMapMutex;
   OperationItemMapType theItemMap;
  
   osg::ref_ptr<ossimPlanetQtActivityWidget::Callback> theCallback;
};

#endif
