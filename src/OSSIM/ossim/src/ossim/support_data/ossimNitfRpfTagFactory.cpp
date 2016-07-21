//*******************************************************************
//
// LICENSE: MIT  see top level LICENSE.txt
// 
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfRpfTagFactory.cpp 23666 2015-12-14 20:01:22Z rashadkm $
#include <ossim/support_data/ossimNitfRpfTagFactory.h>
#include <ossim/support_data/ossimRpfHeader.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimTrace.h>

RTTI_DEF1(ossimNitfRpfTagFactory, "ossimNitfRpfTagFactory", ossimNitfTagFactory);

static const ossimString REGISTERED_RPF_HEADER_TAG = "RPFHDR";
static const ossimString REGISTERED_RPF_DES_TAG    = "RPFDES";

static ossimTrace traceDebug = ossimTrace("ossimNitfRpfTagFactory:debug");

ossimNitfRpfTagFactory::ossimNitfRpfTagFactory()
{
}

ossimNitfRpfTagFactory::~ossimNitfRpfTagFactory()
{
}

ossimNitfRpfTagFactory* ossimNitfRpfTagFactory::instance()
{
   static ossimNitfRpfTagFactory inst;
   return &inst;
}

ossimRefPtr<ossimNitfRegisteredTag> ossimNitfRpfTagFactory::create(const ossimString &tagName)const
{
   ossimString temp(ossimString(tagName).trim().upcase());
   
   if(temp == REGISTERED_RPF_HEADER_TAG)
   {
      return new ossimRpfHeader;
   }
   if(temp == REGISTERED_RPF_DES_TAG)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN) << "WARNING  ossimNitfRpfTagFactory::create: Reader for REGISTERED_RPF_DES_TAG not implemented yet" << std::endl;
      }
   }
   
   return 0;
}
