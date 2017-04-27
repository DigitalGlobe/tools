// CSMSensorModelLoader.h
//
// This is the class responsible for loading in a sensor model from an image and
// creating a TSMSensorModel from it.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef CSM_SENSOR_MODEL_LOADER_H
#define CSM_SENSOR_MODEL_LOADER_H 1

#include <csmApi/csmApiExport.h>
#include <csmApi/TSM/TSMSensorModel.h>
#include <vector>

using namespace std;

class CSMAPI_DLL CSMSensorModelLoader 
{
	public:
		static vector<string> getAvailablePluginNames( const char* pPluginDirectory, std::string& pError );

		static vector<string> getAvailableSensorModelNames( const char* pPluginDirectory, const char* pPluginName, std::string& pError );

		static TSMSensorModel* newSensorModel( const char* pPluginDirectory, const char* pPluginName,
											   const char* pSensorModelName, const char* pInputImage, 
											   std::string& pError, bool AllPrintOff);

		static TSMSensorModel* newSensorModel( const char* pPluginDirectory, const char* pPluginName,
											   const char* pSensorModelName, const char* pInputImage,
											   const char* pSensorState, std::string& pError, bool AllPrintOff);

		enum imageTypeSelections { NITF20, NITF21, NOT_NITF };
		static imageTypeSelections determineNITFType(const char* pInputImage);

	private:
		CSMSensorModelLoader();
};


#endif

