//----------------------------------------------------------------------------
// File:  ElevMgr.h
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Class declaration for ElevMgr.  Handles ossim initialization.
//
//----------------------------------------------------------------------------
// $Id: ElevMgr.cpp 20183 2011-10-20 13:14:35Z dburken $

#include <oms/ElevMgr.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/elevation/ossimElevManager.h>

oms::ElevMgr* oms::ElevMgr::m_instance = 0;

oms::ElevMgr::~ElevMgr()
{
}

oms::ElevMgr* oms::ElevMgr::instance()
{
   if (!m_instance)
   {
      m_instance = new oms::ElevMgr();
   }
   return m_instance;
}

double oms::ElevMgr::getHeightAboveEllipsoid(double latitude, double longitude)
{
   return ossimElevManager::instance()->getHeightAboveEllipsoid( ossimGpt( latitude,
                                                                           longitude,
                                                                           0.0 ) );
}

double oms::ElevMgr::getHeightAboveMSL(double latitude, double longitude)
{
   return ossimElevManager::instance()->getHeightAboveMSL( ossimGpt( latitude,
                                                                     longitude,
                                                                     0.0 ) ) ;
}

void oms::ElevMgr::getCellsForBounds( const std::string& connectionString,
                                      const double& minLat,
                                      const double& minLon,
                                      const double& maxLat,
                                      const double& maxLon,
                                      std::vector<std::string>& cells,
                                      ossim_uint32 maxNumberOfCells )
{
   ossimElevManager::instance()->getCellsForBounds( connectionString,
                                                    minLat,
                                                    minLon,
                                                    maxLat,
                                                    maxLon,
                                                    cells, 
                                                    maxNumberOfCells );
}

void oms::ElevMgr::getCellsForBounds( const double& minLat,
                                      const double& minLon,
                                      const double& maxLat,
                                      const double& maxLon,
                                      std::vector<std::string>& cells,
                                      ossim_uint32 maxNumberOfCells )
{
   ossimElevManager::instance()->getCellsForBounds( minLat,
                                                    minLon,
                                                    maxLat,
                                                    maxLon,
                                                    cells, 
                                                    maxNumberOfCells );
}

oms::ElevMgr::ElevMgr()
{
}

oms::ElevMgr::ElevMgr( const oms::ElevMgr& /* obj */ )
{
}

const oms::ElevMgr& oms::ElevMgr::operator=( const oms::ElevMgr& /* obj */ )
{
   return *this;
}



