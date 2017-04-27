#include <ossimPlanetQt/ossimPlanetQtLayerOperation.h>
#include <QtGui/QMessageBox>

static const int NO_SHADER_INDEX = 0;
static const int TOP_INDEX = NO_SHADER_INDEX + 1;
static const int REFERENCE_INDEX = TOP_INDEX + 1;
static const int OPACITY_INDEX = REFERENCE_INDEX + 1;
static const int HORIZONTAL_SWIPE_INDEX = OPACITY_INDEX + 1;
static const int VERTICAL_SWIPE_INDEX = HORIZONTAL_SWIPE_INDEX + 1;
static const int BOX_SWIPE_INDEX = VERTICAL_SWIPE_INDEX + 1;
static const int CIRCLE_SWIPE_INDEX =  BOX_SWIPE_INDEX + 1;
static const int ABSOLUTE_DIFFERENCE_INDEX = CIRCLE_SWIPE_INDEX + 1;
static const int FALSE_COLOR_REPLACEMENT_INDEX = ABSOLUTE_DIFFERENCE_INDEX + 1;
static const char* operationTypeText []=
{
 "NO SHADER",
 "Top",
 "Reference",
 "Opacity",
 "Horizontal Swipe",
 "Vertical Swipe",
 "Box Swipe",
 "Circle Swipe",
 "Absolute Difference",
 "False Color Replacement"};
   
ossimPlanetQtLayerOperation::ossimPlanetQtLayerOperation(QWidget *parent)
   :QDialog(parent)
{
   setupUi(this);
   theOperationType->addItem(operationTypeText[NO_SHADER_INDEX]);
   theOperationType->addItem(operationTypeText[TOP_INDEX]);
   theOperationType->addItem(operationTypeText[REFERENCE_INDEX]);
   theOperationType->addItem(operationTypeText[OPACITY_INDEX]);
   theOperationType->addItem(operationTypeText[HORIZONTAL_SWIPE_INDEX]);
   theOperationType->addItem(operationTypeText[VERTICAL_SWIPE_INDEX]);
   theOperationType->addItem(operationTypeText[BOX_SWIPE_INDEX]);
   theOperationType->addItem(operationTypeText[CIRCLE_SWIPE_INDEX]);
   theOperationType->addItem(operationTypeText[ABSOLUTE_DIFFERENCE_INDEX]);
   theOperationType->addItem(operationTypeText[FALSE_COLOR_REPLACEMENT_INDEX]);
  
}

#if 0
void ossimPlanetQtLayerOperation::setLand(osg::ref_ptr<ossimPlanetLand> land)
{
   theLand = land.get();
   if(theLand.valid())
   {
      if(theLand->getCurrentShader().valid())
      {
         populateControl();
      }
      else
      {
         QMessageBox::warning(this, "Layer Operation Warning", "GL Shader support not available", QMessageBox::Ok, QMessageBox::Ok);
      }
   }
}
#endif

//osg::ref_ptr<ossimPlanetLand> ossimPlanetQtLayerOperation::land()
//{
//   return theLand.get();
//}

//const osg::ref_ptr<ossimPlanetLand> ossimPlanetQtLayerOperation::land()const
//{
//   return theLand.get();
//}

void ossimPlanetQtLayerOperation::populateControl()
{
   blockSignals(true);
#if 0
   if(theLand.get())
   {
      ossimPlanetShaderProgramSetup::ossimPlanetFragmentShaderType fragType =
         theLand->getCurrentShader()->fragmentType();

      switch(fragType)
      {
         case ossimPlanetShaderProgramSetup::NO_SHADER:
         {
            theOperationType->setCurrentIndex(NO_SHADER_INDEX);
            theParameterStack->setCurrentWidget(theParameterStack->widget(2));
            break;
         }
         case ossimPlanetShaderProgramSetup::TOP:
         {
            theOperationType->setCurrentIndex(TOP_INDEX);
            theParameterStack->setCurrentWidget(theParameterStack->widget(2));
            break;
         }
         case ossimPlanetShaderProgramSetup::REFERENCE:
         {
            theOperationType->setCurrentIndex(REFERENCE_INDEX);
            theParameterStack->setCurrentWidget(theParameterStack->widget(2));
            break;
         }
         case ossimPlanetShaderProgramSetup::OPACITY:
         {
            theOperationType->setCurrentIndex(OPACITY_INDEX);
            
            theParameterStack->setCurrentWidget(theParameterStack->widget(0));
            osg::ref_ptr<osg::Uniform> param = theLand->getCurrentShader()->getUniform("param");
            if(param.valid())
            {
               float minValue = theOpacitySlider->minimum();
               float maxValue = theOpacitySlider->maximum();
               float v = .5;
               param->get(v);
               theOpacitySlider->setSliderPosition((int)(minValue + (maxValue-minValue)*v));
            }
            break;
         }
         case ossimPlanetShaderProgramSetup::HORIZONTAL_SWIPE:
         {
            theOperationType->setCurrentIndex(HORIZONTAL_SWIPE_INDEX);
            theParameterStack->setCurrentWidget(theParameterStack->widget(1));
             osg::ref_ptr<osg::Uniform> param = theLand->getCurrentShader()->getUniform("param");
            if(param.valid())
            {
               float minValue = theSwipeSlider->minimum();
               float maxValue = theSwipeSlider->maximum();
               float v = .5;
               param->get(v);
               theSwipeSlider->setSliderPosition((int)(minValue + (maxValue-minValue)*v));
            }       
            break;
         }
         case ossimPlanetShaderProgramSetup::VERTICAL_SWIPE:
         {
            theOperationType->setCurrentIndex(VERTICAL_SWIPE_INDEX);
            theParameterStack->setCurrentWidget(theParameterStack->widget(1));
             osg::ref_ptr<osg::Uniform> param = theLand->getCurrentShader()->getUniform("param");
            if(param.valid())
            {
               float minValue = theSwipeSlider->minimum();
               float maxValue = theSwipeSlider->maximum();
               float v = .5;
               param->get(v);
               theSwipeSlider->setSliderPosition((int)(minValue + (maxValue-minValue)*v));
            }       
            break;
         }
         case ossimPlanetShaderProgramSetup::BOX_SWIPE:
         {
            theOperationType->setCurrentIndex(BOX_SWIPE_INDEX);
            theParameterStack->setCurrentWidget(theParameterStack->widget(1));
             osg::ref_ptr<osg::Uniform> param = theLand->getCurrentShader()->getUniform("param");
            if(param.valid())
            {
               float minValue = theSwipeSlider->minimum();
               float maxValue = theSwipeSlider->maximum();
               float v = .5;
               param->get(v);
               theSwipeSlider->setSliderPosition((int)(minValue + (maxValue-minValue)*v));
            }       
            break;
         }
         case ossimPlanetShaderProgramSetup::CIRCLE_SWIPE:
         {
            theOperationType->setCurrentIndex(CIRCLE_SWIPE_INDEX);
           theParameterStack->setCurrentWidget(theParameterStack->widget(1));
             osg::ref_ptr<osg::Uniform> param = theLand->getCurrentShader()->getUniform("param");
            if(param.valid())
            {
               float minValue = theSwipeSlider->minimum();
               float maxValue = theSwipeSlider->maximum();
               float v = .5;
               param->get(v);
               theSwipeSlider->setSliderPosition((int)(minValue + (maxValue-minValue)*v));
            }       
            break;
         }
         case ossimPlanetShaderProgramSetup::ABSOLUTE_DIFFERENCE:
         {
            theOperationType->setCurrentIndex(ABSOLUTE_DIFFERENCE_INDEX);
            theParameterStack->setCurrentWidget(theParameterStack->widget(2));
            break;
         }
         case ossimPlanetShaderProgramSetup::FALSE_COLOR_REPLACEMENT:
         {
            theOperationType->setCurrentIndex(FALSE_COLOR_REPLACEMENT_INDEX);
            theParameterStack->setCurrentWidget(theParameterStack->widget(2));
            break;
         }
         default:
         {
            break;
         }
      }
   }
#endif
   blockSignals(false);
}

void ossimPlanetQtLayerOperation::on_theOperationType_activated(const QString& v)
{
#if 0
   if(!theLand.valid()) return;

   if(v == operationTypeText[NO_SHADER_INDEX])
   {
      theLand->setCurrentFragmentShaderType(ossimPlanetShaderProgramSetup::NO_SHADER);
   }
   else if(v == operationTypeText[TOP_INDEX])
   {
      theLand->setCurrentFragmentShaderType(ossimPlanetShaderProgramSetup::TOP);
   }
   else if(v == operationTypeText[REFERENCE_INDEX])
   {
      theLand->setCurrentFragmentShaderType(ossimPlanetShaderProgramSetup::REFERENCE);
   }
   else if(v == operationTypeText[OPACITY_INDEX])
   {
      theLand->setCurrentFragmentShaderType(ossimPlanetShaderProgramSetup::OPACITY);
   }
   else if(v == operationTypeText[HORIZONTAL_SWIPE_INDEX])
   {
      theLand->setCurrentFragmentShaderType(ossimPlanetShaderProgramSetup::HORIZONTAL_SWIPE);
   }
   else if(v == operationTypeText[VERTICAL_SWIPE_INDEX])
   {
      theLand->setCurrentFragmentShaderType(ossimPlanetShaderProgramSetup::VERTICAL_SWIPE);
   }
   else if(v == operationTypeText[BOX_SWIPE_INDEX])
   {
      theLand->setCurrentFragmentShaderType(ossimPlanetShaderProgramSetup::BOX_SWIPE);
   }
   else if(v == operationTypeText[CIRCLE_SWIPE_INDEX])
   {
      theLand->setCurrentFragmentShaderType(ossimPlanetShaderProgramSetup::CIRCLE_SWIPE);
   }
   else if(v == operationTypeText[ABSOLUTE_DIFFERENCE_INDEX])
   {
      theLand->setCurrentFragmentShaderType(ossimPlanetShaderProgramSetup::ABSOLUTE_DIFFERENCE);
   }
   else if(v == operationTypeText[FALSE_COLOR_REPLACEMENT_INDEX])
   {
       theLand->setCurrentFragmentShaderType(ossimPlanetShaderProgramSetup::FALSE_COLOR_REPLACEMENT);
   }
   
   populateControl();
#endif
}

void ossimPlanetQtLayerOperation::on_theOpacitySlider_valueChanged(int value)
{
#if 0
   if(theLand.valid())
   {
      float minValue = theOpacitySlider->minimum();
      float maxValue = theOpacitySlider->maximum();
      
      float v = ((value - minValue)/
                 (maxValue-minValue));

      osg::ref_ptr<osg::Uniform> param = theLand->getCurrentShader()->getUniform("param");
      if(param.valid())
      {
         param->set(v);
         theLand->setRedrawFlag(true);
      }
   }
#endif
}

void ossimPlanetQtLayerOperation::on_theSwipeSlider_valueChanged(int value)
{
#if 0
   if(theLand.valid())
   {
      float minValue = theSwipeSlider->minimum();
      float maxValue = theSwipeSlider->maximum();
      
      float v = ((value - minValue)/
                 (maxValue-minValue));

      osg::ref_ptr<osg::Uniform> param = theLand->getCurrentShader()->getUniform("param");
      if(param.valid())
      {
         param->set(v);
         theLand->setRedrawFlag(true);
      }
   }
#endif
}

