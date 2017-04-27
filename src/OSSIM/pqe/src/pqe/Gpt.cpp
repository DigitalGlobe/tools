//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
// Description:
//
// LATITUDE AND LONGITUDE VALUES ARE IN DEGREES.
//
//*******************************************************************
// $Id$

#include <iostream>
#include <iomanip>
#include <sstream>

#include <pqe/Gpt.h>
#include <pqe/Common.h>
#include <pqe/EcefPoint.h>
#include <pqe/EcefVector.h>
#include <pqe/Datum.h>
#include <pqe/Ellipsoid.h>

std::ostream& pqe::Gpt::print(std::ostream& os, pqe_uint32 precision) const
{
   // Capture the original flags.
   std::ios_base::fmtflags f = os.flags();

   os << std::setiosflags(std::ios_base::fixed)
      << std::setprecision(precision);
   os << "( ";

   if(isLatNan())
   {
      os << "nan" << ", ";
   }
   else
   {
      os << latd() << ", ";
   }
   if(isLonNan())
   {
      os << "nan" << ", ";
   }
   else
   {
      os << lond() << ", ";
   }
   if(isHgtNan())
   {
      os << "nan" << ", ";
   }
   else
   {
      // millimeter precision for height
      os << std::setprecision(3) << height() << ", ";
   }
   
   os << (theDatum?theDatum->code().c_str():"") << " )";

   // Reset flags.
   os.setf(f);

   return os;
}

std::ostream& pqe::operator<<(std::ostream& os, const pqe::Gpt& pt)
{
   return pt.print(os);
}

std::string pqe::Gpt::toString(pqe_uint32 precision) const
{
   std::ostringstream os;
   print(os, precision);
   return os.str();
}


void pqe::Gpt::toPoint(const std::string& s)
{
   std::istringstream is(s);
   is >> *this;
}

std::istream& pqe::operator>>(std::istream& is, pqe::Gpt &pt)
{
   //---
   // Expected input format:
   // ( 30.00000000000000, -90.00000000000000, 0.00000000000000, WGE )
   //   -----latitude----  -----longitude----  ------height----  datum
   //---

   // Start with a nan point.
   pt.makeNan();

   // Check the stream.
   if (!is) return is;
   
   const int SZ = 64; // Handle real big number...
   std::string os;
   char buf[SZ];
   char c = 0;

   //---
   // LATITUDE SECTION:
   //---
   
   // Grab data up to the first comma.
   is.get(buf, SZ, ',');

   if (!is) return is;

   // Copy to ossim string.
   os = buf;
   
   // Get rid of the '(' if there is any.
   std::string::size_type pos = os.find('(');
   if (pos != std::string::npos)
   {
      os.erase(pos, 1);
   }   

   // Check for nan lat value.
   if ( !os.empty() && (os.find("nan") == std::string::npos) )
   {
      std::istringstream is(os);
      is >> pt.lat;
   }
   else
   {
      pt.latd(pqe::nan());
   }

   // Eat the comma that we stopped at.
   while (c != ',')
   {
      is.get(c);
      if (!is) break;
   }
   
   //---
   // LONGITUDE SECTION:
   //---
   
   // Grab the data up to the next ','
   is.get(buf, SZ, ',');

   if (!is) return is;

   // Copy to ossim string.
   os = buf;

   // Check for nan lon value.
   if ( !os.empty() && (os.find("nan") == std::string::npos) )
   {
      std::istringstream is(os);
      is >> pt.lon;
   }
   else
   {
      pt.lond(pqe::nan());
   }

   // Eat the comma that we stopped at.
   c = 0;
   while (c != ',')
   {
      is.get(c);
      if (!is) break;
   }

   //---
   // HEIGHT SECTION:
   //---
   
   // Grab the data up to the ','
   is.get(buf, SZ, ',');

   if (!is) return is;

   // Copy to ossim string.
   os = buf;

   // Check for nan hgt value.
   if ( !os.empty() && (os.find("nan") == std::string::npos) )
   {
      std::istringstream is(os);
      is >> pt.hgt;
   }
   else
   {
      pt.height(pqe::nan());
   }

   // Eat the comma that we stopped at.
   c = 0;
   while (c != ',')
   {
      is.get(c);
      if (!is) break;
   }

   //---
   // DATUM SECTION:
   //---
   
   // Grab the data up to the ')'
   is.get(buf, SZ, ')');

   if (!is) return is;

   // Copy to ossim string.
   os = buf;
   // os.trim(); // Just in case datum factory doesn't handle spaces.
   
   const pqe::Datum* datum = pqe::DatumFactory::instance()->create(os);
   if (datum)
   {
      pt.datum(datum);
   }
   else
   {
      pt.datum(pqe::DatumFactory::instance()->wgs84());
   }

   // Gobble the trailing ")".
   c = 0;
   while (c != ')')
   {
      is.get(c);
      if (!is) break;
   }

   // Finished
   return is;
}

//*****************************************************************************
//  COPY CONSTRUCTOR: pqe::Gpt(pqe::Gpt) 
//*****************************************************************************
pqe::Gpt::Gpt(const pqe::Gpt& src)
   : lat(src.lat),
     lon(src.lon),
     hgt(src.hgt)
{
   theDatum = src.datum();
   if(!theDatum)
   {
      theDatum = pqe::DatumFactory::instance()->wgs84();
   }
//   limitLonTo180();
}

//*****************************************************************************
//  CONSTRUCTOR:  pqe::Gpt(pqe::EcefPoint, datum)
//*****************************************************************************
pqe::Gpt::Gpt(const pqe::EcefPoint& ecef_point,
              const pqe::Datum*     datum)
   :
      theDatum (datum)
{
   if(!theDatum)
   {
      theDatum = pqe::DatumFactory::instance()->wgs84();
   }
   if(ecef_point.isNan())
   {
      makeNan();
   }
   else
   {
      theDatum->ellipsoid()->XYZToLatLonHeight(ecef_point.x(),
                                               ecef_point.y(),
                                               ecef_point.z(),
                                               lat,
                                               lon,
                                               hgt);
      
   }
}

//*****************************************************************************
//  METHOD: pqe::Gpt::operator =
//*****************************************************************************
const pqe::Gpt& pqe::Gpt::operator = (const pqe::Gpt &aPt)
{
   lat = aPt.lat;
   lon = aPt.lon;
   hgt = aPt.hgt;
   
   if(aPt.datum())
   {
      theDatum = aPt.datum();
   }
   if(!theDatum)
   {
      theDatum = pqe::DatumFactory::instance()->wgs84();
   }
   
   return *this;
}

//*****************************************************************************
//  METHOD: pqe::Gpt::changeDatum
//*****************************************************************************
void pqe::Gpt::changeDatum(const pqe::Datum *datum)
{
   if(datum == theDatum) return;

   // only shift if all values lat and lon is good
   if(!isLatNan() && !isLonNan()) 
   {
      if(datum)
      {
	double h = hgt;
        *this = datum->shift(*this);
        if(pqe::isnan(h))
        {
           hgt = h;
        }
        theDatum = datum;
      }
   }
}

//*****************************************************************************
//  METHOD: pqe::Gpt::distanceTo(pqe::Gpt)
//
//  Computes straight-line distance to arg point, in meters.
//
//*****************************************************************************
double pqe::Gpt::distanceTo(const pqe::Gpt& arg_pt) const
{
   pqe::EcefPoint p1 (*this);
   pqe::EcefPoint p2 (arg_pt);

   return (p1 - p2).magnitude();
}

//*****************************************************************************
//  METHOD: pqe::Gpt::metersPerDegree(double)
//
//  Computes geodetic arc lengths at this ground point.
//
//*****************************************************************************
pqe::Dpt pqe::Gpt::metersPerDegree() const
{
   pqe::Dpt result;

   double radius = theDatum->ellipsoid()->geodeticRadius(lat);
   result.y =  RAD_PER_DEG * radius;
   result.x =  result.y * pqe::cosd(lat);

   return result;
}
