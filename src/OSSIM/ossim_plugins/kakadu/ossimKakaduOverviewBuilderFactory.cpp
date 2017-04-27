//----------------------------------------------------------------------------
// 
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description: The ossim kakadu overview builder factory.
//
//----------------------------------------------------------------------------
// $Id: ossimKakaduOverviewBuilderFactory.cpp 23664 2015-12-14 14:17:27Z dburken $

#include "ossimKakaduOverviewBuilderFactory.h"
#include "ossimKakaduNitfOverviewBuilder.h"

ossimKakaduOverviewBuilderFactory* ossimKakaduOverviewBuilderFactory::instance()
{
   static ossimKakaduOverviewBuilderFactory inst;
   return &inst;
}

ossimKakaduOverviewBuilderFactory::~ossimKakaduOverviewBuilderFactory()
{
}

ossimOverviewBuilderBase* ossimKakaduOverviewBuilderFactory::createBuilder(
   const ossimString& typeName) const
{
   ossimRefPtr<ossimOverviewBuilderBase> result = new  ossimKakaduNitfOverviewBuilder();
   if ( result->hasOverviewType(typeName) == false )
   {
      result = 0;
   }
   
   return result.release();
}

void ossimKakaduOverviewBuilderFactory::getTypeNameList(
   std::vector<ossimString>& typeList) const
{
   ossimRefPtr<ossimOverviewBuilderBase> builder = new  ossimKakaduNitfOverviewBuilder();
   builder->getTypeNameList(typeList);
   builder = 0;
}

ossimKakaduOverviewBuilderFactory::ossimKakaduOverviewBuilderFactory()
{
}

ossimKakaduOverviewBuilderFactory::ossimKakaduOverviewBuilderFactory(
   const ossimKakaduOverviewBuilderFactory& /* obj */)
{
}

void ossimKakaduOverviewBuilderFactory::operator=(
   const ossimKakaduOverviewBuilderFactory& /* rhs */)
{
}

