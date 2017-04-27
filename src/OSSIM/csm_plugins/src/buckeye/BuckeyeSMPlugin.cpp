//    FILENAME:   BuckeyeSMPlugin.cpp
//
//    DESCRIPTION:
//
//    This file provides implementation for methods declared in the
//    BuckeyeSMPlugin class.
//
//    NOTES:
//
//		HISTORY:
//
//		13 Dec 2009		File created.

#define BUCKEYE_LIBRARY

#include <algorithm>
#include <string>
#include <iostream>

#include "BuckeyeSMPlugin.h"
#include "BuckeyeSensorModel.h"
#include "TSMWarning.h"
#include "TSMError.h"
#include "TSMISDFilename.h"
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimCsvFile.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/projection/ossimBuckeyeSensor.h>
///////////////////////////


#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimLsrRay.h>
#include <ossim/base/ossimLsrSpace.h>
#include <ossim/base/ossimMatrix4x4.h>
#include <ossim/base/ossimCsvFile.h>
#include <ossim/elevation/ossimElevManager.h>
/////////////////////////

const char* BuckeyeSMPlugin::PLUGIN_NAME  = "BUCKEYE_SN00";
const BuckeyeSMPlugin BuckeyeSMPlugin::theRegisteringObject;

#ifndef TESTVTS
#define TESTVTS 0		//???TEMP
#endif
//*****************************************************************************
// BuckeyeSMPlugin::BuckeyeSMPlugin
//*****************************************************************************
BuckeyeSMPlugin::BuckeyeSMPlugin()
{
#if TESTVTS
  std::cout << "constructing an BuckeyeSMPlugin object";
#endif
}

//*****************************************************************************
// BUCKEYESMPlugiError requesting plugin namesn::constructSensorModelFromState
//*****************************************************************************
TSMWarning *BuckeyeSMPlugin::constructSensorModelFromState(
                                  const std::string& sensor_model_state,
                                  TSMSensorModel*& sensor_model) const throw (TSMError)
{
	TSMWarning *tsmWarn = NULL;
	std::string myname("BuckeyeSMPlugin::constructSensorModelFromState");
	TSMSensorModel* result = new BuckeyeSensorModel();



/*	int covarianceOffset = BuckeyeSensorModel::NUMBER_OF_PARAMETERS * 4 + 3;  // covarianceOffset = 95
	int numCovarianceRows = BuckeyeSensorModel::NUMBER_OF_PARAMETERS;		  // numCovarianceRows = 23	
	int numCovarianceCols = BuckeyeSensorModel::NUMBER_OF_PARAMETERS;		  // numCovarianceCols = 23	
	int covarianceSize = numCovarianceRows * numCovarianceCols * 2;				  // covarianceSize = 1058 	
	int sigmaOffset = covarianceOffset + (covarianceSize);

    std::cout << "***********sensor_model_state =" << sensor_model_state << std::endl;
	std::string delimiter (1, ':');
	std::vector<std::string> tokens;
	BuckeyeSMPlugin::tokenize(sensor_model_state, tokens, delimiter);

	if (tokens[0].find(BuckeyeSMPlugin::PLUGIN_NAME) == std::string::npos) 
	{
		TSMError tsmError(TSMError::INVALID_SENSOR_MODEL_STATE, "Unknown type of sensor mode state.", myname);
		throw tsmError;
	}
	if (tokens.size() - 3 != ((unsigned int)(BuckeyeSensorModel::NUMBER_OF_PARAMETERS) * 4 + covarianceSize) + (BuckeyeSensorModel::NUMBER_OF_PARAMETERS + 4))
	{
		TSMError tsmError(TSMError::INVALID_SENSOR_MODEL_STATE, "Incorrect number of parameters.", myname);
		throw tsmError;
	}

	//   for (unsigned int i = 1; i < tokens.size() - 1; i += 4) {
	// Note:
	//                        Elements:      0      :     1     :       2       :      3
	// These values come in groups of 4 (orig_value : orig_type : current_value : current_type)
	for (unsigned int i = 0; i < (unsigned int)BuckeyeSensorModel::NUMBER_OF_PARAMETERS; i++) 
	{
		TSMMisc::Param_CharType originalType = TSMMisc::Param_CharType((std::atoi(tokens[(i * 4 + 1) + 1].c_str())));		// Element 1
		TSMMisc::Param_CharType currentType = TSMMisc::Param_CharType((std::atoi(tokens[(i * 4 + 1) + 3].c_str())));		// Element 3
		result->setOriginalParameterValue((int)i, std::atof(tokens[(i * 4 + 1)].c_str()));									// Element 0
		result->setCurrentParameterValue((int)i, std::atof(tokens[(i * 4 + 1) + 2].c_str()));								// Element 2
		result->setParameterType((int)i, originalType);
	}

	result->setImageIdentifier(tokens[tokens.size() - 2]);

	BuckeyeSensorModel *bkSensorModel = static_cast<BuckeyeSensorModel*>(result);

	bkSensorModel->setReferenceDateAndTime(tokens[tokens.size() - 1]);

	for (int i = 0; i < numCovarianceRows; i++) 
	{
		for (int j = 0; j < numCovarianceCols; j++) 
		{
			result->setOriginalParameterCovariance(i, j, std::atof(tokens[covarianceOffset + (i * numCovarianceCols * 2) + (j * 2)].c_str()));
			result->setCurrentParameterCovariance(i, j, std::atof(tokens[covarianceOffset + (i * numCovarianceCols * 2) + (j * 2) + 1].c_str()));
		}
	}
	  
	// put the sigma values into the addedParameters
	for (int i = 0; i < BuckeyeSensorModel::NUMBER_OF_PARAMETERS; i++) 
	{
		bkSensorModel->setAddedParameters(i, std::atof(tokens[sigmaOffset + i].c_str()));
	}

	// Store the variables that define the: 
	//    pix2mm
	//    imageWidth
	//    imageHeight
	//    range_age
	// NOTE: These parameters are actually stored as constants and need to have an index when you add them that is greater then 23 (for now)
	for (int i = BuckeyeSensorModel::NUMBER_OF_PARAMETERS; i < BuckeyeSensorModel::NUMBER_OF_PARAMETERS+4; i++) 
	{
		int index = i - BuckeyeSensorModel::NUMBER_OF_PARAMETERS;
		bkSensorModel->setAddedParameters(i, std::atof(tokens[sigmaOffset + BuckeyeSensorModel::NUMBER_OF_PARAMETERS + index].c_str()));
	}

	fillOriginalCovarianceMatrix(result);
	fillCurrentCovarianceMatrix(result);

	theList.push_back(result);
	sensor_model = result;
*/
	return tsmWarn;
}

//*****************************************************************************
// BuckeyeSMPlugin::constructSensorModelFromISD
//*****************************************************************************
TSMWarning *BuckeyeSMPlugin::constructSensorModelFromISD(
                                  const tsm_ISD&   image_support_data,
                                  const std::string& sensor_model_name,
                                  TSMSensorModel*& sensor_model) const throw (TSMError)
{
   TSMWarning * tsmWarn = NULL;
   std::string myname("BuckeyeSMPlugin::constructSensorModelFromISD");
 
   bool isISDSupported = false;
  //canSensorModelBeConstructedFromISD(image_support_data, sensor_model_name, isISDSupported);
   const filenameISD * filenameObject = dynamic_cast<const filenameISD*>(&image_support_data);
   ossimFilename frameMetaGsti;
   ossimRefPtr<ossimImageHandler> ih;
   ossimFilename frameNumber;

   if(filenameObject)
   {
      ossimFilename file(filenameObject->_filename);
     if(file.ext() == "tif")
	  {
	     std::cout << "********* tif\n";		// Temporary -- Tiff development is underway but not operational
	  }
	  ossimFilename dir(file.path());
	  frameNumber = file.fileNoExtension();
	  frameMetaGsti = dir.dirCat("FrameMeta_GSTI.txt");
	  ih = ossimImageHandlerRegistry::instance()->open(file);
	  if(frameMetaGsti.exists()&&ih.valid())
	  {
	     isISDSupported = true;
	  }
   }
	
   BuckeyeSensorModel* result = 0;
   if (isISDSupported) 
   {
      ossimDpt pixelSize(.0068, .0068);
	  // Note:  This system only works for Square pixels.......  
	  ossimDpt principalPoint(-0.1853,1.2428);
	  double focalLength = 211.0211;
	  ossimIrect imageRect = ih->getBoundingRect();
	  
	  double roll  = 0.0;
	  double pitch = 0.0;
	  double yaw   = 0.0;

	  ossimGpt platformPosition;
	  bool frameNumberFound = false;

	  ossimCsvFile csv(" \t");		// we will use tab or spaces as separator
      if(csv.open(frameMetaGsti))
      {
         if(csv.readHeader())
         {
            ossimRefPtr<ossimCsvFile::Record> record;
            while( ((record = csv.nextRecord()).valid()) && !frameNumberFound)
            {
               if( (*record)["Frame#"] == frameNumber)
               {
                  frameNumberFound = true;
                  roll = (*record)["Roll(deg)"].toDouble();
                  pitch = (*record)["Pitch(deg)"].toDouble();
                  yaw = (*record)["Yaw(deg)"].toDouble();
                  platformPosition = ossimGpt((*record)["Lat(deg)"].toDouble(),
				                              (*record)["Lon(deg)"].toDouble(),
                                              (*record)["HAE(m)"].toDouble());
					
			   }
			}
		}
		
		if(frameNumberFound)
		{

		   result = new BuckeyeSensorModel();

		   BuckeyeSensorModel *bkSensorModel = static_cast<BuckeyeSensorModel*>(result);

		   bkSensorModel->buckeye->setRollPitchHeading(roll, pitch, yaw);
		   bkSensorModel->buckeye->setFocalLength(focalLength);
		   bkSensorModel->buckeye->setPlatformPosition(platformPosition);

		   bkSensorModel->buckeye->setPrincipalPoint(principalPoint);
		   bkSensorModel->buckeye->setPixelSize(pixelSize);
		   ossimSmacCallibrationSystem* distortion = new ossimSmacCallibrationSystem(1.06545826E-002, -1.58699040E-005, 1.12448462E-009, -8.67450914E-013, 0,
																					  5.92909E-006, -1.66339E-005, 0, 0);  

		   bkSensorModel->buckeye->setLensDistortion(distortion);
		   bkSensorModel->buckeye->setImageRect(imageRect);			
		   bkSensorModel->buckeye->updateModel();
			
		   bkSensorModel->setSensorPosition(platformPosition);
		   bkSensorModel->setPrincipalPoint(principalPoint);
		   bkSensorModel->setFocalLength(focalLength);
			
		   bkSensorModel->setPixelSize(pixelSize);	
		   bkSensorModel->setImageSize(imageRect.height(), imageRect.width());

		   bkSensorModel->setReferenceDateAndTime("Date TIME");
#if 0
		   testBuckeyeCSMSensor(bkSensorModel);
#endif
			
		}
      }
  }
  if(!result)
  {
	  TSMError tsmError = TSMError(TSMError::ISD_NOT_SUPPORTED, "Unable to create sensor model from ISD.", myname);
	  throw tsmError;
  }
  
  sensor_model = result; 
  return tsmWarn;
}

//*****************************************************************************
// BuckeyeSMPlugin::getSensorModelNameFromSensorModelState
//*****************************************************************************
TSMWarning *BuckeyeSMPlugin::getSensorModelNameFromSensorModelState(
                                  const std::string& sensor_model_state,
                                  std::string& sensor_model_name) const throw (TSMError)
{
   TSMWarning *tsmWarn = NULL;
   std::string myname("BuckeyeSMPlugin::getSensorModelNameFromSensorModelState");

   std::string delimiter (1, ':');
   std::vector<std::string> tokens;
//       std::cout << "1***********sensor_model_state =" << sensor_model_state << std::endl;
   BuckeyeSMPlugin::tokenize(sensor_model_state, tokens, delimiter);

   if (tokens[0].find(PLUGIN_NAME) == std::string::npos) 
   {
      TSMError tsmError(TSMError::INVALID_SENSOR_MODEL_STATE, "Invalid sensor model state.", myname);
      throw tsmError;
   }

   sensor_model_name = tokens[0];

   return tsmWarn;
}

//*****************************************************************************
// BuckeyeSMPlugin::canSensorModelBeConstructedFromState
//*****************************************************************************
TSMWarning *BuckeyeSMPlugin::canSensorModelBeConstructedFromState(
                                  const std::string& sensor_model_name,
                                  const std::string& sensor_model_state,
                                  bool& constructible) const throw (TSMError)
{
  TSMWarning *tsmWarn = NULL;
  std::string myname("BuckeyeSMPlugin::canSensorModelBeConstructedFromState");
  if (sensor_model_name.find(PLUGIN_NAME) != std::string::npos) 
  {
      constructible = 1;
  }
  else
  {
      constructible = 0;
      tsmWarn = new TSMWarning(TSMWarning::UNKNOWN_WARNING,
			       "Sensor Model Name does not match",
			       myname);
  }
  
  return tsmWarn;
}

//*****************************************************************************
// BuckeyeSMPlugin::canSensorModelBeConstructedFromISD
//*****************************************************************************

TSMWarning *BuckeyeSMPlugin::canSensorModelBeConstructedFromISD(
                                  const tsm_ISD&       image_support_data,
                                  const std::string&   sensor_model_name,
                                  bool&                constructible) const throw (TSMError)
{
  TSMWarning *tsmWarn = NULL;
  std::string myname("BuckeyeSMPlugin::canSensorModelBeConstructedFromISD");
//  std::cout << "**** in canSensorModelBeConstructedFromISD\n";
  if (sensor_model_name.find(PLUGIN_NAME) != std::string::npos && containsRequiredTags(image_support_data)) 
  {
      constructible = 1;
  }
  else
  {
      constructible = 0;
      tsmWarn = new TSMWarning(TSMWarning::UNKNOWN_WARNING,
			   "Sensor Model unable to be constructed from ISD",
			   myname);
  }
  
  return tsmWarn;
}

//*****************************************************************************
// BuckeyeSMPlugin::containsRequiredTags
//*****************************************************************************
bool BuckeyeSMPlugin::containsRequiredTags(
											const tsm_ISD&	image_support_data) {
	std::string isdFormat;
	
	image_support_data.getFormat(isdFormat);
	bool result = false;
	if(isdFormat == "FILENAME")
	{
		const filenameISD * filenameObject = dynamic_cast<const filenameISD*>(&image_support_data);
		if(filenameObject)
		{
			ossimFilename file(filenameObject->_filename);
			ossimFilename dir(file.path());
			if(file.ext() == "tif")
			{
		       std::cout << "********* tif\n";
			}
			ossimFilename frameMetaGsti = dir.dirCat("FrameMeta_GSTI.txt");
			ossimRefPtr<ossimImageHandler> ih = ossimImageHandlerRegistry::instance()->open(file);
			if(frameMetaGsti.exists()&&ih.valid())
			{
				result = true;
			}
		}
	}

	return result;
}

//*****************************************************************************
// BuckeyeSMPlugin::tokenize
//*****************************************************************************
void BuckeyeSMPlugin::tokenize(const std::string& str,
                                   std::vector<std::string>& tokens,
                                   const std::string& delimiters) {
   // Skip delimiters at beginning.
   std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
   // Find first "non-delimiter".
   std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

   while (std::string::npos != pos || std::string::npos != lastPos) 
   {
      // Found a token, add it to the vector.
      tokens.push_back(str.substr(lastPos, pos - lastPos));
      // Skip delimiters.  Note the
      // "not_of"
      lastPos = str.find_first_not_of(delimiters, pos);
      // Find next
      // "non-delimiter"
      pos = str.find_first_of(delimiters, lastPos);
   }
}

//*****************************************************************************
// BuckeyeSMPlugin::fillOriginalCovarianceMatrix
//*****************************************************************************
void BuckeyeSMPlugin::fillOriginalCovarianceMatrix(TSMSensorModel*& sensorModel) {
   std::string myname("BuckeyeSMPlugin::fillOriginalCovarianceMatrix");

	std::cout << "ERROR - fillOriginalCovarianceMatrix is a STUB " << std::endl;
}


//*****************************************************************************
// BuckeyeSMPlugin::fillCurrentCovarianceMatrix
//*****************************************************************************
void BuckeyeSMPlugin::fillCurrentCovarianceMatrix(TSMSensorModel*& sensorModel) 
{
   std::string myname("BuckeyeSMPlugin::fillCurrentCovarianceMatrix");
   std::cout << "ERROR - fillCurrentCovarianceMatrix is a STUB\n";
}

TSMWarning *BuckeyeSMPlugin::canISDBeConvertedToSensorModelState(
         const tsm_ISD&   image_support_data, 
         const std::string& sensor_model_name,
         bool& convertible) const throw (TSMError) 
{
   convertible = false;

   return NULL;
}
                                     
TSMWarning *BuckeyeSMPlugin::convertISDToSensorModelState( 
      const tsm_ISD& image_support_data,
      const std::string& sensor_model_name,
      std::string& sensor_model_state) const throw (TSMError) 
{
   std::string myname("BuckeyeSMPlugin::convertISDToSensorModelState");
   TSMError tsmError(TSMError::UNKNOWN_ERROR, "Unable to convert ISD to sensor model state.", myname);
   throw tsmError;

   return NULL;
}
