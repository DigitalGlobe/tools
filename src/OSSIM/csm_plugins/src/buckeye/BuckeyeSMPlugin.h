//    FILENAME:   BuckeyeSMPlugin.h
//
//    DESCRIPTION:
//
//    Crystal View version of SMPlugin.
//
//    NOTES:
//
//    The static instance of the class is responsible for registering the class
//    with "the list" by invoking the appropriate base class constructor. It
//    does this by invoking its own private "int" constructor.
//

#ifndef __BuckeyeSMPlugin_H
#define __BuckeyeSMPlugin_H

#include <list>
#include <string>
#include <vector>
#include "TSMPlugin.h"
#include "TSMError.h"
#include "TSMWarning.h"
#include "TSMMisc.h"
#include "BuckeyeSensorModel.h"

class TSMSensorModel;

#ifdef _WIN32
# ifdef BUCKEYE_LIBRARY
#  define BUCKEYE_EXPORT_API __declspec(dllexport)
# else
#  define BUCKEYE_EXPORT_API __declspec(dllimport)
# endif
#else
#  define BUCKEYE_EXPORT_API
#endif

class BuckeyeSMPlugin : public TSMPlugin
{
public:

   //---
   // Constructors/Destructor
   //---

   virtual BUCKEYE_EXPORT_API ~BuckeyeSMPlugin() {}

   // TSM Plugin Descriptors
   virtual BUCKEYE_EXPORT_API TSMWarning *getManufacturer(
                                  std::string& manufacturer_name) 
                                    const throw (TSMError) 
   {
		manufacturer_name = "IAI";
		return NULL;
	}

   virtual BUCKEYE_EXPORT_API TSMWarning *getReleaseDate(
                                  std::string&  release_date) const throw (TSMError)
   {
		release_date = "20060809";
		return NULL;
	}

   // Sensor Model Availability
   virtual BUCKEYE_EXPORT_API TSMWarning *getNSensorModels(
                                  int& n_sensor_models) const throw (TSMError) 
   {
		n_sensor_models = 1;
		return NULL;
	}
   
   virtual BUCKEYE_EXPORT_API TSMWarning *getSensorModelName(
                                  const int& sensor_model_index,
                                  std::string& sensor_model_name) 
                                    const throw (TSMError) 
   {
		sensor_model_name = "BUCKEYE_SN00_00";
		return NULL;
	}
   
   // Sensor Model Descriptors
   virtual BUCKEYE_EXPORT_API TSMWarning *getSensorModelVersion(
                                  const std::string &sensor_model_name,
                                  int& version) const throw (TSMError) 
   {
		version = 0;
		return NULL;
	}

   virtual BUCKEYE_EXPORT_API TSMWarning *canSensorModelBeConstructedFromISD(
                                  const tsm_ISD&       image_support_data,
                                  const std::string&   sensor_model_name,
                                  bool&                constructible) const throw (TSMError);
      // If the sensor_model_name matches with the sensor model name
      // in ISD, then announce that the sensor model can be
      // constructed.

   virtual BUCKEYE_EXPORT_API TSMWarning *canSensorModelBeConstructedFromState(
                                  const std::string& sensor_model_name,
                                  const std::string& sensor_model_state,
                                  bool& constructible) const throw (TSMError);
      // If the sensor_model_name matches with the sensor model name
      // in state, then announce that the sensor model can be
      // constructed.

   virtual BUCKEYE_EXPORT_API TSMWarning *constructSensorModelFromState(
                                  const std::string& sensor_model_state,
                                  TSMSensorModel*& sensor_model) const throw (TSMError);

   virtual BUCKEYE_EXPORT_API TSMWarning *constructSensorModelFromISD(
                                  const tsm_ISD&   image_support_data,
                                  const std::string& sensor_model_name,
                                  TSMSensorModel*& sensor_model) const throw (TSMError);
   
   virtual BUCKEYE_EXPORT_API TSMWarning *
     getSensorModelNameFromSensorModelState(
                                  const std::string& sensor_model_state,
                                  std::string& sensor_model_name) 
                                    const throw (TSMError);
   
   // Image Support Data Conversions
   virtual BUCKEYE_EXPORT_API TSMWarning *canISDBeConvertedToSensorModelState(
                                  const tsm_ISD&   image_support_data,
                                  const std::string& sensor_model_name,
                                  bool& convertible) const throw (TSMError);
   
   virtual BUCKEYE_EXPORT_API TSMWarning *convertISDToSensorModelState(
                                  const tsm_ISD& image_support_data,
                                  const std::string& sensor_model_name,
                                  std::string& sensor_model_state) 
                                    const throw (TSMError);

   //---
   // Accessors
   //---

   virtual BUCKEYE_EXPORT_API TSMWarning *getPluginName(std::string&     pluginName) 
      const  throw (TSMError)
   {
		pluginName = PLUGIN_NAME; return NULL;
   }

private:

   //---
   // Types
   //---

   typedef std::list < TSMSensorModel* > BUCKEYEModels;

   //---
   // Constructors
   //---

   BuckeyeSMPlugin();
      //>This special constructor is responsible for registering this plugin by
      // invoking the special "int" base class constructor. Since this is
      // private, only a member of this class, e.g. theRegisteringObject, can
      // invoke it.

   //---
   // Data Members
   //---
   static BUCKEYE_EXPORT_API const char* PLUGIN_NAME;
      //> This data member is the name of this plugin.
      //<  
	
//	static const char* SENSOR_MODEL_NAME;
//	static const int SENSOR_MODEL_VERSION;

   mutable BUCKEYEModels theList;

   static const BuckeyeSMPlugin theRegisteringObject;
	
   //---
   // Methods
   //---
   static bool containsRequiredTags(
      const tsm_ISD&	image_support_data);

   static void tokenize(
         const std::string& str,
         std::vector<std::string>& tokens,
         const std::string& delimiters = " ");

   static void fillOriginalCovarianceMatrix(
         TSMSensorModel*& sensorModel);
   
   static void fillCurrentCovarianceMatrix(
         TSMSensorModel*& sensorModel);
}; // BuckeyeSMPlugin

#endif // __BuckeyeSMPlugin_H
