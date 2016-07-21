//-----------------------------------------------------------------------------
// File:  ElevMgr.h
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Class declaration for ElevMgr. Wrapper for ossimElevManager
// class.
//
//-----------------------------------------------------------------------------
// $Id$
#ifndef omsElevMgr_HEADER
#define omsElevMgr_HEADER 1

#include <oms/Constants.h>
#include <string>
#include <vector>

namespace oms
{
   class OMSDLL ElevMgr
   {
   public:

      /** @brief destructor */
      ~ElevMgr();

      /**
       * @brief Instance method.
       *
       * @return The instance of this class.
       */
      static oms::ElevMgr* instance();

      /**
       * @brief Get height above ellipsoid.
       *
       * This is geoid offset plus height above MSL.
       * 
       * @param latitude
       * @param longitude
       * @return Height above ellipsoid.
       */
      double getHeightAboveEllipsoid(double latitude, double longitude);

      /**
       * @brief Get height above mean sea level(MSL).
       * @param latitude
       * @param longitude
       * @return Height above MSL..
       */
      double getHeightAboveMSL(double latitude, double longitude);

      /**
       * @brief Gets a list of elevation cells needed to cover bounding box.
       * @param connectionString Typically elevation repository, e.g.:
       * "/data1/elevation/srtm/1arc"
       * @param minLat Minimum latitude of bounding box.
       * @param minLon Minimum longitude of bounding box.
       * @param maxLat Maximum latitude of bounding box.
       * @param maxLon Maximum longitude of bounding box.
       * @param cells Initialized by this.
       * @param maxNumberOfCells Value of 0 indicates return as many as you can.  Any positive
       *        number will only return that number of cells.
       */
      void getCellsForBounds( const std::string& connectionString,
                              const double& minLat,
                              const double& minLon,
                              const double& maxLat,
                              const double& maxLon,
                              std::vector<std::string>& cells,
                              ossim_uint32 maxNumberOfCells=0 );      

      /**
       * @brief Gets a list of elevation cells needed to cover bounding box.
       *
       * This implementation sorts elevation repositories by resolution, best
       * first, then searches cells for bounds. Search is stopped on the first
       * repository that has cells.
       * 
       * @param minLat Minimum latitude of bounding box.
       * @param minLon Minimum longitude of bounding box.
       * @param maxLat Maximum latitude of bounding box.
       * @param maxLon Maximum longitude of bounding box.
       * @param cells Initialized by this.
        * @param maxNumberOfCells Value of 0 indicates return as many as you can.  Any positive
       *        number will only return that number of cells.
      */
      void getCellsForBounds( const double& minLat,
                              const double& minLon,
                              const double& maxLat,
                              const double& maxLon,
                              std::vector<std::string>& cells,
                              ossim_uint32 maxNumberOfCells=0 );      

  private:
      
      /** @brief default constructor - hidden from use */
      ElevMgr();

      /** @brief copy constructor - hidden from use */
      ElevMgr(const oms::ElevMgr& obj);

      /** @brief assignment operator - hidden from use */ 
      const oms::ElevMgr& operator=(const oms::ElevMgr& obj);

      /** @brief The single static instance of this class. */
      static oms::ElevMgr* m_instance;
   };

} // End of namespace oms.

#endif /* #ifndef omsElevMgr_HEADER */
