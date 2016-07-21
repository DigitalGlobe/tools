// CSMSensorModelLoader.cpp
//
// This is the class responsible for loading in a sensor model from an image and
// creating a TSMSensorModel from it.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <csmApi/CSMSensorModelLoader.h>
#include <csmApi/CSMOutput.h>
#include <csmApi/SMManager.h>
#include <csmApi/TSM/TSMPlugin.h>
#include <csmApi/TSM/TSMISDFilename.h>
#include <csmApi/TSM/TSMISDNITF20.h>
#include <csmApi/TSM/TSMISDNITF21.h>
#include <cstdio>
#include <cstring>
using namespace CSMCallWrapper;

// defined in vts_isd.cpp
extern TSMWarning *initNITF20ISD (NITF_2_0_ISD *isd, std::string fname);
extern TSMWarning *initNITF21ISD (NITF_2_1_ISD *isd, std::string fname);

CSMSensorModelLoader::CSMSensorModelLoader()
{
} // CSMSensorModelLoader::CSMSensorModelLoader()


vector<string> CSMSensorModelLoader::getAvailablePluginNames( const char* pPluginDirectory, std::string& pError )
{
	vector<string> returnVector;
//	std::cout << "pPluginDirectory =" << pPluginDirectory << std::endl;

	try {
		// Make sure the input data is not NULL
		if( !pPluginDirectory ) {
			pError = "Plugin Directory must be non-NULL.";
			return returnVector;
		}
		
		// Store the input parameters into C++ strings
		std::string pluginDirectoryStr = pPluginDirectory;

		// Load all of the libraries in the plugin directory with a classification no higher
		// then the one passed in
		SMManager::instance().loadLibraries(pPluginDirectory);
		
		// Attempt to find plugins
		TSMPlugin::TSMPluginList* pluginList = NULL;
		call( TSMPlugin::getList( pluginList ) );

		if( !pluginList ) {
			pError = "No plugins were found in directory: \"" + pluginDirectoryStr + "\"";
			return returnVector;
		}

		// Get the list of plugins
		(TSMPlugin::TSMPluginList)* pluginList;
		call( TSMPlugin::getList(pluginList) );
		for( TSMPlugin::TSMPluginList::const_iterator i = pluginList->begin(); i != pluginList->end(); i++ ) {

			// Store the name of the plugin
			std::string aPluginName;
			call( (*i)->getPluginName(aPluginName) );
			returnVector.push_back(aPluginName);
		}

		return returnVector;

	} // try
	catch(...) {
	}

	pError = "Exception thrown during call.";
	return returnVector;

} // CSMSensorModelLoader::getAvailablePluginNames()


vector<string> CSMSensorModelLoader::getAvailableSensorModelNames( const char* pPluginDirectory,
																   const char* pPluginName, 
																   std::string& pError )
{
	vector<string> returnVector;

	try {
		// Make sure the input data is not NULL
		if( !pPluginDirectory || !pPluginName ) {
			pError = "Plugin Directory and Plugin Name must be non-NULL.";
			return returnVector;
		}
		// Store the input parameters into C++ strings
		std::string pluginDirectoryStr = pPluginDirectory;
		std::string pluginNameStr      = pPluginName;

		// Load all of the libraries in the plugin directory
		SMManager::instance().loadLibraries(pPluginDirectory);
		
		// Attempt to find sensor models for the plugin
		TSMPlugin::TSMPluginList* pluginList = NULL;
		call( TSMPlugin::getList( pluginList ) );

		if( !pluginList ) {
			pError = "No plugins were found in directory: \"" + pluginDirectoryStr + "\"";
			return returnVector;
		}

		// Get the list of plugins
		(TSMPlugin::TSMPluginList)* pluginList;
		call( TSMPlugin::getList(pluginList) );
		for( TSMPlugin::TSMPluginList::const_iterator i = pluginList->begin(); i != pluginList->end(); i++ ) {

			// Go through the plugins looking for the desired name
			std::string aPluginName;
			call( (*i)->getPluginName(aPluginName) );
			if( aPluginName != pluginNameStr )
				continue;
	
			// Print out the name of the sensor models associated with this plugin
			int numSensorModels;
			call( (*i)->getNSensorModels(numSensorModels) );
			for( int j = 0; j < numSensorModels; j++ ) {
				std::string aSensorName;
				call( (*i)->getSensorModelName( j, aSensorName ) );
				returnVector.push_back((string)aSensorName);
			}

			return returnVector;
		}

		pError = "No plugins were found with the name \"" + pluginNameStr + "\"";
		return returnVector;

	} // try
	catch(...) {
	}
	pError = "Exception thrown during call.";
	return returnVector;

} // CSMSensorModelLoader::getAvailableSensorModelNames()


TSMSensorModel* CSMSensorModelLoader::newSensorModel( const char* pPluginDirectory, const char* pPluginName,
													  const char* pSensorModelName, const char* pInputImage,
													  std::string& pError, bool AllPrintOff)
{
	return CSMSensorModelLoader::newSensorModel( pPluginDirectory, pPluginName, pSensorModelName, pInputImage,
												 NULL, pError, AllPrintOff );
} // CSMSensorModelLoader::newSensorModel()


TSMSensorModel* CSMSensorModelLoader::newSensorModel( const char* pPluginDirectory, const char* pPluginName,
													  const char* pSensorModelName, const char* pInputImage,
													  const char* pSensorState, std::string& pError, bool AllPrintOff )
{
	try {
		// Make sure the input data is not NULL
		if( !pPluginDirectory || !pPluginName || !pSensorModelName || !pInputImage) {
			pError = "Plugin Directory, Plugin Name, Sensor Model Name, and Image Name must be non-NULL.";
			return NULL;
		}

		// Store the input parameters into C++ strings
		std::string pluginDirectoryStr = pPluginDirectory;
		std::string pluginNameStr      = pPluginName;
		std::string sensorModelNameStr = pSensorModelName;
		std::string imageNameStr       = pInputImage;
		// Load all of the libraries in the plugin directory
		SMManager::instance().loadLibraries(pPluginDirectory);
		// Attempt to load the desired plugin
		TSMPlugin::TSMPluginList* pluginList = NULL;
		call( TSMPlugin::getList( pluginList ) );
		if( !pluginList ) {
			pError = "No plugins were found in directory: \"" + pluginDirectoryStr + "\"";
			return NULL;
		}

		// Get the list of plugin

		(TSMPlugin::TSMPluginList)* pluginList;
		call( TSMPlugin::getList(pluginList) );
		int counter = 1;
		for( TSMPlugin::TSMPluginList::const_iterator i = pluginList->begin(); i != pluginList->end(); i++ ) {

			// Print out the name of the plugin
			std::string aPluginName;
			call( (*i)->getPluginName(aPluginName) );

			// Print out the name of the sensor models associated with this plugin.
			int numSensorModels;
			call( (*i)->getNSensorModels(numSensorModels) );
			try
			{
				for( int j = 0; j < numSensorModels; j++ ) {
					std::string aSensorName;
					call( (*i)->getSensorModelName( j, aSensorName ) );
				}
			}
			catch (...) {}

			counter++;
		}


		TSMPlugin* plugin = NULL;
		call( TSMPlugin::findPlugin( pPluginName, plugin ) );
		
		if( plugin == NULL ) {
			pError = "No plugin with the name \"" + pluginNameStr + "\" was found in the path \""
						+ pluginDirectoryStr + "\"";
			return NULL;
		}

		// Load the sensor model using the appropriate information
		TSMSensorModel* sensorModel = NULL;
		if( pSensorState != NULL) 
		{
			// See if it's possible to construct the sensor model from the state
			bool constructible;
			call( plugin->canSensorModelBeConstructedFromState(pSensorModelName, pSensorState, constructible) );
			if( !constructible ) {
				pError = "The speciified state cannot be used to construct a sensor model.";
				return NULL;
			}

			// Create the sensor model from the plugin and state
			call( plugin->constructSensorModelFromState( pSensorState, sensorModel ) );
		}
		else {
			tsm_ISD* tsmIsd = NULL;

			NITF_2_0_ISD nitf20Isd;
			NITF_2_1_ISD nitf21Isd;
			// Load the image support data
			filenameISD fnameIsd;
			fnameIsd._filename = imageNameStr;

			// See if it's possible to construct the sensor model from the filename ISD
			bool constructible;
			constructible = false;
			try {
				call( plugin->canSensorModelBeConstructedFromISD(fnameIsd, pSensorModelName, constructible) );
			}
			catch(...) 
			{
				constructible = false;
			}

			if (constructible) 
			{
				//if (!AllPrintOff)
				cout << "Constructing sensor model via filename" << endl;
				tsmIsd = &fnameIsd;
			}
			// Else, try a NITF 2.x ISD
			else {
				TSMWarning* tsmWarn = NULL;
				try {
					//if (!AllPrintOff)
					tsmWarn = initNITF20ISD(&nitf20Isd, imageNameStr);
					call(plugin->canSensorModelBeConstructedFromISD(nitf20Isd, pSensorModelName, constructible));
				} 
				catch (TSMError *err) 
				{
					if (!AllPrintOff)
						std::cout << err->getFunction() << '\n' << err->getMessage() << '\n';
				}
				catch(...) 
				{
					constructible = false;
				}

				if (constructible) {
					tsmIsd = &nitf20Isd;
				}
				else {
					try {
						//if (!AllPrintOff)
						cout << "Constructing sensor model via initNITF21ISD" << endl;
						tsmWarn = initNITF21ISD(&nitf21Isd, imageNameStr);
						call(plugin->canSensorModelBeConstructedFromISD(nitf21Isd, pSensorModelName, constructible));
					} 
					catch (TSMError *err) 
					{
						if (!AllPrintOff)
							std::cout << err->getFunction() << '\n' << err->getMessage() << '\n';
					}
					catch(...) 
					{
						constructible = false;
					}
					
					if (constructible) {
						tsmIsd = &nitf21Isd;
					}
				}
			}
			
			// Create the sensor model from the plugin and image support data
			if (tsmIsd != NULL) {
				call( plugin->constructSensorModelFromISD(*tsmIsd, pSensorModelName, sensorModel));
			}
		}

		if( sensorModel == NULL ) {
			cout << "-------UNABLE 1 ------" << std::endl;
			pError = "Unable to create sensor model \"" + sensorModelNameStr + "\" using the image \"" + imageNameStr + "\"";
			return NULL;
		}

		return sensorModel;

	} // try
	catch( TSMError error ) {
		GSETCSMOutput::printError( cout, &error );
		pError = "Error during call: " + error.getFunction() + ": " + error.getMessage();
	}
	catch(...) {
		pError = "Exception thrown during call.";
	}
	return NULL;
} // CSMSensorModelLoader::newSensorModel()


CSMSensorModelLoader::imageTypeSelections CSMSensorModelLoader::determineNITFType( const char* pInputImage )
{
	
	imageTypeSelections returnType = NOT_NITF; // default is that it is not an NITF
    FILE *filePtr;
	filePtr = fopen(pInputImage, "rt"); 
	if (filePtr != NULL)
	{
		char line[80];

		// file format is label:value
		if ( fgets(line, 80, filePtr) != NULL)
		{
			if (strncmp(line, "NITF02.1", 8) == 0)
			{
				returnType = NITF21;
			}
			else if (strncmp(line, "NITF02.0", 8) == 0)
			{
				returnType = NITF20;
			}
			else
			{
				returnType = NOT_NITF;
			}
		}
	}
	fclose(filePtr);		

	return returnType;
}
