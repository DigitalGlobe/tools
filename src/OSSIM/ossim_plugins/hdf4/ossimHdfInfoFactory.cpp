//----------------------------------------------------------------------------
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Mingjie Su
//
// Description: Factory for info objects.
// 
//----------------------------------------------------------------------------
// $Id: ossimHdfInfoFactory.cpp 2645 2011-05-26 15:21:34Z oscar.kramer $

#include "ossimHdfInfoFactory.h"
#include <ossim/support_data/ossimInfoFactory.h>
#include <ossim/support_data/ossimInfoBase.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimRefPtr.h>
#include "ossimHdfInfo.h"

ossimHdfInfoFactory::~ossimHdfInfoFactory()
{}

ossimHdfInfoFactory* ossimHdfInfoFactory::instance()
{
   static ossimHdfInfoFactory sharedInstance;

   return &sharedInstance;
}

ossimInfoBase* ossimHdfInfoFactory::create(const ossimFilename& file) const
{
   ossimRefPtr<ossimInfoBase> result = 0;
   result = new ossimHdfInfo();
   if ( result->open(file) )
   {
      return result.release();
   }
   return 0;
}

ossimHdfInfoFactory::ossimHdfInfoFactory()
{}

ossimHdfInfoFactory::ossimHdfInfoFactory(const ossimHdfInfoFactory& /* obj */ )
{}

const ossimHdfInfoFactory& ossimHdfInfoFactory::operator=(
   const ossimHdfInfoFactory& /* rhs */)
{
   return *this;
}


