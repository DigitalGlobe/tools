#ifndef ossimPlanetQtActivityThread_HEADER
#define ossimPlanetQtActivityThread_HEADER
#include <QtCore/QThread>
#include <QtCore/QMutex>

class ossimPlanetQtActivityItem;
class ossimPlanetQtMessageActivity;
class ossimPlanetQtActivityThread : public QThread
{
public:
   ossimPlanetQtActivityThread(ossimPlanetQtActivityItem* item, QObject * parent = 0);
   ossimPlanetQtActivityThread(QObject* parent = 0);
   virtual void setActivityItem(ossimPlanetQtActivityItem* item);
   
   virtual void run();

protected:
   ossimPlanetQtActivityItem* theItem;
};

#endif
