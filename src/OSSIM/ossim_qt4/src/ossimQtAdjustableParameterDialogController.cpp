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
// $Id: ossimQtAdjustableParameterDialogController.cpp 21853 2012-10-21 20:20:17Z dburken $

#include <ossimQt/ossimQtAdjustableParameterDialogController.h>
#include <ossimQt/ossimQtAdjustableParameterDialog.h>
#include <ossimQt/ossimQtSliderTableItem.h>
#include <ossim/base/ossimAdjustableParameterInterface.h>
#include <ossim/base/ossimVisitor.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossimQt/ossimQtDisplayUtility.h>
#include <Qt3Support/Q3Table>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QSlider>
#include <QtGui/QPushButton>
#include <Qt3Support/Q3FileDialog>



static const int NAME_INDEX       = 0;
static const int SIGMA_INDEX      = 1;
static const int PARAMETER_INDEX  = 2;
static const int SLIDER_INDEX     = 3;
static const int VALUE_INDEX      = 4;


ossimQtAdjustableParameterDialogController::ossimQtAdjustableParameterDialogController(ossimQtAdjustableParameterDialog* dialog)
{
   theGeomAdjustmentFlag = true;
   theDialog = dialog;
   theObject = NULL;
   theAdjustableParameterInterface = NULL;
   connect(theDialog->theAdjustableParameterTable, SIGNAL(valueChanged(int, int)), this, SLOT(valueChanged(int, int)));
   connect(theDialog->theResetButton, SIGNAL(clicked()), this, SLOT(resetTable()));

}


void ossimQtAdjustableParameterDialogController::setInformation(ossimConnectableObject* object,
								ossimAdjustableParameterInterface* adjInterface)
{
  theObject = object;
  theAdjustableParameterInterface = adjInterface;
  if(theAdjustableParameterInterface)
  {
     if(theAdjustableParameterInterface->getNumberOfAdjustableParameters() < 1)
     {
        theAdjustableParameterInterface->initAdjustableParameters();
     }
  }
  transferToDialog();
}

void ossimQtAdjustableParameterDialogController::setTypeToGeomAdjustment()
{
   theGeomAdjustmentFlag = true;
   theDialog->theSaveButton->setShown(theGeomAdjustmentFlag);
}

void ossimQtAdjustableParameterDialogController::clearTypeAdjustment()
{
   theGeomAdjustmentFlag = false;
   theDialog->theSaveButton->setShown(theGeomAdjustmentFlag);
}

ossimFilename ossimQtAdjustableParameterDialogController::getDefaultFilename()const
{
   ossimFilename result;
   if(theObject)
   {
      // ossimImageHandler* tempObj = (ossimImageHandler*)theObject->
      // findObjectOfType("ossimImageHandler",
      // ossimConnectableObject::CONNECTABLE_DIRECTION_INPUT, true);

      ossimTypeNameVisitor visitor( ossimString("ossimImageHandler"),
                                    true, // firstofTypeFlag
                                    (ossimVisitor::VISIT_INPUTS |
                                     ossimVisitor::VISIT_CHILDREN) );
      theObject->accept( visitor );
      ossimRefPtr<ossimImageHandler> ih = visitor.getObjectAs<ossimImageHandler>(0);
      if( ih.valid() )
      {
         result = ih->getFilename();
         result.setExtension("geom");
      }
   }

   return result;
}

void ossimQtAdjustableParameterDialogController::transferToDialog()
{
   clearAllFields();
   
   transferToTable();
   transferToList();   
}

void ossimQtAdjustableParameterDialogController::transferToTable()
{
  theDialog->theAdjustableParameterTable->setNumRows(0);
  if(theAdjustableParameterInterface)
    {
      int numAdjustables = theAdjustableParameterInterface->getNumberOfAdjustableParameters();
      if(numAdjustables > 0)
	{
	  if(theDialog->theAdjustableParameterTable->numRows() != numAdjustables)
	    {
	      theDialog->theAdjustableParameterTable->setNumRows(numAdjustables);
	    }
	  for(int idx = 0; idx < numAdjustables; ++idx)
	    {
	      ossimString sigmaValue     = ossimString::toString(theAdjustableParameterInterface->getParameterSigma(idx));
	      ossimString parameterValue = ossimString::toString(theAdjustableParameterInterface->getAdjustableParameter(idx));
          ossimString valueOffset    = ossimString::toString(theAdjustableParameterInterface->computeParameterOffset(idx));
	      if(sigmaValue == ".") sigmaValue = "0";
	      if(parameterValue == ".") parameterValue = "0";
	      if(valueOffset == ".") valueOffset = "0";

	      theDialog->theAdjustableParameterTable->setText(idx, 
							      NAME_INDEX, 
							      theAdjustableParameterInterface->getParameterDescription(idx).c_str());
	      theDialog->theAdjustableParameterTable->setText(idx, 
							      SIGMA_INDEX, 
							      sigmaValue.c_str());
	      theDialog->theAdjustableParameterTable->setText(idx, 
							      PARAMETER_INDEX, 
							      parameterValue.c_str());
	      theDialog->theAdjustableParameterTable->setText(idx, 
							      VALUE_INDEX, 
							      valueOffset.c_str());
	      
	      ossimQtSliderTableItem* sliderItem = new ossimQtSliderTableItem(theDialog->theAdjustableParameterTable,
									      Q3TableItem::WhenCurrent);
	      sliderItem->setTrackingFlag(false);
	      sliderItem->setMinValue(-100);
	      sliderItem->setMaxValue(100);
              double value         = theAdjustableParameterInterface->getAdjustableParameter(idx);
	      sliderItem->setCurrentValue((int)(value*100.0));
	      theDialog->theAdjustableParameterTable->setItem(idx, SLIDER_INDEX, 
							      sliderItem);
	    }
	}
    }

}

void ossimQtAdjustableParameterDialogController::transferToList()
{
   theDialog->theAdjustmentSelectionBox->clear();
   if(theDialog&&theAdjustableParameterInterface)
   {
      ossim_uint32 n = theAdjustableParameterInterface->getNumberOfAdjustments();
      ossim_int32 idx = 0;
      
      for(idx = 0; idx < (int)n; ++idx)
      {
         theDialog->theAdjustmentSelectionBox->insertItem(ossimString::toString(idx).c_str());
      }
      ossim_uint32 adjIdx = theAdjustableParameterInterface->getCurrentAdjustmentIdx();
      theDialog->theAdjustmentSelectionBox->setCurrentText(ossimString::toString(adjIdx).c_str());
      theDialog->theAdjustmentDescriptionInput->setText(theAdjustableParameterInterface->getAdjustmentDescription().c_str());
   }
}

void ossimQtAdjustableParameterDialogController::clearAllFields()
{
  if(theDialog)
    {
      theDialog->theAdjustmentSelectionBox->clear();
      theDialog->theAdjustmentDescriptionInput->setText("");
      theDialog->theAdjustableParameterTable->setNumRows(0);
    }
}

void ossimQtAdjustableParameterDialogController::valueChanged( int row, 
							       int col)
{
  if(col == SLIDER_INDEX)
  {
    theAdjustableParameterInterface->setDirtyFlag(true);
    ossimQtSliderTableItem* sliderItem = (ossimQtSliderTableItem*)theDialog->theAdjustableParameterTable->item(row, col);

    int value = sliderItem->slider()->value();
    double multiplier = (double)value/100.0;
    theAdjustableParameterInterface->setAdjustableParameter(row, multiplier, true);
    sliderItem->setCurrentValue(sliderItem->slider()->value());
    ossimString parameterValue = ossimString::toString(theAdjustableParameterInterface->getAdjustableParameter(row)).trim('0');
    if(parameterValue == ".") parameterValue = "0";

    theDialog->theAdjustableParameterTable->setText(row, 
						    PARAMETER_INDEX, 
						    parameterValue.c_str());
    
    ossimString valueOffset    = ossimString::toString(theAdjustableParameterInterface->computeParameterOffset(row)).trim('0');
    
    if(valueOffset == ".") valueOffset = "0";
    
    theDialog->theAdjustableParameterTable->setText(row, 
                                                    VALUE_INDEX, 
                                                    valueOffset.c_str());
    ossimQtDisplayUtility::flushAllOutputs(theObject);
  }
  else if(col == SIGMA_INDEX)
  {
     theAdjustableParameterInterface->setDirtyFlag(true);
     ossimString sigma = theDialog->theAdjustableParameterTable->text(row, col).ascii();
     theAdjustableParameterInterface->setParameterSigma(row, sigma.toDouble(), true);
     ossimQtDisplayUtility::flushAllOutputs(theObject);
     transferToTable();
  }
  else if(col == PARAMETER_INDEX)
  {
     theAdjustableParameterInterface->setDirtyFlag(true);
     ossimString param = theDialog->theAdjustableParameterTable->text(row, col).ascii();
     theAdjustableParameterInterface->setAdjustableParameter(row, param.toDouble(), true);
     ossimQtDisplayUtility::flushAllOutputs(theObject);
     transferToTable();
  }
  else if(col == VALUE_INDEX)
  {
     theAdjustableParameterInterface->setDirtyFlag(true);
     double center   = theAdjustableParameterInterface->getParameterCenter(row);
     double sigma    = theAdjustableParameterInterface->getParameterSigma(row);
     double minValue = center - sigma;
     double maxValue = center + sigma;
     double value    = ossimString(theDialog->theAdjustableParameterTable->text(row, col).ascii()).toDouble();
     double x = 0.0;

     if(sigma != 0.0)
     {
        //
        // sigma*x + center = value;
        // x = (value - center)/sigma
        x = (value - center)/sigma;

        value = center + x*sigma;

        if(value < minValue)
        {
           x = -1;
        }
        else if(value >maxValue)
        {
           x = 1.0;
        }
        theAdjustableParameterInterface->setAdjustableParameter(row, x, true);
        ossimQtDisplayUtility::flushAllOutputs(theObject);
        transferToTable();
     }
  }
}

void ossimQtAdjustableParameterDialogController::resetTable()
{
   if(!theAdjustableParameterInterface)
   {
      return;
   }
   int numAdjustables = theAdjustableParameterInterface->getNumberOfAdjustableParameters();
   if(numAdjustables > 0)
   {
      theAdjustableParameterInterface->setDirtyFlag(true);
      theAdjustableParameterInterface->resetAdjustableParameters(true);
      transferToTable();
      ossimQtDisplayUtility::flushAllOutputs(theObject);
   }
}

void ossimQtAdjustableParameterDialogController::keepAdjustment()
{
   if(theAdjustableParameterInterface&&theDialog)
   {
      theAdjustableParameterInterface->setDirtyFlag(true);
      theAdjustableParameterInterface->keepAdjustment();
      transferToDialog();
   }
}

void ossimQtAdjustableParameterDialogController::saveAdjustment()
{
   if(!theGeomAdjustmentFlag||!theAdjustableParameterInterface) return;
   
   if(theFilename == "")
   {
      theFilename = getDefaultFilename();

      QString file = Q3FileDialog::getSaveFileName(theFilename.c_str(),
                                                  "*.geom");
      if(file != "")
      {
         theFilename = file.ascii();
         ossimKeywordlist kwl;
         theAdjustableParameterInterface->getBaseObject()->saveState(kwl);

         if(kwl.write(theFilename))
         {
            theAdjustableParameterInterface->setDirtyFlag(false);
         }
      }
   }
}

void ossimQtAdjustableParameterDialogController::copyAdjustment()
{
   if(theAdjustableParameterInterface&&theDialog)
   {
      theAdjustableParameterInterface->setDirtyFlag(true);
      theAdjustableParameterInterface->copyAdjustment(true);
      transferToDialog();
   }   
}

void ossimQtAdjustableParameterDialogController::deleteAdjustment()
{
   if(theAdjustableParameterInterface&&theDialog)
   {
      theAdjustableParameterInterface->setDirtyFlag(true);
      theAdjustableParameterInterface->eraseAdjustment(true);
      if(theAdjustableParameterInterface->getNumberOfAdjustments() < 1)
      {
         theAdjustableParameterInterface->initAdjustableParameters();
      }
      transferToDialog();
      ossimQtDisplayUtility::flushAllOutputs(theObject);
   }   
}

void ossimQtAdjustableParameterDialogController::adjustmentDescriptionChanged()
{
   if(theAdjustableParameterInterface&&theDialog)
   {
      theAdjustableParameterInterface->setDirtyFlag(true);
      theAdjustableParameterInterface->setAdjustmentDescription(theDialog->theAdjustmentDescriptionInput->text().ascii());
   }   
}

void ossimQtAdjustableParameterDialogController::selectionListChanged()
{
   if(theAdjustableParameterInterface&&theDialog)
   {
      theAdjustableParameterInterface->setDirtyFlag(true);
      ossim_uint32 idx = ossimString(theDialog->theAdjustmentSelectionBox->currentText().ascii()).toUInt32();
      theAdjustableParameterInterface->setCurrentAdjustment(idx, true);
      transferToDialog();
      ossimQtDisplayUtility::flushAllOutputs(theObject);

   }
}


