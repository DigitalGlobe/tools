//*******************************************************************
// Copyright (C) 2003 ImageLinks Inc. 
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
// 
// Description:
// 
// Main interface between the histogram remapper dialog box and the histogram
// remapper from the image chain.
//
//*************************************************************************
// $Id: ossimQtHistogramController.cpp 23664 2015-12-14 14:17:27Z dburken $

#include <vector>
using namespace std;

#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>
#include <QtCore/QStringList>
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>
#include <Qt3Support/Q3ButtonGroup>
#include <QtGui/QSlider>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <ossimQt/ossimQtHistogramController.h>
#include <ossimQt/ossimQtHistogramDialog.h>
#include <ossimQt/ossimQtHistogramWidget.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/imaging/ossimHistogramRemapper.h>
#include <ossim/base/ossimPropertyEvent.h>
#include <ossim/base/ossimTrace.h>
#include <ossimQt/ossimQtDisplayUtility.h>

ossimFilename ossimQtHistogramController::theLatestOutputFilePath = ".";

static ossimTrace traceDebug("ossimQtHistogramController:degug");
ossimQtHistogramController::ossimQtHistogramController(ossimQtHistogramDialog* dialog)
   :
      ossimConnectableObjectListener(),
      theDialog(dialog),
      theHistogramRemapper(NULL),
      theCallBackDisabled(false),
      theStretchModeComboBoxItem(0),
      theBandComboBoxItem(0)
{
}

ossimQtHistogramController::~ossimQtHistogramController()
{
   if (theHistogramRemapper)
   {
      theHistogramRemapper->removeListener((ossimConnectableObjectListener*)this);
      theHistogramRemapper = NULL;
   }
}

void ossimQtHistogramController::objectDestructingEvent(ossimObjectDestructingEvent& event)
{
   if (theHistogramRemapper)
   {
      if(event.getObject() == PTR_CAST(ossimObject, theHistogramRemapper))
      {      
         theDialog->close(true);
      }
   }
}

ossimFilename ossimQtHistogramController::getLatestOutputFilePath()
{
   return theLatestOutputFilePath;
}

void ossimQtHistogramController::setHistogramRemapper(ossimHistogramRemapper* remapper)
{
   if(theHistogramRemapper)
   {
      theHistogramRemapper->removeListener((ossimConnectableObjectListener*)this);
   }
   
   theHistogramRemapper = remapper;
   
   if(theHistogramRemapper)
   {
      theHistogramRemapper->addListener((ossimConnectableObjectListener*)this);
   }

   updateDialog();
}

void ossimQtHistogramController::setHistogramFile(const ossimFilename& file)
{
   // Grab the latest directory.
   theLatestOutputFilePath = file.path();
   
   if (theHistogramRemapper)
   {
      if (theHistogramRemapper->openHistogram(file) && theDialog)
      {
         // Rebuild up theBandComboBox menu.
         QString item = "master";
         theDialog->theBandComboBox->clear();
         theDialog->theBandComboBox->insertItem(item);

         const ossim_uint32 BANDS
            = theHistogramRemapper->getNumberOfOutputBands();
         for (ossim_uint32 band = 0; band < BANDS; ++band)
         {
            item = ossimString::toString(band+1).c_str();
            theDialog->theBandComboBox->insertItem(item);
         }
         // This will paint an average of all bands on the widget.
         theDialog->theHistogramWidget->setPaintMode(
            ossimQtHistogramWidget::PAINT_AVG);

         updateDialog();

         refresh();
      }
      else
      {
         // reset the file
         updateDialog();
         
         QString caption("Warning:");
         QString text = file.c_str();
         text += " could not be opened!";
         
         // Give the user an already open
         QMessageBox::warning( theDialog,
                               caption,
                               text,
                               QMessageBox::Ok,
                               Qt::NoButton);
      }
   }
}

void ossimQtHistogramController::applyButtonClicked() const
{
   if (!theHistogramRemapper) return;
}

void ossimQtHistogramController::resetButtonClicked()
{
   if (!theHistogramRemapper) return;

   bool old_state = theHistogramRemapper->getEnableFlag();
   
   theHistogramRemapper->reset();

   updateDialog();

   if (old_state != theHistogramRemapper->getEnableFlag())
   {
      refresh();
   }
}

void ossimQtHistogramController::enableCheckBoxClicked() const
{
   if (!theHistogramRemapper) return;

   bool current_state = theHistogramRemapper->getEnableFlag();
   bool new_state = theDialog->theEnableCheckBox->isChecked();

   if (current_state != new_state)
   {
      theHistogramRemapper->setEnableFlag(new_state);
      refresh();
   }
}

void ossimQtHistogramController::bandComboBoxActivated(int item)
{
   if (!theHistogramRemapper) return;

   if (item != theBandComboBoxItem)
   {
      theBandComboBoxItem = item;
      const ossim_uint32 BAND = getBand();
      if (BAND == ossimQtHistogramController::MASTER)
      {
         // Setting paint mode forces an update.
         theDialog->theHistogramWidget->setPaintMode(
            ossimQtHistogramWidget::PAINT_AVG);
      }
      else
      {
         // Setting paint mode forces an update.         
         theDialog->theHistogramWidget->setBand(BAND);
         theDialog->theHistogramWidget->setPaintMode(
            ossimQtHistogramWidget::PAINT_BAND);
      }
      updateDialog();
   }
}

void ossimQtHistogramController::stretchModeComboBoxActivated(int item)
{
   if (theHistogramRemapper && !theCallBackDisabled)
   {
      if (item != theStretchModeComboBoxItem)
      {
         // Assuming user wants the filter enabled since the mode was changed.
         theHistogramRemapper->setEnableFlag(true);
         
         theStretchModeComboBoxItem = item;
         ossimHistogramRemapper::StretchMode mode =
            static_cast<ossimHistogramRemapper::StretchMode>(item);
         
         //---
         // The true argument is for rebuild table now so the dialog will get
         // updated correctly.
         //---
         theHistogramRemapper->setStretchMode(mode, true);
         
         // Update the dialog.
         updateDialog();
         
         // Force a repaint.
         refresh();
      }
   }
}

void ossimQtHistogramController::lowClipPercentReturnPressed()
{
   if (!theHistogramRemapper || theCallBackDisabled) return;

   QString s = theDialog->theLowClipPercentLineEdit->text();
   ossimString os = s.toStdString();
   ossim_float64 d = os.toDouble();
   d = d / 100.0;

   bool dirty = false;
   const ossim_uint32 BAND = getBand();
   if (BAND == ossimQtHistogramController::MASTER)
   {
      if (d != theHistogramRemapper->getLowNormalizedClipPoint())
      {
         dirty = true;
         theHistogramRemapper->setLowNormalizedClipPoint(d);
      }
   }
   else
   {
      if (d != theHistogramRemapper->getLowNormalizedClipPoint(BAND))
      {
         dirty = true;
         theHistogramRemapper->setLowNormalizedClipPoint(d, BAND);
      }
   }

   if (dirty)
   {
      updateDialog();
      refresh();
   }
}

void ossimQtHistogramController::highClipPercentReturnPressed()
{
   if (!theHistogramRemapper || theCallBackDisabled) return;

   QString s = theDialog->theHighClipPercentLineEdit->text();
   ossimString os = s.toStdString();
   ossim_float64 d = os.toDouble();
   d = 1.0 - (d / 100.0);

   bool dirty = false;
   const ossim_uint32 BAND = getBand();
   if (BAND == ossimQtHistogramController::MASTER)
   {
      if (d != theHistogramRemapper->getHighNormalizedClipPoint())
      {
         dirty = true;
         theHistogramRemapper->setHighNormalizedClipPoint(d);
      }
   }
   else if (d != theHistogramRemapper->getHighNormalizedClipPoint(BAND))
   {
      dirty = true;
      theHistogramRemapper->setHighNormalizedClipPoint(d, BAND);
   }

   if (dirty)
   {
      updateDialog();
      refresh();
   }
}

void ossimQtHistogramController::lowClipValueReturnPressed()
{
   if (!theHistogramRemapper || theCallBackDisabled) return;

   QString s = theDialog->theLowClipValueLineEdit->text();
   ossimString os = s.toStdString();
   ossim_float64 d = os.toDouble();

   bool dirty = false;
   const ossim_uint32 BAND = getBand();
   if (BAND == ossimQtHistogramController::MASTER)
   {
      if (d != theHistogramRemapper->getLowClipPoint())
      {
         dirty = true;
         theHistogramRemapper->setLowClipPoint(d);
      }
   }
   else if (d != theHistogramRemapper->getLowClipPoint(BAND))
   {
      dirty = true;
      theHistogramRemapper->setLowClipPoint(d, BAND);
   }

   if (dirty)
   {
      updateDialog();
      refresh();
   }
}

void ossimQtHistogramController::highClipValueReturnPressed()
{
   if (!theHistogramRemapper || theCallBackDisabled) return;

   QString s = theDialog->theHighClipValueLineEdit->text();
   ossimString os = s.toStdString();
   ossim_float64 d = os.toDouble();

   bool dirty = false;
   const ossim_uint32 BAND = getBand();
   if (BAND == ossimQtHistogramController::MASTER)
   {
      if (d != theHistogramRemapper->getHighClipPoint())
      {
         dirty = true;
         theHistogramRemapper->setHighClipPoint(d);
      }
   }
   else if (d != theHistogramRemapper->getHighClipPoint(BAND))
   {
      dirty = true;
      theHistogramRemapper->setHighClipPoint(d, BAND);
   }

   if (dirty)
   {
      updateDialog();
      refresh();
   }
}

void ossimQtHistogramController::midPointReturnPressed()
{
   if (!theHistogramRemapper || theCallBackDisabled) return;

   QString s = theDialog->theMidPointLineEdit->text();
   ossimString os = s.toStdString();
   ossim_float64 d = os.toDouble();

   bool dirty = false;
   const ossim_uint32 BAND = getBand();
   if (BAND == ossimQtHistogramController::MASTER)
   {
      if (d != theHistogramRemapper->getMidPoint())
      {
         dirty = true;
         theHistogramRemapper->setMidPoint(d);
      }
   }
   else if (d != theHistogramRemapper->getMidPoint(BAND))
   {
      dirty = true;
      theHistogramRemapper->setMidPoint(d, BAND);
   }

   if (dirty)
   {
      updateDialog();
      refresh();
   }
}

void ossimQtHistogramController::outputMinValueReturnPressed()
{
   if (!theHistogramRemapper || theCallBackDisabled) return;

   QString s = theDialog->theOutputMinValue->text();
   ossimString os = s.toStdString();
   ossim_float64 d = os.toDouble();

   bool dirty = false;
   const ossim_uint32 BAND = getBand();
   if (BAND == ossimQtHistogramController::MASTER)
   {
      if (d != theHistogramRemapper->getMinOutputValue())
      {
         dirty = true;
         theHistogramRemapper->setMinOutputValue(d);
      }
   }
   else if (d != theHistogramRemapper->getMinOutputValue(BAND))
   {
      dirty = true;
      theHistogramRemapper->setMinOutputValue(d, BAND);
   }

   if (dirty)
   {
      updateDialog();
      refresh();
   } 
}

void ossimQtHistogramController::outputMaxValueReturnPressed()
{
   if (!theHistogramRemapper || theCallBackDisabled) return;
   
   QString s = theDialog->theOutputMaxValue->text();
   ossimString os = s.toStdString();
   ossim_float64 d = os.toDouble();

   bool dirty = false;
   const ossim_uint32 BAND = getBand();
   if (BAND == ossimQtHistogramController::MASTER)
   {
      if (d != theHistogramRemapper->getMaxOutputValue())
      {
         dirty = true;
         theHistogramRemapper->setMaxOutputValue(d);
      }
   }
   else if (d != theHistogramRemapper->getMaxOutputValue(BAND))
   {
      dirty = true;
      theHistogramRemapper->setMaxOutputValue(d, BAND);
   }

   if (dirty)
   {
      updateDialog();
      refresh();
   } 
}

void ossimQtHistogramController::updateDialog()
{
   //---
   // Since with the dialog box can trigger the callbacks set this
   // so that the receivers of callback events don't execute.
   //---
   theCallBackDisabled = true;

   setModeComboBox();
   setEnableCheckBox();
   setLowClipPercentage();
   setHighClipPercentage();
   setLowClipValue();
   setHighClipValue();
   setMidPoint();
   setOutputMinValue();
   setOutputMaxValue();
   setHistogramFile();

   // Enable the valueChanged event methods...
   theCallBackDisabled = false;
}

void ossimQtHistogramController::setModeComboBox() const
{
   if (!theHistogramRemapper) return;

   int mode = theHistogramRemapper->getStretchMode();

   if (mode == ossimHistogramRemapper::STRETCH_UNKNOWN)
   {
      mode = 0;
   }
   
   theDialog->theStretchModeComboBox->setCurrentItem(mode);
}

void ossimQtHistogramController::setEnableCheckBox() const
{
   
   if (!theHistogramRemapper) return;
   
   bool state = theHistogramRemapper->getEnableFlag();
   theDialog->theEnableCheckBox->setChecked(state);
}

void ossimQtHistogramController::setLowClipPercentage() const
{
   if (!theHistogramRemapper) return;

   const ossim_uint32 BAND = getBand();

   ossim_float64 clip = 0.0;
   
   if (BAND == ossimQtHistogramController::MASTER)
   {
      clip = theHistogramRemapper->getLowNormalizedClipPoint();
   }
   else
   {
      clip = theHistogramRemapper->getLowNormalizedClipPoint(BAND);
   }

   clip = clip * 100.0;
   QString s = ossimString::toString(clip, 4).c_str();
   theDialog->theLowClipPercentLineEdit->setText(s);
}

void ossimQtHistogramController::setHighClipPercentage() const
{
   if (!theHistogramRemapper) return;

   const ossim_uint32 BAND = getBand();

   ossim_float64 clip = 0.0;
   
   if (BAND == ossimQtHistogramController::MASTER)
   {
      clip = theHistogramRemapper->getHighNormalizedClipPoint();
   }
   else
   {
      clip = theHistogramRemapper->getHighNormalizedClipPoint(BAND);
   }

   clip = 100.0 - (clip * 100.0);
   QString s = ossimString::toString(clip, 4).c_str();
   theDialog->theHighClipPercentLineEdit->setText(s);
}

void ossimQtHistogramController::setLowClipValue() const
{
   if (!theHistogramRemapper) return;

   const ossim_uint32 BAND = getBand();

   ossim_float64 clip = 0.0;
   
   if (BAND == ossimQtHistogramController::MASTER)
   {
      clip = theHistogramRemapper->getLowClipPoint();
   }
   else
   {
      clip = theHistogramRemapper->getLowClipPoint(BAND);
   }

   QString s = ossimString::toString(clip, 0).c_str();
   theDialog->theLowClipValueLineEdit->setText(s);
}

void ossimQtHistogramController::setHighClipValue() const
{
   if (!theHistogramRemapper) return;

   const ossim_uint32 BAND = getBand();

   ossim_float64 clip = 0.0;
   
   if (BAND == ossimQtHistogramController::MASTER)
   {
      clip = theHistogramRemapper->getHighClipPoint();
   }
   else
   {
      clip = theHistogramRemapper->getHighClipPoint(BAND);
   }

   QString s = ossimString::toString(clip, 0).c_str();
   theDialog->theHighClipValueLineEdit->setText(s);
}

void ossimQtHistogramController::setMidPoint() const
{
   if (!theHistogramRemapper) return;

   const ossim_uint32 BAND = getBand();

   ossim_float64 value = 0.0;
   
   if (BAND == ossimQtHistogramController::MASTER)
   {
      value = theHistogramRemapper->getMidPoint();
   }
   else
   {
      value = theHistogramRemapper->getMidPoint(BAND);
   }
   
   QString s = ossimString::toString(value, 0).c_str();
   theDialog->theMidPointLineEdit->setText(s);
}

void ossimQtHistogramController::setOutputMinValue() const
{
   if (!theHistogramRemapper) return;

   const ossim_uint32 BAND = getBand();

   ossim_float64 value = 0.0;
   
   if (BAND == ossimQtHistogramController::MASTER)
   {
      value = theHistogramRemapper->getMinOutputValue();
   }
   else
   {
      value = theHistogramRemapper->getMinOutputValue(BAND);
   }
   
   QString s = ossimString::toString(value, 0).c_str();
   theDialog->theOutputMinValue->setText(s);
}

void ossimQtHistogramController::setOutputMaxValue() const
{
   if (!theHistogramRemapper) return;

   const ossim_uint32 BAND = getBand();

   ossim_float64 value = 0.0;
   
   if (BAND == ossimQtHistogramController::MASTER)
   {
      value = theHistogramRemapper->getMaxOutputValue();
   }
   else
   {
      value = theHistogramRemapper->getMaxOutputValue(BAND);
   }
   
   QString s = ossimString::toString(value, 0).c_str();
   theDialog->theOutputMaxValue->setText(s);
}

void ossimQtHistogramController::setHistogramFile() const
{
   if (!theHistogramRemapper) return;

   ossimFilename f = theHistogramRemapper->getHistogramFile();
   
   QString s = f.c_str();
   theDialog->theHistogramFileLineEdit->setText(s); 
}

void ossimQtHistogramController::refresh() const
{
   if (!theHistogramRemapper) return;

   setEnableCheckBox();
   ossimQtDisplayUtility::flushAllOutputs(theHistogramRemapper);
}

ossim_uint32 ossimQtHistogramController::getBand() const
{
   ossim_int32 band = theBandComboBoxItem;
   if (band == 0)
   {
      return ossimQtHistogramController::MASTER;
   }

   return static_cast<ossim_uint32>(band-1);
}

// Private to disallow copy constructor, assignment operator use...
ossimQtHistogramController::ossimQtHistogramController(const ossimQtHistogramController&)
   :
      ossimConnectableObjectListener(),
      theDialog(NULL),
      theHistogramRemapper(NULL),
      theCallBackDisabled(false)
{
}

ossimQtHistogramController& ossimQtHistogramController::operator=(const ossimQtHistogramController&)
{
   return *this;
}

