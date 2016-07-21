///*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// DESCRIPTION:
//   Contains implementation of class Ellipsoid. The implementation is
//   actually for an OBLATE SPHEROID (x.radius = y.radius) as Earth is
//   considered.
//
// SOFTWARE HISTORY:
//>
//   06Aug2001  Garrett Potts, Oscar Kramer
//              Initial coding.
//<
//*****************************************************************************
// $Id$

#include <cmath>

#include <pqe/Ellipsoid.h>

#include <pqe/EcefRay.h>
#include <pqe/EcefPoint.h>
#include <pqe/EcefVector.h>
#include <pqe/Matrix4x4.h>

pqe::Ellipsoid::Ellipsoid(const pqe::Ellipsoid &ellipsoid)
   :
      theName(ellipsoid.theName),
      theCode(ellipsoid.theCode),
      theA(ellipsoid.theA),
      theB(ellipsoid.theB),
      theFlattening(ellipsoid.theFlattening),
      theA_squared(ellipsoid.theA_squared),
      theB_squared(ellipsoid.theB_squared),
      theEccentricitySquared(ellipsoid.theEccentricitySquared)
{
}

pqe::Ellipsoid::Ellipsoid(const std::string &name,
                          const std::string &code,
                          const pqe_float64 &a,
                          const pqe_float64 &b)
   :
      theName(name),
      theCode(code),
      theA(a),
      theB(b),
      theA_squared(a*a),
      theB_squared(b*b),
      theEccentricitySquared(0.0)
{
   computeFlattening();
   
   theEccentricitySquared = 2*theFlattening - theFlattening*theFlattening;
}

pqe::Ellipsoid::Ellipsoid()
   :
   theName("WGS 84"),
   theCode("WE"),
   theA(6378137.0),
   theB(6356752.3142),
   theA_squared(0.0),
   theB_squared(0.0),
   theEccentricitySquared(0.0)
{
   theA_squared = theA * theA;
   theB_squared = theB * theB;

   computeFlattening();
   
   theEccentricitySquared = 2*theFlattening - theFlattening*theFlattening;

}

pqe::Ellipsoid::Ellipsoid(const pqe_float64 &a,
                          const pqe_float64 &b)
   :
      theName(""), // initialize to empty
      theCode(""),
      theA(a),
      theB(b),
      theA_squared(a*a),
      theB_squared(b*b)
{
   computeFlattening();

   theEccentricitySquared = 2*theFlattening - theFlattening*theFlattening;
 }

const std::string& pqe::Ellipsoid::name()const
{
   return theName;
}

const std::string& pqe::Ellipsoid::code()const
{
   return theCode;
}

const pqe_float64& pqe::Ellipsoid::a()const
{
   return theA;
}

const pqe_float64& pqe::Ellipsoid::b()const
{
   return theB;
}
      
const pqe_float64& pqe::Ellipsoid::getA()const
{
   return theA;
}

const pqe_float64& pqe::Ellipsoid::getB()const
{
   return theB;
}

const pqe_float64& pqe::Ellipsoid::getFlattening() const
{
   return theFlattening;
}
      
void pqe::Ellipsoid::setA(pqe_float64 a)
{
   theA = a;computeFlattening();
}

void pqe::Ellipsoid::setB(pqe_float64 b)
{
   theB = b;computeFlattening();
}

void pqe::Ellipsoid::setAB(pqe_float64 a, pqe_float64 b)
{
   theA = a;
   theB = b; computeFlattening();
}
      
pqe_float64 pqe::Ellipsoid::eccentricitySquared() const
{
   return theEccentricitySquared;
}

pqe_float64 pqe::Ellipsoid::flattening() const
{
   return theFlattening;
}
      
pqe_float64 pqe::Ellipsoid::eccentricity() const
{
   return std::sqrt(theEccentricitySquared);
}

//*****************************************************************************
//  METHOD: Ellipsoid::nearestIntersection
//  
//*****************************************************************************
bool pqe::Ellipsoid::nearestIntersection(const pqe::EcefRay &ray,
                                         pqe::EcefPoint& rtnPt) const
{
   return nearestIntersection(ray, 0.0, rtnPt);
}

//*****************************************************************************
//  METHOD: Ellipsoid::nearestIntersection
//  
//   geographic objects that are derive this class will asssume that
//   the reference datum is wgs84 and that the ray origin is a
//   geocentric coordinate relative to the wgs84 datum.  Will return
//   true if the object was intersected and false otherwise.
//  
//   The nearest intersection will use the ray sphere intersection
//   found in most ray tracers.  We will take a Ray defined by the
//   parametric equation:
//  
//     x = x0 + dxt
//     y = y0 + dyt
//     z = z0 + dzt
//  
//   and intersect this with the equation of a spheroid:
//  
//     x^2/theXRadius^2 + y^2/theYRadius^2 + z^2/theZRadius^2 = 1
//  
//  
//   the intersection is achived by substituting the parametric line
//   into the equation of the sphereroid.  By doing this you should
//   get a quadratic in t and the equation should look like this:
//  
//    a*t^2 + b*t + c = 0
//  
//      let a = dx^2/theXRadius^2 + dy^2/theYRadius^2 + dz^2/theZRadius^2
//      let b = 2*(x0*dx/theXRadius^2 +y0*dy/theYRadius^2 + z0*dz/theZRadius^2
//      let c = x0^2/theXRadius^2 + y0^2/theYRadius^2 + z0^2/theZRadius^2 - 1
//  
//  
//    Now solve the quadratic (-b +- sqrt(b^2 - 4ac) ) / 2a
//  
//    After solving for t, the parameter is applied to the ray to determine
//    the 3D point position in X,Y,Z, passed back in rtnPt. The boolean
//    "true" is returned if an intersection was found.
//
//*****************************************************************************
bool pqe::Ellipsoid::nearestIntersection(const pqe::EcefRay& ray,
                                         const pqe_float64&  offset,
                                         pqe::EcefPoint&     rtnPt) const
{
   pqe_float64 A_squared = (theA + offset)*(theA + offset);
   pqe_float64 B_squared = (theB + offset)*(theB + offset);

//    pqe_float64 adjustedOffset = offset/theA;
//    pqe_float64 bOverA = theB/theA;
//    pqe_float64 A_squared = (1.0+adjustedOffset)*(1.0+adjustedOffset);
//    pqe_float64 B_squared = ((bOverA+adjustedOffset)*(bOverA+adjustedOffset));//(theB + offset)*(theB + offset);
   
   //***
   // get the origin and direction of ray:
   //***
   pqe::EcefPoint  start = ray.origin();   
   pqe::EcefVector direction = ray.direction();
//    start = pqe::EcefPoint(start.x()/theA,
//                           start.y()/theA,
//                           start.z()/theA);
   //***
   // Solve the coefficents of the quadratic formula
   //***
   pqe_float64 a = ((direction.x() * direction.x())/A_squared) +
              ((direction.y() * direction.y())/A_squared) +
              ((direction.z() * direction.z())/B_squared);

   pqe_float64 b = 2.0*( ((start.x()*direction.x())/A_squared) +
                    ((start.y()*direction.y())/A_squared) +
                    ((start.z()*direction.z())/B_squared) );

   pqe_float64 c = ((start.x()*start.x())/A_squared) +
              ((start.y()*start.y())/A_squared) +
              ((start.z()*start.z())/B_squared) - 1.0;
   
   //***
   // solve the quadratic
   //***
   pqe_float64 root = b*b - 4*a*c;
   pqe_float64 t;
   if(root < 0.0)
   {
      return false;
   }
   else
   {
      pqe_float64 squareRoot = std::sqrt(root);
      pqe_float64 t1 = (-b + squareRoot ) / (2.0*a);
      pqe_float64 t2 = (-b - squareRoot ) / (2.0*a);

      //***
      // sort t1 and t2 and take the nearest intersection if they
      // are in front of the ray.
      //***
      if(t2 < t1)
      {
         pqe_float64 temp = t1;
         t1 = t2;
         t2 = temp;
      }     

       if(t1 > 0.0)
          t = t1;
       else
          t = t2;
//      t = t1;
   }

   //***
   // Now apply solved t to ray to extrapolate correct distance to intersection
   //***
   bool rtnval = false;
   if (t >= 0)
   {
      rtnval = true;
      rtnPt  = ray.extend(t); 
//       rtnPt  = ray.extend(t*theA); 
   }
      
   return rtnval; 
}


//*****************************************************************************
//  METHOD: Ellipsoid::evaluate(ColumnVector3d)
//  
//  Returns neg number if inside, 0 if on, and pos number if outside of
//  ellipsoid.
//  
//*****************************************************************************
pqe_float64 pqe::Ellipsoid::evaluate(const pqe::EcefPoint &location)const
{
   //***
   // get the axis
   //***
   return (((location.x() * location.x())/theA_squared) +
           ((location.y() * location.y())/theA_squared) +
           ((location.z() * location.z())/theB_squared) - 1.0);   
}
 
//*****************************************************************************
//  METHOD: Ellipsoid::gradient()  version A
//  
//  Returns vector normal to the ellipsoid at point specified.
//  
//*****************************************************************************
void pqe::Ellipsoid::gradient(const pqe::EcefPoint& location,
                              pqe::EcefVector&      result) const
{
   result.x() = (2.0*location.x())/theA_squared;
   result.y() = (2.0*location.y())/theA_squared;
   result.z() = (2.0*location.z())/theB_squared;
}

//*****************************************************************************
//  METHOD: Ellipsoid::gradient()  version B
//  
//  Returns vector normal to the ellipsoid at point specified.
//  
//*****************************************************************************
pqe::EcefVector
pqe::Ellipsoid::gradient(const pqe::EcefPoint &location)const
{
   pqe::EcefVector result;
   gradient(location, result);
   return result;
}
 
//*****************************************************************************
//  METHOD: Ellipsoid::prinRadiiOfCurv()
//  
//  Computes the meridional radius and prime vertical at given point.
//  
//*****************************************************************************
void pqe::Ellipsoid::prinRadiiOfCurv(const pqe::EcefPoint& location,
                                           pqe_float64& merRadius,
                                           pqe_float64& primeVert) const
{
   pqe_float64 lat, lon, hgt;
   XYZToLatLonHeight(location.x(), location.y(), location.z(), lat, lon, hgt);
   
   pqe_float64 sinPhi = std::sin(lat*RAD_PER_DEG);
   pqe_float64 phiFac = 1.0 - theEccentricitySquared*sinPhi*sinPhi;
   primeVert = theA / std::sqrt(phiFac);
   merRadius = theA*(1.0-theEccentricitySquared) /
      std::sqrt(phiFac*phiFac*phiFac);
}

 
//*****************************************************************************
//  METHOD: Ellipsoid::jacobianWrtEcef()
//  
//  Forms Jacobian of partials of geodetic WRT ECF at given point.
//           -                           -
//           | pLat/pX  pLat/pY  pLat/pZ |
//    jMat = | pLon/pX  pLon/pY  pLon/pZ |
//           | pHgt/pX  pHgt/pY  pHgt/pZ |
//           -                           -
//  
//*****************************************************************************
void pqe::Ellipsoid::jacobianWrtEcef(const pqe::EcefPoint& location,
                                           pqe::Matrix& jMat) const
{
   pqe_float64 primeVert;
   pqe_float64 merRadius;
   pqe_float64 lat, lon, hgt;
   
   XYZToLatLonHeight(location.x(), location.y(), location.z(), lat, lon, hgt);
   prinRadiiOfCurv(location, merRadius, primeVert);
   
   pqe_float64 sinPhi = std::sin(lat*RAD_PER_DEG);
   pqe_float64 cosPhi = std::cos(lat*RAD_PER_DEG);
   pqe_float64 sinLam = std::sin(lon*RAD_PER_DEG);
   pqe_float64 cosLam = std::cos(lon*RAD_PER_DEG);
   pqe_float64 N_plus_h = primeVert + hgt;
   pqe_float64 M_plus_h = merRadius + hgt;
   
   jMat(1,1) = -sinPhi * cosLam / M_plus_h;
   jMat(2,1) = -sinLam / (cosPhi * N_plus_h);
   jMat(3,1) = cosPhi * cosLam;
   jMat(1,2) = -sinPhi * sinLam / M_plus_h;
   jMat(2,2) =  cosLam / (cosPhi * N_plus_h);
   jMat(3,2) = cosPhi * sinLam;
   jMat(1,3) = cosPhi / M_plus_h;
   jMat(2,3) = 0.0;
   jMat(3,3) = sinPhi;
}

 
//*****************************************************************************
//  METHOD: Ellipsoid::jacobianWrtGeo()
//  
//  Forms Jacobian of partials of ECF WRT geodetic at given point.
//           -                           -
//           | pX/pLat  pX/pLon  pX/pHgt |
//    jMat = | pY/pLat  pY/pLon  pY/pHgt |
//           | pZ/pLat  pZ/pLon  pZ/pHgt |
//           -                           -
//  
//*****************************************************************************
void pqe::Ellipsoid::jacobianWrtGeo(const pqe::EcefPoint& location,
                                          pqe::Matrix& jMat) const
{
   pqe_float64 primeVert;
   pqe_float64 merRadius;
   pqe_float64 lat, lon, hgt;
   
   XYZToLatLonHeight(location.x(), location.y(), location.z(), lat, lon, hgt);
   prinRadiiOfCurv(location, merRadius, primeVert);
   
   pqe_float64 sinPhi = std::sin(lat*RAD_PER_DEG);
   pqe_float64 cosPhi = std::cos(lat*RAD_PER_DEG);
   pqe_float64 sinLam = std::sin(lon*RAD_PER_DEG);
   pqe_float64 cosLam = std::cos(lon*RAD_PER_DEG);
   pqe_float64 N_plus_h = primeVert + hgt;
   pqe_float64 M_plus_h = merRadius + hgt;
   
   jMat(1,1) = -M_plus_h * sinPhi * cosLam;
   jMat(2,1) = -M_plus_h * sinPhi * sinLam;
   jMat(3,1) =  M_plus_h * cosPhi;
   jMat(1,2) = -N_plus_h * cosPhi * sinLam;
   jMat(2,2) =  N_plus_h * cosPhi * cosLam;
   jMat(3,2) = 0.0;
   jMat(1,3) = cosPhi * cosLam;
   jMat(2,3) = cosPhi * sinLam;
   jMat(3,3) = sinPhi;
}


//*****************************************************************************
//  METHOD: Ellipsoid::geodeticRadius()
//  
//  Computes the "geodetic" radius for a given latitude in degrees
//  
//*****************************************************************************
pqe_float64 pqe::Ellipsoid::geodeticRadius(const pqe_float64& lat) const
{
   pqe_float64 sin_lat = pqe::sind(lat);
   pqe_float64 sin2_lat = sin_lat*sin_lat;
   
   return (theA_squared/
           std::sqrt(theA_squared - theA_squared*sin2_lat +
                     theB_squared*sin2_lat));
}

void pqe::Ellipsoid::latLonHeightToXYZ(pqe_float64 lat,
                                       pqe_float64 lon,
                                       pqe_float64 height,
                                       pqe_float64 &x,
                                       pqe_float64 &y,
                                       pqe_float64 &z)const
{
    pqe_float64 sin_latitude = pqe::sind(lat);
    pqe_float64 cos_latitude = pqe::cosd(lat);
    pqe_float64 N = theA /
       std::sqrt( 1.0 - theEccentricitySquared*sin_latitude*sin_latitude);
    x = (N+height) * cos_latitude * pqe::cosd(lon);
    y = (N+height) * cos_latitude * pqe::sind(lon);
    z = (N*(1-theEccentricitySquared)+height)*sin_latitude;
}

void pqe::Ellipsoid::XYZToLatLonHeight(pqe_float64 x,
                                       pqe_float64 y,
                                       pqe_float64 z,
                                       pqe_float64& lat,
                                       pqe_float64& lon,
                                       pqe_float64& height)const
{
   pqe_float64 d = std::sqrt(x*x + y*y);

   pqe_float64 phi2 = z / ((1 - theEccentricitySquared) * d);
   pqe_float64 p = 1.0;
   pqe_float64 phi1 = 0.0;
   pqe_float64 N1 = 0.0;
   pqe_float64 height1 = 0.0;
   int iterIdx = 0;
   const int MAX_ITER = 10;
   if (std::fabs(phi2) > 1e-16 )
   {
      while ( (p > 1e-17) && (iterIdx < MAX_ITER))
      {
         phi1 = phi2;
         N1 = theA / std::sqrt(1.0 - (theEccentricitySquared * std::pow(std::sin(phi1), 2.0)));
         height1 = (d / std::cos(phi1) - N1);
         phi2 = atan((z / d) * (1.0 + (theEccentricitySquared * N1 * std::sin(phi1)) / z));
         p = std::fabs(phi2 - phi1);
         ++iterIdx;
         /* printf("phi: %e   phi2: %e   p: %e  \n", phi1, phi2, p); */
      }                                                                                           
   }                                                                                                   
   else
   {                                                                                              
      phi1 = phi2;                                                                                
      N1 = theA / std::sqrt(1.0 - (theEccentricitySquared * std::pow(std::sin(phi1), 2.0)));                    
      height1 = (d / std::cos(phi1)) - N1;                                                             
   }                                                                                                   

   /* *Latitude = phi2 * 180/PI; */
   /* *Longitude = atan2(Y, X) * 180/PI; */
   lat = phi2*DEG_PER_RAD; 
   lon = atan2(y, x)*DEG_PER_RAD;                                                                
   height = height1; 

#if 0
    pqe_float64 p = std::sqrt(x*x + y*y);
    pqe_float64 theta = atan(z*theA/ (p*theB));
    pqe_float64 eDashSquared = (theA*theA - theB*theB)/
                          (theB*theB);

    pqe_float64 sin_theta = std::sin(theta);
    pqe_float64 cos_theta = std::cos(theta);

    lat = atan( (z + eDashSquared*theB*sin_theta*sin_theta*sin_theta) /
                     (p - theEccentricitySquared*theA*cos_theta*cos_theta*cos_theta) );
    lon = atan2(y,x);

    pqe_float64 sin_latitude = std::sin(lat);
    pqe_float64 N = theA / std::sqrt( 1.0 - theEccentricitySquared*sin_latitude*sin_latitude);

    height = p/std::cos(lat) - N;
    lat*=DEG_PER_RAD;
    lon*=DEG_PER_RAD;
#endif
}

void pqe::Ellipsoid::computeLocalToWorldTransformFromXYZ(
   pqe_float64 x,
   pqe_float64 y,
   pqe_float64 z,
   pqe::Matrix4x4& localToWorld) const
{
   localToWorld = pqe::Matrix4x4::createIdentity();
   pqe::Matrix& m = localToWorld.getData();

   // put in the translation
   m[0][3] = x;
   m[1][3] = y;
   m[2][3] = z;

    // normalize X,Y,Z
    pqe_float64 inverse_length = 1.0/std::sqrt(x*x + y*y + z*z);
    
    x *= inverse_length;
    y *= inverse_length;
    z *= inverse_length;

    pqe_float64 length_XY = std::sqrt(x*x + y*y);
    pqe_float64 inverse_length_XY = 1.0/length_XY;

    // Vx = |(-Y,X,0)|
    m[0][0] = -y*inverse_length_XY;
    m[1][0] = x*inverse_length_XY;
    m[2][0] = 0.0;

    // Vy = /(-Z*X/(std::sqrt(X*X+Y*Y), -Z*Y/(std::sqrt(X*X+Y*Y),std::sqrt(X*X+Y*Y))| 
    pqe_float64 Vy_x = -z*x*inverse_length_XY;
    pqe_float64 Vy_y = -z*y*inverse_length_XY;
    pqe_float64 Vy_z = length_XY;
    inverse_length = 1.0/std::sqrt(Vy_x*Vy_x + Vy_y*Vy_y + Vy_z*Vy_z);            
    m[0][1] = Vy_x*inverse_length;
    m[1][1] = Vy_y*inverse_length;
    m[2][1] = Vy_z*inverse_length;

    // Vz = (X,Y,Z)
    m[0][2] = x;
    m[1][2] = y;
    m[2][2] = z;
   
}

bool pqe::Ellipsoid::operator==(const pqe::Ellipsoid& rhs)const
{
   return ( (theName == rhs.theName)&&
            (theCode == rhs.theCode)&&
            (theB    == rhs.theB)&&
            (theFlattening == rhs.theFlattening));
}

void pqe::Ellipsoid::computeFlattening()
{
   theFlattening = (theA - theB)/theA;
}
