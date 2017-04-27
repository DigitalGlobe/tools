//*****************************************************************************
// License:  See top level LICENSE.txt file.
//
// DESCRIPTION:
//   Contains implementation of class ossimMapProjectionFactory
//
//*****************************************************************************
//  $Id: ossimCsmProjectionFactory.cpp 13037 2008-06-18 15:12:55Z gpotts $
#include "ossimCsmSensorModel.h"
//#ifdef __APPLE__
//#include <csmapi/csmapi.h>
//#else
#include <csmApi/CSMSensorModelLoader.h>
//#endif
#include <ossim/elevation/ossimElevManager.h>

RTTI_DEF1(ossimCsmSensorModel, "ossimCsmSensorModel", ossimSensorModel);

ossimCsmSensorModel::ossimCsmSensorModel()
:m_model(0),
m_pluginDir(""), 
m_pluginName(""),
m_sensorName("")
{
}

ossimCsmSensorModel::ossimCsmSensorModel(const ossimCsmSensorModel& src)
:m_model(0),
m_pluginDir(""), 
m_pluginName(""),
m_sensorName(""){
   setSensorModel(src.m_imageFile, src.m_pluginDir, src.m_pluginName, src.m_sensorName);
}

ossimCsmSensorModel::~ossimCsmSensorModel()
{
   if(m_model)
   {
      delete m_model;
      m_model = 0;
   }
}


void ossimCsmSensorModel::imagingRay(const ossimDpt& image_point,
                                     ossimEcefRay&   image_ray) const
{
   if(m_model)
   {
      double locus[6] = {0.0, 0.0, 0.0,
      0.0, 0.0, 0.0};
      double AP = 0.0;
      m_model->imageToRemoteImagingLocus(image_point.y, image_point.x, locus, AP);
      ossimEcefVector v(locus[3], locus[4], locus[5]);	
      image_ray.setOrigin(ossimEcefPoint(locus[0], locus[1], locus[2]));
      image_ray.setDirection(v);
   } 
}

void ossimCsmSensorModel::lineSampleHeightToWorld(const ossimDpt& image_point,
                                                  const double&   heightEllipsoid,
                                                  ossimGpt&       worldPoint) const
{
   if (!insideImage(image_point))
   {
      worldPoint.makeNan();
      worldPoint = extrapolate(image_point, heightEllipsoid);
   }
   else
   {
      //***
      // First establish imaging ray from image point:
      //***
      ossimEcefRay ray;
      imagingRay(image_point, ray);
      ossimEcefPoint Pecf (ray.intersectAboveEarthEllipsoid(heightEllipsoid));
      worldPoint = ossimGpt(Pecf);
   }
   
}

void ossimCsmSensorModel::lineSampleToWorld(const ossimDpt& image_point,
                                            ossimGpt&       gpt) const
{
   if(image_point.hasNans())
   {
      gpt.makeNan();
      return;
   }
   //***
   // Determine imaging ray and invoke elevation source object's services to
   // intersect ray with terrain model:
   //***
   ossimEcefRay ray;
   imagingRay(image_point, ray);
   ossimElevManager::instance()->intersectRay(ray, gpt);   
}

bool ossimCsmSensorModel::setSensorModel(const ossimFilename& imageFile, 
                                         const ossimFilename& pluginDir, 
                                         const ossimString& pluginName,
                                         const ossimString& sensorName)
{
   if(m_model)
   {
      delete m_model; m_model = 0;
   }
   ossimString error;
   m_pluginDir  = pluginDir;
   m_pluginName = pluginName;
   m_sensorName = sensorName;
   m_imageFile  = imageFile;
   if(!m_pluginDir.exists()||!m_imageFile.exists()) return false;
   if(!m_sensorName.empty()&&!m_pluginName.empty())
   {
      m_model = CSMSensorModelLoader::newSensorModel(m_pluginDir.c_str(), 
                                                     m_pluginName, 
                                                     m_sensorName.c_str(), 
                                                     m_imageFile.c_str(), error, false);
      if(!error.empty()&&m_model)
      {
         delete m_model;
         m_model = 0;
      }
   }
   else
   {
      std::vector<string> pluginNames = CSMSensorModelLoader::getAvailablePluginNames(m_pluginDir, error );    
      ossim_uint32 idx = 0;
      
      for(idx = 0; ((idx < pluginNames.size())&&(!m_model)); ++idx)
      {
         std::vector<string> sensorModelNames = CSMSensorModelLoader::getAvailableSensorModelNames( pluginDir, 
                                                                                                   pluginNames[idx].c_str(),  error );    
         ossim_uint32 idx2=0;
         for(idx2 = 0; ((idx2 < sensorModelNames.size())&&(!m_model)); ++idx2)
         { 
            error = "";
            TSMSensorModel* model = CSMSensorModelLoader::newSensorModel(pluginDir, 
                                                                         pluginNames[idx].c_str(), 
                                                                         sensorModelNames[idx2].c_str(), 
                                                                         m_imageFile.c_str(), error, false);
            if(model&&error.empty())
            {
               m_sensorName = sensorModelNames[idx2].c_str();
               m_pluginName = pluginNames[idx];
               m_model = model;
            }
            else if(model)
            {
               delete model;
               model = 0;
            }
         }
      }
   }
   if(m_model)
   {
      int nLines, nSamples;
      m_model->getImageSize(nLines, nSamples);	
      
      theImageClipRect = ossimIrect(-nLines, -nSamples, 2*nLines, 2*nSamples);
      theRefImgPt = theImageClipRect.midPoint();
      int nParams = 0;
      m_model->getNumParameters(nParams);
      int idx = 0;
      ossimString name;
      resizeAdjustableParameterArray(nParams);
      double paramValue = 0.0;
      TSMMisc::Param_CharType paramType;
      for(idx = 0; idx < nParams; ++idx)
      {
         m_model->getParameterName(idx, name);
         m_model->getCurrentParameterValue(idx, paramValue);
         setParameterCenter(idx, paramValue);
         setAdjustableParameter(idx, 0.0, 1.0);
         setParameterDescription(idx, name.c_str());
         m_model->getParameterType(idx, paramType); 
         setParameterUnit(idx, "");
      }
      try
      {
         computeGsd();
      }
      catch (...) 
      {
      }
      
   }
   
   return (m_model != 0);
}


void ossimCsmSensorModel::updateModel()
{
   if(!m_model) return;
   
   int nParams = getNumberOfAdjustableParameters();
   
   
   int idx = 0;
   for(idx = 0; idx < nParams; ++idx)
   {
      m_model->setCurrentParameterValue(idx, computeParameterOffset(idx));
   }
}

bool ossimCsmSensorModel::saveState(ossimKeywordlist& kwl,
                                    const char* prefix) const
{
   bool result = ossimSensorModel::saveState(kwl, prefix);
   
   if(result)
   {
      //      ossimString state;
      //      if(m_model->getSensorModelState(state))
      //      {
      //          kwl.add(prefix, "state", state);
      //      }
      //      else
      {
         kwl.add(prefix, "plugin_dir", m_pluginDir, true);
         kwl.add(prefix, "plugin_name", m_pluginName, true);
         kwl.add(prefix, "sensor_name", m_sensorName, true);
         kwl.add(prefix, "image_file", m_imageFile, true);
      }
   }
   
   return result;
}

bool ossimCsmSensorModel::loadState(const ossimKeywordlist& kwl,
                                    const char* prefix)
{
   bool result = ossimSensorModel::loadState(kwl, prefix);
   if(result)
   {
      ossimString plugin_dir = kwl.find(prefix, "plugin_dir");
      ossimString plugin_name = kwl.find(prefix, "plugin_name");
      ossimString sensor_name = kwl.find(prefix, "sensor_name");
      ossimString image_file  = kwl.find(prefix, "image_file");
      
      result = setSensorModel(image_file.c_str(), plugin_dir.c_str(), plugin_name.c_str(), sensor_name.c_str());
      
   }
   
   return result;
}
