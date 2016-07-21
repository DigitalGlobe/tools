
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

#ifndef omsTileCacheSupport_HEADER
#define omsTileCacheSupport_HEADER 1

#include <oms/Constants.h>
#include <oms/Envelope.h>
#include <string>
namespace oms
{
   class TileCacheSupportPrivateData;
   class OMSDLL TileCacheSupport
   {
   public:
      friend class TileCacheSupportPrivateData;
      virtual ~TileCacheSupport();
      TileCacheSupport(unsigned int tileWidth=256,
                       unsigned int tileHeight=256,
                       const std::string& epsgCode="EPSG:4326");
      bool openImage(const std::string& file);
      int getNumberOfEntries()const;
      long int getWidth(int entry, int rlevel=0)const;
      long int getHeight(int entry, int rlevel=0)const;
      Envelope getEnvelope(int entry)const;
      double getDegreesPerPixel(int entry, int rlevel=0)const;
      double getMetersPerPixel(int entry, int rlevel=0)const;
      int getNumberOfResolutionLevels(int entry)const;

      bool findBestResolutionsGeographic(int entry, int* minLevel, int* maxLevel, 
                                         const ossim_float64* resLevels, int nResLevels)const;
      int getOutputScalarType(int entry)const;
      int getBitDepth(int entry)const;

   protected:
      TileCacheSupportPrivateData* m_privateData;
   };
}

#endif
