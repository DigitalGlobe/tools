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
// $Id: ossimQtStringListPropertyController.cpp 12141 2007-12-07 18:05:13Z gpotts $
#include <Qt3Support/Q3TextEdit>
#include <Qt3Support/Q3ListBox>
#include <Qt3Support/Q3WidgetStack>
#include <ossimQt/ossimQtStringListPropertyController.h>
#include <ossimQt/ossimQtStringListPropertyDialog.h>
#include <ossim/base/ossimStringListProperty.h>

static const int UNCONSTRAINED_WIDGET_IDX = 0;
static const int CONSTRAINED_WIDGET_IDX   = 1;

ossimQtStringListPropertyController::ossimQtStringListPropertyController(ossimQtStringListPropertyDialog* dialog)
   :theDialog(dialog),
    theProperty(0)
{
}

ossimQtStringListPropertyController::~ossimQtStringListPropertyController()
{
   if(theProperty)
   {
      delete theProperty;
      theProperty = 0;
   }
}

void ossimQtStringListPropertyController::setOssimProperty(const ossimStringListProperty* prop)
{
   if(theProperty)
   {
      delete theProperty;
      theProperty = 0;
   }
   theProperty = (ossimStringListProperty*)prop->dup();
   transferPropertyToDialog();
}

const ossimStringListProperty* ossimQtStringListPropertyController::getOssimProperty()const
{
   return theProperty;
}

void ossimQtStringListPropertyController::applyButton()
{
   transferPropertyFromDialog();
   emit apply(theProperty);
}

void ossimQtStringListPropertyController::transferPropertyToDialog()
{
   if(theProperty->hasConstraints())
   {
      theDialog->theTextEdit->setText("");
      theDialog->theWidgetStack->raiseWidget(CONSTRAINED_WIDGET_IDX);
      theDialog->theTextEdit->setText(((ossimProperty*)theProperty)->valueToString().c_str());
   }
   else
   {
      ossim_uint32 idx = 0;
      theDialog->theWidgetStack->raiseWidget(UNCONSTRAINED_WIDGET_IDX);
      theDialog->theConstraintListbox->clear();
      theDialog->theValueListbox->clear();

      for(idx = 0; idx < theProperty->getNumberOfValues(); ++idx)
      {
         theDialog->theValueListbox->insertItem(theProperty->getValueAt(idx).c_str());
      }
      for(idx = 0; idx < theProperty->getNumberOfContraints(); ++idx)
      {
         theDialog->theConstraintListbox->insertItem(theProperty->getConstraintAt(idx).c_str());
      }
   }
}

void ossimQtStringListPropertyController::transferPropertyFromDialog()
{
   
   theProperty->setModifiedFlag(true);
}

