#ifndef ossimPlanetQtActivityThreadQueueItem_HEADER
#define ossimPlanetQtActivityThreadQueueItem_HEADER
#include <QtGui/QTreeWidgetItem>
#include <ossimPlanetQt/ossimPlanetQtActivityItem.h>
#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>
#include <ossimPlanetQt/ossimPlanetQtExport.h>

class ossimPlanetQtActivityThread;
class OSSIMPLANETQT_DLL ossimPlanetQtActivityThreadQueueItem : public QObject, public QTreeWidgetItem
{
   Q_OBJECT
public:
   ossimPlanetQtActivityThreadQueueItem(QTreeWidget* parent,
                                        const QString& queueName,
                                        ossimPlanetQtActivityThread* thread=0,
                                        int type = Type);


   virtual void start();
   virtual void updateGui();
   bool deletableFlag()const;
   void setDeletableFlag(bool flag);
  
   QThread* thread();
   
protected slots:
   void finished();
   void terminated();
  
protected:
   int nextChildToProcess()const;

   bool                         theDeletableFlag;
   ossimPlanetQtActivityThread* theThread;
   bool theStartFlag;
   mutable QMutex theMutex;
};

#endif
