#ifndef ossimPlanetQtRuler_HEADER
#define ossimPlanetQtRuler_HEADER
#include <ossimPlanetQt/ui_ossimPlanetQtRuler.h>
#include <QtGui/QDialog>
#include <osg/Vec3d>
#include <osg/ref_ptr>
#include <ossimPlanet/ossimPlanet.h>
#include <ossimPlanet/ossimPlanetGeoRefModel.h>
#include <ossimPlanetQt/ossimPlanetQtExport.h>

class ossimPlanetQtViewer;
class ossimPlanetGeoRefModel;
class ossimPlanetMeasureTool;
class OSSIMPLANETQT_DLL ossimPlanetQtRuler : public QDialog, public Ui::ossimPlanetQtRuler
{
   Q_OBJECT
public:
   ossimPlanetQtRuler(QWidget* parent);
   virtual ~ossimPlanetQtRuler();
   void setPlanet(osg::ref_ptr<ossimPlanet> planet);
   void setGlWidget(ossimPlanetQtViewer* widget);
   
protected slots:
   void glWidgetSignalMouseMoveEvent(QMouseEvent* e);
   void glWidgetSignalMousePressEvent(QMouseEvent* e);
   void glWidgetSignalMouseReleaseEvent(QMouseEvent* e);
   void mouseNavStateChange(int state);
   
protected:
   void setDistance();

   ossimPlanetMeasureTool*            theTool;
   osg::ref_ptr<ossimPlanet> thePlanet;
   osg::ref_ptr<ossimPlanetGeoRefModel> theModel;
   ossimPlanetQtViewer*             theGlWidget;
   bool                               theFirstPointSet;
   osg::Vec3d                         theStartPointXYZ;
   osg::Vec3d                         theStartPointLatLonHgt;

   osg::Vec3d                         theEndPointXYZ;
   osg::Vec3d                         theEndPointLatLonHgt;
};

#endif
