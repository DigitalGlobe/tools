//    FILENAME:   BuckeyeSensorModel.h
//
//    DESCRIPTION:
//
//    Header for BuckeyeSensorModel class.
//
//    NOTES:
//
//              HISTORY:
//              22 Feb 2010             Initial creation
//
//
//      Sensor Model Methods:
//  TRD         Function
//  Sect
//  ------  -----------------------------------------------------
//      5.12.1  tsmSensorModel::groundToImage();
//      5.12.2  tsmSensorModel::imageToGround();
//      5.12.3  tsmSensorModel::imageToProximateImagingLocus();
//      5.12.4  tsmSensorModel::imageToRemoteImagingLocus();
//      5.12.5  tsmSensorModel::computeGroundPartials();
//      5.12.6  tsmSensorModel::computeSensorPartials();
//      5.12.7  tsmSensorModel::getCurrentParameterCovariance();
//      5.12.8  tsmSensorModel::setCurrentParameterCovariance();
//      5.12.9  tsmSensorModel::setOriginalParameterCovariance();
//      5.12.10 tsmSensorModel::getOriginalParameterCovariance();
//      5.12.11 tsmSensorModel::getTrajectoryIdentifier();
//      5.12.12 tsmSensorModel::getReferenceDateAndTime();
//      5.12.13 tsmSensorModel::getImageTime();
//      5.12.14 tsmSensorModel::getSensorPosition();
//      5.12.15 tsmSensorModel::getSensorVelocity();
//      5.12.16 tsmSensorModel::setCurrentParameterValue();
//      5.12.17 tsmSensorModel::getCurrentParameterValue();
//      5.12.18 tsmSensorModel::getParameterName();
//      5.12.19 tsmSensorModel::getNumParameters();
//      5.12.20 tsmSensorModel::setOriginalParameterValue();
//      5.12.21 tsmSensorModel::getOriginalParameterValue();
//      5.12.22 tsmSensorModel::getParameterType();
//      5.12.23 tsmSensorModel::getPedigree()
//      5.12.24 tsmSensorModel::getImageIdentifier();
//      5.12.25 tsmSensorModel::setImageIdentifier();
//      5.12.26 tsmSensorModel::getSensorIdentifier();
//      5.12.27 tsmSensorModel::getPlatformIdentifier();
//      5.12.28 tsmSensorModel::getImageSize();
//      5.12.29 tsmSensorModel::getSensorModelState();
//      5.12.30 tsmSensorModel::getValidAltitudeRange();
//      5.12.31 tsmSensorModel::getIlluminationDirection();
//      5.12.32 tsmSensorModel::getNumSystematicErrorCorrections();
//      5.12.33 tsmSensorModel::getSystematicErrorCorrectionName();
//      5.12.34 tsmSensorModel::setCurrentSystematicErrorCorrectionSwitch();
//      5.12.35 tsmSensorModel::getCurrentSystematicErrorCorrectionSwitch();
//      5.12.36 tsmSensorModel::getReferencePoint();
//      5.12.37 tsmSensorModel::setReferencePoint();
//      5.12.38 tsmSensorModel::getSensorModelName();
//      5.12.39 tsmSensorModel::setParameterType();
//      5.12.40 tsmSensorModel::getCovarianceModel();
//      5.12.41 tsmSensorModel::getUnmodeledError();
//      5.12.42 tsmSensorModel::getUnmodeledCrossCovariance();
//      5.12.43 tsmSensorModel::getCollectionIdentifier();
//      5.12.44 tsmSensorModel::isParameterShareable();
//      5.12.45 tsmSensorModel::getParameterSharingCriteria();

#ifndef __BuckeyeSensorModel_H
#define __BuckeyeSensorModel_H

#include "TSMSensorModel.h"
#include "TSMWarning.h"
#include "TSMError.h"
#include <ossim/matrix/newmat.h>
#include <ossim/matrix/newmatio.h>

//#include "ossimSensorModel.h"
#include <ossim/projection/ossimSensorModel.h>
#include <ossim/projection/ossimSmacCallibrationSystem.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/projection/ossimBuckeyeSensor.h>

using namespace std;
using namespace NEWMAT;

typedef struct parameter {
	double originalValue;
	double currentValue;
	TSMMisc::Param_CharType type;
} parameter;

class BuckeyeSensorModel : public TSMSensorModel   // public ossimSensorModel
{
public:
   ossimBuckeyeSensor *buckeye;
   //---
   // Constructors/Destructor
   //---

   BuckeyeSensorModel();

   virtual ~BuckeyeSensorModel();

   //---
   // Modifiers
   //---
   virtual TSMWarning *groundToImage(
										const double &x,
										const double &y,
										const double &z,
										double& line,
										double& sample,
										double& achieved_precision,
										const double &desired_precision = 0.001)
     throw (TSMError);


      //> The groundToImage() method converts x, y and z (meters) in
      //ground space (ECEF) to line and sample (pixels) in image
      //space.  <

   virtual TSMWarning *groundToImage(
										const double& x,
										const double& y,
										const double& z,
										const double groundCovariance[9],
										double& line,
										double& sample,
										double imageCovariance[4],
										double& achieved_precision,
										const double& desired_precision = 0.001)
        throw (TSMError);

      //> This method converts a given ground point into line and sample
      //  (pixels) in image space and returns accuracy information
      //  associated with the image and ground coordinates.
      //<

	

   virtual TSMWarning *imageToGround(
										const double& line, 
										const double& sample, 
										const double& height,
										double& x, 
										double& y, 
										double& z,
										double& achieved_precision, 
										const double& desired_precision = 0.001)
        throw (TSMError) ;
            
      //> This method converts a given line and sample (pixels) in image space
      //  to a ground point. 
      //<

   virtual TSMWarning *imageToGround(
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
										const double& desired_precision = 0.001)
        throw (TSMError) ;

      //> This method converts a given line and sample (pixels) in image space
      //  to a ground point and returns accuracy information associated with
      //  the image and ground coordinates. 
      //<

   virtual TSMWarning *imageToProximateImagingLocus(
										const double& line, 
										const double& sample, 
										const double& x, 
										const double& y, 
										const double& z, 
										double locus[6],
										double& achieved_precision, 
										const double& desired_precision = 0.001)
        throw (TSMError) ;

      //> The imageToProximateImagingLocus() method computes a proximate
      //  imaging locus, a vector approximation of the imaging locus for the
      //  given line and sample nearest the given x, y and z or at the given
      //  height. The precision of this calculation refers to the locus's 
      //  origin and does not refer to the locus's orientation.
      //<

   virtual TSMWarning *imageToRemoteImagingLocus(
										const double& line, 
										const double& sample, 
										double locus[6], 
										double& achieved_precision, 
										const double& desired_precision = 0.001)
        throw (TSMError) ;

      //> The imageToRemoteImagingLocus() method computes locus, a vector
      //  approximation of the imaging locus for the given line and sample.
      //  The precision of this calculation refers only to the origin of the
      //  locus vector and does not refer to the locus's orientation. For an
      //  explanation of the remote imaging locus, see the section at the 
      //  beginning of this document.
      //<

   //---
   // Uncertainty Propagation
   //---

   virtual TSMWarning *computeGroundPartials(
										const double& x, 
										const double& y, 
										const double& z, 
										double partials[6])
        throw (TSMError) ;

      //> The computeGroundPartials method calculates the partial
      //  derivatives (partials) of image position (both line and sample)
      //  with respect to ground coordinates at the given ground 
      //  position x, y, z.
      //  Upon successful completion, computeGroundPartials() produces the
      //  partial derivatives as follows:
      // 
      //  partials [0] = line wrt x
      //  partials [1] = line wrt y
      //  partials [2] = line wrt z
      //  partials [3] = sample wrt x
      //  partials [4] = sample wrt y
      //  partials [5] = sample wrt z
      //<
   
   virtual TSMWarning *computeSensorPartials(
										const int& index, 
										const double& x, 
										const double& y, 
										const double& z, 
										double& line_partial, 
										double& sample_partial, 
										double& achieved_precision, 
										const double& desired_precision = 0.001)
        throw (TSMError) ;

   virtual TSMWarning *computeSensorPartials(
										const int& index, 
										const double& line, 
										const double& sample, 
										const double& x, 
										const double& y, 
										const double& z, 
										double& line_partial, 
										double& sample_partial, 
										double& achieved_precision, 
										const double& desired_precision = 0.001)
       throw (TSMError);


      //> The computeSensorPartials() method calculates the partial 
      //  derivatives of image position (both line and sample) with 
      //  respect to the given sensor parameter (index) at the given 
      //  ground position.
      //  Two versions of the method are provided. The first method,
      //  computeSensorPartials(), takes in only necessary information. 
      //  It performs groundToImage() on the ground coordinate and then 
      //  calls the second form of the method with the obtained line 
      //  and sample. If the calling function has already performed 
      //  groundToImage with the ground coordinate, it may call the second
      //  method directly since it may be significantly faster than the
      //  first. The results are unpredictable if the line and sample 
      //  provided do not correspond to the result of calling groundToImage()
      //  with the given ground position (x, y, and z).
      //<

   virtual TSMWarning *getCovarianceModelType(int &ret_int) throw (TSMError);

      //> This method return the default type of error model for which
      //  data exists.
      //<

   virtual TSMWarning *getCurrentParameterCovariance(
										const int& index1, 
										const int& index2,
										double &covariance)
               throw (TSMError);

   virtual TSMWarning *setCurrentParameterCovariance(
										const int& index1, 
										const int& index2, 
										const double& covariance)
     throw (TSMError);

      //> The first form of the currentParameterCovariance() method 
      //  returns the covariance of the specified parameter pair 
      //  (index1, index2). The variance of the given parameter can be 
      //  obtained by using the same value for index1 and index2.
      //  The second form of currentParameterCovariance() method is
      //  used to set the covariance value of the specified parameter pair.
      //<

   virtual TSMWarning *getCovarianceModel(tsm_CovarianceModel*& covModel)
		throw (TSMError);
 
      //> This method supplies error parameters pertaining to the given type.
      //  The parameter values are necessary to perform error analysis and 
      //  propagation for the given type.
      //<

   virtual TSMWarning *setOriginalParameterCovariance(
										const int& index1, 
										const int& index2, 
										const double& covariance)
		throw (TSMError);

   virtual TSMWarning *getOriginalParameterCovariance(
										const int&       index1, 
										const int&       index2,
										double &covariance)
      throw (TSMError);

      //> The first form of originalParameterCovariance() method sets
      //  the covariance of the specified parameter pair (index1, index2).
      //  The variance of the given parameter can be set using the same 
      //  value for index1 and index2. 
      //  The second form of originalParameterCovariance() method gets
      //  the covariance of the specified parameter pair (index1, index2). 
      //  The variance of the given parameter can be obtained using the 
      //  same value for index1 and index2.
      //<

   //---
   // Time and Trajectory
   //---

   virtual TSMWarning *getTrajectoryIdentifier(std::string& trajectoryId)
		throw (TSMError);

      //> This method returns a unique identifer to indicate which 
      //  trajectory was used to acquire the image. This ID is unique for
      //  each sensor type on an individual path.
      //<

   virtual TSMWarning *getReferenceDateAndTime(std::string& date_and_time)
		throw (TSMError) ;

      //> This method returns the time in seconds at which the specified
      //  pixel was imaged. The time provide is relative to the reference
      //  date and time given by the getReferenceDateAndTime() method and
      //  can be used to represent time offsets within the trajectory 
      //  associated with the given image.
      //<

   TSMWarning *setReferenceDateAndTime(const std::string& date_and_time)
		throw (TSMError) ;

      //> This method returns the time in seconds at which the specified
      //  pixel was imaged. The time provide is relative to the reference
      //  date and time given by the getReferenceDateAndTime() method and
      //  can be used to represent time offsets within the trajectory 
      //  associated with the given image.
      //<

   virtual TSMWarning *getImageTime(
									const double& line, 
									const double& sample, 
									double&       time) 
		throw (TSMError);

      //> The getImageTime() method returns the time in seconds at which
      //  the pixel specified by line and sample was imaged. The time
      //  provided is relative to the reference date and time given by
      //  getReferenceDateAndTime.
      //<

   virtual TSMWarning *getSensorPosition(
									const double&          line,
									const double&          sample, 
									double&                x, 
									double&                y, 
									double&                z) 
		throw (TSMError) ;

      //> The getSensorPosition() method returns the position of
      //  the physical sensor at the given position in the image.
      //<

   virtual TSMWarning *getSensorPosition(
									const double&          time,
									double&                x, 
									double&                y, 
									double&                z) 
		throw (TSMError) ;

   virtual TSMWarning *getSensorVelocity(
								   const double&    line,
								   const double&    sample, 
								   double&          vx, 
								   double&          vy, 
								   double&          vz) 
		throw (TSMError) ;

      //> The getSensorVelocity() method returns the velocity
      //  of the physical sensor at the given position in the image.
      //<

   virtual TSMWarning *getSensorVelocity(
								   const double&    time,
								   double&          vx, 
								   double&          vy, 
								   double&          vz) 
		throw (TSMError) ;

   //---
   // Sensor Model Parameters
   //---

   virtual TSMWarning *setCurrentParameterValue(
									   const int&          index, 
									   const double&       value)
		throw (TSMError) ;

   virtual TSMWarning *getCurrentParameterValue(const int& index, double &ret_double) 
		throw (TSMError) ;

      //> The first form of currentParameterValue() is used to set the 
      //  value of the adjustable parameter indicated by index.
      // 
      //  The second form of currentParameterValue()returns the value
      //  of the adjustable parameter given by index.
      //<

   virtual TSMWarning *getParameterName(
									const int&       index, 
									std::string&     name) 
		throw (TSMError) ;

      //> This method returns the name for the sensor model parameter
      //  indicated by the given index.
      //<

   virtual TSMWarning *getNumParameters(int &ret_int) throw (TSMError) ;

      //> This method returns the number of sensor model parameters.
      //<

   virtual TSMWarning *setOriginalParameterValue(
								   const int&      index,
								   const double&   value)
     throw (TSMError);

   virtual TSMWarning *getOriginalParameterValue(
									const int& index, 
									double &ret_double) 
     throw (TSMError) ;

      //> The first form of the originalParameterValue() method is 
      //  used to set the original parameter value of the indexed 
      //  parameter and to set the parameter type indicated by 
      //  parameterType.
      //  The second form of the originalParameterValue() method 
      //  returns the value of the adjustable parameter given by 
      //  index.
      //<

	virtual TSMWarning* setParameterType(
										 const int& index,
										 const TSMMisc::Param_CharType &type) 
	throw (TSMError);

	virtual TSMWarning *getParameterType(
								const int&       index, 
								TSMMisc::Param_CharType &Type)
     throw (TSMError);

      //> The parameterType() method returns the type of the parameter 
      //  given by index. 
      //<

   //---
   // Sensor Model Information
   //---

   virtual TSMWarning *getPedigree(std::string& pedigree) 
	   throw (TSMError);

      //> The getPedigree() method returns a character string that
      //  identifies the sensor, the model type, its mode of acquisition
      //  and processing path. For example, an image that could produce
      //  either an optical sensor model or a cubic rational polynomial
      //  model would produce different pedigrees for each case.
      //<

   virtual TSMWarning *getImageIdentifier(std::string&    imageId) 
	   throw (TSMError) ;

      //> This method returns the unique identifier to indicate the imaging 
      //  operation associated with this sensor model.
      //<

   virtual TSMWarning *setImageIdentifier(const std::string& imageId) 
		throw (TSMError) ;

      //> This method sets the unique identifier for the image to which the
      //  sensor model pertains.
      //<

   virtual TSMWarning *getSensorIdentifier(std::string& sensorId) 
	   throw (TSMError) ;

      //> The getSensorIdentifier() method returns sensorId to indicate
      //  which sensor was used to acquire the image. This sensorId is 
      //  meant to uniquely identify the sensor used to make the image.
      //<

   virtual TSMWarning *getPlatformIdentifier(std::string& platformId) 
	   throw (TSMError) ;

      //> The getPlatformIdentifier() method returns platformId to indicate
      //  which platform was used to acquire the image. This platformId
      //  is meant to uniquely identify the platform used to collect the image.
      //<

   virtual TSMWarning *getReferencePoint(
										double&          x, 
										double&          y, 
										double&          z) 
		throw (TSMError) ;

      //> This method returns the ground point indicating the general location
      //  of the image.
      //<

   virtual TSMWarning *setReferencePoint(
										const double&        x, 
										const double&        y, 
										const double&        z) 
		throw (TSMError) ;

      //> This method sets the ground point indicating the general location
      //  of the image.
      //<

   virtual TSMWarning *getImageSize(
										int&     num_lines,
										int&     num_samples) 
		throw (TSMError) ;

      //> This method returns the number of lines and samples in the imaging
      //  operation.
      //<

   //---
   //  Monoscopic Mensuration
   //---

   virtual TSMWarning *getValidAltitudeRange(
									   double& minAltitude,
									   double& maxAltitude) 
		throw (TSMError);

      //> The validAltitudeRange() method returns the minimum and maximum
      //  altitudes that describe the range of validity of the model. For
      //  example, the model may not be valid at altitudes above the altitude
      //  of the sensor for physical models. 
      //<

   virtual TSMWarning *getIlluminationDirection(
									   const double& x, 
									   const double& y, 
									   const double& z, 
									   double& direction_x,
									   double& direction_y,
									   double& direction_z) 
		throw (TSMError) ;

      //> The getIlluminationDirection() method calculates the direction of 
      //  illumination at the given ground position x, y, z.
      //<


   //---
   //  Error Correction
   //---

   virtual TSMWarning *getNumSystematicErrorCorrections(int &ret_int) 
     throw (TSMError) ;

      //> The numSystematicErrorCorrections() method returns the number
      //  of systematic error corrections defined for the sensor model.
      //<

   virtual TSMWarning *getSystematicErrorCorrectionName(
													   const int& index,
													   std::string &name) 
	throw (TSMError) ;

      //> This method returns the name for the sensor model parameter
      //  indicated by the given index.
      //<

   virtual TSMWarning *setCurrentSystematicErrorCorrectionSwitch(
													   const int& index,
													   const bool &value,
													   const TSMMisc::Param_CharType& parameterType) 
     throw (TSMError);

   virtual TSMWarning *getCurrentSystematicErrorCorrectionSwitch(
														const int& index, bool &ret_bool) 
	throw (TSMError);

      //> The first form of currentSystematicErrorCorrectionSwitch() is
      //  used to set the switch of the systematic error correction
      //  indicated by index.
      //
      //  The second form of currentSystematicErrorCorrectionSwitch()
      //  returns the value of the systematic error correction switch
      //  given by index.
      //<


   virtual TSMWarning *getSensorModelName(std::string& name) 
	   throw (TSMError) ;
	//> This method returns the name of the sensor model.

   //---
   // Sensor Model State
   //---

   virtual TSMWarning *getSensorModelState(std::string &ret_string) 
			throw (TSMError) ;

      //> This method returns the current state of the model in an 
      //  intermediate form. This intermediate form can then processed,
      //  for example, by saving to file so that this model 
      //  can be instantiated at a later date. The derived SensorModel 
      //  is responsible for saving all information needed to restore 
      //  itself to its current state from this intermediate form.
      //  A NULL pointer is returned if it is not possible to save the
      //  current state.
      //<

   virtual TSMWarning* getUnmodeledError(
										const double, 
										const double,
										double*)
		throw (TSMError);

   virtual TSMWarning* getUnmodeledCrossCovariance(
														const double,
														const double,
														const double,
														const double,
														double*) 
		throw (TSMError);

   virtual TSMWarning* getCollectionIdentifier(std::string& collectionId)
		throw (TSMError);

   virtual TSMWarning* isParameterShareable(
											   const int& index,
											   bool& shareable)
   throw (TSMError);

   virtual TSMWarning* getParameterSharingCriteria(
												   const int& index,
												   bool& requireModelNameMatch,
												   bool& requireSensorIdMatch,
												   bool& requirePlatformIdMatch,
												   bool& requireCollectionIdMatch,
												   bool& requireTrajectoryIdMatch,
												   bool& requireDateTimeMatch,
												   double& allowableTimeDelta)
   throw (TSMError);

void    setSensorPosition(ossimGpt& platformPosition);
ossimGpt getSensorPosition();

void     setPrincipalPoint(ossimDpt principalPoint);
ossimDpt getPrincipalPoint();

void    setFocalLength(double focalLength);
double  getFocalLength();

/*void setRadialLensDistortion(double RadialLensDistortion1,
											    double RadialLensDistortion2,
												double RadialLensDistortion3,
												double Decentering1,
												double Decentering2);

void setAfineDistance(double affineDist1 = 0.0;
										  double affineDist2 = 0.0);
void setBoresightingOffset(double offset1 = 0.0,
											   double offset2 = 0.0,
											   double offset3 = 0.0);
void setSensorTranslation(double sensorTrans1 = 0.0;
											  double sensorTrans2 = 0.0;
											  double SensorTrans3 = 0.0);
void setSlantRange(double slantRange = 0.0);   // For now a value of 0 is an error 
*/
	
void setPixelSize(ossimDpt pixelSize);	
void setImageSize(double line, double sample);

void imagingRay(const ossimDpt& image_point,
									ossimEcefRay&   image_ray) const;

   // This method returns the rotation matrix.

private:
	
		static const int NUMBER_OF_PARAMETERS = 23;
		std::vector<parameter> parameters;

        std::string imageID;
        std::string imageDateTime;

		Matrix		originalCovariance;
        Matrix		currentCovariance;

        double		referencePointX;
        double		referencePointY;
        double		referencePointZ;
        bool		referencePointSet;
                	
		ossimDpt       m_pixelSize;      // in millimeters
		ossimDpt       m_principalPoint; // in millimeters
		ossimEcefPoint m_ecefPlatformPosition;
	
		ossimDpt	m_ImageSize; 
		double		m_focalLength;
	
		ossimRefPtr<ossimSmacCallibrationSystem> m_lensDistortion;
		
		static const std::string PARAMETER_NAMES[NUMBER_OF_PARAMETERS];
        static const char* SENSOR_MODEL_NAME;

};

#endif // __BuckeyeSensorModel_H
