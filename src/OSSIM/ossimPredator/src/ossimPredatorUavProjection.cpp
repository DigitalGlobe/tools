#include <ossimPredator/ossimPredatorUavProjection.h>

ossimPredatorUavProjection::ossimPredatorUavProjection()
{
}

ossimPredatorUavProjection::~ossimPredatorUavProjection()
{
}

void ossimPredatorUavProjection::setParameters(ossim_uint32 imageWidth,
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
                                               )
{
   theWidth                 = imageWidth;
   theHeight                = imageHeight;
   theSensorPosition        = sensorPosition;
   theSensorEcefPosition    = theSensorPosition;
   theFrameCenter           = frameCenter;
   theFrameCenterEcef       = theFrameCenter;
   theObliquityAngle        = obliquityAngle;
   theAngleToNorth          = angleToNorth;
   theTargetWidthInMeters   = targetWidthInMeters;
   theSlantRange            = slantRange;
   theFrameCenter           = frameCenter;
   theHorizontalFieldOfView = hfov;
   theVerticalFieldOfView   = vfov;
   thePlatformHeading       = platformHeading;
   thePlatformPitch         = platformPitch;
   thePlatformRoll          = platformRoll;
   
   
   std::cout << "ROLL = " << thePlatformRoll << std::endl;
   std::cout << "Pitch = " << thePlatformPitch << std::endl;
   std::cout << "Obliq = " << theObliquityAngle << std::endl;
   updateModel();
}

void ossimPredatorUavProjection::updateModel()
{
   theRefImgPt = ossimDpt(theWidth*.5,
                          theHeight*.5);

#if 1
   double r = ossim::degreesToRadians(thePlatformRoll );
   double obliquity = ossim::degreesToRadians(theObliquityAngle );
   double p = ossim::degreesToRadians(thePlatformPitch );
   double y = ossim::degreesToRadians(theAngleToNorth);
   NEWMAT::Matrix rollM   = ossimMatrix3x3::create(1, 0, 0,
                                                   0, cos(r), sin(r),
                                                   0, -sin(r), cos(r));
   NEWMAT::Matrix pitchM  = ossimMatrix3x3::create(cos(p), 0, -sin(p),
                                                   0,      1, 0,
                                                   sin(p), 0, cos(p));
   NEWMAT::Matrix yawM    = ossimMatrix3x3::create(cos(y), sin(y), 0,
                                                   -sin(y), cos(y), 0,
                                                   0,0,1); 
   NEWMAT::Matrix obliquityM  = ossimMatrix3x3::create(cos(obliquity), 0, -sin(obliquity),
                                                   0,      1, 0,
                                                   sin(obliquity), 0, cos(obliquity));
   m_air2Ned = rollM*yawM*obliquityM;
//   m_air2Ned = rollM*pitchM*yawM*obliquityM;
   double sin_lat = ossim::sind(theSensorPosition.latd());
   double cos_lat = ossim::cosd(theSensorPosition.latd());
   double sin_lon = ossim::sind(theSensorPosition.lond());
   double cos_lon = ossim::cosd(theSensorPosition.lond());
   m_ecef2Ned = (ossimMatrix3x3::create(0,0,1,0,1,0,-1,0,0)*
                 ossimMatrix3x3::create(cos_lat, 0, sin_lat, 0,1,0, -sin_lat,0,cos_lat)*
                 ossimMatrix3x3::create(cos_lon,sin_lon,0, -sin_lon, cos_lon, 0, 0, 0, 1));
   m_ecef2NedInverse = m_ecef2Ned.t();
   //   theOrientationMatrix = rollM*pitchM*yawM;
#else   
   // get the Lsr matrix which is oriented nadir to the platform position
   //
   ossimLsrSpace lsrSpace(theSensorPosition);//, thePlatformHeading);
   ossimMatrix4x4 lsrMatrix(lsrSpace.lsrToEcefRotMatrix());
   theOrientationMatrix = lsrMatrix.getData()
   //                              *ossimMatrix4x4::createRotationXMatrix(thePlatformPitch, OSSIM_RIGHT_HANDED)
   //                              *ossimMatrix4x4::createRotationZMatrix(thePlatformHeading, OSSIM_RIGHT_HANDED)
   *ossimMatrix4x4::createRotationZMatrix(theAngleToNorth, OSSIM_RIGHT_HANDED)
   *ossimMatrix4x4::createRotationXMatrix(thePlatformRoll, OSSIM_RIGHT_HANDED)
   *ossimMatrix4x4::createRotationYMatrix(theObliquityAngle, OSSIM_RIGHT_HANDED);
#endif
//   theOrientationMatrixInverse = theOrientationMatrix.i();
   
}

ossimObject* ossimPredatorUavProjection::dup()const
{
   return new ossimPredatorUavProjection(*this);
}

void ossimPredatorUavProjection::imagingRay(const ossimDpt& image_point,
                                            ossimEcefRay&   image_ray) const
{
#if 1
   ossimDpt film (image_point - theRefImgPt);
   film.x/=(theWidth);
   film.y/=(theHeight);
   ossim_float64 rotX = film.y*(theVerticalFieldOfView*.5);
   ossim_float64 rotY = film.x*(theHorizontalFieldOfView*.5);
   
  
   ossimColumnVector3d cam_ray_dir (0.0,
                                    0.0,
                                    1.0);
   ossimEcefVector     ecf_ray_dir ( m_ecef2NedInverse*m_air2Ned*
                                    ossimMatrix3x3::createRotationYMatrix(rotY, OSSIM_LEFT_HANDED)* 
                                    ossimMatrix3x3::createRotationXMatrix(rotX, OSSIM_LEFT_HANDED)*
                                    cam_ray_dir);
   ecf_ray_dir.normalize();
   
   image_ray.setOrigin(theSensorPosition);
   image_ray.setDirection(ecf_ray_dir);
   
#else
   ossimDpt film (image_point - theRefImgPt);
   film.x/=(theWidth);
   film.y/=(theHeight);
   ossim_float64 rotX = film.y*(theVerticalFieldOfView*.5);
   ossim_float64 rotY = film.x*(theHorizontalFieldOfView*.5);
   
   ossimColumnVector3d cam_ray_dir (0.0,
                                    0.0,
                                    -1.0);
   ossimEcefVector     ecf_ray_dir (theOrientationMatrix*
                                    ossimMatrix4x4::createRotationYMatrix(rotY, OSSIM_RIGHT_HANDED)* 
                                    ossimMatrix4x4::createRotationXMatrix(rotX, OSSIM_RIGHT_HANDED)*
                                    cam_ray_dir);
   ecf_ray_dir.normalize();
   
   image_ray.setOrigin(theSensorPosition);
   image_ray.setDirection(ecf_ray_dir);
#endif
}

bool ossimPredatorUavProjection::useForward()const
{
   return true;
}

void ossimPredatorUavProjection::lineSampleToWorld(const ossimDpt& image_point,
                                                   ossimGpt&       gpt) const
{
   ossimEcefRay ray;
   imagingRay(image_point, ray);
   ossimElevManager::instance()->intersectRay(ray, gpt);         
}

void ossimPredatorUavProjection::lineSampleHeightToWorld(const ossimDpt& image_point,
                                                         const double&   heightEllipsoid,
                                                         ossimGpt&       worldPoint) const
{
   ossimEcefRay ray;
   imagingRay(image_point, ray);
   ossimEcefPoint Pecf (ray.intersectAboveEarthEllipsoid(heightEllipsoid));
   worldPoint = ossimGpt(Pecf);
}

ossimGpt ossimPredatorUavProjection::origin()const
{
   return theSensorPosition;
}

bool ossimPredatorUavProjection::saveState(ossimKeywordlist& kwl,
                                           const char* prefix)const
{
   kwl.add(prefix,
           "type",
           "ossimUavProjection",
           true);
   kwl.add(prefix,
           "sensor_origin",
           theSensorPosition.toString(),
           true);
   kwl.add(prefix,
           "frame_center",
           theFrameCenter.toString(),
           true);
   kwl.add(prefix,
           "image_width",
           theWidth,
           true);
   kwl.add(prefix,
           "image_height",
           theHeight,
           true);
   kwl.add(prefix,
           "hfov",
           theHorizontalFieldOfView,
           true);
   kwl.add(prefix,
           "vfov",
           theVerticalFieldOfView,
           true);
   kwl.add(prefix,
           "obliquity_angle",
           theObliquityAngle,
           true);
   kwl.add(prefix,
           "angle_to_north",
           theAngleToNorth,
           true);
   kwl.add(prefix,
           "target_width",
           theTargetWidthInMeters,
           true);
   kwl.add(prefix,
           "slant_range",
           theSlantRange,
           true);
   kwl.add(prefix,
           "platform_heading",
           thePlatformHeading,
           true);
   kwl.add(prefix,
           "platform_pitch",
           thePlatformPitch,
           true);
   kwl.add(prefix,
           "platform_roll",
           thePlatformRoll,
           true);
   
   return true;
}

bool ossimPredatorUavProjection::loadState(const ossimKeywordlist& kwl,
                                           const char* prefix)
{
   const char* sensor_origin = kwl.find(prefix, "sensor_origin");
   const char* frame_center = kwl.find(prefix, "frame_center");
   const char* image_width  = kwl.find(prefix, "image_width");
   const char* image_height = kwl.find(prefix, "image_height");
   const char* hfov = kwl.find(prefix, "hfov");
   const char* vfov = kwl.find(prefix, "vfov");
   const char* obliquity_angle = kwl.find(prefix, "obliquity_angle");
   const char* angle_to_north = kwl.find(prefix, "angle_to_north");
   const char* target_width = kwl.find(prefix, "target_width");
   const char* slant_range = kwl.find(prefix, "slant_range");
   const char* h = kwl.find(prefix, "platform_heading");
   const char* p = kwl.find(prefix, "platform_pitch");
   const char* r = kwl.find(prefix, "platform_roll");
   if(sensor_origin&&
      image_height&&
      image_width&&
      hfov&&
      vfov&&
      obliquity_angle&&
      angle_to_north&&
      frame_center&&
      slant_range&&
      h&&
      p&&
      r)
   {
      ossimGpt origin;
      ossimGpt frameCenter;
      origin.toPoint(sensor_origin);
      frameCenter.toPoint(frame_center);
      setParameters(ossimString(image_width).toUInt32(),
                    ossimString(image_height).toUInt32(),
                    origin,
                    frameCenter,
                    ossimString(h).toFloat64(),
                    ossimString(p).toFloat64(),
                    ossimString(r).toFloat64(),
                    ossimString(hfov).toFloat64(),
                    ossimString(vfov).toFloat64(),
                    ossimString(obliquity_angle).toFloat64(),
                    ossimString(angle_to_north).toFloat64(),
                    ossimString(target_width).toFloat64(),
                    ossimString(slant_range).toFloat64()
                    );
      return true;
   }
   return false;
}
