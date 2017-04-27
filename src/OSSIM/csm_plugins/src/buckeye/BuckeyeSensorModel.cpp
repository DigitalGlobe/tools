//    FILENAME:   BuckeyeSensorModel.cpp
//
//    DESCRIPTION:
//
//    Implementation of BuckeyeSensorModel class.
//
//    NOTES:
//			
//		HISTORY:
//	 19 Feb 2009		Initial release
//

#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>
#include "BuckeyeSensorModel.h"
#include <ossim/projection/ossimBuckeyeSensor.h>

//#include <ossim/kbool/bool_globals.h> 
//#include <ossim/base/ossimAdjustableParameterInterface.h>

// Declaration of constants
const std::string BuckeyeSensorModel::PARAMETER_NAMES[BuckeyeSensorModel::NUMBER_OF_PARAMETERS] = {
                        "Sensor X",                   // 0     
                        "Sensor Y",                   // 1    
                        "Sensor Z",                   // 2  
                        "Sensor Pitch",               // 3
                        "Sensor Roll",                // 4 
                        "Sensor Azimuth",             // 5
                        "Principal Point Offset: x",  // 6
                        "Principal Point Offset: y",  // 7
                        "Focal Length",               // 8
                        "Radial Lens Distortion: 1",  // 9
                        "Radial Lens Distortion: 2",  // 10
                        "Radial Lens Distortion: 3",  // 11 
                        "Decentering: 1",             // 12   
                        "Decentering: 2",             // 13    
                        "Affine Distance: 1",         // 14 
                        "Affine Distance: 2",         // 15  
                        "Boresighting Offset: 1",     // 16
                        "Boresighting Offset: 2",     // 17
                        "Boresighting Offset: 3",     // 18
                        "Sensor Translation: 1",      // 19
                        "Sensor Translation: 2",      // 20
                        "Sensor Translation: 3",      // 21 
                        "Slant Range"};               // 22

const char* BuckeyeSensorModel::SENSOR_MODEL_NAME = "BUCKEYE_SN00";  


#ifndef TESTBKSM
#define TESTBKSM 0
#endif
//*****************************************************************************
// BuckeyeSensorModel Constructor
//*****************************************************************************
BuckeyeSensorModel::BuckeyeSensorModel()
{
	parameters.resize(NUMBER_OF_PARAMETERS);
    originalCovariance.ReSize(NUMBER_OF_PARAMETERS, NUMBER_OF_PARAMETERS);
    originalCovariance = 0.0;
    currentCovariance.ReSize(NUMBER_OF_PARAMETERS, NUMBER_OF_PARAMETERS);
    currentCovariance = 0.0;

    imageID = "\0";
    imageDateTime = "\0";

    referencePointX = 0.0;
    referencePointY = 0.0;
    referencePointZ = 0.0;
    referencePointSet = false;
	buckeye = new ossimBuckeyeSensor;
}

//*****************************************************************************
// BuckeyeSensorModel Destructor
//*****************************************************************************
BuckeyeSensorModel::~BuckeyeSensorModel()
{
}

//*****************************************************************************
// BuckeyeSensorModel::groundToImage
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::groundToImage(
				       const double &x,
				       const double &y,
				       const double &z,
				       double& line,
				       double& sample,
				       double& achieved_precision,
				       const double &desired_precision)
  throw (TSMError)
{
    ossimGpt worldPoint(x, y, z);
	ossimDpt image_point;
	buckeye->worldToLineSample(worldPoint, image_point);

	line   = image_point.x;
    sample = image_point.y;
	
	achieved_precision=0;
	
	TSMWarning *tsmWarn = NULL;
	return tsmWarn;
	
}
	
TSMWarning *BuckeyeSensorModel::groundToImage(
                   const double& x,
                   const double& y,
                   const double& z,
                   const double groundCovariance[9],
                   double& line,
                   double& sample,
                   double imageCovariance[4],
                   double& achieved_precision,
                   const double& desired_precision)  throw (TSMError)
{
   std::string myname("BuckeyeSensorModel::groundToImage");
   TSMWarning *tsmWarn = NULL;

// Should be replace with a call that uses the ground Covariance.
	
   groundToImage(x, y, z, line, sample, achieved_precision, desired_precision);
	
   return tsmWarn;
}

//*****************************************************************************
// BuckeyeSensorModel::imageToGround
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::imageToGround(
											  const double& line, 
											  const double& sample, 
											  const double& height,
											  double& x, 
											  double& y, 
											  double& z,
											  double& achieved_precision, 
											  const double& desired_precision)  throw (TSMError)
{
	std::string myname("BuckeyeSensorModel::imageToGroundOSSIM");
	TSMWarning *tsmWarn = NULL;
	
    ossimDpt image_point(sample, line);
	ossimGpt worldPoint;

	buckeye->lineSampleHeightToWorld(image_point, height, worldPoint);

	ossimEcefPoint ecfPoint(worldPoint);

	x = ecfPoint.x();
	y = ecfPoint.y();
	z = ecfPoint.z();

// This section is for the development of the precision calculation
//	ossimEcefRay ray;
//	ossimDpt refImagePoint(0.0,0.0);
//	double centerSampleVal = m_ImageSize.x/2;  // This can be fractional if the image is odd sized. 
//	double centerLineVal   = m_ImageSize.y/2; 
//	Matrix xy(2, 1);
//	xy = 0.0;
//	xy(1, 1) =  (sample - centerSampleVal) * pix2mm;	// distance from the Center to the Sample in mm
//	xy(2, 1) =  (line   - centerLineVal)   * pix2mm;	// distance from the Center to the line in mm
//	Matrix ppx(2,1); 
//	ossimDpt principalPoint = getPrincipalPoint();
//	ppx	<<	principalPoint.y << principalPoint.x;
//	Matrix dx(2, 1);
//	dx = 0.0;
//  dx = xy - ppx;
	
    achieved_precision = 0;
	return tsmWarn;
}

            
TSMWarning *BuckeyeSensorModel::imageToGround(
                   const double& line, 
                   const double& sample, 
                   const double imageCovariance[4],
                   const double& height,
                   const double& heightVariance,
                   double& x, 
                   double& y, 
                   double& z,
                   double groundCovariance[9],
                   double& achieved_precision, 
                   const double& desired_precision)  throw (TSMError)
{
   std::string myname("BuckeyeSensorModel::imageToGround");
   TSMWarning *tsmWarn = NULL;

#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::imageToGround form 2 called." << std::endl;
#endif

// Should be replace with a call that uses the variances
   imageToGround(line, sample, height, x, y, z, achieved_precision, desired_precision);
	
   return tsmWarn;
}


//*****************************************************************************
// BuckeyeSensorModel::imageToProximateImagingLocus
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::imageToProximateImagingLocus(
                   const double& line, 
                   const double& sample, 
                   const double& x, 
                   const double& y, 
                   const double& z, 
                   double locus[6],
                   double& achieved_precision, 
                   const double& desired_precision)  throw (TSMError)
{
   TSMWarning *tsmWarn = NULL;
   std::string myname("BuckeyeSensorModel::imageToProximateImagingLocus");
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::imageToProximateImagingLocus called." << std::endl;
#endif

   imageToRemoteImagingLocus(line, sample, locus, achieved_precision, desired_precision);
	
   return tsmWarn;
}


//*****************************************************************************
// BuckeyeSensorModel::imageToProximateImagingLocus
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::imageToRemoteImagingLocus(
                   const double& line, 
                   const double& sample, 
                   double locus[6], 
                   double& achieved_precision, 
                   const double& desired_precision)  throw (TSMError)
{
   TSMWarning *tsmWarn = NULL;
   std::string myname("BuckeyeSensorModel::imageToRemoteImagingLocus");
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::imageToRemoteImagingLocus called." << std::endl;
#endif
	
	ossimGpt llh(m_ecefPlatformPosition);
    double height = llh.hgt - 10;	
	
	double x, y, z = 0.0;
	double achPrec_imageToGround = 0.0;
	
	TSMWarning *tsmWarn_imageToGround = NULL;
		
	tsmWarn_imageToGround = imageToGround(line, sample, height, x, y, z, achPrec_imageToGround);
	
	if (tsmWarn_imageToGround != NULL)
	{
		return tsmWarn_imageToGround;
	}
	
	ossimEcefRay ray;
	ossimDpt image_point(sample, line);
	buckeye->imagingRay(image_point, ray);
	ossimEcefPoint origin(ray.origin());
	ossimEcefVector direction(ray.direction());
	locus[0]=origin.x();
	locus[1]=origin.y();
	locus[2]=origin.z();
	locus[3]=direction.x();
	locus[4]=direction.y();
	locus[5]=direction.z();
	
    return tsmWarn;
}

   //---
   // Uncertainty Propagation
   //---

//*****************************************************************************
// BuckeyeSensorModel::computeGroundPartials
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::computeGroundPartials(
                   const double& x, 
                   const double& y, 
                   const double& z, 
                   double partials[6])  throw (TSMError)
{
   TSMWarning *tsmWarn = NULL;
   std::string myname("BuckeyeSensorModel::computeGroundPartials");
   
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::computeGroundPartials called." 
	     << std::endl;
#endif
	std::cout << "ERROR - computeGroundPartials is a STUB" << std::endl;
	return tsmWarn;
}

   
//*****************************************************************************
// BuckeyeSensorModel::computeSensorPartials
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::computeSensorPartials(
                   const int& index, 
                   const double& x, 
                   const double& y, 
                   const double& z, 
                   double& line_partial, 
                   double& sample_partial, 
                   double& achieved_precision, 
                   const double& desired_precision) throw (TSMError)
{
   TSMWarning *tsmWarn = NULL;
   std::string myname("BuckeyeSensorModel::computeSensorPartials");
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::computeSensorPartials form 1 called." 
	     << std::endl;
#endif

	std::cout << "ERROR - computeSensorPartials is a STUB" << std::endl;
	return tsmWarn;
}

TSMWarning *BuckeyeSensorModel::computeSensorPartials(
                   const int& index, 
                   const double& line, 
                   const double& sample, 
                   const double& x, 
                   const double& y, 
                   const double& z, 
                   double& line_partial, 
                   double& sample_partial, 
                   double& achieved_precision, 
                   const double& desired_precision)  throw (TSMError)
{
   TSMWarning *tsmWarn = NULL;
   std::string myname("BuckeyeSensorModel::computeSensorPartials");
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::computeSensorPartials form 2 called." 
	     << std::endl;
#endif
   std::cout << "ERROR computeSensorPartials #2 is a STUB" << std::endl;
	
   return tsmWarn;
}


//*****************************************************************************
// BuckeyeSensorModel::covarianceModelType
//*****************************************************************************
TSMWarning*  BuckeyeSensorModel::getCovarianceModelType(int &ret_int)  throw (TSMError)
{
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::covarianceModelType called." 
	     << std::endl;
#endif

   ret_int = 0;

   return NULL;
}


//*****************************************************************************
// BuckeyeSensorModel::currentParameterCovariance
//*****************************************************************************
TSMWarning* BuckeyeSensorModel::getCurrentParameterCovariance( 
                     const int& index1, 
                     const int& index2,
		     double & covar) throw (TSMError)
{
   std::string myname("BuckeyeSensorModel::getCurrentParameterCovariance"); 
#if TESTBKSM
   std::cout 
     << "  BuckeyeSensorModel::currentParameterCovariance form 1 called." 
     << std::endl;
#endif

   if (index1 >= NUMBER_OF_PARAMETERS || index2 >= NUMBER_OF_PARAMETERS)
   {
      TSMError tsmError(TSMError::INDEX_OUT_OF_RANGE, "Out of range for covariace matrix.", myname);
      throw tsmError;
   }

   covar = currentCovariance(index1 + 1, index2 + 1);

   return NULL;
}

TSMWarning *BuckeyeSensorModel::setCurrentParameterCovariance(
                   const int& index1, 
                   const int& index2, 
                   const double& covariance) throw (TSMError)
{
   std::string myname("BuckeyeSensorModel::setCurrentParameterCovariance"); 
#if TESTBKSM
   std::cout 
     << "  BuckeyeSensorModel::currentParameterCovariance form 2 called." 
     << endl;
#endif

   if (index1 >= NUMBER_OF_PARAMETERS || index2 >= NUMBER_OF_PARAMETERS) 
   {
      TSMError tsmError(TSMError::INDEX_OUT_OF_RANGE, "Out of range for covariace matrix.", myname);
      throw tsmError;
   }

   currentCovariance(index1 + 1, index2 + 1) = covariance;

   return NULL;
}


//*****************************************************************************
// BuckeyeSensorModel::getCovarianceModel
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::getCovarianceModel(
                    tsm_CovarianceModel*& covModel) throw (TSMError)
{
   TSMWarning *tsmWarn = NULL;
   std::string myname("BuckeyeSensorModel::getCovarianceModel");
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::getCovarianceModel called." 
     << std::endl;
#endif

   tsmWarn = new TSMWarning(TSMWarning::DATA_NOT_AVAILABLE, "No parameters available.", myname);

   return tsmWarn;
}
   

//*****************************************************************************
// BuckeyeSensorModel::originalParameterCovariance
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::setOriginalParameterCovariance(
                   const int& index1, 
                   const int& index2, 
                   const double& covariance) throw (TSMError)
{
   std::string myname("BuckeyeSensorModel::setOriginalParameterCovariance");
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::originalParameterCovariance form 1 called." 
     << endl;
#endif

   if (index1 >= NUMBER_OF_PARAMETERS || index2 >= NUMBER_OF_PARAMETERS ||
         index1 < 0 || index2 < 0) 
   {
      TSMError tsmError(TSMError::INDEX_OUT_OF_RANGE, "Out of range for covariace matrix.", myname);
      throw tsmError;
   }

   originalCovariance(index1 + 1, index2 + 1) = covariance;

   return NULL;
}

TSMWarning *BuckeyeSensorModel::getOriginalParameterCovariance(
																const int&       index1, 
																const int&       index2,
																double & covariance) throw (TSMError)
{
   std::string myname("BuckeyeSensorModel::getOriginalParameterCovariance");
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::originalParameterCovariance form 2 called." 
     << std::endl;
#endif

   if (index1 >= NUMBER_OF_PARAMETERS || 
	   index2 >= NUMBER_OF_PARAMETERS ||
       index1 < 0 || 
	   index2 < 0) 
   {
      TSMError tsmError(TSMError::INDEX_OUT_OF_RANGE, "Out of range for covariace matrix.", myname);
      throw tsmError;
   }

   covariance = originalCovariance(index1 + 1, index2 + 1);

   return NULL;
}


   //---
   // Time and Trajectory
   //---

//*****************************************************************************
// BuckeyeSensorModel::getTrajectoryIdentifier
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::getTrajectoryIdentifier(
                   std::string&         trajectoryId)  throw (TSMError)
{
   std::string myname("BuckeyeSensorModel::getTrajectoryIdentifier");
   TSMWarning *tsmWarn = NULL;
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::getTrajectoryIdentifier called." 
	     << std::endl;
#endif

   trajectoryId = "\0";

   tsmWarn = new TSMWarning(TSMWarning::DATA_NOT_AVAILABLE, "Trajectory ID unavailable.", myname);

   return tsmWarn;
}

//*****************************************************************************
// BuckeyeSensorModel::getReferenceDateAndTime
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::getReferenceDateAndTime(
                   std::string&          date_and_time)  throw (TSMError)
{
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::getReferenceDateAndTime called." 
	     << std::endl;
#endif

   date_and_time = imageDateTime;

   return NULL;
}

//*****************************************************************************
// BuckeyeSensorModel::setReferenceDateAndTime
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::setReferenceDateAndTime(
                   const std::string& date_and_time)  throw (TSMError)
{
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::setReferenceDateAndTime called." 
	     << std::endl;
#endif

   imageDateTime = date_and_time;

   return NULL;
}

//*****************************************************************************
// BuckeyeSensorModel::getImageTime
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::getImageTime(
												const double& line, 
												const double& sample, 
												double&       time)  
	throw (TSMError)
{
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::getImageTime called." << std::endl;
#endif

   time = 0.0;

   return NULL;
}


//*****************************************************************************
// BuckeyeSensorModel::getSensorPosition
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::getSensorPosition(
                   const double&          line,
                   const double&          sample, 
                   double&                x, 
                   double&                y, 
                   double&                z)  throw (TSMError)
{
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::getSensorPosition called." 
	     << std::endl;
#endif
	x = m_ecefPlatformPosition.x();
	y = m_ecefPlatformPosition.y();
	z = m_ecefPlatformPosition.z();

   return NULL;
}


TSMWarning *BuckeyeSensorModel::getSensorPosition(
                   const double&          time,
                   double&                x, 
                   double&                y, 
                   double&                z)  throw (TSMError)
{
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::getSensorPosition called." 
	     << std::endl;
#endif

   x = m_ecefPlatformPosition.x();
   y = m_ecefPlatformPosition.y();
   z = m_ecefPlatformPosition.z();

   return NULL;
}

//*****************************************************************************
// BuckeyeSensorModel::getSensorVelocity
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::getSensorVelocity(
                   const double&    line,
                   const double&    sample, 
                   double&          vx, 
                   double&          vy, 
                   double&          vz)  throw (TSMError)
{
   std::string myname("BuckeyeSensorModel::getSensorVelocity");
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::getSensorVelocity called." 
	     << std::endl;
#endif

   vx = 0.0;
   vy = 0.0;
   vz = 0.0;

   TSMError tsmError(TSMError::UNSUPPORTED_FUNCTION, "Function not supported.", myname);
   throw tsmError;

   return NULL;
}

TSMWarning *BuckeyeSensorModel::getSensorVelocity(
                   const double&    time,
                   double&          vx, 
                   double&          vy, 
                   double&          vz)  throw (TSMError)
{
   std::string myname("BuckeyeSensorModel::getSensorVelocity");
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::getSensorVelocity called." 
	     << std::endl;
#endif

   vx = 0.0;
   vy = 0.0;
   vz = 0.0;

   TSMError tsmError(TSMError::UNSUPPORTED_FUNCTION, "Function not supported.", myname);
   throw tsmError;

   return NULL;
}

   //---
   // Sensor Model Parameters
   //---

//*****************************************************************************
// BuckeyeSensorModel::currentParameterValue
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::getCurrentParameterValue(
                   const int&          index, 
                   double&       value)
  throw (TSMError)
{
   std::string myname("BuckeyeSensorModel::getCurrentParameterValue");
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::currentParameterValue form 1 called." 
	     << std::endl;
#endif

   if (index < 0 || index >= NUMBER_OF_PARAMETERS) {
      TSMError tsmError(TSMError::INDEX_OUT_OF_RANGE, "Invalid index.", myname);
      throw tsmError;
   }

   value = parameters[index].currentValue;

   return NULL;
}

TSMWarning *BuckeyeSensorModel::setCurrentParameterValue(
			const int& index, const double &param_val) 
  throw (TSMError)
{
   std::string myname("BuckeyeSensorModel::setCurrentParameterValue");
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::currentParameterValue form 2 called." 
     << std::endl;
   std::cout << "currentParameter[" << index << "] = " << param_val << std::endl;
#endif

   if (index < 0 || index >= NUMBER_OF_PARAMETERS) {
      TSMError tsmError(TSMError::INDEX_OUT_OF_RANGE, "Invalid index.", myname);
      throw tsmError;
   }

   parameters[index].currentValue = param_val;

   return NULL;
}



//*****************************************************************************
// BuckeyeSensorModel::getParameterName
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::getParameterName(
                   const int&       index, 
                   std::string&     name) throw (TSMError)
{
   std::string myname("BuckeyeSensorModel::getParameterName");
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::getParameterName called." << std::endl;
#endif

   if (index < 0 || index >= NUMBER_OF_PARAMETERS) 
   {
      TSMError tsmError(TSMError::INDEX_OUT_OF_RANGE, "Invalid index.", myname);
      throw tsmError;
   }
   
   name = PARAMETER_NAMES[index];

   return NULL;
}


//*****************************************************************************
// BuckeyeSensorModel::getNumParameters
//*****************************************************************************
TSMWarning * BuckeyeSensorModel::getNumParameters(int & numparam) 
  throw (TSMError)
{
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::getNumParameters called." << std::endl;
#endif

   numparam = NUMBER_OF_PARAMETERS;

   return NULL;
}


//*****************************************************************************
// BuckeyeSensorModel::originalParameterValue
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::getOriginalParameterValue(
														const int&      index,
														double&         value) throw (TSMError)
{
   std::string myname("BuckeyeSensorModel::getOriginalParameterValue");
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::originalParameterValue form 1 called." 
	     << std::endl;
#endif

   if (index < 0 || index >= NUMBER_OF_PARAMETERS)
   {
      TSMError tsmError(TSMError::INDEX_OUT_OF_RANGE, "Invalid index.", myname);
      throw tsmError;
   }

   value = parameters[index].originalValue;
//	std::cout << "ERROR\n";
   return NULL;
}

TSMWarning  *BuckeyeSensorModel::setOriginalParameterValue(
															const int&       index, 
															const double &value)
  throw (TSMError)
{
   std::string myname("BuckeyeSensorModel::setOriginalParameterValue");
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::originalParameterValue form 2 called." 
    << std::endl;
#endif

   if (index < 0 || index >= NUMBER_OF_PARAMETERS) 
   {
      TSMError tsmError(TSMError::INDEX_OUT_OF_RANGE, "Invalid index.", myname);
      throw tsmError;
   }

   parameters[index].originalValue = value;

  return NULL;
}


//*****************************************************************************
// BuckeyeSensorModel::parameterType
//*****************************************************************************

TSMWarning  *BuckeyeSensorModel::getParameterType(
												const int&       index,
												TSMMisc::Param_CharType &ret) throw (TSMError)
{
   std::string myname("BuckeyeSensorModel::getParameterType");
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::parameterType called." << std::endl;
#endif

   if (index < 0 || index >= NUMBER_OF_PARAMETERS) 
   {
      TSMError tsmError(TSMError::INDEX_OUT_OF_RANGE, "Invalid index.", myname);
      throw tsmError;
   }

   ret = parameters[index].type;

   return NULL;
}

TSMWarning* BuckeyeSensorModel::setParameterType(
	const int& index,
	const TSMMisc::Param_CharType& parameterType) throw (TSMError)
{
   string myname("BuckeyeSensorModel::setParameterType");
   if (index < 0 || index >= NUMBER_OF_PARAMETERS) 
   {
      TSMError tsmError(TSMError::INDEX_OUT_OF_RANGE, "Invalid index.", myname);
      throw tsmError;
   }

   if (parameterType < TSMMisc::NONE || parameterType > TSMMisc::EXACT) 
   {
      TSMError tsmError(TSMError::BOUNDS, "Invalid parameter type", myname);
      throw tsmError;
   }

//	std::cout << "ERROR \n";
   parameters[index].type = parameterType;

   return NULL;
}


   //---
   // Sensor Model Information
   //---

//*****************************************************************************
// BuckeyeSensorModel::getPedigree
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::getPedigree(
		               std::string& pedigree) throw (TSMError)
{
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::getPedigree called." << std::endl;
#endif

   pedigree = "Buckeye Sensor";

   return NULL;
}


//*****************************************************************************
// BuckeyeSensorModel::getImageIdentifier
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::getImageIdentifier(
                   std::string&    imageId) throw (TSMError)
{
   std::string myname("BuckeyeSensorModel::getImageIdentifier");
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::getImageIdentifier called." 
	     << std::endl;
#endif

   imageId = imageID;

   return NULL;
}


//*****************************************************************************
// BuckeyeSensorModel::setImageIdentifier
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::setImageIdentifier(
                   const std::string& imageId) throw (TSMError)
{
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::setImageIdentifier called." 
	     << std::endl;
#endif

   imageID = imageId;

   return NULL;
}


//*****************************************************************************
// BuckeyeSensorModel::getSensorIdentifier
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::getSensorIdentifier(
                       std::string& sensorId) throw (TSMError)
{
   TSMWarning* tsmWarn = NULL;
   std::string myname("BuckeyeSensorModel::getSensorIdentifier");
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::getSensorIdentifier called." << std::endl;
#endif

   sensorId = "\0";

   tsmWarn = new TSMWarning(TSMWarning::DATA_NOT_AVAILABLE, "Sensor identifier unavailable.", myname);

   return tsmWarn;
}


//*****************************************************************************
// BuckeyeSensorModel::getPlatformIdentifier
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::getPlatformIdentifier(
                   std::string&     platformId) throw (TSMError)
{
   TSMWarning* tsmWarn = NULL;
   std::string myname("BuckeyeSensorModel::getPlatformIdentifier");
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::getPlatformIdentifier called." 
	     << std::endl;
#endif

   platformId = "\0";

   tsmWarn = new TSMWarning(TSMWarning::DATA_NOT_AVAILABLE, "Platform identifier unavailable.", myname);

   return tsmWarn;
}


//*****************************************************************************
// BuckeyeSensorModel::getReferencePoint
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::getReferencePoint(
                   double&          x, 
                   double&          y, 
                   double&          z) throw (TSMError)
{
   std::string myname("BuckeyeSensorModel::getReferencePoint");
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::getReferencePoint called." 
     << std::endl;
#endif

   if (referencePointSet) { 
      x = referencePointX;
      y = referencePointY;
      z = referencePointZ;
   }
   else {
      TSMError tsmError(TSMError::UNKNOWN_ERROR, "Reference point not set.", myname);
      throw tsmError;
   }
            
   return NULL;
}


//*****************************************************************************
// BuckeyeSensorModel::setReferencePoint
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::setReferencePoint(
                const double&        x, 
                   const double&        y, 
                   const double&        z) throw (TSMError)
{
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::setReferencePoint called." << std::endl;
#endif
      
   referencePointX = x; 
   referencePointY = y;
   referencePointZ = z;
   referencePointSet = true;

   return NULL;
}


//*****************************************************************************
// BuckeyeSensorModel::getImageSize 
//*****************************************************************************

TSMWarning *BuckeyeSensorModel::getImageSize(
                    int&     num_lines,
                    int&     num_samples) throw (TSMError)
{
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::getImageSize called." << std::endl;
#endif
   num_lines   = (int)m_ImageSize.y;
   num_samples = (int)m_ImageSize.x;

   return NULL;
}

   //---
   //  Monoscopic Mensuration
   //---

//*****************************************************************************
// BuckeyeSensorModel::validAltitudeRange
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::getValidAltitudeRange(
                            double& minAltitude,
                            double& maxAltitude) throw (TSMError)
{
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::validAltitudeRange called." 
	     << std::endl;
#endif

   minAltitude = -99999;
   maxAltitude = 99999;

   return NULL;
}


//*****************************************************************************
// BuckeyeSensorModel::getIlluminationDirection
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::getIlluminationDirection(
                            const double& x, 
                            const double& y, 
                            const double& z, 
                            double& direction_x,
                            double& direction_y,
                            double& direction_z) throw (TSMError)
{
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::getIlluminationDirection called." 
	     << std::endl;
#endif

   direction_x = 0.0;
   direction_y = 0.0;
   direction_z = 0.0;

   return NULL;
}

   //---
   //  Error Correction
   //---

//*****************************************************************************
// BuckeyeSensorModel::numSystematicErrorCorrections
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::getNumSystematicErrorCorrections(int &ret) 
  throw (TSMError)

{
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::numSystematicErrorCorrections called." 
             << std::endl;
#endif

   ret = 0;

   return NULL;
}


//*****************************************************************************
// BuckeyeSensorModel::getSystematicErrorCorrectionName
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::getSystematicErrorCorrectionName(
					     const int& index,
					     std::string &name) 
  throw (TSMError)
{
   std::string myname("BuckeyeSensorModel::getSystematicErrorCorrectionName");
#if TESTBKSM
   std::cout 
     << "  BuckeyeSensorModel::getSystematicErrorCorrectionName called."
     << std::endl;
#endif

   TSMError tsmError(TSMError::INDEX_OUT_OF_RANGE, "Invalid index.", myname);
   throw tsmError;

   return NULL;
}

//*****************************************************************************
// BuckeyeSensorModel::currentSystematicErrorCorrectionSwitch
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::setCurrentSystematicErrorCorrectionSwitch(
                                       const int& index,
                                       const bool& value,
                                       const TSMMisc::Param_CharType& parameterType)
  throw (TSMError)
{
   std::string myname("BuckeyeSensorModel::setCurrentSystematicErrorCorrectionSwitch");
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::currentSystematicErrorCorrectionSwitch form 1 called."
     << std::endl;
#endif

   TSMError tsmError(TSMError::INDEX_OUT_OF_RANGE, "Invalid index.", myname);
   throw tsmError;

   return NULL;
}

TSMWarning *BuckeyeSensorModel::getCurrentSystematicErrorCorrectionSwitch(
                                       const int& index, bool &val) 
  throw (TSMError)
{
   std::string myname("BuckeyeSensorModel::getCurrentSystematicErrorCorrectionSwitch");
#if TESTBKSM
   std::cout 
     << "  BuckeyeSensorModel::currentSystematicErrorCorrectionSwitch form 2 called."
     << std::endl;
#endif

   TSMError tsmError(TSMError::INDEX_OUT_OF_RANGE, "Invalid index.", myname);
   throw tsmError;

   return NULL;
}

//*****************************************************************************
// BuckeyeSensorModel::getSensorModelName 
//*****************************************************************************

TSMWarning *BuckeyeSensorModel::getSensorModelName(
                                       std::string& name) throw (TSMError)
{
#if TESTBKSM
   std::cout << "  BuckeyeSensorModel::getSensorModelName called." 
     << std::endl;
#endif

   name = BuckeyeSensorModel::SENSOR_MODEL_NAME;

   return NULL;
}


   //---
   // Sensor Model State
   //---

//*****************************************************************************
// BuckeyeSensorModel::getSensorModelState
//*****************************************************************************
TSMWarning *BuckeyeSensorModel::getSensorModelState(std::string & state) 
  throw (TSMError)
{
	TSMWarning *tsmWarn = NULL;
	#if TESTBKSM
		std::cout << "  BuckeyeSensorModel::getSensorModelState called." << std::endl;
	#endif

/*	state.clear();
	state = (std::string)BuckeyeSensorModel::SENSOR_MODEL_NAME + ':';
	std::stringstream sstream;
	sstream.precision(15);

	// Write out the Parameter information
	for (int i = 0; i < BuckeyeSensorModel::NUMBER_OF_PARAMETERS; i++) 
	{
		sstream << parameters[i].originalValue; 
		state = state + sstream.str();
		state = state + ':';
		sstream.str("");
		sstream << parameters[i].type;
		state = state + sstream.str();
		state = state + ':';
		sstream.str("");
		sstream << parameters[i].currentValue; 
		state = state + sstream.str();
		state = state + ':';
		sstream.str("");
		sstream << parameters[i].type;
		state = state + sstream.str();
		state = state + ':';
		sstream.str("");
	}

	state += imageID + ':';
	state += imageDateTime + ':';

	// Write out the Covariance information
	for (int i = 1; i <= BuckeyeSensorModel::NUMBER_OF_PARAMETERS; i++) 
	{
		for (int j = 1; j <= BuckeyeSensorModel::NUMBER_OF_PARAMETERS; j++) 
		{
			sstream << originalCovariance(i, j);
			state = state + sstream.str();
			state = state + ':';                                                                        
			sstream.str("");
			sstream << currentCovariance(i, j);
			state = state + sstream.str();
			state = state + ':';
			sstream.str("");
		}  
	}

	// Write out the sigma information
	for (int i = 0; i < BuckeyeSensorModel::NUMBER_OF_PARAMETERS; i++) 
	{
		double thisAddedParameter = 0.0;
		this->getCurrentAddedParameters(i, thisAddedParameter);
		sstream << thisAddedParameter;
		state = state + sstream.str();
		state = state + ':';
		sstream.str("");
	}

	// Writeout the variables that define the: 
	//    pix2mm
	//    imageWidth
	//    imageHeight
	//    range_age
	for (int i = 0; i < 4; i++) 
	{
		double thisAddedParameter = 0.0;
		this->getCurrentAddedParameters(i + BuckeyeSensorModel::NUMBER_OF_PARAMETERS, thisAddedParameter);
		sstream << thisAddedParameter;
		state = state + sstream.str();
		sstream.str("");
		if (i < 4)
		{
			state = state + ':';
		}
	}

	// Remove trailing delimiter
	state.resize(state.length() - 1);
*/
	return tsmWarn;
}

TSMWarning* BuckeyeSensorModel::getUnmodeledError(
   const double line, 
   const double sample,
   double covariance[4])
throw (TSMError)
{
   string myname("BuckeyeSensorModel::getUnmodeledError");
   TSMWarning* tsmWarn = NULL;
   tsmWarn = new TSMWarning(TSMWarning::DATA_NOT_AVAILABLE, "Unmodeled error unavailable.", myname);

   return NULL;
}

TSMWarning* BuckeyeSensorModel::getUnmodeledCrossCovariance(
        const double pt1Line,
        const double pt1Sample,
        const double pt2Line,
        const double pt2Sample,
        double crossCovariance[4])
throw (TSMError)
{
   string myname("BuckeyeSensorModel::getUnmodeledCrossCovariance");
   TSMWarning* tsmWarn = new TSMWarning(TSMWarning::DATA_NOT_AVAILABLE, "Unmodeled cross covariance unavailable.", myname);

   return tsmWarn;
}

TSMWarning* BuckeyeSensorModel::getCollectionIdentifier(
	std::string& collectionId)
throw (TSMError)
{
	string myname("BuckeyeSensorModel::getCollectionIdentifier");
    TSMWarning* tsmWarn = new TSMWarning(TSMWarning::DATA_NOT_AVAILABLE, "Collection identifier unavailable.", myname);

	return tsmWarn;
}

TSMWarning* BuckeyeSensorModel::isParameterShareable(
	const int& index,
	bool& shareable)
throw (TSMError) 
{
	string myname("BuckeyeSensorModel::isParameterShareable");
    TSMWarning* tsmWarn = new TSMWarning(TSMWarning::DATA_NOT_AVAILABLE, "Information unavailable.", myname);

	return tsmWarn;
}

TSMWarning* BuckeyeSensorModel::getParameterSharingCriteria(
	const int& index,
	bool& requireModelNameMatch,
	bool& requireSensorIdMatch,
	bool& requirePlatformIdMatch,
	bool& requireCollectionIdMatch,
	bool& requireTrajectoryIdMatch,
	bool& requireDateTimeMatch,
	double& allowableTimeDelta)
throw (TSMError)
{
	string myname("BuckeyeSensorModel::getParameterSharingCriteria");
    TSMWarning* tsmWarn = new TSMWarning(TSMWarning::DATA_NOT_AVAILABLE, "Information unavailable.", myname);

	return tsmWarn;
}

// Utility functions
void BuckeyeSensorModel::setPixelSize(ossimDpt pixelSize)
{
	m_pixelSize = pixelSize;
}

void BuckeyeSensorModel::setImageSize(double line, double sample)
{
	m_ImageSize = ossimDpt(sample,line);
}


void BuckeyeSensorModel::setSensorPosition(ossimGpt& platformPosition)
{
	ossimEcefPoint g_ecf(platformPosition);
	m_ecefPlatformPosition = g_ecf;
    return;
}

ossimGpt BuckeyeSensorModel::getSensorPosition()
{
   return ossimGpt(m_ecefPlatformPosition);
}


void BuckeyeSensorModel::setPrincipalPoint(ossimDpt principalPoint)
{
    m_principalPoint = principalPoint;
	return;
}

ossimDpt BuckeyeSensorModel::getPrincipalPoint()
{
	return m_principalPoint;
}

void BuckeyeSensorModel::setFocalLength(double focalLength)
{
    m_focalLength = focalLength;
	return;
}

double BuckeyeSensorModel::getFocalLength()
{
	return m_focalLength;
}

/*void BuckeyeSensorModel::setRadialLensDistortion(double RadialLensDistortion1,
											    double RadialLensDistortion2,
												double RadialLensDistortion3,
												double Decentering1,
												double Decentering2)
{
	this->setOriginalParameterValue(9, RadialLensDistortion1);
	this->setCurrentParameterValue(9, RadialLensDistortion1);

	this->setOriginalParameterValue(10, RadialLensDistortion2);
	this->setCurrentParameterValue(10, RadialLensDistortion2);

	this->setOriginalParameterValue(11, RadialLensDistortion3);
	this->setCurrentParameterValue(11, RadialLensDistortion3);

	this->setOriginalParameterValue(12, Decentering1);
	this->setCurrentParameterValue(12, Decentering1);

	this->setOriginalParameterValue(13, Decentering2);
	this->setCurrentParameterValue(13, Decentering2);

	return;
}
*/

/*void BuckeyeSensorModel::setAfineDistance(double affineDist1 = 0.0;
										  double affineDist2 = 0.0)
{
   parameters[14] = affineDist1;
   parmaeters[15] = affineDist2;
   return;
}
						
void BuckeyeSensorModel::setBoresightingOffset(double offset1 = 0.0,
											   double offset2 = 0.0,
											   double offset3 = 0.0)
{
    parameters[16] = offset1;
	parameters[17] = offset2;
	parameters[18] = offset3;
	return;
}

void BuckeyeSensorModel::setSensorTranslation(double sensorTrans1 = 0.0;
											  double sensorTrans2 = 0.0;
											  double SensorTrans3 = 0.0)
{
    parameters[19] = sensorTrans1;
	parameters[20] = sensorTrans2;
	parameters[21] = sensorTrans3;
	return;
}

void BuckeyeSensorModel::setSlantRange(double slantRange = 0.0)   // For now a value of 0 is an error 
{
//    if (slantRange == 0.0)
//	{
//	   slantRange = ???? (Calculate it)
//	}
	parameters[22] = slantRange;
}
*/
