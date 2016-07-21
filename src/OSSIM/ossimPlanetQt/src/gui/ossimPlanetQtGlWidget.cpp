#include <ossimPlanetQt/ossimPlanetQtGlWidget.h>
#include <QtGui/QCursor>
#include <QtCore/QTimer>
#include <QtCore/QRect>
#include <QtCore/QEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QLayout>
#include <QtGui/QFrame>
#include <ossimPlanet/ul.h>
#include <ossimPlanet/ossimPlanetTerrain.h>
#include <OpenThreads/ScopedLock>
#include <osgUtil/LineSegmentIntersector>
#include <osg/io_utils>
#include <osgGA/GUIEventAdapter>

ossimPlanetQtGlWidget::ossimPlanetQtGlWidget( QWidget * parent, const QGLWidget * shareWidget, Qt::WindowFlags f)
:QGLWidget(parent, shareWidget, f),
theMouseNavigationFlag(true),
thePassAllUnhandledEventsFlag(true)
{
   init();
}

ossimPlanetQtGlWidget::ossimPlanetQtGlWidget( const QGLFormat & format, QWidget * parent, const QGLWidget * shareWidget, Qt::WindowFlags f )
:QGLWidget(format, parent, shareWidget, f),
theMouseNavigationFlag(true),
thePassAllUnhandledEventsFlag(true)
{
   init();
}

ossimPlanetQtGlWidget::~ossimPlanetQtGlWidget()
{
}

void ossimPlanetQtGlWidget::init()
{
   theGraphicsWindow = new osgViewer::GraphicsWindowEmbedded(0,0,width(),height());
   setAcceptDrops(true);
}

void ossimPlanetQtGlWidget::doIdleAnimationFrame()
{
   frameIfNeeded();
}

void ossimPlanetQtGlWidget::setMouseNavigationFlag(bool flag)
{
   theMouseNavigationFlag = flag;
}

bool ossimPlanetQtGlWidget::getMouseNavigationFlag()const
{
   return theMouseNavigationFlag;
}

void ossimPlanetQtGlWidget::resizeGL( int width, int height )
{
   width = ossim::max(64, width);
   height = ossim::max(64, height);
   theGraphicsWindow->getEventQueue()->windowResize(0, 0, width, height );
   theGraphicsWindow->resized(0,0,width,height);
}

void ossimPlanetQtGlWidget::keyPressEvent( QKeyEvent* event )
{
   theGraphicsWindow->getEventQueue()->keyPress( qtKeyToOsg(event) );
   addModifiers(theGraphicsWindow->getEventQueue(), event->modifiers());
   if(thePassAllUnhandledEventsFlag)
   {
      event->ignore();
   }
}

void ossimPlanetQtGlWidget::keyReleaseEvent( QKeyEvent* event )
{
   theGraphicsWindow->getEventQueue()->keyRelease( qtKeyToOsg(event) );
   addModifiers(theGraphicsWindow->getEventQueue(), event->modifiers());
   if(thePassAllUnhandledEventsFlag)
   {
      event->ignore();
   }
}

void ossimPlanetQtGlWidget::mousePressEvent( QMouseEvent* event )
{
   int button = 0;
   switch(event->button())
   {
      case(Qt::LeftButton): button = 1; break;
      case(Qt::MidButton): button = 2; break;
      case(Qt::RightButton): button = 3; break;
      case(Qt::NoButton): button = 0; break;
      default: button = 0; break;
   }
   theGraphicsWindow->getEventQueue()->mouseButtonPress(event->x(), event->y(), button);
   addModifiers(theGraphicsWindow->getEventQueue(), event->modifiers());
   if(thePassAllUnhandledEventsFlag)
   {
      event->ignore();
   }
   emit signalMousePressEvent(event);
}

void ossimPlanetQtGlWidget::mouseReleaseEvent( QMouseEvent* event )
{
   int button = 0;
   switch(event->button())
   {
      case(Qt::LeftButton): button = 1; break;
      case(Qt::MidButton): button = 2; break;
      case(Qt::RightButton): button = 3; break;
      case(Qt::NoButton): button = 0; break;
      default: button = 0; break;
   }
   theGraphicsWindow->getEventQueue()->mouseButtonRelease(event->x(), event->y(), button);
   addModifiers(theGraphicsWindow->getEventQueue(), event->modifiers());
   if(thePassAllUnhandledEventsFlag)
   {
      event->ignore();
   }
   emit signalMouseReleaseEvent(event);
}

void ossimPlanetQtGlWidget::mouseMoveEvent( QMouseEvent* event )
{
   theGraphicsWindow->getEventQueue()->mouseMotion(event->x(), event->y());
   addModifiers(theGraphicsWindow->getEventQueue(), event->modifiers());
   emit signalMouseMoveEvent(event);
   
}

void ossimPlanetQtGlWidget::mouseDoubleClickEvent( QMouseEvent * event )
{
   int button = 0;
   switch(event->button())
   {
      case(Qt::LeftButton): button = 1; break;
      case(Qt::MidButton): button = 2; break;
      case(Qt::RightButton): button = 3; break;
      case(Qt::NoButton): button = 0; break;
      default: button = 0; break;
   }
   theGraphicsWindow->getEventQueue()->mouseDoubleButtonPress(event->x(), event->y(), button);
   addModifiers(theGraphicsWindow->getEventQueue(), event->modifiers());
   if(thePassAllUnhandledEventsFlag)
   {
      event->ignore();
   }
}

void ossimPlanetQtGlWidget::wheelEvent(QWheelEvent * event)
{
   theGraphicsWindow->getEventQueue()->mouseScroll(event->delta()>0?osgGA::GUIEventAdapter::SCROLL_UP:osgGA::GUIEventAdapter::SCROLL_DOWN);
   addModifiers(theGraphicsWindow->getEventQueue(), event->modifiers());
   if(thePassAllUnhandledEventsFlag)
   {
      event->ignore();
   }
}

void ossimPlanetQtGlWidget::dropEvent ( QDropEvent * event )
{
   emit signalDropEvent(event);
}

void ossimPlanetQtGlWidget::dragEnterEvent(QDragEnterEvent *event)
{
   emit signalDragEnterEvent(event);
}

osgGA::GUIEventAdapter::KeySymbol ossimPlanetQtGlWidget::qtKeyToOsg(QKeyEvent * e)const
{
   int qtKey = e->key();
   switch(qtKey)
   {
      case Qt::Key_Up:
      {
         return(osgGA::GUIEventAdapter::KEY_Up);
      }
      case Qt::Key_Down:
      {
         return (osgGA::GUIEventAdapter::KEY_Down);
      }
      case Qt::Key_Left:
      {
         return (osgGA::GUIEventAdapter::KEY_Left);
      }
      case Qt::Key_Right:
      {
         return (osgGA::GUIEventAdapter::KEY_Right);
      }
      case Qt::Key_Return:
      {
         return (osgGA::GUIEventAdapter::KEY_Return);
      }
      default:
      {
         if((qtKey >= Qt::Key_A)&&(qtKey <= Qt::Key_Z))
         {
            QString s = e->text();
            std::string stdString = s.toStdString();
            char c = *stdString.begin();
            return (osgGA::GUIEventAdapter::KeySymbol)(c);  
         }
      }
   }
   
   return (osgGA::GUIEventAdapter::KeySymbol)(qtKey);
}

void ossimPlanetQtGlWidget::addModifiers(osg::ref_ptr<osgGA::EventQueue> eventQueue, Qt::KeyboardModifiers modifier)
{
   if(!eventQueue.valid()) return;
   unsigned int modKeyMask = 0;
   osgGA::GUIEventAdapter* adapter = eventQueue->getCurrentEventState();
   if(!adapter) return;
   if(modifier & Qt::ShiftModifier)
   {
      modKeyMask|=osgGA::GUIEventAdapter::MODKEY_SHIFT;
   }
   if(modifier & Qt::ControlModifier)
   {
      modKeyMask|=osgGA::GUIEventAdapter::MODKEY_CTRL;
   }
   if(modifier & Qt::AltModifier)
   {
      modKeyMask|=osgGA::GUIEventAdapter::MODKEY_ALT;
   }
   if(modifier & Qt::MetaModifier)
   {
      modKeyMask|=osgGA::GUIEventAdapter::MODKEY_META;
   }
   adapter->setModKeyMask(modKeyMask);
}

ossimPlanetQtViewer::ossimPlanetQtViewer(QWidget * parent, const QGLWidget * shareWidget, Qt::WindowFlags f)
:ossimPlanetQtGlWidget( parent, shareWidget, f ),
theTimerInterval(0),
theQTimer(0)
{
   setViewer(new ossimPlanetViewer);
   setCurrentSimulationTime();
   setTargetFramesPerSecond(60);
   theCurrentSimulationTime = USE_REFERENCE_TIME;
   timer()->setInterval(theTimerInterval);
   timer()->setSingleShot(false);
   timer()->start();
}

ossimPlanetQtViewer::ossimPlanetQtViewer( const QGLFormat & format, QWidget * parent, const QGLWidget * shareWidget, Qt::WindowFlags f )
:ossimPlanetQtGlWidget(format, parent, shareWidget, f),
theTimerInterval(0),
theQTimer(0)
{
   setViewer(new ossimPlanetViewer);
   setCurrentSimulationTime();
   setTargetFramesPerSecond(60);
   theCurrentSimulationTime = USE_REFERENCE_TIME;
   timer()->setInterval(theTimerInterval);
   timer()->setSingleShot(false);
   timer()->start();
}

ossimPlanetQtViewer::~ossimPlanetQtViewer()
{
   clearPointersInViewer(); 
}

void ossimPlanetQtViewer::setViewer(ossimPlanetViewer* viewer)
{
   clearPointersInViewer();
   ossim_uint32 idx = 0;
   theViewer = viewer;
   if(theViewer.valid())
   {
      theViewer->getCamera()->setGraphicsContext(getGraphicsWindow());
      osgViewer::ViewerBase::Cameras cameraList;
      theViewer->getCameras(cameraList);
      for(idx = 0; idx < cameraList.size();++idx)
      {
         cameraList[idx]->setGraphicsContext(getGraphicsWindow());
      }
      // initialize some size for the view port for this canvas if it doesn't have any
      //
      int w=width(), h=height();
      
      // let's default to something
      w = w>0?w:10;
      h = h>0?h:10;
      if((w > 0)&&(h>0))
      {
         theViewer->getCamera()->setViewport(new osg::Viewport(0,0,w,h));
        
        // theViewer->getCamera()->setProjectionMatrixAsPerspective(0.286321, 
        //                                                          static_cast<double>(w)/
         //                                                         static_cast<double>(h), 
         //                                                         0.0000007839, 
         //                                                         50);
         theViewer->getCamera()->setProjectionMatrixAsPerspective(45, 
                                                                  static_cast<double>(w)/
                                                                  static_cast<double>(h), 
                                                                  1, 
                                                                  50);
      }
   }
}

void ossimPlanetQtViewer::resizeGL( int width, int height )
{
   width = ossim::max(64, width);
   height = ossim::max(64, height);
   ossimPlanetQtGlWidget::resizeGL(width, height);
   if(theViewer.valid()&&theViewer->getCamera())
   {
      theViewer->getCamera()->getViewport()->setViewport(0,0,width,height);
      double fovy, aspectRatio, znear, zfar;
      theViewer->getCamera()->getProjectionMatrixAsPerspective(fovy, aspectRatio, znear, zfar);
      aspectRatio = static_cast<double>(width)/static_cast<double>(height);
      theViewer->getCamera()->setProjectionMatrixAsPerspective(fovy, 
                                                               aspectRatio, 
                                                               znear, 
                                                               zfar);
   }
}

void ossimPlanetQtViewer::clearPointersInViewer()
{
   if(theViewer.valid())
   {
      ossim_uint32 idx = 0;
      osgViewer::ViewerBase::Cameras cameraList;
      theViewer->getCameras(cameraList);
      for(idx = 0; idx < cameraList.size();++idx)
      {
         cameraList[idx]->setGraphicsContext(0);
      }
   }
}
void ossimPlanetQtViewer::setTargetFrameTimeInMilliseconds(float millisecondRate)
{
   theFrameRateLimiter = millisecondRate;
}

void ossimPlanetQtViewer::setTargetFramesPerSecond(float framesPerSecond)
{
   setTargetFrameTimeInMilliseconds(1000.0/framesPerSecond);
}

QTimer *ossimPlanetQtViewer::timer()
{
   if(!theQTimer)
   {
      theQTimer=new QTimer(this);
      connect(theQTimer,SIGNAL(timeout()),this,SLOT(doIdleAnimationFrame()));
   }
   return theQTimer;
}

#if 0
void ossimPlanetQtViewer::requestRedraw()
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theDrawMutex);
   theRequestRedrawFlag = true;
   if(!timer()->isActive())
   {
      timer()->setInterval(theTimerInterval);
      timer()->start();
   }
}
#endif
#if 0
void ossimPlanetQtViewer::requestContinuousUpdate(bool needed)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theDrawMutex);
   theRequestContinuousUpdateFlag = needed;
   if(needed)
   {
      if(!timer()->isActive())
      {
        timer()->setInterval(theTimerInterval);
        timer()->start();
      }
   }
}
#endif

ossimPlanetViewer* ossimPlanetQtViewer::viewer()
{
   return theViewer.get();
}

void ossimPlanetQtViewer::setCurrentSimulationTime(double simtime)
{
   theCurrentSimulationTime = simtime; 
}

void ossimPlanetQtViewer::frameIfNeeded()
{
   //OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theDrawMutex);
   bool doVBlankLimit = false;
   osg::Timer_t beginT = osg::Timer::instance()->tick();
   osg::Timer_t endT;
   if(theViewer->continuousUpdateFlag()||
      theViewer->getAndSetRedrawFlag(false))
   {
      updateGL();
      doVBlankLimit = format().swapInterval() < 0;
      endT = osg::Timer::instance()->tick();
  }
   else
   {
      noRenderCycle();
      doVBlankLimit = true;
      endT = osg::Timer::instance()->tick();
   }
   if(doVBlankLimit)
   {
     double test = osg::Timer::instance()->delta_m(beginT, endT);
     if(test < theFrameRateLimiter)
      {
         ulMilliSecondSleep((int)(theFrameRateLimiter-test));
      }
   }
#if 0
   std::cout << "---" << std::endl;
   std::cout << (getDatabasePager()->requiresUpdateSceneGraph()||getDatabasePager()->requiresCompileGLObjects()) << std::endl;
   std::cout << getDatabasePager()->getFileRequestListSize() << std::endl
            << getDatabasePager()->getDataToCompileListSize() << std::endl;
#endif
}

void ossimPlanetQtViewer::noRenderCycle()//double simtime)
{
   if(!theViewer)return;
   theViewer->advance(theCurrentSimulationTime);
   getGraphicsWindow()->getEventQueue()->frame(theViewer->getViewerFrameStamp()->getReferenceTime());
   theViewer->eventTraversal();
   theViewer->updateTraversal();
}

void ossimPlanetQtViewer::paintGL()
{
   if(theViewer.valid())
   {
      theViewer->frame(theCurrentSimulationTime);
   }
}

void ossimPlanetQtViewer::mouseMoveEvent( QMouseEvent* event )
{
   ossimPlanetQtGlWidget::mouseMoveEvent(event);
}
