#ifndef ossimPredatorUavProjection_HEADER
#define ossimPredatorUavProjection_HEADER
#include <ossim/base/ossimMatrix4x4.h>
#include <ossim/base/ossimLsrSpace.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/elevation/ossimElevManager.h>
#include <ossim/projection/ossimSensorModel.h>
#include <ossimPredator/ossimPredatorExport.h>

class OSSIMPREDATOR_DLL ossimPredatorUavProjection : public ossimSensorModel
{
public:
   ossimPredatorUavProjection();
   virtual ~ossimPredatorUavProjection();
   void setParameters(ossim_uint32 imageWidth,
                      ossim_uint32 imageHeight,
                      const ossimGpt& sensorPosition,
                      const ossimGpt& frameCenter,
                      const ossim_float64 platformHeading,
                      const ossim_float64 platformPitch,
                      const ossim_float64 platformRoll,
                      ossim_float64 hfov,
                      ossim_float64 vfov,
                      ossim_float64 obliquityAngle,
                      ossim_float64 angleToNorth,
                      ossim_float64 targetWidthInMeters,
                      ossim_float64 slantRange
                      );

   virtual void updateModel();
   virtual ossimObject* dup()const;
   virtual void imagingRay(const ossimDpt& image_point,
                           ossimEcefRay&   image_ray) const;
   virtual bool useForward()const;
   void lineSampleToWorld(const ossimDpt& image_point,
                          ossimGpt&       gpt) const;
   virtual void lineSampleHeightToWorld(const ossimDpt& image_point,
                                        const double&   heightEllipsoid,
                                        ossimGpt&       worldPoint) const;
   virtual ossimGpt origin()const;

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix = 0)const;
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix = 0);
protected:
   ossim_float32           theWidth;
   ossim_float32           theHeight;
   ossimGpt                theSensorPosition;
   ossimEcefPoint          theSensorEcefPosition;
   ossimEcefPoint          theAdjNadirSensorEcefPosition;
   ossimGpt                theFrameCenter;
   ossimEcefPoint          theFrameCenterEcef;
   ossim_float64           thePlatformHeading;
   ossim_float64           thePlatformPitch;
   ossim_float64           thePlatformRoll;
   ossim_float64           theHorizontalFieldOfView;
   ossim_float64           theVerticalFieldOfView;
   ossim_float64           theObliquityAngle;
   ossim_float64           theAngleToNorth;
   ossim_float64           theTargetWidthInMeters;
   ossim_float64           theSlantRange;
   NEWMAT::Matrix          theProjectionMatrix;
   NEWMAT::Matrix          theOrientationMatrix;
   NEWMAT::Matrix          theOrientationMatrixInverse;
   
   
   NEWMAT::Matrix m_air2Ned;
   NEWMAT::Matrix m_ecef2Ned;
   NEWMAT::Matrix m_ecef2NedInverse;
   
};

#endif //ossimPredatorUavProjection_HEADER
