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
// $Id: ossimQtTopographicCorrectionDialogController.cpp 21853 2012-10-21 20:20:17Z dburken $

#include <ossimQt/ossimQtDisplayUtility.h>
#include <ossimQt/ossimQtTopographicCorrectionDialogController.h>
#include <ossimQt/ossimQtTopographicCorrectionDialog.h>
#include <ossim/imaging/ossimTopographicCorrectionFilter.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimVisitor.h>
#include <ossim/support_data/ossimFfL7.h>
#include <QtGui/QComboBox>
#include <Qt3Support/Q3Table>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <Qt3Support/Q3FileDialog>


ossimQtTopographicCorrectionDialogController::ossimQtTopographicCorrectionDialogController(ossimQtTopographicCorrectionDialog* dialog)
  :theDialog(dialog),
   theObject(NULL)
{
  if(theDialog)
    {
      connect(theDialog->theCorrectionTypeComboBox, SIGNAL(activated (const QString&)),
	      this, SLOT(typeActivated(const QString&)));
      connect(theDialog->theApplyButton, SIGNAL(clicked()),
	      this, SLOT(applyChanges()));
      connect(theDialog->theImportParametersButton, SIGNAL(clicked()),
              this, SLOT(importParametersButtonClicked()));
    }
}


void ossimQtTopographicCorrectionDialogController::typeActivated(const QString& type)
{
  if(theObject)
    {
      if(type == "Minnaert")
	{
	  theObject->setTopoCorrectionType(ossimTopographicCorrectionFilter::TOPO_CORRECTION_MINNAERT);
	}
      else
	{
	  theObject->setTopoCorrectionType(ossimTopographicCorrectionFilter::TOPO_CORRECTION_COSINE);
	}
      transferToDialog();
      applyChanges();
    }
}

void ossimQtTopographicCorrectionDialogController::setObject(ossimConnectableObject* obj)
{
  theObject = PTR_CAST(ossimTopographicCorrectionFilter, obj);
  if(theObject)
    {
      transferToDialog();
    }
  else
    {
      theDialog->close(true);
    }
}

void ossimQtTopographicCorrectionDialogController::transferToDialog()
{
  if(!theDialog || !theObject)
    {
      return;
    }
  QString typeValue = "Cosine";
  
  if(theObject->getTopoCorrectionType() == ossimTopographicCorrectionFilter::TOPO_CORRECTION_MINNAERT)
    {
      typeValue = "Minnaert";
    }
  
  theDialog->theCorrectionTypeComboBox->setCurrentText(typeValue);
  int idx = 0;

  if(theObject->getTopoCorrectionType() == ossimTopographicCorrectionFilter::TOPO_CORRECTION_MINNAERT)
    {
      theDialog->theAdjustableParameterTable->show();
      const std::vector<double>& kValues = theObject->getKValues();
      theDialog->theAdjustableParameterTable->setNumRows(kValues.size());
      theDialog->theAdjustableParameterTable->horizontalHeader()->setLabel( 0, "K values" );
      for(idx = 0; idx < (int)kValues.size();++idx)
	{
	  theDialog->theAdjustableParameterTable->setText(idx, 0, QString(ossimString::toString(kValues[idx]).c_str()));
	}
    }
  else
    {
      theDialog->theAdjustableParameterTable->hide();
    }
  const vector<double>& gainValues = theObject->getGainValues();
  const vector<double>& biasValues = theObject->getBiasValues();
  transferParametersToDialog(gainValues, biasValues, theObject->getElevationAngle(), theObject->getAzimuthAngle());
  
}

void ossimQtTopographicCorrectionDialogController::applyChanges()
{
  if(!theObject||!theDialog)
    {
      return;
    }
  int idx = 0;
  if(theObject->getTopoCorrectionType() == ossimTopographicCorrectionFilter::TOPO_CORRECTION_MINNAERT)
    {
      for(idx = 0; idx < (int)theDialog->theAdjustableParameterTable->numRows();++idx)
	{
	  ossimString tempK = theDialog->theAdjustableParameterTable->text(idx, 0).ascii();
	  theObject->setK(idx, tempK.toDouble());
	}
    }
  std::vector<double> gainValues(theDialog->theGainBiasTable->numRows());
  std::vector<double> biasValues(theDialog->theGainBiasTable->numRows());

  for(idx = 0; idx < (int)theDialog->theGainBiasTable->numRows();++idx)
    {
      ossimString tempGain =  theDialog->theGainBiasTable->text(idx, 0).ascii();
      ossimString tempBias =  theDialog->theGainBiasTable->text(idx, 1).ascii();
      
      gainValues[idx] = tempGain.toDouble();
      biasValues[idx] = tempBias.toDouble();
    }
  theObject->setGainValues(gainValues);
  theObject->setBiasValues(biasValues);

  theObject->setElevationAngle(ossimString(theDialog->theSunElevationInput->text().ascii()).toDouble());
  theObject->setAzimuthAngle(ossimString(theDialog->theSunAzimuthInput->text().ascii()).toDouble());


  ossimQtDisplayUtility::flushAllOutputs(theObject);
}

void ossimQtTopographicCorrectionDialogController::importParametersButtonClicked()
{
   // ossimConnectableObject* obj = theObject->findObjectOfType("ossimImageHandler",
   // ossimConnectableObject::CONNECTABLE_DIRECTION_INPUT);
   // ossimImageHandler* handler = PTR_CAST(ossimImageHandler, obj);

   ossimTypeNameVisitor visitor( ossimString("ossimImageHandler"),
                                 true, // firstofTypeFlag
                                 (ossimVisitor::VISIT_INPUTS |
                                  ossimVisitor::VISIT_CHILDREN) );
   theObject->accept( visitor );
   ossimRefPtr<ossimImageHandler> handler = visitor.getObjectAs<ossimImageHandler>(0);
   
   if( handler.valid() )
   {
      ossimFilename imageFile = handler->getFilename();
      ossimFilename result;

      result = getLandsatHeader(imageFile);

      if(result !="")
      {
         QString filename = Q3FileDialog::getOpenFileName(result.c_str(),
                                                         "Landsat header (*.fst *.FST);;All files (* *.*)",
                                                         theDialog,
                                                         NULL,
                                                         "Choose parameters file to import");
         if(filename != "")
         {
            ossimRefPtr<ossimFfL7> headerL7 = new ossimFfL7(filename.ascii());
            if(!headerL7->getErrorStatus())
            {
               transferParametersToDialog(headerL7->theGain,
                                          headerL7->theBias,
                                          headerL7->theSunElevation,
                                          headerL7->theSunAzimuth);
            }
         }
      }
   }
}

ossimFilename ossimQtTopographicCorrectionDialogController::getLandsatHeader(const ossimFilename& imageFileName)
{
   ossimFilename imageFile = imageFileName;
   ossimFilename result;
   
   imageFile.setExtension("fst");
   if(imageFile.exists())
   {
      result = imageFile;
   }
   else
   {
      imageFile.setExtension("FST");
      if(imageFile.exists())
      {
         result = imageFile;
      }
      else
      {
         ossimDirectory dir;
         
         if(dir.open(imageFile.path()))
         {
            vector<ossimFilename> fileList;
            
            dir.findAllFilesThatMatch(fileList,
                                      ".*.fst");
            if(fileList.size() <1)
            {
               dir.findAllFilesThatMatch(fileList,
                                         ".*.FST");
            }
            for(int i = 0; i < (int)fileList.size(); ++i)
            {
               if(fileList[i].contains("_hpn")||
                  fileList[i].contains("_hrf")||
                  fileList[i].contains("_htm"))
               {
                  result = fileList[i];
                  break;
               }
            }
         }
      }
   }
   return result;
}


void ossimQtTopographicCorrectionDialogController::transferParametersToDialog(const std::vector<double>& gainValues,
                                                                              const std::vector<double>& biasValues,
                                                                              double sunElevation,
                                                                              double sunAzimuth)
{
   theDialog->theGainBiasTable->setNumRows(gainValues.size());
   int idx;
   for(idx = 0; idx < (int)gainValues.size(); ++idx)
   {
      theDialog->theGainBiasTable->setText(idx, 0, ossimString::toString(gainValues[idx]).c_str());
      theDialog->theGainBiasTable->setText(idx, 1, ossimString::toString(biasValues[idx]).c_str());
   }
   theDialog->theSunElevationInput->setText(ossimString::toString(sunElevation).c_str());
   theDialog->theSunAzimuthInput->setText(ossimString::toString(sunAzimuth).c_str());
}
