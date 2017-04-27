#include <ossimPlanetQt/ossimPlanetQtVideoControlWidget.h>
#include <iostream>

ossimPlanetQtVideoControlWidget::ossimPlanetQtVideoControlWidget(QWidget * parent,
                                                                 Qt::WindowFlags f)
   :QWidget(parent, f)
{
   setupUi(this);
//    theCallback = new ossimPlanetQtVideoControlCallback((Ui::ossimPlanetQtVideoControlWidget*)this);
}

ossimPlanetQtVideoControlWidget::~ossimPlanetQtVideoControlWidget()
{
   if(theVideoLayerNode.valid())
   {
//       theVideoLayerNode->removeCallback(theCallback.get());
//       theCallback->theVideoLayerNode = 0;
   }
}

void ossimPlanetQtVideoControlWidget::setVideoLayerNode(osg::ref_ptr<ossimPlanetVideoLayerNode> videoNode)
{
   if(theVideoLayerNode.valid())
   {
//       theVideoLayerNode->removeCallback(theCallback.get());
      // need to unset callbacks
   }
   
   theVideoLayerNode = videoNode;
//    theCallback->theUi = this;
//    theCallback->theVideoLayerNode = videoNode;
   
   init();
}

void ossimPlanetQtVideoControlWidget::init()
{
   theReferenceTimeSlider->setSliderPosition(0);
   if(theVideoLayerNode.valid())
   {
//       theCallback->theSliderValue = (int)theVideoLayerNode->referenceTime();
//       theReferenceTimeSlider->setRange(0, (int)theVideoLayerNode->duration());
//       theReferenceTimeSlider->setValue(theCallback->theSliderValue);
//       theVideoLayerNode->addCallback(theCallback.get());

      switch(theVideoLayerNode->status())
      {
         case ossimPlanetVideoLayerNode::STATUS_PLAYING:
         {
            thePlayButton->setText("||");  
            break;
         }
         case ossimPlanetVideoLayerNode::STATUS_PAUSED:
         {
            thePlayButton->setText(">");
            break;
         }
         default:
         {
            thePlayButton->setText(">");
            break;
         }
      }
   }
}

void ossimPlanetQtVideoControlWidget::on_theReferenceTimeSlider_valueChanged(int value)
{
   if(theVideoLayerNode.valid())
   {
     theVideoLayerNode->setReferenceTime(value);
   }
}

void ossimPlanetQtVideoControlWidget::on_theReferenceTimeSlider_sliderPressed()
{
   if(theCallback.valid())
   {
      theCallback->setEnableFlag(false);
   }
}

void ossimPlanetQtVideoControlWidget::on_theReferenceTimeSlider_sliderReleased()
{
   if(theCallback.valid())
   {
      theCallback->setEnableFlag(true);
   }
}

void ossimPlanetQtVideoControlWidget::on_thePlayButton_clicked(bool)
{
   if(!theVideoLayerNode.valid()) return;
   if(thePlayButton->text() == "||")
   {
      thePlayButton->setText(">");
      theVideoLayerNode->pause();
   }
   else
   {
      thePlayButton->setText("||");
      theVideoLayerNode->play();
   }
}

void ossimPlanetQtVideoControlWidget::on_theFrameStepLeftButton_clicked(bool)
{
   
}

void ossimPlanetQtVideoControlWidget::on_theFrameStepRightButton_clicked(bool)
{
}
