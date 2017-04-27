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

#include <ossimjni/ElevMgr.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/elevation/ossimElevManager.h>

ossimjni::ElevMgr* ossimjni::ElevMgr::m_instance = 0;

ossimjni::ElevMgr::~ElevMgr()
{
}

ossimjni::ElevMgr* ossimjni::ElevMgr::instance()
{
   if (!m_instance)
   {
      m_instance = new ossimjni::ElevMgr();
   }
   return m_instance;
}

double ossimjni::ElevMgr::getHeightAboveEllipsoid(double latitude, double longitude)
{
   return ossimElevManager::instance()->getHeightAboveEllipsoid( ossimGpt( latitude,
                                                                           longitude,
                                                                           0.0 ) );
}

double ossimjni::ElevMgr::getHeightAboveMSL(double latitude, double longitude)
{
   return ossimElevManager::instance()->getHeightAboveMSL( ossimGpt( latitude,
                                                                     longitude,
                                                                     0.0 ) ) ;
}

void ossimjni::ElevMgr::getCellsForBounds( const std::string& connectionString,
                                           const double& minLat,
                                           const double& minLon,
                                           const double& maxLat,
                                           const double& maxLon,
                                           std::vector<std::string>& cells )
{
   ossimElevManager::instance()->getCellsForBounds( connectionString,
                                                    minLat,
                                                    minLon,
                                                    maxLat,
                                                    maxLon,
                                                    cells );
}

void ossimjni::ElevMgr::getCellsForBounds( const double& minLat,
                                           const double& minLon,
                                           const double& maxLat,
                                           const double& maxLon,
                                           std::vector<std::string>& cells )
{
   ossimElevManager::instance()->getCellsForBounds( minLat,
                                                    minLon,
                                                    maxLat,
                                                    maxLon,
                                                    cells );
}

ossimjni::ElevMgr::ElevMgr()
{
}

ossimjni::ElevMgr::ElevMgr( const ossimjni::ElevMgr& /* obj */ )
{
}

const ossimjni::ElevMgr& ossimjni::ElevMgr::operator=( const ossimjni::ElevMgr& /* obj */ )
{
   return *this;
}



