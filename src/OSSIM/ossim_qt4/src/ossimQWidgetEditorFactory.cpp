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
// Contains class definition for ossimQWidgetEditorFactory.
// 
//*******************************************************************
// $Id: ossimQWidgetEditorFactory.cpp 23664 2015-12-14 14:17:27Z dburken $

#include <iostream>
using namespace std;

#include <QtGui/QWidget>
#include <ossim/base/ossimObject.h>
#include <ossimQt/ossimQWidgetEditorFactory.h>
#include <ossimQt/ossimQtBandSelectorDialog.h>
#include <ossimQt/ossimQtBrightnessContrastDialog.h>
#include <ossimQt/ossimQtHistogramDialog.h>
#include <ossimQt/ossimQtHsiRemapperDialog.h>
#include <ossimQt/ossimQtPropertyDialog.h>
#include <ossimQt/ossimQtPropertyDialogController.h>
#include <ossimQt/ossimQtRLevelFilterDialog.h>
#include <ossim/imaging/ossimBandSelector.h>
#include <ossim/imaging/ossimBrightnessContrastSource.h>
#include <ossim/imaging/ossimHistogramRemapper.h>
#include <ossim/imaging/ossimHsiRemapper.h>
#include <ossim/imaging/ossimFusionCombiner.h>
#include <ossim/imaging/ossimRLevelFilter.h>
#include <ossim/base/ossimAdjustableParameterInterface.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossimQt/ossimQtAdjustableParameterDialog.h>
#include <ossimQt/ossimQtAdjustableParameterDialogController.h>
#include <ossim/imaging/ossimTopographicCorrectionFilter.h>
#include <ossimQt/ossimQtTopographicCorrectionDialog.h>
#include <ossimQt/ossimQtTopographicCorrectionDialogController.h>

ossimQWidgetEditorFactory* ossimQWidgetEditorFactory::theInstance = NULL;

ossimQWidgetEditorFactory::~ossimQWidgetEditorFactory()
{
   if (theInstance)
   {
      delete theInstance;
      theInstance = NULL;
   }
}

ossimQWidgetEditorFactory* ossimQWidgetEditorFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimQWidgetEditorFactory;
   }
   return theInstance;
}

QWidget* ossimQWidgetEditorFactory::createEditor(ossimObject* obj,
                                                 QWidget* parent,
                                                 Qt::WFlags f) const
{
   if (!obj)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimQWidgetEditorFactory::createEditor: NULL obj passed in!" << std::endl;
      return (QWidget*) NULL;
   }
   if (!parent)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimQWidgetEditorFactory::createEditor: NULL parent passed in!" << std::endl;
      return (QWidget*) NULL;
   }

   if( PTR_CAST(ossimBandSelector, obj) )
   {
      ossimQtBandSelectorDialog* bsd
         = new ossimQtBandSelectorDialog(parent,
                                         "ossimQtBandSelectorDialog", 
                                         false,
                                         f);
      
      // pass the band selector pointer to the dialog box.
      bsd->setBandSelector( PTR_CAST(ossimBandSelector, obj) );
      return bsd;
   }
   if( PTR_CAST(ossimBrightnessContrastSource, obj) )
   {
      ossimQtBrightnessContrastDialog* bcd
         = new ossimQtBrightnessContrastDialog(parent,
                                               "ossimQtBrightnessContrastDialog", 
                                               false,
                                               f);
      
      // pass the band selector pointer to the dialog box.
      bcd->setFilter( PTR_CAST(ossimBrightnessContrastSource, obj) );
      return bcd;
   }
   else if ( PTR_CAST(ossimHsiRemapper, obj) )
   {
      ossimQtHsiRemapperDialog* hsid
         = new ossimQtHsiRemapperDialog(parent,
                                        "ossimQtHsiRemapperDialog", 
                                        false,
                                        f);
      
      // pass the band selector pointer to the dialog box.
      hsid->setHsiRemapper( PTR_CAST(ossimHsiRemapper, obj) );
      return hsid;
   }
   else if ( PTR_CAST(ossimHistogramRemapper, obj) )
   {
      ossimQtHistogramDialog* hd
         = new ossimQtHistogramDialog(parent,
                                      "ossimQtHistogramDialog",
                                      false,
                                      f);
   
      // pass the remapper selector pointer to the dialog box.
      hd->setHistogramRemapper( PTR_CAST(ossimHistogramRemapper, obj) );
      return hd;
   }
   else if ( PTR_CAST(ossimRLevelFilter, obj) )
   {
      ossimQtRLevelFilterDialog* rd
         = new ossimQtRLevelFilterDialog(parent);
   
      // pass the filter selector pointer to the dialog box.
      rd->setRLevelFilter( PTR_CAST(ossimRLevelFilter, obj) );
      return rd;
   }
   else if( PTR_CAST(ossimFusionCombiner, obj))
   {
      ossimAdjustableParameterInterface* adjInterface = PTR_CAST(ossimAdjustableParameterInterface,
                                                                 obj);
      if(adjInterface)
      {
        ossimQtAdjustableParameterDialog* adjParamDialog = new ossimQtAdjustableParameterDialog(parent);
        
        adjParamDialog->controller()->setInformation(PTR_CAST(ossimFusionCombiner, obj), adjInterface);
        adjParamDialog->controller()->clearTypeAdjustment();
        return adjParamDialog;
      }
   }
   else if( PTR_CAST(ossimTopographicCorrectionFilter, obj))
   {
      ossimQtTopographicCorrectionDialog* dialog = new ossimQtTopographicCorrectionDialog();
      dialog->controller()->setObject(PTR_CAST(ossimConnectableObject, obj));
      return dialog;
   }
   else if ( PTR_CAST(ossimConnectableObject, obj) )
   {
      ossimQtPropertyDialog* dialog
         = new ossimQtPropertyDialog(parent, "Property editor", false, f);
      
      dialog->controller()->setObject(PTR_CAST(ossimConnectableObject, obj));
      return dialog;
   }
   return (QWidget*) NULL;
}

ossimQWidgetEditorFactory::ossimQWidgetEditorFactory()
{}

ossimQWidgetEditorFactory::ossimQWidgetEditorFactory(const ossimQWidgetEditorFactory&) : ossimQWidgetFactoryInterface()
{}

void ossimQWidgetEditorFactory::operator=(const ossimQWidgetEditorFactory&)
{}

