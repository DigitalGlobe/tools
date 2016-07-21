//----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Datum factory class.
//
//----------------------------------------------------------------------------
// $Id$

#include <pqe/DatumFactory.h>
#include <pqe/Wgs84Datum.h>

pqe::DatumFactory* pqe::DatumFactory::theInstance = 0;

const pqe::Datum* pqe::DatumFactory::theWgs84Datum = new pqe::Wgs84Datum();

pqe::DatumFactory::~DatumFactory()
{
}

pqe::DatumFactory* pqe::DatumFactory::instance()
{
   if (!theInstance)
   {
      theInstance = new pqe::DatumFactory();
   }
   return theInstance;
}

const pqe::Datum* pqe::DatumFactory::wgs84() const
{
   return theWgs84Datum;
}

const pqe::Datum* pqe::DatumFactory::create(const std::string& code) const
{
   const pqe::Datum* result = 0;
   if (code.find("WGE") != std::string::npos)
   {
      result = theWgs84Datum;
   }
   return result;
}

pqe::DatumFactory::DatumFactory()
{
}

pqe::DatumFactory::DatumFactory(const pqe::DatumFactory& /* obj */)
{
}

const pqe::DatumFactory& pqe::DatumFactory::operator=(const pqe::DatumFactory& /* obj */)
{
   return *this;
}
