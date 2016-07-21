//-----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Ellipsoid factory class.
//
//-----------------------------------------------------------------------------
// $Id$

#include <iostream>

#include <pqe/EllipsoidFactory.h>
#include <pqe/Ellipsoid.h>

pqe::EllipsoidFactory* pqe::EllipsoidFactory::theInstance = 0;

// Default ellipsoid constructor is WGS 84.
const pqe::Ellipsoid* pqe::EllipsoidFactory::theWgs84Ellipsoid = 0;

// new pqe::Ellipsoid();


pqe::EllipsoidFactory::~EllipsoidFactory()
{
}

pqe::EllipsoidFactory* pqe::EllipsoidFactory::instance()
{
   if (!theInstance)
   {
      theInstance = new pqe::EllipsoidFactory();
   }
   return theInstance;
}

const pqe::Ellipsoid* pqe::EllipsoidFactory::wgs84() const
{
   if ( !theWgs84Ellipsoid )
   {
      // Default Ellipsoid constructor is wqs 84.
      theWgs84Ellipsoid = new Ellipsoid();
   }
   return theWgs84Ellipsoid;
}

pqe::EllipsoidFactory::EllipsoidFactory()
{
}

pqe::EllipsoidFactory::EllipsoidFactory(const pqe::EllipsoidFactory& /* obj */)
{
}

const pqe::EllipsoidFactory& pqe::EllipsoidFactory::operator=(const pqe::EllipsoidFactory& /* obj */)
{
   return *this;
}
