//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
//*******************************************************************
//  $Id$
#include <pqe/Datum.h>
#include <pqe/Common.h>
#include <pqe/Gpt.h>
#include <pqe/Ellipsoid.h>

void pqe::Datum::molodenskyShift(pqe_float64 a,
                                 pqe_float64 da,
                                 pqe_float64 f,
                                 pqe_float64 df,
                                 pqe_float64 dx,
                                 pqe_float64 dy,
                                 pqe_float64 dz,
                                 pqe_float64 Lat_in,
                                 pqe_float64 Lon_in,
                                 pqe_float64 Hgt_in,
                                 pqe_float64 &Lat_out,
                                 pqe_float64 &Lon_out,
                                 pqe_float64 &Hgt_out
                                 )const
{
  /* Begin Molodensky_Shift */
  /*
   *  This function shifts geodetic coordinates using the Molodensky method.
   *
   *    a         : Semi-major axis of source ellipsoid in meters  (input)
   *    da        : Destination a minus source a                   (input)
   *    f         : Flattening of source ellipsoid                 (input)
   *    df        : Destination f minus source f                   (input)
   *    dx        : X coordinate shift in meters                   (input)
   *    dy        : Y coordinate shift in meters                   (input)
   *    dz        : Z coordinate shift in meters                   (input)
   *    Lat_in    : Latitude in radians.                           (input)
   *    Lon_in    : Longitude in radians.                          (input)
   *    Hgt_in    : Height in meters.                              (input)
   *    Lat_out   : Calculated latitude in radians.                (output)
   *    Lon_out   : Calculated longitude in radians.               (output)
   *    Hgt_out   : Calculated height in meters.                   (output)
   */
  pqe_float64 tLon_in;   // temp longitude
  pqe_float64 e2;        // Intermediate calculations for dp, dl
  pqe_float64 ep2;       // Intermediate calculations for dp, dl
  pqe_float64 sin_Lat;   // sin(Latitude_1)
  pqe_float64 sin2_Lat;  // (sin(Latitude_1))^2
  pqe_float64 sin_Lon;   // sin(Longitude_1)
  pqe_float64 cos_Lat;   // cos(Latitude_1)
  pqe_float64 cos_Lon;   // cos(Longitude_1)
  pqe_float64 w2;        // Intermediate calculations for dp, dl
  pqe_float64 w;         // Intermediate calculations for dp, dl
  pqe_float64 w3;        // Intermediate calculations for dp, dl
  pqe_float64 m;         // Intermediate calculations for dp, dl
  pqe_float64 n;         // Intermediate calculations for dp, dl
  pqe_float64 dp;        // Delta phi
  pqe_float64 dp1;       // Delta phi calculations
  pqe_float64 dp2;       // Delta phi calculations
  pqe_float64 dp3;       // Delta phi calculations
  pqe_float64 dl;        // Delta lambda
  pqe_float64 dh;        // Delta height
  pqe_float64 dh1;       // Delta height calculations
  pqe_float64 dh2;       // Delta height calculations

  if(pqe::isnan(Hgt_in))
  {
     Hgt_in = 0.0;
  }
  
  if (Lon_in > M_PI)
    tLon_in = Lon_in - (2*M_PI);
  else
     tLon_in = Lon_in;
  
  e2 = 2 * f - f * f;
  ep2 = e2 / (1 - e2);
  sin_Lat = sin(Lat_in);
  cos_Lat = cos(Lat_in);
  sin_Lon = sin(tLon_in);
  cos_Lon = cos(tLon_in);
  sin2_Lat = sin_Lat * sin_Lat;
  w2 = 1.0 - e2 * sin2_Lat;
  w = sqrt(w2);
  w3 = w * w2;
  m = (a * (1.0 - e2)) / w3;
  n = a / w;
  dp1 = cos_Lat * dz - sin_Lat * cos_Lon * dx - sin_Lat * sin_Lon * dy;
  dp2 = ((e2 * sin_Lat * cos_Lat) / w) * da;
  dp3 = sin_Lat * cos_Lat * (2.0 * n + ep2 * m * sin2_Lat) * (1.0 - f) * df;
  dp = (dp1 + dp2 + dp3) / (m + Hgt_in);
  dl = (-sin_Lon * dx + cos_Lon * dy) / ((n + Hgt_in) * cos_Lat);
  dh1 = (cos_Lat * cos_Lon * dx) + (cos_Lat * sin_Lon * dy) + (sin_Lat * dz);
  dh2 = -(w * da) + ((a * (1 - f)) / w) * sin2_Lat * df;
  dh = dh1 + dh2;

  Lat_out = Lat_in + dp;
  Lon_out = Lon_in + dl;
  Hgt_out = Hgt_in + dh;

  if (Lon_out > (M_PI * 2))
     Lon_out -= 2*M_PI;
  if (Lon_out < (- M_PI))
     Lon_out += 2*M_PI;
}

