//-----------------------------------------------------------------------------
// File:  Info.h
//
// License:  See top level LICENSE.txt file.
//
//
// Description: Wrapper class for ossimInfo with swig readable interfaces.
//
//-----------------------------------------------------------------------------
// $Id: Info.h 22320 2013-07-19 15:21:03Z dburken $

#ifndef omsEnvelope_HEADER
#define omsEnvelope_HEADER 1

#include <oms/Constants.h>
#include <string>
namespace oms
{
   class OMSDLL Envelope
   {
   public:
      Envelope(double minx=0.0, double miny=0.0,
             double maxx=0.0, double maxy=0.0, 
             const std::string& epsgCode = "EPSG:4326")
      :m_minx(minx),
      m_miny(miny),
      m_maxx(maxx),
      m_maxy(maxy),
      m_epsgCode(epsgCode)
      {

      }

      const std::string& getEpsgCode()const{return m_epsgCode;}

      double getMinX()const{return m_minx;}
      double getMinY()const{return m_miny;}
      double getMaxX()const{return m_maxx;}
      double getMaxY()const{return m_maxy;}

      void setMinX(const double& maxx){m_minx = maxx;}
      void setMinY(const double& miny){m_miny = miny;}
      void setMaxX(const double& maxx){m_maxx = maxx;}
      void setMaxY(const double& maxy){m_maxy = maxy;}

      std::string toString()const;
      
   protected:
      double m_minx;
      double m_miny;
      double m_maxx;
      double m_maxy;

      std::string m_epsgCode;
   };
}
#endif
