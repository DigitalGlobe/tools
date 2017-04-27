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
#include "ossimH5Info.h"

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

#if 1
   // cout << "Calling ossimH5Info ***********************" << endl;
   result = new ossimH5Info();
   if ( result->open(file) )
   {
      return result.release();
   }
#endif

   // cout << "Calling ossimHdfInfo ***********************" << endl;

   // Note old info.  Left in as it handles hdf4.
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


