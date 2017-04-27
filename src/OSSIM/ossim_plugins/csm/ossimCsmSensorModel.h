//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file
//
// Author:  Bill Watkins
//
// Description: //
//----------------------------------------------------------------------------
// $Id: ossimPngPluginInit.cpp 11046 2007-05-25 18:03:03Z gpotts $
#ifndef ossimCsmSensorModel_HEADER
#define ossimCsmSensorModel_HEADER
#include <ossim/projection/ossimSensorModel.h>
#include <ossim/base/ossimFilename.h>
//#ifdef __APPLE__
//#include <csmapi/csmapi.h>
//#else
#include <csmApi/TSM/TSMSensorModel.h>
//#endif
class ossimCsmSensorModel : public ossimSensorModel
{
public:
   ossimCsmSensorModel();
   ossimCsmSensorModel(const ossimCsmSensorModel& src);
   virtual ~ossimCsmSensorModel();
   ossimObject* dup()const{return new ossimCsmSensorModel(*this);}
   inline virtual bool useForward()const {return false;} //!image to ground faster   
   virtual void imagingRay(const ossimDpt& image_point,
                           ossimEcefRay&   image_ray) const;

   virtual void lineSampleHeightToWorld(const ossimDpt& image_point,
                                        const double&   height,
                                        ossimGpt&       world_point) const;
   virtual void lineSampleToWorld(const ossimDpt& image_point,
                                   ossimGpt&       world_point) const;
   virtual void updateModel();
   bool setSensorModel(const ossimFilename& imageFile, 
                       const ossimFilename& pluginDir,
                       const ossimString& pluginName = "", 
                       const ossimString& sensorName="");   


   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
protected:
   
   TSMSensorModel* m_model;
   ossimFilename m_pluginDir;
   ossimString m_pluginName;
   ossimString m_sensorName;   
   ossimFilename m_imageFile;
TYPE_DATA
};

#endif
