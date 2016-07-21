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
// $Id: ossimQtMatrixPropertyController.cpp 12279 2008-01-07 20:03:22Z dburken $
#include <Qt3Support/Q3Table>
#include <ossimQt/ossimQtMatrixPropertyController.h>
#include <ossimQt/ossimQtMatrixPropertyDialog.h>
#include <ossim/base/ossimMatrixProperty.h>

ossimQtMatrixPropertyController::ossimQtMatrixPropertyController(ossimQtMatrixPropertyDialog* dialog)
   :theDialog(dialog),
    theProperty(0)
{
   connect(theDialog->theMatrix,
           SIGNAL(valueChanged(int, int)),
           this, SLOT(valueChanged(int, int)));
}

void ossimQtMatrixPropertyController::setOssimProperty(ossimRefPtr<ossimMatrixProperty> prop)
{
   if(prop.valid())
   {
      theProperty = (ossimMatrixProperty*)prop->dup();
   }
   
   transferPropertyToDialog();
}

const ossimRefPtr<ossimMatrixProperty> ossimQtMatrixPropertyController::getOssimProperty()const
{
   return theProperty;
}

void ossimQtMatrixPropertyController::applyButton()
{
   emit changed(theProperty);
}

void ossimQtMatrixPropertyController::zeroButton()
{
   theProperty->zero();
   transferPropertyToDialog();
   emit changed(theProperty);
}

void ossimQtMatrixPropertyController::valueChanged(int row, int col)
{
   if(!theProperty) return;
   
   (*theProperty)(row, col) = theDialog->theMatrix->item(row, col)->text().toDouble();
   
   emit changed(theProperty);
}

void ossimQtMatrixPropertyController::normalizeButton()
{
   theProperty->normalize();
   transferPropertyToDialog();
   emit changed(theProperty);
}

void ossimQtMatrixPropertyController::transferPropertyToDialog()
{
   if(theDialog)
   {
      if(theProperty.valid())
      {
         theDialog->theMatrix->setNumRows(theProperty->getNumberOfRows());
         theDialog->theMatrix->setNumCols(theProperty->getNumberOfCols());
      }

      int rowIdx = 0;
      int colIdx = 0;
      for(rowIdx = 0; rowIdx < theProperty->getNumberOfRows(); ++rowIdx)
      {
         for(colIdx = 0; colIdx < theProperty->getNumberOfCols(); ++colIdx)
         {
            ossimString value = ossimString::toString((*theProperty)(rowIdx,
                                                                     colIdx));
            
            value = value.trim('0');
            if(value == ".")
            {
               value = "0.0";
            }
            else if(value == "")
            {
               value = "0.0";
            }
            else if(value[value.size()-1] == '.')
            {
               value += "0";
            }
            theDialog->theMatrix->setText(rowIdx, colIdx, value.c_str());
         }
      }
   }
}

void ossimQtMatrixPropertyController::transferPropertyFromDialog()
{
   if(theDialog)
   {
      if(theProperty.valid())
      {
         int colIdx = 0;
         int rowIdx = 0;
         
         theProperty->resize(theDialog->theMatrix->numRows(),
                             theDialog->theMatrix->numCols());
         for(rowIdx = 0; rowIdx < theProperty->getNumberOfRows(); ++ rowIdx)
         {
            for(colIdx = 0; colIdx < theProperty->getNumberOfCols(); ++colIdx)
            {
               (*theProperty)(rowIdx, colIdx) = theDialog->theMatrix->item(rowIdx, colIdx)->text().toDouble();
            }
         }
      }
   }
}
