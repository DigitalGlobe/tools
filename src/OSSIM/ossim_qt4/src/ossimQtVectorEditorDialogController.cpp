//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimQtVectorEditorDialogController.cpp 20936 2012-05-18 14:45:00Z oscarkramer $
#include <map>
#include <sstream>

#include <Qt3Support/Q3ListBox>
#include <QtGui/QComboBox>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QCheckBox>
#include <Qt3Support/Q3GroupBox>
#include <QtGui/QColorDialog>
#include <Qt3Support/Q3WidgetStack>
#include <ossimQt/ossimQtVectorEditorDialogController.h>
#include <ossimQt/ossimQtVectorEditorDialog.h>
#include <ossim/imaging/ossimVpfTileSource.h>
#include <ossim/vec/ossimVpfLibrary.h>
#include <ossim/base/ossimPropertyEvent.h>
#include <ossim/font/ossimFontFactoryRegistry.h>
#include <ossimQt/ossimQtDisplayUtility.h>
ossimQtVectorEditorDialogController::ossimQtVectorEditorDialogController()
  :theDialog(NULL),
   theVectorSource(NULL),
   theCurrentFeatureIdx(-1)
{
   theTransferingToDialogFlag = false;  
}

ossimQtVectorEditorDialogController::ossimQtVectorEditorDialogController(ossimQtVectorEditorDialog* dialog)
  :theDialog(dialog),
   theVectorSource(NULL)
{
   theTransferingToDialogFlag = false;  
}


ossimQtVectorEditorDialogController::~ossimQtVectorEditorDialogController()
{

}



bool ossimQtVectorEditorDialogController::setObject(ossimConnectableObject* obj)
{
  if(PTR_CAST(ossimVpfTileSource, obj))
    {
      theVectorSource = obj;
    }
  else
    {
      theVectorSource = NULL;
    }

  if(obj)
    {
      if(theVectorSource)
	{
	  initialize();
	}
      return (theVectorSource!=NULL);
    }
  initialize();
  return true;
}

void ossimQtVectorEditorDialogController::changeToFeature(ossim_int32 idx)
{
   theCurrentFeatureIdx = idx;
   
   transferCurrentToDialog();  
}
  

void ossimQtVectorEditorDialogController::initialize()
{
   populateFamilyNames();
  if(PTR_CAST(ossimVpfTileSource, theVectorSource))
    {
      initializeVpfTileSource();
    }
  transferCurrentToDialog();
  transferListToDialog();
}

void ossimQtVectorEditorDialogController::initializeVpfTileSource()
{
  ossimVpfTileSource* vpfTileSource = PTR_CAST(ossimVpfTileSource, theVectorSource);

  if(vpfTileSource)
    {
      vpfTileSource->getAllFeatures(theVpfFeatureList);
      theFeatureInfoList.clear();
      
      for(ossim_uint32 idx = 0; idx < theVpfFeatureList.size();++idx)
	{
	  ossimQtVectorEditorFeatureInfo info;
	  if(theVpfFeatureList[idx]->isPoint())
	    {
	      info.theFeatureType     = ossimQtVectorEditorFeatureInfoType_POINT;
	    }
	  else if(theVpfFeatureList[idx]->isLine())
	    {
 	      info.theFeatureType     = ossimQtVectorEditorFeatureInfoType_LINE;
	    }
	  else if(theVpfFeatureList[idx]->isPolygon())
	    {
 	      info.theFeatureType     = ossimQtVectorEditorFeatureInfoType_POLYGON;
	    }
	  else if(theVpfFeatureList[idx]->isText())
	    {
 	      info.theFeatureType     = ossimQtVectorEditorFeatureInfoType_TEXT;
	    }
	  info.theFeatureId       = (theVpfFeatureList[idx]->getCoverage().getLibrary()->getName() + " " + 
				     theVpfFeatureList[idx]->getCoverage().getName() + " " + 
				     theVpfFeatureList[idx]->getName());
	  info.theColor           = theVpfFeatureList[idx]->getPenColor();
	  info.theThickness       = theVpfFeatureList[idx]->getThickness();
	  info.theFillFlag        = theVpfFeatureList[idx]->getFillEnabledFlag();
	  info.theEnabledFlag     = theVpfFeatureList[idx]->getEnabledFlag();

          info.thePointRadius          = theVpfFeatureList[idx]->getPointRadius();
	  theVpfFeatureList[idx]->getFontInformation(info.theFontInformation);

	  theFeatureInfoList.push_back(info);
	}
    }
  if(theFeatureInfoList.size())
    {
      theCurrentFeatureIdx = 0;
    }
  else
  {
     theCurrentFeatureIdx = -1;
  }
}


void ossimQtVectorEditorDialogController::apply()
{
  if(PTR_CAST(ossimVpfTileSource, theVectorSource))
    {
      applyVpfTileSource();
    }
}

void ossimQtVectorEditorDialogController::applyVpfTileSource()
{
  ossimVpfTileSource* vpfTileSource = PTR_CAST(ossimVpfTileSource, theVectorSource);

  if(vpfTileSource)
    {
       for(ossim_uint32 idx = 0; idx < theVpfFeatureList.size();++idx)
       {
	  theVpfFeatureList[idx]->setEnabledFlag(theFeatureInfoList[idx].theEnabledFlag);
          
	  theVpfFeatureList[idx]->setColor(theFeatureInfoList[idx].theColor);
	  theVpfFeatureList[idx]->setThickness(theFeatureInfoList[idx].theThickness);
          theVpfFeatureList[idx]->setFillEnabledFlag(theFeatureInfoList[idx].theFillFlag);
          theVpfFeatureList[idx]->setFontInformation(theFeatureInfoList[idx].theFontInformation);
	  theVpfFeatureList[idx]->setDrawingFeaturesToAnnotation();
	  theVpfFeatureList[idx]->setPointRadius(theFeatureInfoList[idx].thePointRadius);
       }
       
       vpfTileSource->transformObjects();
       vpfTileSource->computeBoundingRect();
       ossimPropertyEvent propEvt(vpfTileSource);
       vpfTileSource->fireEvent(propEvt);
       vpfTileSource->propagateEventToOutputs(propEvt);
       
       ossimQtDisplayUtility::flushAllOutputs(vpfTileSource);
    }
}


void ossimQtVectorEditorDialogController::transferListToDialog()
{
  theDialog->theFeatureListbox->clear();

  for(int idx = 0; idx < (int)theFeatureInfoList.size(); ++idx)
    {
      theDialog->theFeatureListbox->insertItem(theFeatureInfoList[idx].theFeatureId.c_str());
    }

  if(theFeatureInfoList.size())
    {
       theCurrentFeatureIdx = 0;
       theDialog->theFeatureListbox->setSelected(0, true);
    }
}

void ossimQtVectorEditorDialogController::transferCurrentToDialog()
{
   theTransferingToDialogFlag = true;  
   if((theCurrentFeatureIdx >= 0)&&
      (theCurrentFeatureIdx < (int)theFeatureInfoList.size())&&
      (theDialog))
   {
      switch(theFeatureInfoList[theCurrentFeatureIdx].theFeatureType)
      {
         case ossimQtVectorEditorFeatureInfoType_POINT:
         {
	    theDialog->theFeaturePropertiesGroupBox->setTitle("Point Properties");
            ossimString os = ossimString::toString(theFeatureInfoList[theCurrentFeatureIdx].thePointRadius.x, 5);
            os += " ";
            os += ossimString::toString(theFeatureInfoList[theCurrentFeatureIdx].thePointRadius.y, 5);
	    theDialog->thePointRadiusInput->setText(os.c_str());
            
	    theDialog->thePropertyWidgetStack->raiseWidget(0);
 	    theDialog->thePointEnabledCheckBox->setChecked(theFeatureInfoList[theCurrentFeatureIdx].theEnabledFlag);
            theDialog->thePointFilledCheckBox->setChecked(theFeatureInfoList[theCurrentFeatureIdx].theFillFlag);
	    theDialog->thePointColorFrame->setPaletteBackgroundColor(QColor(theFeatureInfoList[theCurrentFeatureIdx].theColor.getR(),
									    theFeatureInfoList[theCurrentFeatureIdx].theColor.getG(),
									    theFeatureInfoList[theCurrentFeatureIdx].theColor.getB()));
	    break;
         }
         case ossimQtVectorEditorFeatureInfoType_LINE:
         {
	    theDialog->theFeaturePropertiesGroupBox->setTitle("Line Properties");
	    theDialog->thePropertyWidgetStack->raiseWidget(1);
 	    theDialog->theLineEnabledCheckBox->setChecked(theFeatureInfoList[theCurrentFeatureIdx].theEnabledFlag);
	    theDialog->theLineColorFrame->setPaletteBackgroundColor(QColor(theFeatureInfoList[theCurrentFeatureIdx].theColor.getR(),
									   theFeatureInfoList[theCurrentFeatureIdx].theColor.getG(),
									   theFeatureInfoList[theCurrentFeatureIdx].theColor.getB()));
	    theDialog->theLineThicknessInput->setValue(theFeatureInfoList[theCurrentFeatureIdx].theThickness);
	    break;
         }
         case ossimQtVectorEditorFeatureInfoType_POLYGON:
         {
	    theDialog->theFeaturePropertiesGroupBox->setTitle("Polygon Properties");
	    theDialog->thePropertyWidgetStack->raiseWidget(2);
	    theDialog->thePolygonEnabledCheckBox->setChecked(theFeatureInfoList[theCurrentFeatureIdx].theEnabledFlag);
            theDialog->thePolygonFilledCheckBox->setChecked(theFeatureInfoList[theCurrentFeatureIdx].theFillFlag);
	    theDialog->thePolygonColorFrame->setPaletteBackgroundColor(QColor(theFeatureInfoList[theCurrentFeatureIdx].theColor.getR(),
									      theFeatureInfoList[theCurrentFeatureIdx].theColor.getG(),
									      theFeatureInfoList[theCurrentFeatureIdx].theColor.getB()));
	    theDialog->thePolygonThicknessInput->setValue(theFeatureInfoList[theCurrentFeatureIdx].theThickness);
	    break;
         }
         case ossimQtVectorEditorFeatureInfoType_TEXT:
         {
	    theDialog->theFeaturePropertiesGroupBox->setTitle("Text Properties");
	    theDialog->thePropertyWidgetStack->raiseWidget(3);
	    theDialog->theFontEnabledCheckBox->setChecked(theFeatureInfoList[theCurrentFeatureIdx].theEnabledFlag);
	    theDialog->theFontColorFrame->setPaletteBackgroundColor(QColor(theFeatureInfoList[theCurrentFeatureIdx].theColor.getR(),
                                                                           theFeatureInfoList[theCurrentFeatureIdx].theColor.getG(),
                                                                           theFeatureInfoList[theCurrentFeatureIdx].theColor.getB()));
            theDialog->theFontPointSizeSpinBox->setValue(theFeatureInfoList[theCurrentFeatureIdx].theFontInformation.thePointSize.x);
            double rotation = theFeatureInfoList[theCurrentFeatureIdx].theFontInformation.theRotation;
            theDialog->theFontRotationInput->setText(ossimString::toString(rotation).c_str());
            populateStyleWithCurrent();
            theDialog->theFontStyleNameComboBox->setCurrentText(theFeatureInfoList[theCurrentFeatureIdx].theFontInformation.theStyleName.c_str());
            theDialog->theFontFamilyNameComboBox->setCurrentText(theFeatureInfoList[theCurrentFeatureIdx].theFontInformation.theFamilyName.c_str());
            ossimString scaleX = ossimString::toString(theFeatureInfoList[theCurrentFeatureIdx].theFontInformation.theScale.x, 8);
            ossimString scaleY = ossimString::toString(theFeatureInfoList[theCurrentFeatureIdx].theFontInformation.theScale.y, 8);
            ossimString shearX = ossimString::toString(theFeatureInfoList[theCurrentFeatureIdx].theFontInformation.theShear.x, 8);;
            ossimString shearY = ossimString::toString(theFeatureInfoList[theCurrentFeatureIdx].theFontInformation.theShear.y, 8);;
            theDialog->theFontScaleInput->setText((scaleX + " " + scaleY).c_str());
            theDialog->theFontShearInput->setText((shearX + " " + shearY).c_str());
            break;
         }
         default:
         {
	    break;
         }
      }
   }
   theTransferingToDialogFlag = false;  
}

void ossimQtVectorEditorDialogController::transferFromDialogCurrent()
{
  if((theCurrentFeatureIdx >= 0)&&
     (theCurrentFeatureIdx < (int)theFeatureInfoList.size())&&
     (theDialog))
    {
      switch(theFeatureInfoList[theCurrentFeatureIdx].theFeatureType)
	{
	case ossimQtVectorEditorFeatureInfoType_POINT:
	  {
	    stringstream radiusStream(theDialog->thePointRadiusInput->text().ascii());
	    
	    radiusStream >> theFeatureInfoList[theCurrentFeatureIdx].thePointRadius.x
			 >> theFeatureInfoList[theCurrentFeatureIdx].thePointRadius.y;
	    
             theFeatureInfoList[theCurrentFeatureIdx].theEnabledFlag = theDialog->thePointEnabledCheckBox->isChecked();
             theFeatureInfoList[theCurrentFeatureIdx].theFillFlag  = theDialog->thePointFilledCheckBox->isChecked();
             QColor qColor = theDialog->thePointColorFrame->paletteBackgroundColor();
             ossimRgbVector color(qColor.red(),
                                  qColor.green(),
                                  qColor.blue());
             theFeatureInfoList[theCurrentFeatureIdx].theColor = color;
             break;
	  }
	case ossimQtVectorEditorFeatureInfoType_LINE:
	  {
 	    theFeatureInfoList[theCurrentFeatureIdx].theEnabledFlag = theDialog->theLineEnabledCheckBox->isChecked();
	    QColor qColor = theDialog->theLineColorFrame->paletteBackgroundColor();
	    ossimRgbVector color(qColor.red(),
				 qColor.green(),
				 qColor.blue());
	    theFeatureInfoList[theCurrentFeatureIdx].theColor = color;
            theFeatureInfoList[theCurrentFeatureIdx].theThickness = theDialog->theLineThicknessInput->value();
	    break;
	  }
	case ossimQtVectorEditorFeatureInfoType_POLYGON:
	  {
             theFeatureInfoList[theCurrentFeatureIdx].theEnabledFlag = theDialog->thePolygonEnabledCheckBox->isChecked();
             theFeatureInfoList[theCurrentFeatureIdx].theFillFlag  = theDialog->thePolygonFilledCheckBox->isChecked();
             QColor qColor = theDialog->thePolygonColorFrame->paletteBackgroundColor();
             ossimRgbVector color(qColor.red(),
                                  qColor.green(),
                                  qColor.blue());
             theFeatureInfoList[theCurrentFeatureIdx].theColor = color;
             theFeatureInfoList[theCurrentFeatureIdx].theThickness = theDialog->thePolygonThicknessInput->value();
	    break;
	  }
	case ossimQtVectorEditorFeatureInfoType_TEXT:
	  {
 	    theFeatureInfoList[theCurrentFeatureIdx].theEnabledFlag = theDialog->theFontEnabledCheckBox->isChecked();
             QColor qColor = theDialog->theFontColorFrame->paletteBackgroundColor();
             ossimRgbVector color(qColor.red(),
                                  qColor.green(),
                                  qColor.blue());
             theFeatureInfoList[theCurrentFeatureIdx].theColor = color;
             theFeatureInfoList[theCurrentFeatureIdx].theFontInformation.thePointSize.x = theDialog->theFontPointSizeSpinBox->value();
             theFeatureInfoList[theCurrentFeatureIdx].theFontInformation.thePointSize.y = theDialog->theFontPointSizeSpinBox->value();
             theFeatureInfoList[theCurrentFeatureIdx].theFontInformation.theRotation    = ossimString(theDialog->theFontRotationInput->text().ascii()).toDouble();
             theFeatureInfoList[theCurrentFeatureIdx].theFontInformation.theStyleName   = ossimString(theDialog->theFontStyleNameComboBox->currentText().ascii());
             theFeatureInfoList[theCurrentFeatureIdx].theFontInformation.theFamilyName  = ossimString(theDialog->theFontFamilyNameComboBox->currentText().ascii());
             std::stringstream scaleStream(theDialog->theFontScaleInput->text().ascii());
             std::stringstream shearStream(theDialog->theFontShearInput->text().ascii());
             scaleStream >> theFeatureInfoList[theCurrentFeatureIdx].theFontInformation.theScale.x
                         >> theFeatureInfoList[theCurrentFeatureIdx].theFontInformation.theScale.y;
             shearStream >> theFeatureInfoList[theCurrentFeatureIdx].theFontInformation.theShear.x
                         >> theFeatureInfoList[theCurrentFeatureIdx].theFontInformation.theShear.y;
	    break;
	  }
	default:
	  {
	    break;
	  }
	}
    }
  if(theDialog->theAutoApplyCheckBox->isChecked())
  {
     apply();
  }
}


void ossimQtVectorEditorDialogController::pointColor()
{
    bool ok;
    QRgb rgb = QColorDialog::getRgba(theDialog->thePointColorFrame->paletteBackgroundColor().rgb(), &ok, theDialog);
    if(ok)
    {
	theDialog->thePointColorFrame->setPaletteBackgroundColor(QColor(rgb));
	transferFromDialogCurrent();
    }
}

void ossimQtVectorEditorDialogController::lineColor()
{
    bool ok;
    QRgb rgb = QColorDialog::getRgba(theDialog->theLineColorFrame->paletteBackgroundColor().rgb(), &ok, theDialog);
    if(ok)
    {
	theDialog->theLineColorFrame->setPaletteBackgroundColor(QColor(rgb));
	transferFromDialogCurrent();
    }
}

void ossimQtVectorEditorDialogController::polygonColor()
{
    bool ok;
    QRgb rgb = QColorDialog::getRgba(theDialog->thePolygonColorFrame->paletteBackgroundColor().rgb(), &ok, theDialog);
    if(ok)
    {
	theDialog->thePolygonColorFrame->setPaletteBackgroundColor(QColor(rgb));
	transferFromDialogCurrent();
    }
}

void ossimQtVectorEditorDialogController::fontColor()
{
    bool ok;
    QRgb rgb = QColorDialog::getRgba(theDialog->theFontColorFrame->paletteBackgroundColor().rgb(), &ok, theDialog);
    if(ok)
    {
	theDialog->theFontColorFrame->setPaletteBackgroundColor(QColor(rgb));
	transferFromDialogCurrent();
    }
}

void ossimQtVectorEditorDialogController::pointEnabledToggled()
{
   if(!theTransferingToDialogFlag)
   {
      transferFromDialogCurrent();
   }
}

void ossimQtVectorEditorDialogController::lineEnabledToggled()
{
   if(!theTransferingToDialogFlag)
   {
      transferFromDialogCurrent();
   }
}

void ossimQtVectorEditorDialogController::polygonEnabledToggled()
{
   if(!theTransferingToDialogFlag)
   {
      transferFromDialogCurrent();
   }
}

void ossimQtVectorEditorDialogController::fontEnabledToggled()
{
   if(!theTransferingToDialogFlag)
   {
      transferFromDialogCurrent();
   }
}

void ossimQtVectorEditorDialogController::lineThicknessChanged()
{
   if(!theTransferingToDialogFlag)
   {
      transferFromDialogCurrent();
   }
   
}

void ossimQtVectorEditorDialogController::polygonThicknessChanged()
{
   if(!theTransferingToDialogFlag)
   {
      transferFromDialogCurrent();
   }
}


void ossimQtVectorEditorDialogController::polygonFilledToggled()
{
   if(!theTransferingToDialogFlag)
   {
      transferFromDialogCurrent();
   }
}

void ossimQtVectorEditorDialogController::pointFilledToggled()
{
   if(!theTransferingToDialogFlag)
   {
      transferFromDialogCurrent();
   }
}

void ossimQtVectorEditorDialogController::fontFamilyName()
{
   if(!theTransferingToDialogFlag)
   {
      transferFromDialogCurrent();
   }
}

void ossimQtVectorEditorDialogController::fontPointSize()
{
   if(!theTransferingToDialogFlag)
   {
      transferFromDialogCurrent();
   }
}

void ossimQtVectorEditorDialogController::fontRotation()
{
   if(!theTransferingToDialogFlag)
   {
      transferFromDialogCurrent();
   }
}

void ossimQtVectorEditorDialogController::fontShear()
{
   if(!theTransferingToDialogFlag)
   {
      transferFromDialogCurrent();
   }
}

void ossimQtVectorEditorDialogController::fontScale()
{
   if(!theTransferingToDialogFlag)
   {
      transferFromDialogCurrent();
   }
}

void ossimQtVectorEditorDialogController::fontStyle()
{
   if(!theTransferingToDialogFlag)
   {
      transferFromDialogCurrent();
   }
}

void ossimQtVectorEditorDialogController::disableAll()
{
   bool needUpdate = false;
   for(int idx = 0; idx < (int)theFeatureInfoList.size();++idx)
   {
      if(theFeatureInfoList[idx].theEnabledFlag)
      {
         needUpdate = true;
      }
      
      theFeatureInfoList[idx].theEnabledFlag = false;
   }
   transferCurrentToDialog();

   if(needUpdate)
   {
      if(theDialog->theAutoApplyCheckBox->isChecked())
      {
         apply();
      }
   }
}

void ossimQtVectorEditorDialogController::enableAll()
{
   bool needUpdate = false;
   for(int idx = 0; idx < (int)theFeatureInfoList.size();++idx)
   {
      if(!theFeatureInfoList[idx].theEnabledFlag)
      {
         needUpdate = true;
      }
      
      theFeatureInfoList[idx].theEnabledFlag = true;
   }
   transferCurrentToDialog();

   if(needUpdate)
   {
      if(theDialog->theAutoApplyCheckBox->isChecked())
      {
         apply();
      }
   }
}

void ossimQtVectorEditorDialogController::populateStyleWithCurrent()
{
   theDialog->theFontStyleNameComboBox->clear();
   std::vector<ossimFontInformation> fontInformationList;
   
   ossimFontFactoryRegistry::instance()->getFontInformationFamilyName(fontInformationList,
                                                                      theFeatureInfoList[theCurrentFeatureIdx].theFontInformation.theFamilyName);

   for(int idx = 0; idx < (int)fontInformationList.size(); ++idx)
   {
      theDialog->theFontStyleNameComboBox->insertItem(fontInformationList[idx].theStyleName.c_str());
   }
   
}

void ossimQtVectorEditorDialogController::populateFamilyNames()
{
   theDialog->theFontFamilyNameComboBox->clear();
   std::vector<ossimFontInformation> fontInformationList;
   std::map<ossimString, ossimString, ossimStringLtstr> uniqueMap;
   ossimFontFactoryRegistry::instance()->getFontInformation(fontInformationList);

   for(int idx = 0; idx < (int)fontInformationList.size(); ++idx)
   {
      if(uniqueMap.find(fontInformationList[idx].theFamilyName.c_str()) == uniqueMap.end())
      {
         theDialog->theFontFamilyNameComboBox->insertItem(fontInformationList[idx].theFamilyName.c_str());
         uniqueMap.insert(make_pair(fontInformationList[idx].theFamilyName.c_str(),
                          ossimString("")));
      }
   }
}
