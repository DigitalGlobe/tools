#ifndef ossimPlanetQtLayerOperation_HEADER
#define ossimPlanetQtLayerOperation_HEADER
#include <QtGui/QDialog>
#include <ossimPlanetQt/ui_ossimPlanetQtLayerOperation.h>
//#include <ossimPlanet/ossimPlanetLand.h>
#include <ossimPlanetQt/ossimPlanetQtExport.h>

class OSSIMPLANETQT_DLL ossimPlanetQtLayerOperation : public QDialog, public Ui::ossimPlanetQtLayerOperation
{
   Q_OBJECT

public:
   ossimPlanetQtLayerOperation(QWidget *parent=0);

//   void setLand(osg::ref_ptr<ossimPlanetLand> land);
//   osg::ref_ptr<ossimPlanetLand> land();
//   const osg::ref_ptr<ossimPlanetLand> land()const;

protected slots:
   void on_theOpacitySlider_valueChanged(int value);
   void on_theSwipeSlider_valueChanged(int value);
   void on_theOperationType_activated(const QString& v);
protected:
   void populateControl();
   
//   osg::ref_ptr<ossimPlanetLand> theLand;
};

#endif
