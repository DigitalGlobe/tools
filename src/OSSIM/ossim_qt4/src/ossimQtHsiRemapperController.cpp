//*******************************************************************
// Copyright (C) 2003 ImageLinks Inc. 
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
// Author:  David Burken <dburken@imagelinks.com>
// 
// Description:
// Main interface between the hsi remapper dialog box and the hsi remapper
// from the image chain.
//
//*************************************************************************
// $Id: ossimQtHsiRemapperController.cpp 12141 2007-12-07 18:05:13Z gpotts $

#include <vector>
using namespace std;

#include <QtGui/QCheckBox>
#include <QtCore/QStringList>
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>
#include <Qt3Support/Q3ButtonGroup>
#include <QtGui/QSlider>
#include <QtGui/QLabel>
// #include "ossimQtListBox.h"
#include <ossimQt/ossimQtHsiRemapperController.h>
#include <ossimQt/ossimQtHsiRemapperDialog.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/imaging/ossimHsiRemapper.h>
#include <ossim/base/ossimPropertyEvent.h>
#include <ossim/base/ossimTrace.h>
#include <ossimQt/ossimQtDisplayUtility.h>

static ossimTrace traceDebug("ossimQtHsiRemapperController:degug");

ossimQtHsiRemapperController::ossimQtHsiRemapperController(ossimQtHsiRemapperDialog* dialog)
   :
      ossimConnectableObjectListener(),
      theDialog(dialog),
      theHsiRemapper(NULL),
      theSelectedGroup(ossimQtHsiRemapperController::ALL),
      theCallBackDisabled(false)
{
}

ossimQtHsiRemapperController::~ossimQtHsiRemapperController()
{
   if (theHsiRemapper)
   {
      theHsiRemapper->removeListener((ossimConnectableObjectListener*)this);
      theHsiRemapper = NULL;
   }
}

void ossimQtHsiRemapperController::objectDestructingEvent(ossimObjectDestructingEvent& event)
{
   if (theHsiRemapper)
   {
      if(event.getObject() == PTR_CAST(ossimObject, theHsiRemapper))
      {      
         theDialog->close(true);
      }
   }
}

void ossimQtHsiRemapperController::setHsiRemapper(ossimHsiRemapper* remapper)
{
   if(theHsiRemapper)
   {
      theHsiRemapper->removeListener((ossimConnectableObjectListener*)this);
   }
   
   theHsiRemapper = remapper;
   
   if(theHsiRemapper)
   {
      theHsiRemapper->addListener((ossimConnectableObjectListener*)this);
   }

   updateSliders();
}

void ossimQtHsiRemapperController::redButtonClicked()
{
   if (theSelectedGroup != ossimQtHsiRemapperController::RED)
   {
      theSelectedGroup = ossimQtHsiRemapperController::RED;
      updateSliders();
   }
}

void ossimQtHsiRemapperController::yellowButtonClicked()
{
   if (theSelectedGroup != ossimQtHsiRemapperController::YELLOW)
   {
      theSelectedGroup = ossimQtHsiRemapperController::YELLOW;
      updateSliders();
   }
}

void ossimQtHsiRemapperController::greenButtonClicked()
{
   if (theSelectedGroup != ossimQtHsiRemapperController::GREEN)
   {
      theSelectedGroup = ossimQtHsiRemapperController::GREEN;
      updateSliders();
   }
}

void ossimQtHsiRemapperController::cyanButtonClicked()
{
   if (theSelectedGroup != ossimQtHsiRemapperController::CYAN)
   {
      theSelectedGroup = ossimQtHsiRemapperController::CYAN;
      updateSliders();
   }
}

void ossimQtHsiRemapperController::blueButtonClicked()
{
   if (theSelectedGroup != ossimQtHsiRemapperController::BLUE)
   {
      theSelectedGroup = ossimQtHsiRemapperController::BLUE;
      updateSliders();
   }
}

void ossimQtHsiRemapperController::magentaButtonClicked()
{
   if (theSelectedGroup != ossimQtHsiRemapperController::MAGENTA)
   {
      theSelectedGroup = ossimQtHsiRemapperController::MAGENTA;
      updateSliders();
   }
}

void ossimQtHsiRemapperController::allButtonClicked()
{
   if (theSelectedGroup != ossimQtHsiRemapperController::ALL)
   {
      theSelectedGroup = ossimQtHsiRemapperController::ALL;
      updateSliders();
   }
}

void ossimQtHsiRemapperController::hueOffsetValueChanged(int value) const
{
   if (!theHsiRemapper || theCallBackDisabled) return;

   double new_value = value;
   double old_value = theHsiRemapper->getHueOffset(theSelectedGroup);
      
   if (old_value != new_value)
   {
      theHsiRemapper->setHueOffset(theSelectedGroup, new_value);
      setHueOffsetSlider();
      refresh();
   }
}

void ossimQtHsiRemapperController::hueLowRangeValueChanged(int value) const
{
   if (!theHsiRemapper ||
       theSelectedGroup == ossimQtHsiRemapperController::ALL ||
       theCallBackDisabled) return;

   double new_value = value;
   double old_value = theHsiRemapper->getHueLowRange(theSelectedGroup);
      
   if (old_value != new_value)
   {
      theHsiRemapper->setHueLowRange(theSelectedGroup, new_value);
      setHueLowRangeSlider();
      
      // Check to see if value took, if not, reset the slider.
      old_value = theHsiRemapper->getHueLowRange(theSelectedGroup);
      if (old_value == new_value)
      {
         // value took
         refresh();
      }
   }
}

void ossimQtHsiRemapperController::hueHighRangeValueChanged(int value) const
{
   if (!theHsiRemapper ||
       theSelectedGroup == ossimQtHsiRemapperController::ALL ||
       theCallBackDisabled) return;

   double new_value = value;
   double old_value = theHsiRemapper->getHueHighRange(theSelectedGroup);
      
   if (old_value != new_value)
   {
      theHsiRemapper->setHueHighRange(theSelectedGroup, new_value);
      setHueHighRangeSlider();
      
      // Check to see if value took, if not, reset the slider.
      old_value = theHsiRemapper->getHueHighRange(theSelectedGroup);
      if (old_value == new_value)
      {
         // value took
         refresh();
      }
   }
}

void ossimQtHsiRemapperController::hueBlendRangeValueChanged(int value) const
{
   if (!theHsiRemapper ||
       theSelectedGroup == ossimQtHsiRemapperController::ALL ||
       theCallBackDisabled) return;
   
   double new_value = value;
   double old_value = theHsiRemapper->getHueBlendRange(theSelectedGroup);
      
   if (old_value != new_value)
   {
      theHsiRemapper->setHueBlendRange(theSelectedGroup, new_value);
      setHueBlendRangeSlider();
      
      // Check to see if value took, if not, reset the slider.
      old_value = theHsiRemapper->getHueBlendRange(theSelectedGroup);
      if (old_value == new_value)
      {
         // value took
         refresh();
      }
   }
}

void ossimQtHsiRemapperController::saturationOffsetValueChanged(int value) const
{
   if (!theHsiRemapper || theCallBackDisabled) return;

   double new_value = value;
   new_value = new_value / 200.0;  // -200.0 == -1.0, 200 == 1.0
   double old_value = theHsiRemapper->getSaturationOffset(theSelectedGroup);
      
   if (old_value != new_value)
   {
      theHsiRemapper->setSaturationOffset(theSelectedGroup, new_value);
      setSaturationOffsetSlider();
      refresh();
   }
}

void ossimQtHsiRemapperController::intensityOffsetValueChanged(int value) const
{
   if (!theHsiRemapper || theCallBackDisabled) return;

   double new_value = value;
   new_value = new_value / 200.0;  // -200.0 == -1.0, 200 == 1.0
   double old_value = theHsiRemapper->getIntensityOffset(theSelectedGroup);
      
   if (old_value != new_value)
   {
      theHsiRemapper->setIntensityOffset(theSelectedGroup, new_value);
      setIntensityOffsetSlider();
      refresh();
   }
}

void ossimQtHsiRemapperController::lowIntensityClipValueChanged(int value) const
{
   if (!theHsiRemapper || theCallBackDisabled) return;
   
   double new_value = value;
   new_value = new_value / 400.0;  // 400 == 1.0, 0 == 0.0
   double old_value = theHsiRemapper->getMasterIntensityLowClip();
      
   if (old_value != new_value)
   {
      theHsiRemapper->setMasterIntensityLowClip(new_value);
      setIntensityLowClipSlider();

      old_value = theHsiRemapper->getMasterIntensityLowClip();
      if (old_value == new_value)
      {
         refresh();
      }
   }
}

void ossimQtHsiRemapperController::highIntensityClipValueChanged(int value) const
{
   if (!theHsiRemapper || theCallBackDisabled) return;
   
   double new_value = value;
   new_value = new_value / 400.0; // 400 == 1.0, 0 == 0.0
   double old_value = theHsiRemapper->getMasterIntensityHighClip();
      
   if (old_value != new_value)
   {
      theHsiRemapper->setMasterIntensityHighClip(new_value);
      setIntensityHighClipSlider();
      
      // Make sure it took, if not reset the slider.
      old_value = theHsiRemapper->getMasterIntensityHighClip();
      if (old_value == new_value)
      {
         refresh();
      }
   }
}

void ossimQtHsiRemapperController::whiteObjectClipValueChanged(int value) const
{
   if (!theHsiRemapper || theCallBackDisabled) return;
   
   double new_value = value;
   new_value = new_value / 400.0;  // 400 == 1
   double old_value = theHsiRemapper->getWhiteObjectClip();
      
   if (old_value != new_value)
   {
      theHsiRemapper->setWhiteObjectClip(new_value);
      setWhiteObjectClipSlider();
      
      // Make sure it took, if not reset the slider.
      old_value = theHsiRemapper->getWhiteObjectClip();
      if (old_value == new_value)
      {
         refresh();
      }
   }
}

void ossimQtHsiRemapperController::enableButtonClicked() const
{
   if (!theHsiRemapper) return;

   bool current_state = theHsiRemapper->getEnableFlag();
   bool new_state = theDialog->theEnableButton->isChecked();

   if (current_state != new_state)
   {
      theHsiRemapper->setEnableFlag(new_state);
      refresh();
   }
}

void ossimQtHsiRemapperController::resetGroupButtonClicked()
{
   if (!theHsiRemapper) return;

   bool old_state = theHsiRemapper->getEnableFlag();

   theHsiRemapper->resetGroup(theSelectedGroup);
   updateSliders();

   bool new_state = theHsiRemapper->getEnableFlag();

   if (old_state || new_state) // If both disabled don't refresh.
   {
      refresh();
   }
}

void ossimQtHsiRemapperController::resetAllButtonClicked()
{
   if (!theHsiRemapper) return;

   bool old_state = theHsiRemapper->getEnableFlag();

   theHsiRemapper->resetAll();
   updateSliders();

   bool new_state = theHsiRemapper->getEnableFlag();

   if (old_state || new_state) // If both disabled don't refresh.
   {
      refresh();
   }
}

int ossimQtHsiRemapperController::getColorGroupId() const
{
   QAbstractButton* button = theDialog->theColorRangeButtonGroup->selected();
   return theDialog->theColorRangeButtonGroup->id(button);
}

void ossimQtHsiRemapperController::updateSliders()
{
   //---
   // Since playing with sliders can trigger the valueChanged set this
   // so that the receivers of the valueChanged event don't execute.
   //---
   theCallBackDisabled = true;
   
   if (theSelectedGroup == ossimQtHsiRemapperController::ALL)
   {
      // Disable the range sliders.
      theDialog->theHueLowRangeSlider->setDisabled(true);
      theDialog->theHueHighRangeSlider->setDisabled(true);
      theDialog->theHueBlendRangeSlider->setDisabled(true);

      // Enable the low, high and white object clip sliders.
      theDialog->theLowIntensityClipSlider->setEnabled(true);
      theDialog->theHighIntensityClipSlider->setEnabled(true);
      theDialog->theWhiteObjectClipSlider->setEnabled(true);
   }
   else
   {
      // Enable the range sliders.
      theDialog->theHueLowRangeSlider->setEnabled(true);
      theDialog->theHueHighRangeSlider->setEnabled(true);
      theDialog->theHueBlendRangeSlider->setEnabled(true);

      // Disable the low, high and white object clip sliders.
      theDialog->theLowIntensityClipSlider->setDisabled(true);
      theDialog->theHighIntensityClipSlider->setDisabled(true);
      theDialog->theWhiteObjectClipSlider->setDisabled(true);
   }

   switch (theSelectedGroup)
   {
      case ossimQtHsiRemapperController::RED:
         theDialog->theHueLowRangeSlider->setRange(-45, 15);
         theDialog->theHueHighRangeSlider->setRange(-15, 45);
         break;

      case ossimQtHsiRemapperController::YELLOW:
         theDialog->theHueLowRangeSlider->setRange(15, 75);
         theDialog->theHueHighRangeSlider->setRange(45, 105);
         break;

      case ossimQtHsiRemapperController::GREEN:
         theDialog->theHueLowRangeSlider->setRange(75, 135);
         theDialog->theHueHighRangeSlider->setRange(105, 165);
         break;

      case ossimQtHsiRemapperController::CYAN:
         theDialog->theHueLowRangeSlider->setRange(135, 195);
         theDialog->theHueHighRangeSlider->setRange(165, 225);
         break;

      case ossimQtHsiRemapperController::BLUE:
         theDialog->theHueLowRangeSlider->setRange(195, 255);
         theDialog->theHueHighRangeSlider->setRange(225, 285);
         break;

      case ossimQtHsiRemapperController::MAGENTA:
         theDialog->theHueLowRangeSlider->setRange(255, 315);
         theDialog->theHueHighRangeSlider->setRange(285, 345);
         break;

      default:
         break;
   }
   
   // Set individual slider values.
   setHueOffsetSlider();
   setHueLowRangeSlider();
   setHueHighRangeSlider();
   setHueBlendRangeSlider();
   setSaturationOffsetSlider();
   setIntensityOffsetSlider();
   setIntensityLowClipSlider();
   setIntensityHighClipSlider();
   setWhiteObjectClipSlider();
   setEnableCheckBox();

   // Enable the valueChanged event methods...
   theCallBackDisabled = false;
}

void ossimQtHsiRemapperController::setHueOffsetSlider() const
{
   if (!theHsiRemapper) return;

   double d = theHsiRemapper->getHueOffset(theSelectedGroup);

   int value = static_cast<int>(d);

   theDialog->theHueOffsetSlider->setValue(value);
   QString s = ossimString::toString(d, 0).c_str();
   theDialog->theHueOffsetValueLabel->setText(s);
}

void ossimQtHsiRemapperController::setHueLowRangeSlider() const
{
   if (!theHsiRemapper || !theDialog->theHueLowRangeSlider->isEnabled())
   {
      return;
   }

   double d = theHsiRemapper->getHueLowRange(theSelectedGroup);

   int value = static_cast<int>(d);

   theDialog->theHueLowRangeSlider->setValue(value);
   QString s = ossimString::toString(d, 0).c_str();
   theDialog->theHueLowRangeValueLabel->setText(s); 
}

void ossimQtHsiRemapperController::setHueHighRangeSlider() const
{
   if (!theHsiRemapper || !theDialog->theHueHighRangeSlider->isEnabled())
   {
      return;
   }

   double d = theHsiRemapper->getHueHighRange(theSelectedGroup);

   int value = static_cast<int>(d);

   theDialog->theHueHighRangeSlider->setValue(value);
   QString s = ossimString::toString(d, 0).c_str();
   theDialog->theHueHighRangeValueLabel->setText(s); 
}

void ossimQtHsiRemapperController::setHueBlendRangeSlider() const
{
   if (!theHsiRemapper || !theDialog->theHueBlendRangeSlider->isEnabled())
   {
      return;
   }

   double d = theHsiRemapper->getHueBlendRange(theSelectedGroup);

   int value = static_cast<int>(d);

   theDialog->theHueBlendRangeSlider->setValue(value);
   QString s = ossimString::toString(d, 0).c_str();
   theDialog->theHueBlendRangeValueLabel->setText(s);  
}

void ossimQtHsiRemapperController::setSaturationOffsetSlider() const
{
   if (!theHsiRemapper) return;

   double d = theHsiRemapper->getSaturationOffset(theSelectedGroup);

   int value = static_cast<int>(d*200.0);

   theDialog->theSaturationOffsetSlider->setValue(value);
   QString s = ossimString::toString(d, 2).c_str();
   theDialog->theSaturationOffsetValueLabel->setText(s); 
}

void ossimQtHsiRemapperController::setIntensityOffsetSlider() const
{
    if (!theHsiRemapper) return;

    double d = theHsiRemapper->getIntensityOffset(theSelectedGroup);

   int value = static_cast<int>(d*200.0);

   theDialog->theIntensityOffsetSlider->setValue(value);
   QString s = ossimString::toString(d, 2).c_str();
   theDialog->theIntensityOffsetValueLabel->setText(s);   
}

void ossimQtHsiRemapperController::setIntensityLowClipSlider() const
{
   if (!theHsiRemapper || !theDialog->theLowIntensityClipSlider->isEnabled())
   {
      return;
   }
   
   double d = theHsiRemapper->getMasterIntensityLowClip();

   int value = static_cast<int>(d*400.0);

   theDialog->theLowIntensityClipSlider->setValue(value);
   QString s = ossimString::toString(d, 2).c_str();
   theDialog->theLowIntensityClipValueLabel->setText(s); 
}

void ossimQtHsiRemapperController::setIntensityHighClipSlider() const
{
   if (!theHsiRemapper || !theDialog->theHighIntensityClipSlider->isEnabled())
   {
      return;
   }
   
   double d = theHsiRemapper->getMasterIntensityHighClip();

   int value = static_cast<int>(d*400.0);

   theDialog->theHighIntensityClipSlider->setValue(value);
   QString s = ossimString::toString(d, 2).c_str();
   theDialog->theHighIntensityClipValueLabel->setText(s);  
}

void ossimQtHsiRemapperController::setWhiteObjectClipSlider() const
{
   if (!theHsiRemapper || !theDialog->theWhiteObjectClipSlider->isEnabled())
   {
      return;
   }
   
   double d = theHsiRemapper->getWhiteObjectClip();

   int value = static_cast<int>(d*400.0);

   theDialog->theWhiteObjectClipSlider->setValue(value);
   QString s = ossimString::toString(d, 2).c_str();
   theDialog->theWhiteObjectClipValueLabel->setText(s);  
}

void ossimQtHsiRemapperController::setEnableCheckBox() const
{
   if (!theHsiRemapper) return;
   
   bool state = theHsiRemapper->getEnableFlag();
   theDialog->theEnableButton->setChecked(state);
}

void ossimQtHsiRemapperController::refresh() const
{
   if (theHsiRemapper)
   {
      setEnableCheckBox();
      ossimQtDisplayUtility::flushAllOutputs(theHsiRemapper);
   }
}

// Private to disallow copy constructor, assignment operator use...
ossimQtHsiRemapperController::ossimQtHsiRemapperController(const ossimQtHsiRemapperController&)
   :
      ossimConnectableObjectListener(),
      theDialog(NULL),
      theHsiRemapper(NULL),
      theSelectedGroup(ossimQtHsiRemapperController::ALL),
      theCallBackDisabled(false)
{
}

ossimQtHsiRemapperController& ossimQtHsiRemapperController::operator=(const ossimQtHsiRemapperController&)
{
   return *this;
}
