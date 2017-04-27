//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// DESCRIPTION:
//  This class maintains the relationship between a local space rectangular
//  (LSR) coordinate system and the earth-centered, earth-fixed (ECEF) system.
//
// SOFTWARE HISTORY:
//
//   09Aug2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//
//*****************************************************************************
//  $Id$

#include <ostream>

#include <pqe/LsrSpace.h>
#include <pqe/Common.h>
#include <pqe/Gpt.h>
#include <pqe/EcefVector.h>

//*****************************************************************************
//  CONSTRUCTOR: pqe::LsrSpace(origin, x_dir, y_dir, int)
//
//  Constructs the space given origin, and X and Y ECEF directions. The int
//  argument is a place holder only and not used.
//  
//*****************************************************************************
pqe::LsrSpace::LsrSpace(const pqe::EcefPoint&  origin,
                        const pqe::EcefVector& x_dir_ecf_vec,
                        const pqe::EcefVector& y_dir_ecf_vec,
                        int   z_not_provided_space_holder)
   : theOrigin (origin)
{
   //***
   // Compute the remaining axis given first two:
   //***
   pqe::ColumnVector3d xdir (x_dir_ecf_vec.data().unit());
   pqe::ColumnVector3d ydir (y_dir_ecf_vec.data().unit());
   pqe::ColumnVector3d zdir (xdir.cross(ydir));

   //***
   // Fill the rotation matrix:
   //***
   theLsrToEcefRotMatrix = pqe::Matrix3x3::create(xdir[0], ydir[0], zdir[0],
                                                  xdir[1], ydir[1], zdir[1],
                                                  xdir[2], ydir[2], zdir[2]);
}

//*****************************************************************************
//  CONSTRUCTOR: pqe::LsrSpace
//  
//  Constructs the space given origin, and X and Z ECEF directions. The int
//  argument is a place holder only and not used.
//  
//*****************************************************************************
pqe::LsrSpace::LsrSpace(const pqe::EcefPoint&  origin,
                        const pqe::EcefVector& x_dir_ecf_vec,
                        int   y_not_provided_space_holder,
                        const pqe::EcefVector& z_dir_ecf_vec)
   : theOrigin (origin)
{
   //***
   // Compute the remaining axis given first two:
   //***
   pqe::ColumnVector3d xdir (x_dir_ecf_vec.data().unit());
   pqe::ColumnVector3d zdir (z_dir_ecf_vec.data().unit());
   pqe::ColumnVector3d ydir (zdir.cross(xdir));

   //***
   // Fill the rotation matrix:
   //***
   theLsrToEcefRotMatrix = pqe::Matrix3x3::create(xdir[0], ydir[0], zdir[0],
                                                  xdir[1], ydir[1], zdir[1],
                                                  xdir[2], ydir[2], zdir[2]);
}

//*****************************************************************************
//  CONSTRUCTOR: pqe::LsrSpace
//  
//  Constructs the space given origin, and Y and Z ECEF directions. The int
//  argument is a place holder only and not used.
//  
//*****************************************************************************
pqe::LsrSpace::LsrSpace(const pqe::EcefPoint&  origin,
                        int   x_not_provided_space_holder,
                        const pqe::EcefVector& y_dir_ecf_vec,
                        const pqe::EcefVector& z_dir_ecf_vec)
   : theOrigin (origin)
{
   //***
   // Compute the remaining axis given first two:
   //***
   pqe::ColumnVector3d ydir (y_dir_ecf_vec.data().unit());
   pqe::ColumnVector3d zdir (z_dir_ecf_vec.data().unit());
   pqe::ColumnVector3d xdir (ydir.cross(zdir));

   //***
   // Fill the rotation matrix:
   //***
   theLsrToEcefRotMatrix = pqe::Matrix3x3::create(xdir[0], ydir[0], zdir[0],
                                                  xdir[1], ydir[1], zdir[1],
                                                  xdir[2], ydir[2], zdir[2]);
}

//*****************************************************************************
//  CONSTRUCTORS: pqe::LsrSpace(pqe::Gpt, y_azimuth)
//
//  This constructor sets up a local coordinate system centered at the
//  specified groundpoint, with the Z-axis normal to the ellipsoid and the
//  Y-axis rotated clockwise from north by the y_azimuth. This angle defaults
//  to 0, producing an East-North-Up system.
//  
//*****************************************************************************
pqe::LsrSpace::LsrSpace(const pqe::Gpt& origin,
                        const double&   y_azimuth)
{
   //***
   // Convert ground point origin to ECEF coordinates:
   //***
   theOrigin = pqe::EcefPoint(origin);
   
   //***
   // Establish the component vectors for ENU system::
   //***
   double sin_lat = pqe::sind(origin.lat);
   double cos_lat = pqe::cosd(origin.lat);
   double sin_lon = pqe::sind(origin.lon);
   double cos_lon = pqe::cosd(origin.lon);
   
   pqe::ColumnVector3d E (-sin_lon,
                          cos_lon,
                          0.0);
   pqe::ColumnVector3d N (-sin_lat*cos_lon,
                          -sin_lat*sin_lon,
                          cos_lat);
   pqe::ColumnVector3d U (E.cross(N));

   //
   // Fill rotation matrix with these components, rotated about the Z axis
   // by the azimuth indicated:
   //
   if (std::abs(y_azimuth) > FLT_EPSILON)
   {
      double cos_azim = pqe::cosd(y_azimuth);
      double sin_azim = pqe::sind(y_azimuth);
      pqe::ColumnVector3d X (cos_azim*E - sin_azim*N);
      pqe::ColumnVector3d Y (sin_azim*E + cos_azim*N);
      pqe::ColumnVector3d Z (X.cross(Y));
      
      theLsrToEcefRotMatrix
         = pqe::Matrix3x3::create(X[0], Y[0], Z[0],
                                  X[1], Y[1], Z[1],
                                  X[2], Y[2], Z[2]);
   }
   else
   {
      //***
      // No azimuth rotation, so simplify:
      //***
      theLsrToEcefRotMatrix = pqe::Matrix3x3::create(E[0], N[0], U[0],
                                                     E[1], N[1], U[1],
                                                     E[2], N[2], U[2]);
   }
}

//*****************************************************************************
//  OPERATOR: ==
//*****************************************************************************
bool pqe::LsrSpace::operator == (const pqe::LsrSpace& that) const
{
   if (theOrigin != that.origin())
      return false;
   
   if (theLsrToEcefRotMatrix != that.theLsrToEcefRotMatrix)
      return false;

   return true;
}
   
//*****************************************************************************
//  OPERATOR: =
//*****************************************************************************
const pqe::LsrSpace& pqe::LsrSpace::operator = (const pqe::LsrSpace& space)
{
   theOrigin = space.theOrigin;
   theLsrToEcefRotMatrix = space.theLsrToEcefRotMatrix;
   return *this;
}

//*****************************************************************************
//  STATIC METHOD: lsrSpaceErrorMessage()
//  
//  Convenience method accessible to all owners of an pqe::LsrSpace for
//  displaying an error message when LSR spaces do not match between
//  objects. All operations between LSR objects must be in a common space.
//  
//*****************************************************************************
std::ostream& pqe::LsrSpace::lsrSpaceErrorMessage(std::ostream& os)
{
   os << "pqe::LsrSpace ERROR: An operation was attempted between two LSR \n"
      << "  objects with differing LSR spaces. This is an illegal condition.\n"
      << "  Please check the data and/or report the error to development."
      << std::endl;

   return os;
}

//*****************************************************************************
//  METHOD: print()
//  
//  Dumps contents to stream for debug purposes. Defaults to cout.
//  
//*****************************************************************************
std::ostream& pqe::LsrSpace::print(std::ostream& stream) const
{
   stream << "(LsrSpace)"
          << "\n   theOrigin = " << theOrigin
          << "\n   theLsrToEcefRotMatrix = \n" << theLsrToEcefRotMatrix;
   return stream;
}

std::ostream& pqe::operator<<(std::ostream& os,
                              const pqe::LsrSpace& instance)
{
   return instance.print(os);
}



   
