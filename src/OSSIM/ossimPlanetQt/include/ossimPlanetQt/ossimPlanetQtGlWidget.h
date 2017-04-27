#ifndef  ossimPlanetQtGlWidget_HEADER
#define ossimPlanetQtGlWidget_HEADER
#include <QtOpenGL/QGLWidget>
#include <iostream>
#include <osg/Node>
#include <osg/Timer>
#include <osg/Matrixd>
#include <osg/Material>
#include <osg/FrameStamp>
#include <osgDB/DatabasePager>
#include <osgGA/GUIEventHandler>
#include <osgGA/EventQueue>
#include <osgGA/EventVisitor>
#include <osgDB/ReadFile>
#include <osgUtil/SceneView>
#include "ossimPlanetQtActionAdapter.h"
#include <ossimPlanet/ossimPlanet.h>
#include <ossimPlanet/ossimPlanetSceneView.h>
#include <ossimPlanet/ossimPlanetManipulator.h>
#include <osgDB/DatabasePager>
//#include <ossimPlanet/ossimPlanetDatabasePager.h>
#include <ossimPlanet/ossimPlanetLookAt.h>
#include <ossimPlanet/ossimPlanetViewer.h>
//#include <osgGA/MatrixManipulator>
#include <osgGA/CameraManipulator>
#include <osgGA/StateSetManipulator>
#include <ossimPlanet/ossimPlanetVisitors.h>
/* #include <ossimPlanet/ossimPlanet.h> */
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <OpenThreads/ReentrantMutex>
class ossimOsgMainFormController;

class ossimPlanetQtEmbedded;
class OSSIMPLANETQT_DLL ossimPlanetQtGlWidget : public QGLWidget
{
   Q_OBJECT
public:
   ossimPlanetQtGlWidget( QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 );
   ossimPlanetQtGlWidget( const QGLFormat & format, QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 );
   virtual ~ossimPlanetQtGlWidget();

   osgViewer::GraphicsWindow* getGraphicsWindow() { return theGraphicsWindow.get(); }
   const osgViewer::GraphicsWindow* getGraphicsWindow() const { return theGraphicsWindow.get(); }
   
   void setMouseNavigationFlag(bool flag);
   bool getMouseNavigationFlag()const;
   
   virtual void dropEvent ( QDropEvent * event );
   virtual void dragEnterEvent(QDragEnterEvent *event);
  
protected slots:
   
   void doIdleAnimationFrame();
   
signals:
   void signalMouseMoveEvent(QMouseEvent* event);
   void signalMousePressEvent(QMouseEvent* event);
   void signalMouseReleaseEvent(QMouseEvent* event);
   void signalMouseDoubleClickEvent(QMouseEvent* event);
   void signalViewPositionChangedLatLonHgtHPR(double lat, double lon, double height,
                                              double heading, double pitch, double roll);
   void signalDropEvent(QDropEvent * event);
   void signalDragEnterEvent(QDragEnterEvent *event);
   
protected:
   void init();
   virtual void frameIfNeeded()=0;
   virtual void resizeGL( int width, int height );
   virtual void keyPressEvent( QKeyEvent* event );
   virtual void keyReleaseEvent( QKeyEvent* event );
   virtual void mousePressEvent( QMouseEvent* event );
   virtual void mouseReleaseEvent( QMouseEvent* event );
   virtual void mouseMoveEvent( QMouseEvent* event );
   virtual void mouseDoubleClickEvent ( QMouseEvent * event );
   virtual void wheelEvent ( QWheelEvent * event );
   osgGA::GUIEventAdapter::KeySymbol qtKeyToOsg(QKeyEvent * e)const;
   void addModifiers(osg::ref_ptr<osgGA::EventQueue> eventQueue, Qt::KeyboardModifiers modifier);
   
   osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> theGraphicsWindow;
   bool         theMouseNavigationFlag;
   bool thePassAllUnhandledEventsFlag;
};
class ossimPlanetQtViewer : public ossimPlanetQtGlWidget
{
public:
   ossimPlanetQtViewer(QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0);
   ossimPlanetQtViewer( const QGLFormat & format, QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 );
   virtual ~ossimPlanetQtViewer();
   void setViewer(ossimPlanetViewer* viewer);
   virtual void paintGL();
   
   virtual void frameIfNeeded();
   virtual void mouseMoveEvent( QMouseEvent* event );
   ossimPlanetViewer* viewer();
   void setCurrentSimulationTime(double simtime = USE_REFERENCE_TIME);
   void setTargetFrameTimeInMilliseconds(float millisecondRate);
   void setTargetFramesPerSecond(float framesPerSecond);
protected:
   virtual void resizeGL( int width, int height );
   void clearPointersInViewer();
   QTimer *timer();    // Ensures that qtimer_ exists
   void noRenderCycle();
   OpenThreads::ReentrantMutex theDrawMutex;
//   bool theRequestRedrawFlag;
//   bool theRequestContinuousUpdateFlag;
   int theTimerInterval;
   osg::Matrixd theCurrentViewMatrix;
   osg::Matrixd theCurrentViewMatrixInverse;
   osg::Vec3d   theCurrentViewLlh;
   osg::Vec3d   theCurrentViewHpr;
   osg::ref_ptr<ossimPlanetViewer> theViewer;
   double theCurrentSimulationTime;
   float theFrameRateLimiter;
private:
   QTimer *theQTimer;	// This  should always be accessed via the timer() method
};

#if 0
class OSSIMPLANETQT_DLL ossimPlanetQtGlWidget : public QGLWidget
{
   Q_OBJECT
public:
    /** The RedrawPolicy controls how the window is updated. */
   enum RedrawPolicy
   {
      ossimPlanetQtGlWidgetRedraw_AS_NEEDED=0,      ///< Only redraw as needed (Default), e.g. when a window is popped forward
      ossimPlanetQtGlWidgetRedraw_CONSTANT=1        ///< Redraw constantly
   };
   ossimPlanetQtGlWidget( QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 );
   ossimPlanetQtGlWidget( const QGLFormat & format, QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 );
   virtual ~ossimPlanetQtGlWidget();


   // Action Adapter interface

   void stopClearPaging();
   void clearPager();
   void startPaging();
   void cancelPager();
   
   void setSceneData(osg::Node *scene);
   void setRedrawPolicy(RedrawPolicy);
   
   /** Get the current redraw mode */
   RedrawPolicy getRedrawPolicy() const;
   void setPassAllUnhandledEventsFlag(bool flag);
   void update();
   double qtMouseXToOsg(int)const;
   double qtMouseYToOsg(int)const;
   double qtMouseXToOsgViewport(int)const;
   double qtMouseYToOsgViewport(int)const;

   /* ossimRefPtr<ossimPlanetLookAt> getLookAt();
    */   
   bool intersectSceneQtCoordinates(osg::Vec3d& pt,
                                    double x,// in qt coordinates
                                    double y, // in qt coordinates
                                    double shift=0.0);
   bool intersectSceneOsgViewportCoordinates(osg::Vec3d& pt,
                                             double x, // in normalized osg viewport coordinates
                                             double y, // in normalized osg viewport coordinates
                                             double shift=0.0);
   bool pickObjectsQtCoordinates(osgUtil::IntersectVisitor::HitList& hits,
                                 osg::Node* startNode,
                                 double x, double y,
                                 double startPointShift=0.0);
   bool pickObjectsOsgViewportCoordinates(osgUtil::IntersectVisitor::HitList& hits,
                                          osg::Node* startNode,
                                          double vx, double vy,
                                          double startPointShift = 0.0);
   void getViewPositionLatLonHgtHPR(double& lat, double& lon, double& height,
                                    double& heading, double& pitch, double& roll)const;
   void setMouseNavigationFlag(bool flag);
   bool getMouseNavigationFlag()const;

   osg::ref_ptr<ossimPlanetSceneView> sceneView();

   double getFramesPerSecond()const;
   double getUpdateTimeInSeconds()const;
   double getCullTimeInSeconds()const;
   double getDrawTimeInSeconds()const;
   virtual void dropEvent ( QDropEvent * event );
   virtual void dragEnterEvent(QDragEnterEvent *event);

   const osg::ref_ptr<ossimPlanetManipulator> manipulator()const;
   void playRecording();
   void startRecording();
   void stopRecording();
   void saveRecording(std::ostream& recording);
   bool loadRecording(std::istream& recording);
   
protected slots:

    void doIdleAnimationFrame();

signals:
    void signalMouseMoveEvent(QMouseEvent* event);
    void signalMousePressEvent(QMouseEvent* event);
    void signalMouseReleaseEvent(QMouseEvent* event);
    void signalMouseDoubleClickEvent(QMouseEvent* event);
    void signalDropEvent(QDropEvent * event);
    void signalDragEnterEvent(QDragEnterEvent *event);
    void signalPaintGl();
protected:
   void init();
   virtual void initializeGL ();
   virtual void resizeGL ( int width, int height );
   virtual void paintGL ();
   virtual void keyPressEvent(QKeyEvent * e);
   virtual void keyReleaseEvent(QKeyEvent * e);
   virtual void mouseDoubleClickEvent ( QMouseEvent * e );
   virtual void mouseMoveEvent(QMouseEvent *);
   virtual void mousePressEvent(QMouseEvent *);
   virtual void mouseReleaseEvent(QMouseEvent *);
   virtual void handleGUIActionUpdate();
   QTimer *timer();    // Ensures that qtimer_ exists
   virtual void wheelEvent ( QWheelEvent * e );   
   virtual void moveEvent ( QMoveEvent * event );

   osgGA::GUIEventAdapter::KeySymbol qtKeyToOsg(QKeyEvent * e)const;
   void addModifiers(Qt::KeyboardModifiers modifier);
   
   bool                                    thePassAllUnhandledEventsFlag;
   osg::ref_ptr<osgGA::EventQueue>         theEventQueue;
   osg::ref_ptr<ossimPlanetSceneView>      theSceneView;
   osg::ref_ptr<osgGA::EventVisitor>       theEventVisitor;
   osg::ref_ptr<osgDB::DatabasePager>  theDatabasePager;
   osg::ref_ptr<osg::FrameStamp>           theFrameStamp;
   osg::ref_ptr<osgGA::GUIEventHandler>    theGuiEventHandler;
   osg::ref_ptr<ossimPlanetManipulator>    theMatrixManipulator;
   osg::ref_ptr<osgGA::StateSetManipulator> theStateSetManipulator;
   ossimPlanetQtActionAdapter              theActionAdapter;
   int                                     theTimerId;
   QTimer *theQTimer;	// This  should always be accessed via the timer() method
/*    bool theContinualUpdateFlag; */
   RedrawPolicy theRedrawPolicy;
   osg::ref_ptr<osg::Material> theMaterial;
   
   static osg::Timer theTimer;
   static const osg::Timer_t theInitialTick;
   unsigned int theTimerInterval;
   bool         theMouseNavigationFlag;
   osg::Matrixd theProjToWindowMatrix;
   osg::Matrixd theWindowToProjMatrix;

   mutable double theUpdateTime;
   mutable double theCullTime;
   mutable double theDrawTime;
   mutable double theFrameTime;

   double theViewLat;
   double theViewLon;
   double theViewHgt;
   double theViewHeading;
   double theViewPitch;
   double theViewRoll;

   osg::ref_ptr<ossimPlanet> thePlanet;
/*    bool theFullScreenToggleFlag; */
/*    QWidget* theSavedParent; */
};

#endif
#endif
