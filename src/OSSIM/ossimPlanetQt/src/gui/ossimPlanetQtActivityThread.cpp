#include <ossimPlanetQt/ossimPlanetQtActivityThread.h>
#include <ossimPlanetQt/ossimPlanetQtActivityItem.h>

ossimPlanetQtActivityThread::ossimPlanetQtActivityThread(ossimPlanetQtActivityItem* item, QObject * parent)
   :QThread(parent),
    theItem(item)
{
}

ossimPlanetQtActivityThread::ossimPlanetQtActivityThread(QObject* parent)
   :QThread(parent),
    theItem(0)
{
}

void ossimPlanetQtActivityThread::setActivityItem(ossimPlanetQtActivityItem* item)
{
   theItem = item;
}

void ossimPlanetQtActivityThread::run()
{
   if(theItem)
   {
      theItem->setThread(this);
      theItem->execute();
   }
}
