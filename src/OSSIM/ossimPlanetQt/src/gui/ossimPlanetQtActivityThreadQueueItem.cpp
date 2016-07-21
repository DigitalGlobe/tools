#include <ossimPlanetQt/ossimPlanetQtActivityThreadQueueItem.h>
#include <ossimPlanetQt/ossimPlanetQtActivityThread.h>
#include <QtGui/QMessageBox>

// ossimPlanetQtActivityThreadQueueItem::ossimPlanetQtActivityThreadQueueItem(QTreeWidget* parent,
//                                                                            const QString& queueName,
//                                                                            int type)
//    :QTreeWidgetItem(parent, type),
//     theStartFlag(false)
// {
//    setText(0, queueName);
//    theThread = new ossimPlanetQtActivityThread();
//    connect(theThread, SIGNAL(finished()), this, SLOT(finished()));
//    connect(theThread, SIGNAL(terminated()), this, SLOT(terminated()));
// }

ossimPlanetQtActivityThreadQueueItem::ossimPlanetQtActivityThreadQueueItem(QTreeWidget* parent,
                                                                           const QString& queueName,
                                                                           ossimPlanetQtActivityThread* thread,
                                                                           int type)
   :QTreeWidgetItem(parent, type),
    theStartFlag(false)
{
   theDeletableFlag = true;
   setText(0, queueName);
   theThread = thread;
   if(!theThread)
   {
      theThread = new ossimPlanetQtActivityThread();  
   }
   connect(theThread, SIGNAL(finished()), this, SLOT(finished()));
   connect(theThread, SIGNAL(terminated()), this, SLOT(terminated()));
}

void ossimPlanetQtActivityThreadQueueItem::start()
{
   QMutexLocker lock(&theMutex);
   if((!theStartFlag)&&
      (childCount() > 0))
   {
      ossimPlanetQtActivityItem* item = dynamic_cast<ossimPlanetQtActivityItem*>(child(0));
      if(item)
      {
         theStartFlag = true;
         theThread->setActivityItem((ossimPlanetQtActivityItem*)child(0));
         theThread->start(QThread::LowestPriority);
      }
   }
}

QThread* ossimPlanetQtActivityThreadQueueItem::thread()
{
   return theThread;
}

void ossimPlanetQtActivityThreadQueueItem::finished()
{
   QMutexLocker lock(&theMutex);
   ossimPlanetQtActivityItem* item = dynamic_cast<ossimPlanetQtActivityItem*>(child(0));
   theStartFlag = false;
   if(item)
   {
      item->setState(ossimPlanetQtActivityItem::FINISHED);
   }
}

void ossimPlanetQtActivityThreadQueueItem::terminated()
{
   QMutexLocker lock(&theMutex);
   ossimPlanetQtActivityItem* item = dynamic_cast<ossimPlanetQtActivityItem*>(child(0));
   theStartFlag = false;
   if(item)
   {
      item->setState(ossimPlanetQtActivityItem::CANCELED);
   }
}

void ossimPlanetQtActivityThreadQueueItem::updateGui()
{
   ossim_uint32 count = childCount();

   if(count < 1) return;

   ossim_uint32 idx = 0;

   ossimPlanetQtActivityItem* item = 0;
   
   for(idx = 0; idx < count; ++idx)
   {
      item = dynamic_cast<ossimPlanetQtActivityItem*>(child(idx));
      if(item)
      {
         item->updateGui();
      }
   }
   item = dynamic_cast<ossimPlanetQtActivityItem*>(child(0));
   if(item)
   {
      // make sure all updates have been transferred.
      item->updateGui();
      switch(item->state())
      {
         case ossimPlanetQtActivityItem::READY:
         {
            theMutex.lock();
            if(!theStartFlag)
            {
               theMutex.unlock();
               start();
            }
            else
            {
               theMutex.unlock();
            }
            break;
         }
         case ossimPlanetQtActivityItem::FINISHED:
         case ossimPlanetQtActivityItem::CANCELED:
         {
            ossimString message = item->errorMessage();
            if(message != "")
            {
               QMessageBox *messageBox = new QMessageBox(0);
               messageBox->setButtonText(QMessageBox::Ok, "Ok");
               messageBox->setText(message.c_str());
               messageBox->setAttribute(Qt::WA_DeleteOnClose);
               messageBox->setModal(false);
               messageBox->show();
            }
            if(theDeletableFlag)
            {
               delete child(0);
            }
            
            break;
         }
         default:
         {
            break;
         }
      }
   }
}
bool ossimPlanetQtActivityThreadQueueItem::deletableFlag()const
{
   return theDeletableFlag;
}

void ossimPlanetQtActivityThreadQueueItem::setDeletableFlag(bool flag)
{
   theDeletableFlag = flag;
}
