#ifndef ossimPlanetQtVideoControlWidget_HEADER
#define ossimPlanetQtVideoControlWidget_HEADER
#include "ui_ossimPlanetQtVideoControlWidget.h"
#include <QtGui/QWidget>
#include "ossimPlanetQtExport.h"
#include <ossimPlanet/ossimPlanetVideoLayerNode.h>

/* class ossimPlanetQtVideoControlCallback : public ossimPlanetVideoLayerNode::Callback */
/* { */
/* public: */
/*    ossimPlanetQtVideoControlCallback(Ui::ossimPlanetQtVideoControlWidget* ui) */
/*       :theUi(ui) */
/*    { */
/*    } */

/*    virtual void propertyChanged(const ossimString& name) */
/*    { */
/*       if(!theUi||!theVideoLayerNode.valid()) return; */
/*    } */
/*    osg::ref_ptr<ossimPlanetVideoLayerNode> theVideoLayerNode; */
/*    Ui::ossimPlanetQtVideoControlWidget* theUi; */
/*    int theSliderValue; */
/* }; */

class OSSIMPLANETQT_DLL ossimPlanetQtVideoControlWidget : public QWidget,
                                                          public Ui::ossimPlanetQtVideoControlWidget
{
   Q_OBJECT
public:
   ossimPlanetQtVideoControlWidget(QWidget * parent = 0,
                                   Qt::WindowFlags f = 0 );
   virtual ~ossimPlanetQtVideoControlWidget();
   void setVideoLayerNode(osg::ref_ptr<ossimPlanetVideoLayerNode> videoNode);
   osg::ref_ptr<ossimPlanetVideoLayerNode> videoLayerNode()
   {
      return theVideoLayerNode;
   }
   const osg::ref_ptr<ossimPlanetVideoLayerNode> videoLayerNode()const
   {
      return theVideoLayerNode;
   }
   void setCallback(osg::ref_ptr<ossimPlanetNodeCallback> callback)
   {
      theCallback = callback;
   }
protected slots:
   virtual void on_theReferenceTimeSlider_valueChanged(int value);
   virtual void on_theReferenceTimeSlider_sliderPressed();
   virtual void on_theReferenceTimeSlider_sliderReleased();
   virtual void on_thePlayButton_clicked(bool checked = false);
   virtual void on_theFrameStepLeftButton_clicked(bool checked = false);
   virtual void on_theFrameStepRightButton_clicked(bool checked = false);
   
protected:
   virtual void init();
   osg::ref_ptr<ossimPlanetVideoLayerNode> theVideoLayerNode;
   osg::ref_ptr<ossimPlanetNodeCallback> theCallback;
};
#endif
