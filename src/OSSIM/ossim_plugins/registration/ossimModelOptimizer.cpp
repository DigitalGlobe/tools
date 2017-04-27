//*******************************************************************
//
// License: MIT
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Frederic Claudel (fclaudel@laposte.net)
//         ICT4EO,Meraka Institute,CSIR
//
// Description: generic model optimization, high-level interface
//
//*************************************************************************
#include "ossimModelOptimizer.h"
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimEcefVector.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimFilenameProperty.h>
#include <ossim/projection/ossimOptimizableProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>

#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime> //for randomizing with time

RTTI_DEF2(ossimModelOptimizer, "ossimModelOptimizer", ossimConnectableObject, ossimProcessInterface);

ossimModelOptimizer::ossimModelOptimizer() :
   ossimConnectableObject(0, 0, 0),
   theTiesetFilename(""),
   theModelDefinitionString("ossimPolynomProjection{1 x y x2 xy y2 x3 y3 xy2 x2y z xz yz}"),
   theGeomOutputFilename("modopt.geom")
{
   //randomize
   std::srand(std::time(0));
}

bool
ossimModelOptimizer::loadGMLTieSet(const ossimString& filepath)
{
   // import GML SimpleTiePoints

   //open file and load full XML
   ossimXmlDocument gmlDoc;
   bool res = gmlDoc.openFile(filepath);
   if (!res) {
      ossimNotify(ossimNotifyLevel_WARN) << 
         "WARNING: ossimModelOptimizer::loadGMLTieSet cannot open file "<<filepath<<"\n";
      return false;
   }
   
   //parse XML structure for ONE set of tie point
   std::vector< ossimRefPtr< ossimXmlNode > > tieSetList;
   gmlDoc.findNodes(ossimString("/") + ossimTieGptSet::TIEPTSET_TAG, tieSetList);

   if (tieSetList.size() != 1)
   {
      ossimNotify(ossimNotifyLevel_WARN) << 
         "WARNING: ossimModelOptimizer::loadGMLTieSet need exactly one element of type "<<
         ossimTieGptSet::TIEPTSET_TAG<<", found "<<tieSetList.size()<<"\n";
      return false;
   }

   //get the TieSet object
   res = theTieSet.importFromGmlNode(tieSetList[0]);
   if (!res) {
      ossimNotify(ossimNotifyLevel_WARN) << 
         "WARNING: ossimModelOptimizer::loadGMLTieSet error when decoding XML tie point info\n";
      return false;
   }

   theTiesetFilename = filepath;
   return true;
}

bool
ossimModelOptimizer::execute()
{
   if(!setupModel(theModelDefinitionString))
   {
      return false;
   }
   ossim_float64 variance_pix2;
   if(optimize(&variance_pix2, NULL)) //TBD: add target variance?
   {
      //display results with unit
      cout<<"RMS="<<std::sqrt(variance_pix2)<<" pixels"<<endl;

      if(theGeomOutputFilename!="")
      {
         if(!exportModel(theGeomOutputFilename)) return false;
      }
   } else {
      return false;
   }

   return true;
}

bool
ossimModelOptimizer::optimize(ossim_float64* result_var_pix2,
                              ossim_float64* target_var_pix2)
{
   //init optional variance to error
   if (result_var_pix2!=NULL) *result_var_pix2 = -1.0;

   if(!theModel.valid()) return false;

   //build optimization proj
   ossimOptimizableProjection* opp = PTR_CAST(ossimOptimizableProjection, theModel.get());

   if (!opp) return false;

   // get meter_per_pixel ratio
   double varRatio;
   if (opp->useForward())
   {
      varRatio = 1.0;
   } else {
      //converts pixel variance (pix2) to image variance (m2)
      ossimDpt mpp(theModel->getMetersPerPixel());
      varRatio = (mpp.x*mpp.x + mpp.y*mpp.y) / 2.0;
   }

   //check that enough info
   unsigned int minTPC = (opp->degreesOfFreedom()+1)/2;
   if (minTPC > theTieSet.size())
   {
      ossimNotify(ossimNotifyLevel_WARN) << 
      "WARNING: ossimModelOptimizer::optimize() should use more than "<<minTPC<<" tie points for model: "<<theModelDefinitionString<<"\n";
   }

   //optimize (using optional target variance)
   ossim_float64  target_var;
   ossim_float64* ptarget_var = NULL;
   if (target_var_pix2)
   {
      target_var = (*target_var_pix2) * varRatio;
      ptarget_var = &target_var;
   }
   
   ossim_float64 variance = opp->optimizeFit(theTieSet, ptarget_var);
   if (variance<0)
   {
      ossimNotify(ossimNotifyLevel_WARN) << 
      "WARNING: ossimModelOptimizer::optimize() failed optimization for "<<theModelDefinitionString<<"\n";
      return false;
   }
   //convert to pixel2 variance
   ossim_float64 variance_pix2 = variance / varRatio;

   //normal exit : return optional variance
   if (result_var_pix2!=NULL) *result_var_pix2 = variance_pix2;
   return true;
}

bool 
ossimModelOptimizer::setupModel(const ossimString& description)
{
   theModel = 0;
   ossimString argProj = description;
   argProj=argProj.after("{").trim();
   if (argProj != "")
   {
      argProj = argProj.before("}");
      argProj.trim();
   }
   ossimString projName=description;
   projName = projName.before("{");

   theModelDefinitionString = description;
   //create projection for rejection
   ossimProjection* inlierProj = ossimProjectionFactoryRegistry::instance()->createProjection(projName);
   if (!inlierProj)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << 
         "FATAL: ossimModelOptimizer::setupModel() can't create projection "<<projName<<"\n";
         return false;
   }
   //check if it is an optimizable projection //TBD: create a factory instead, so to get lists?
   ossimOptimizableProjection* opp = PTR_CAST(ossimOptimizableProjection, inlierProj);
   if (!opp)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << 
         "FATAL: ossimModelOptimizer::setupModel() rejection projection is not optimizable "<<projName<<"\n";
         return false;
   }

   //tune projection parameters
   if (argProj != "")
   {
      bool res = opp->setupOptimizer(argProj);
      if (!res)
      {
         ossimNotify(ossimNotifyLevel_FATAL) << 
         "FATAL: ossimModelOptimizer::setupModel() projection cannot initialize with given args : "<<argProj<<"\n";
         return false;
      }
   }
   theModel = inlierProj;

   return true;
}

bool
ossimModelOptimizer::exportModel(const ossimFilename& geomFile)const
{
   ossimKeywordlist rpc_kwl;
   if(!theModel.valid())
   {
      return false;
   }
   theModel->saveState(rpc_kwl);
   rpc_kwl.write(geomFile);

   //normal exit
   return true;
}

void
ossimModelOptimizer::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property.valid()) return;
   
   ossimString name = property->getName();

   if(name=="model_definition")
   {
      setupModel(property->valueToString());
   }
   else if(name=="gml_tieset_filename")
   {
      loadGMLTieSet(property->valueToString());
   }
   else if(name=="geom_output_filename")
   {
      theGeomOutputFilename = ossimFilename(property->valueToString());
   }
}

ossimRefPtr<ossimProperty>
ossimModelOptimizer::getProperty(const ossimString& name)const
{
   if(name == "model_definition")
   {
      return new ossimStringProperty(name, theModelDefinitionString);
   }
   else if(name == "geom_output_filename")
   {
      ossimFilenameProperty* filenameProp =
         new ossimFilenameProperty(name, theGeomOutputFilename);
      
      filenameProp->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_OUTPUT);
      
      return filenameProp;
   }
   else if(name == "gml_tieset_filename")
   {
      ossimFilenameProperty* filenameProp =
         new ossimFilenameProperty(name, theTiesetFilename);
      
      filenameProp->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_OUTPUT);
      
      return filenameProp;
   }

   return 0;
}

void
ossimModelOptimizer::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   propertyNames.push_back("model_definition");
   propertyNames.push_back("gml_tieset_filename");
   propertyNames.push_back("geom_output_filename");
}

