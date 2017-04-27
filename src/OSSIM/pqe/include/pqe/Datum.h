//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// Contains class declaration for Datum.  This is a class wrapper
// for Geotrans datum.  For more thorough description of each function
// look at the datum.h file.
//*******************************************************************
// $Id$

#ifndef pqe_Datum_HEADER
#define pqe_Datum_HEADER 1

#include <string>

#include <pqe/Constants.h>
#include <pqe/Gpt.h>
#include <pqe/Ellipsoid.h>

namespace pqe
{
   class PQEDLL Datum
   {
   public:
      Datum(const std::string& code,
            const std::string& name,
            const Ellipsoid* anEllipsoid,
            pqe_float64 sigmaX,
            pqe_float64 sigmaY,
            pqe_float64 sigmaZ,
            pqe_float64 westLongitude,
            pqe_float64 eastLongitude,
            pqe_float64 southLatitude,
            pqe_float64 northLatitude)
         :
         theCode(code),
         theName(name),
         theEllipsoid(anEllipsoid),
         theSigmaX(sigmaX),
         theSigmaY(sigmaY),
         theSigmaZ(sigmaZ),
         theWestLongitude(westLongitude),
         theEastLongitude(eastLongitude),
         theSouthLatitude(southLatitude),
         theNorthLatitude(northLatitude)
         {};

      virtual ~Datum(){};
         
      // Argument holds the source point and datum.  Returns another
      // point with this datum.
      //
      virtual pqe::Gpt shift(const pqe::Gpt& aPt)const=0;
      //utility functions to shift to and from the standard coordinates.
      //Users should use the shift instead!!!
      //
      virtual pqe::Gpt       shiftToWgs84(const pqe::Gpt &aPt)const = 0;
      virtual pqe::Gpt       shiftFromWgs84(const pqe::Gpt &aPt)const = 0;
   

      virtual bool  isTheSameAs(const Datum *aDatum)const
      {return this == aDatum;}
      virtual const std::string& code()const{return theCode;}
      virtual const std::string& name()const{return theName;}
      virtual const pqe::Ellipsoid* ellipsoid()const{return theEllipsoid;}
      virtual pqe_float64 sigmaX()const{return theSigmaX;}
      virtual pqe_float64 sigmaY()const{return theSigmaY;}
      virtual pqe_float64 sigmaZ()const{return theSigmaZ;}
      
      virtual pqe_float64 westLongitude()const{return theWestLongitude;}
      virtual pqe_float64 eastLongitude()const{return theEastLongitude;}
      virtual pqe_float64 southLatitude()const{return theSouthLatitude;}
      virtual pqe_float64 northLatitude()const{return theNorthLatitude;}
      
      virtual pqe_float64 param1()const=0;
      virtual pqe_float64 param2()const=0;
      virtual pqe_float64 param3()const=0;
      virtual pqe_float64 param4()const=0;
      virtual pqe_float64 param5()const=0;
      virtual pqe_float64 param6()const=0;
      virtual pqe_float64 param7()const=0;
      
      virtual bool isInside(pqe_float64 latitude, pqe_float64 longitude)const
      {
         return ((theSouthLatitude <= latitude) &&
                 (latitude <= theNorthLatitude) &&
                 (theWestLongitude <= longitude) &&
                 (longitude <= theEastLongitude));
      }
      
      bool operator ==(const Datum& rhs)const
      {
         return ( (theCode == rhs.theCode)&&
                  (theName == rhs.theName)&&
                  (*theEllipsoid == *rhs.theEllipsoid)&&
                  (theSigmaX == rhs.theSigmaX)&&
                  (theSigmaY == rhs.theSigmaY)&&
                  (theSigmaZ == rhs.theSigmaZ)&&
                  (theWestLongitude == rhs.theWestLongitude)&&
                  (theEastLongitude == rhs.theEastLongitude)&&
                  (theSouthLatitude == rhs.theSouthLatitude)&&
                  (theNorthLatitude == rhs.theNorthLatitude));
      }
   protected:
      
      /*!
       * This is directly from Geotrans:
       * Begin Molodensky_Shift
       * This function shifts geodetic coordinates using the Molodensky method.
       *
       *    a         : Semi-major axis of source ellipsoid in meters  (input)
       *    da        : Destination a minus source a                   (input)
       *    f         : Flattening of source ellipsoid                 (input)
       *    df        : Destination f minus source f                   (input)
       *    dx        : X coordinate shift in meters                   (input)
       *    dy        : Y coordinate shift in meters                   (input)
       *    dz        : Z coordinate shift in meters                   (input)
       */
      virtual void    molodenskyShift( double a,
                                       double da,
                                       double f,
                                       double df,
                                       double dx,
                                       double dy,
                                       double dz,
                                       double Lat_in,
                                       double Lon_in,
                                       double Hgt_in,
                                       double &Lat_out,
                                       double &Lon_out,
                                       double &Hgt_out)const;
      bool withinMolodenskyRange(const pqe::Gpt& pt)const
      {
         double lat = pt.latd();
         
         return ((lat < 89.75) && (lat > -89.75));
      }
      
   private:
      std::string           theCode;
      std::string           theName;
      const pqe::Ellipsoid* theEllipsoid;
      
      pqe_float64           theSigmaX;
      pqe_float64           theSigmaY;
      pqe_float64           theSigmaZ;
      
      pqe_float64           theWestLongitude;
      pqe_float64           theEastLongitude;
      pqe_float64           theSouthLatitude;
      pqe_float64           theNorthLatitude;
      
   }; // class Datum

} // namespace pqe

#endif /* #ifndef pqe_Datum_HEADER */
