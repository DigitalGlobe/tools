//*******************************************************************
//
// License:  See top level LICENSE.txt.
//
// Author: Garrett Potts
//
// Description:
//
// Contains class declaration for gpt.
//
// CONSTRUCTOR TAKES LAT/LON IN DEGREES!!!
//
// Height is relative to the ellipsoid in meters.
//
//*******************************************************************
// $Id$

#ifndef pqe_Gpt_HEADER
#define pqe_Gpt_HEADER 1

#include <iosfwd>
#include <string>

#include <pqe/Constants.h>
#include <pqe/Dpt.h>
#include <pqe/DatumFactory.h>

namespace pqe
{
   class Datum;
   class EcefPoint;

   /**
    * Class for geographic point with latitude(lat), longitude(lon),
    * height(hgt) and a pointer to a datum.  Latitude, logitude are stored
    * in decimal degrees.  Height is in meters relative to the ellipsoid.
    */
   class PQEDLL Gpt
   {
   public:
      
      /** Constructor.  The values are assumed to be in DEGREES. */
      Gpt(const pqe_float64 alat=0.0,  // degrees
          const pqe_float64 alon=0.0,  // degrees
          const pqe_float64 ahgt=0.0,  // meters 
          const pqe::Datum* aDatum=pqe::DatumFactory::instance()->wgs84())
         :
         lat(alat),
         lon(alon),
         hgt(ahgt),  // relative to the ellipsoid
         theDatum(aDatum) {}//limitLonTo180();}
         
      /** Copy Constructor: */
      Gpt(const pqe::Gpt& src);
         
      /** Constructor.  Conversion from geocentric to ground. */
      Gpt(const pqe::EcefPoint &aPt,
          const pqe::Datum* aDatum=pqe::DatumFactory::instance()->wgs84());
   
      /**
       * latr().  Returns the latitude in radian measure.  Since
       * our internal format is radians we do not have to do any
       * conversions.
       */
      pqe_float64 latr()const{return lat*RAD_PER_DEG;}
      
      /**
       * Returns the latitude in radian measure.  Since
       * our internal format is radians we do not have to do any
       * conversions.
       */
      void latr(pqe_float64 radianValue){lat = radianValue*DEG_PER_RAD;}
      
      /**
       * Returns the longitude in radian measure.  Since
       * our internal format is radians we do not have to do any
       * conversions.
       */
      pqe_float64 lonr()const{return lon*RAD_PER_DEG;}

      /**
       * Assumes the value being passed in is in radians.
       */
      void lonr(pqe_float64 radianValue)
      {lon = radianValue*DEG_PER_RAD; }//limitLonTo180();}
      
      /**
       * Will convert the radian measure to degrees.
       */
      pqe_float64 latd()const{return lat;}
      
      /**
       * Assumes the passed in value is in degrees.
       */
      void latd(pqe_float64 degreeValue){lat = degreeValue;}

      /**
       * Will convert the radian measure to degrees.
       */
      pqe_float64 lond()const{return lon;}

      /**
       *  Assumes the passed in value is in degrees.
       */
      void lond(pqe_float64 degreeValue){lon = degreeValue; }//limitLonTo180();}

      /**
       * @return Returns the height in meters above the ellipsoid.
       */
      pqe_float64 height()const{return hgt;}
      
      /**
       * Sets the "hgt" data member to height.
       *
       * @param height Height above the ellipsoid in meters.
       */
      void height(pqe_float64 height){hgt = height;}
      
      void makeNan(){lat=pqe::nan(); lon=pqe::nan(); hgt=pqe::nan();}

      bool isNan()const
      {
         return (pqe::isnan(lat)&&pqe::isnan(lon)&&pqe::isnan(hgt));
      }

      bool hasNans()const
      {
         return (pqe::isnan(lat)||pqe::isnan(lon)||pqe::isnan(hgt));
      }

      bool isLatNan()const{return pqe::isnan(lat);}
      bool isLonNan()const{return pqe::isnan(lon);}
      bool isHgtNan()const{return pqe::isnan(hgt);}
      
      std::ostream& print(std::ostream& os, pqe_uint32 precision=15) const;
      
      friend PQEDLL std::ostream& operator<<(std::ostream& os,
                                             const pqe::Gpt& pt);
      
      /**
       * @param precision Output floating point precision.
       * 
       * @return std::string representing point.
       *
       * Output format:
       * ( 30.00000000000000, -90.00000000000000, 0.00000000000000, WGE )
       *   -----latitude----  -----longitude----  ------height----  datum
       */
      std::string toString(pqe_uint32 precision=15) const;
      
      /**
       * Initializes this point from string.  This method opens an istream to
       * s and then calls operator>>.
       *
       * Expected format:
       * ( 30.00000000000000, -90.00000000000000, 0.00000000000000, WGE )
       *  -----latitude----  -----longitude----  ------height----  datum
       *
       * @param s String to initialize from.
       *
       * @see operator>>
       */
      void toPoint(const std::string& s);
      
      /**
       * Method to input the formatted string of the "operator<<".
       * 
       * This method starts by doing a "makeNan" on aPt.  So if anything goes
       * wrong with the stream or parsing aPt could be all or partially nan.
       *
       * @param is Input stream istream to formatted text.
       * @param pt osimGpt to be initialized from stream.
       * @return istream pass in.
       *
       * Expected format:
       * ( 30.00000000000000, -90.00000000000000, 0.00000000000000, WGE )
       *  -----latitude----  -----longitude----  ------height----  datum
       */
      friend PQEDLL std::istream& operator>>(std::istream& is,
                                             pqe::Gpt& pt);
      
      /**
       * datum().  returns the datum associated with this ground.
       *
       */
      const pqe::Datum* datum()const{return theDatum;}
      
      /**
       * Note: this will not do a shift.  This just allows you to set the
       * datum.  If you want an automatic shift to occur then you must call the
       * changeDatum method
       */
      void datum(const pqe::Datum* aDatum){theDatum = aDatum?aDatum:theDatum;}
      
      /**
       * This will actually perform a shift.
       */
      void changeDatum(const pqe::Datum *datum);
      
      const pqe::Gpt& operator = (const pqe::Gpt &aPt);
      bool operator ==(const pqe::Gpt& gpt)const
      {
         return ( (lat == gpt.lat)&&
                  (lon == gpt.lon)&&
                  (hgt == gpt.hgt)&&
                  (theDatum == gpt.theDatum));
      }
      
      /**
       * METHOD: limitLonTo180()
       * Converts the lon data member to a value between -180 and +180:
       */
      void  limitLonTo180()
      { if (lon < -180.0) lon += 360.0; else if (lon > 180.0) lon -= 360.0; }
      
      void clampLon(pqe_float64 low, pqe_float64 high)
      {
         if(lon < low) lon = low;
         if(lon > high) lon = high;
      }

      void clampLat(pqe_float64 low, pqe_float64 high)
      {
         if(lat < low) lat = low;
         if(lat > high) lat = high;
      }
      
      void clampHgt(pqe_float64 low, pqe_float64 high)
      {
         if(hgt < low) hgt = low;
         if(hgt > high) hgt = high;
      }
      
      /**
       * METHOD: distanceTo(Gpt)
       * Computes straight-line distance in meters between this and arg gpt:
       */
      pqe_float64 distanceTo(const pqe::Gpt& arg_gpt) const;
      
      pqe::Dpt metersPerDegree() const;
      
      pqe_float64 lat; //> latitude in degrees measure
      pqe_float64 lon; //> longitude in degrees measure
      
      /**
       * Height in meters above the ellipsiod.
       *
       * @note This is NOT the same as "height msl".  "Height msl" is above
       * the geoid or better know as mean sea level.
       */ 
      pqe_float64 hgt;
      
   private:
      
      /**
       * Know reference location plus an implied ellipsoid.
       */
      const pqe::Datum* theDatum;
      
   }; // End of class Gpt.

} // End of namespace pqe.

#endif
