#include <ossimPlanetQt/ossimPlanetQtRuler.h>
#include <ossimPlanetQt/ossimPlanetQtGlWidget.h>
//#include <ossimPlanet/ossimPlanetLand.h>
#include <QtGui/QMessageBox>
#include <QtGui/QMouseEvent>
#include <QtGui/QCheckBox>
#include <QtGui/QAbstractItemView>
#include <QtGui/QHeaderView>
#include <ossimPlanet/ossimPlanetLayer.h>
#include <osg/Geode>
#include <osg/CameraNode>
#include <osg/Drawable>
#include <osg/io_utils>
#include <ossim/base/ossimUnitConversionTool.h>
#include <ossim/base/ossimUnitTypeLut.h>
#include <ossim/base/ossimPolygon.h>
#include <ossim/base/ossimDrect.h>

enum UNIT_IDX_TYPE
{
   KILOMETERS_IDX = 0,
   METERS_IDX,
   CENTIMETERS_IDX,
   MILLIMETERS_IDX,
   MILES_IDX,
   YARDS_IDX,
   FEET_IDX,
   INCHES_IDX
};

class ossimPlanetMeasureToolDrawable : public osg::Drawable
{
public:
   virtual osg::Object* cloneType() const
      {
         return new ossimPlanetMeasureToolDrawable;
      }
   ossimPlanetMeasureToolDrawable()
      {
         thePointsValidFlag = false;
      }
   virtual osg::Object* clone(const osg::CopyOp&) const
      {
         return new ossimPlanetMeasureToolDrawable(*this);
      }
   virtual void drawImplementation(osg::RenderInfo& state) const
      {
         if(thePointsValidFlag)
         {
            glColor3f(1.0, 1.0, 0.0);
            glLineWidth(1.0);
//            glEnable(GL_LINE_SMOOTH);
//            glEnable(GL_POINT_SMOOTH);
            glBegin(GL_LINES);
              glVertex2d(theStartPoint[0], theStartPoint[1]);
              glVertex2d(theEndPoint[0], theEndPoint[1]);
            glEnd();
            glPointSize(5.0);
            glBegin(GL_POINTS);
              glVertex2d(theStartPoint[0], theStartPoint[1]);
              glVertex2d(theEndPoint[0], theEndPoint[1]);
            glEnd();
            
         }
      }

   void setPointsValidFlag(bool flag)
      {
         thePointsValidFlag = flag;
      }
   
   void setPoints(const osg::Vec3d& startPt,
                  const osg::Vec3d& endPt)
      {
         theStartPoint = startPt;
         theEndPoint   = endPt;
      }
protected:
   bool thePointsValidFlag;
   osg::Vec3d theStartPoint;
   osg::Vec3d theEndPoint;
};

class ossimPlanetMeasureTool : public ossimPlanetLayer
{
public:
   ossimPlanetMeasureTool();
   virtual void traverse(osg::NodeVisitor& nv);
   virtual void execute(const ossimPlanetAction& action)
   {
      ossimPlanetLayer::execute(action);
   }
   void setPointsValidFlag(bool flag)
      {
         thePointsValidFlag = flag;
      }
   void setPoints(const osg::Vec3d& startPt,
                  const osg::Vec3d& endPt);
protected:
   bool                        thePointsValidFlag;
   osg::Vec3d                  theStartPoint;
   osg::Vec3d                  theEndPoint;
   osg::ref_ptr<osg::Viewport> theViewport;
   osg::ref_ptr<osg::CameraNode> theCameraNode;
   osg::Geode* theGeode;
   ossimPlanetMeasureToolDrawable* theDrawable;
   mutable OpenThreads::Mutex theMutex;
};

ossimPlanetMeasureTool::ossimPlanetMeasureTool()
{
   thePointsValidFlag = false;
   theCameraNode = new osg::CameraNode;
   theCameraNode->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
   theCameraNode->setProjectionMatrix(osg::Matrix::ortho2D(0,1024,0,1024));
   theCameraNode->setViewMatrix(osg::Matrix::identity());
   theCameraNode->setClearMask(GL_DEPTH_BUFFER_BIT);
   theCameraNode->setRenderOrder(osg::CameraNode::POST_RENDER);
   theDrawable = new ossimPlanetMeasureToolDrawable;
   theGeode = new osg::Geode();
   osg::StateSet* stateset = theGeode->getOrCreateStateSet();
   stateset->setMode(GL_LIGHTING,
                     osg::StateAttribute::OFF);
   stateset->setMode(GL_COLOR_MATERIAL,
                     osg::StateAttribute::OFF);
   theGeode->setCullingActive(false);
   theDrawable->setSupportsDisplayList(false);
   theGeode->addDrawable(theDrawable);
   theCameraNode->addChild(theGeode);
   addChild(theCameraNode.get());

}

void ossimPlanetMeasureTool::setPoints(const osg::Vec3d& startPt,
                                       const osg::Vec3d& endPt)

{
	setRedrawFlag(true);
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   theStartPoint = startPt;
   theEndPoint   = endPt;
}

void ossimPlanetMeasureTool::traverse(osg::NodeVisitor& nv)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   switch(nv.getVisitorType())
   {
      case osg::NodeVisitor::UPDATE_VISITOR:
      {
         if(theViewport.valid())
         {
            theCameraNode->setProjectionMatrix(osg::Matrix::ortho2D(theViewport->x(),
                                                                    theViewport->width(),
                                                                    theViewport->y(),
                                                                    theViewport->height()));
         }
         
         break;
      }
      case osg::NodeVisitor::CULL_VISITOR:
      {
         osgUtil::CullVisitor* cullVisitor = dynamic_cast<osgUtil::CullVisitor*>(&nv);
         if(cullVisitor)
         {
            int x = cullVisitor->getViewport()->x();
            int y = cullVisitor->getViewport()->y();
            int w = cullVisitor->getViewport()->width();
            int h = cullVisitor->getViewport()->height();
            if(!theViewport.valid())
            {
               theViewport = new osg::Viewport(x,y,w,h);
            }
            else
            {
               if( (theViewport->x() != x)||
                   (theViewport->y() != y)||
                   (theViewport->width() != w)||
                   (theViewport->height() != h))
               {
                  theViewport->setViewport(x, y, w, h);
               }
            }
            theDrawable->setPointsValidFlag(thePointsValidFlag);
            if(thePointsValidFlag)
            {
               const osg::RefMatrix* m = cullVisitor->getMVPW();

               osg::Vec3d startPt = theStartPoint*(*m);
               osg::Vec3d endPt   = theEndPoint*(*m);

               
               
               theDrawable->setPoints(startPt, endPt);
            }
         }
         break;
      }
      case osg::NodeVisitor::EVENT_VISITOR:
      {
#if 0
         osgGA::EventVisitor* ev = dynamic_cast<osgGA::EventVisitor*>(&nv);
         if(ev)
         {
            const osgGA::EventVisitor::EventList& eventList = ev->getEvents();
            osgGA::EventVisitor::EventList::const_iterator iter = eventList.begin();
            if(eventList.size())
            {
               if(((*iter)->getEventType() == osgGA::GUIEventAdapter::FRAME))
               {
                  ev->getActionAdapter()->requestRedraw();
               }
            }
         }
#endif
         break;
      }
   }
   ossimPlanetLayer::traverse(nv);
}


ossimPlanetQtRuler::ossimPlanetQtRuler(QWidget* parent)
   :QDialog(parent)
{
   setupUi(this);
   theGlWidget = 0;
   theFirstPointSet = false;
   theLength->horizontalHeader()->setHidden(1);
   theLength->verticalHeader()->setHidden(1);
//    theLength->item(0, 1)->setText("meters");
   theTool = new ossimPlanetMeasureTool;
   theTool->ref();
   theLength->setRowCount(8);
   theLength->setColumnCount(2);
   
   theLength->setItem(KILOMETERS_IDX, 0, new QTableWidgetItem("0.0"));
   theLength->setItem(KILOMETERS_IDX, 1, new QTableWidgetItem("km"));   

   theLength->setItem(METERS_IDX, 0, new QTableWidgetItem("0.0"));
   theLength->setItem(METERS_IDX, 1, new QTableWidgetItem("m"));   

   theLength->setItem(CENTIMETERS_IDX, 0, new QTableWidgetItem("0.0"));
   theLength->setItem(CENTIMETERS_IDX, 1, new QTableWidgetItem("cm"));

   theLength->setItem(MILLIMETERS_IDX, 0, new QTableWidgetItem("0.0"));
   theLength->setItem(MILLIMETERS_IDX, 1, new QTableWidgetItem("mm"));

   theLength->setItem(MILES_IDX, 0, new QTableWidgetItem("0.0"));
   theLength->setItem(MILES_IDX, 1, new QTableWidgetItem("mi"));

   theLength->setItem(YARDS_IDX, 0, new QTableWidgetItem("0.0"));
   theLength->setItem(YARDS_IDX, 1, new QTableWidgetItem("yd"));

   theLength->setItem(FEET_IDX, 0, new QTableWidgetItem("0.0"));
   theLength->setItem(FEET_IDX, 1, new QTableWidgetItem("ft"));

   theLength->setItem(INCHES_IDX, 0, new QTableWidgetItem("0.0"));
   theLength->setItem(INCHES_IDX, 1, new QTableWidgetItem("in"));

   //    theUnits->addItem(ossimUnitTypeLut::instance()->getEntryString(OSSIM_METERS).c_str());
//    theUnits->addItem(ossimUnitTypeLut::instance()->getEntryString(OSSIM_KILOMETERS).c_str());
//    theUnits->addItem(ossimUnitTypeLut::instance()->getEntryString(OSSIM_CENTIMETERS).c_str());
//    theUnits->addItem(ossimUnitTypeLut::instance()->getEntryString(OSSIM_MILLIMETERS).c_str());
//    theUnits->addItem(ossimUnitTypeLut::instance()->getEntryString(OSSIM_MICRONS).c_str());
//    theUnits->addItem(ossimUnitTypeLut::instance()->getEntryString(OSSIM_MILES).c_str());
//    theUnits->addItem(ossimUnitTypeLut::instance()->getEntryString(OSSIM_YARDS).c_str());
//    theUnits->addItem(ossimUnitTypeLut::instance()->getEntryString(OSSIM_FEET).c_str());
//    theUnits->addItem(ossimUnitTypeLut::instance()->getEntryString(OSSIM_US_SURVEY_FEET).c_str());
//    theUnits->addItem(ossimUnitTypeLut::instance()->getEntryString(OSSIM_INCHES).c_str());
//    theUnits->addItem(ossimUnitTypeLut::instance()->getEntryString(OSSIM_DEGREES).c_str());
//    theUnits->addItem(ossimUnitTypeLut::instance()->getEntryString(OSSIM_MINUTES).c_str());
//    theUnits->addItem(ossimUnitTypeLut::instance()->getEntryString(OSSIM_SECONDS).c_str());
//    theUnits->view()->setWindowFlags(theUnits->view()->windowFlags()|Qt::WindowStaysOnTopHint);
   connect(theMouseNavigationCheckBox, SIGNAL(stateChanged(int)), this, SLOT(mouseNavStateChange(int)));
   setDistance();
   theLength->resizeColumnToContents(1);
//    connect(theUnits->view(), SIGNAL( activated ( const QModelIndex)), this, SLOT(dropDownActivated()));
}

ossimPlanetQtRuler::~ossimPlanetQtRuler()
{
   if(theGlWidget)
   {
      theGlWidget->setMouseNavigationFlag(true);
      theGlWidget->setCursor(Qt::ArrowCursor);
   }
   if(theTool&&thePlanet.valid())   
   {
      if(thePlanet.valid())
      {
         thePlanet->removeChild(theTool);
      }
      theTool->unref();
      theTool = 0;
   }
   theModel = 0;
   thePlanet = 0;
}

void ossimPlanetQtRuler::setPlanet(osg::ref_ptr<ossimPlanet> planet)
{
   if(thePlanet.valid())
   {
      thePlanet->removeChild(theTool);
   }
   thePlanet    = planet.get();
   if(thePlanet.valid())
   {
      thePlanet->addChild(theTool);
      theModel = thePlanet->model().get();
   }
   else
   {
      theModel = 0;
   }
}

void ossimPlanetQtRuler::setGlWidget(ossimPlanetQtViewer* widget)
{
   theGlWidget = widget;
   if(theGlWidget)
   {
      theGlWidget->setMouseTracking(true);
      theGlWidget->setCursor(Qt::CrossCursor);
      theFirstPointSet = false;
      theMouseNavigationCheckBox->blockSignals(true);
      theMouseNavigationCheckBox->setCheckState(theGlWidget->getMouseNavigationFlag()?Qt::Checked:Qt::Unchecked);
      theMouseNavigationCheckBox->blockSignals(false);
      connect(theGlWidget, SIGNAL(signalMouseMoveEvent(QMouseEvent*)),
              this, SLOT(glWidgetSignalMouseMoveEvent(QMouseEvent*)));
      connect(theGlWidget, SIGNAL(signalMouseReleaseEvent(QMouseEvent*)),
              this, SLOT(glWidgetSignalMouseReleaseEvent(QMouseEvent*)));
      connect(theGlWidget, SIGNAL(signalMousePressEvent(QMouseEvent*)),
              this, SLOT(glWidgetSignalMousePressEvent(QMouseEvent*)));
//       setDistance();
   }
}

void ossimPlanetQtRuler::glWidgetSignalMouseMoveEvent(QMouseEvent* e)
{
   if(theFirstPointSet)
   {
      osg::Vec3d pt;
      if(theGlWidget->viewer()->getLatLonHeightAtWindowCoordinate(theEndPointLatLonHgt, e->x(), theGlWidget->height() - e->y()))
      {
         theModel->forward(theEndPointLatLonHgt, theEndPointXYZ);
         setDistance();
         theTool->setPoints(theStartPointXYZ, theEndPointXYZ);
			theTool->setRedrawFlag(true);
      }
   }
}

void ossimPlanetQtRuler::glWidgetSignalMousePressEvent(QMouseEvent* e)
{
  if(!theGlWidget->getMouseNavigationFlag())
   {
      theFirstPointSet = false;
      if(e->button() == Qt::LeftButton)
      {
         if(theGlWidget->viewer()->getLatLonHeightAtWindowCoordinate(theStartPointLatLonHgt, e->x(), theGlWidget->height() - e->y()))
         {
            theFirstPointSet = true;
            theGlWidget->setMouseTracking(true);
            theModel->forward(theStartPointLatLonHgt, theStartPointXYZ);
            theEndPointXYZ = theStartPointXYZ;
            theEndPointLatLonHgt = theStartPointLatLonHgt;
            theTool->setPoints(theStartPointXYZ, theEndPointXYZ);
            theTool->setPointsValidFlag(true);
            setDistance();
         }
      }
   }
}

void ossimPlanetQtRuler::glWidgetSignalMouseReleaseEvent(QMouseEvent* e)
{
   if(e->button() == Qt::LeftButton)
   {
      if(!theFirstPointSet)
      {
         if(theGlWidget->viewer()->getLatLonHeightAtWindowCoordinate(theStartPointLatLonHgt, e->x(), theGlWidget->height() - e->y()))
         {
            theFirstPointSet = true;
            theGlWidget->setMouseTracking(true);
            theModel->forward(theStartPointLatLonHgt, theStartPointXYZ);
            theEndPointXYZ = theStartPointXYZ;
            theEndPointLatLonHgt = theStartPointLatLonHgt;
            theTool->setPoints(theStartPointXYZ, theEndPointXYZ);
            theTool->setPointsValidFlag(true);
				theTool->setRedrawFlag(true);
            setDistance();
         }
      }
      else
      {
         osg::Vec3d endPt;
         if(theGlWidget->viewer()->getLatLonHeightAtWindowCoordinate(endPt, e->x(), theGlWidget->height() - e->y()))
         {
            theEndPointLatLonHgt = endPt; 
            theModel->forward(theEndPointLatLonHgt, theEndPointXYZ);
            setDistance();
            theTool->setPoints(theStartPointXYZ, theEndPointXYZ);
         }
         theGlWidget->setMouseTracking(false);
         theFirstPointSet = false;
      }
   }
}

void ossimPlanetQtRuler::mouseNavStateChange(int state)
{
   if(theGlWidget)
   {
      theGlWidget->setMouseNavigationFlag(theMouseNavigationCheckBox->checkState()!=Qt::Unchecked);
   }
}

void ossimPlanetQtRuler::setDistance()
{
   if(theModel.valid())
   {
      double meters = (theStartPointXYZ-theEndPointXYZ).length()*theModel->getNormalizationScale();
      ossimUnitConversionTool conversionTool(meters, OSSIM_METERS);
      theLength->item(KILOMETERS_IDX, 0)->setText(ossimString::toString(conversionTool.getKilometers()).c_str());
      theLength->item(METERS_IDX, 0)->setText(ossimString::toString(conversionTool.getMeters()).c_str());
      theLength->item(CENTIMETERS_IDX, 0)->setText(ossimString::toString(conversionTool.getCentimeters()).c_str());
      theLength->item(MILLIMETERS_IDX, 0)->setText(ossimString::toString(conversionTool.getMillimeters()).c_str());
      theLength->item(MILES_IDX, 0)->setText(ossimString::toString(conversionTool.getMiles()).c_str());
      theLength->item(YARDS_IDX, 0)->setText(ossimString::toString(conversionTool.getYards()).c_str());
      theLength->item(FEET_IDX, 0)->setText(ossimString::toString(conversionTool.getFeet()).c_str());
      theLength->item(INCHES_IDX, 0)->setText(ossimString::toString(conversionTool.getInches()).c_str());
      theLength->resizeColumnToContents(0);
   }
}

