//*******************************************************************
// Copyright (C) 2004 Garrett Potts
//
// LICENSE: MIT see top level LICENSE.txt for more details
// 
// Author: Garrett Potts
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfCompressionHeader.cpp 23666 2015-12-14 20:01:22Z rashadkm $
#include <ossim/support_data/ossimNitfCompressionHeader.h>
#include <ossim/base/ossimKeywordlist.h>
#include <sstream>

RTTI_DEF1(ossimNitfCompressionHeader, "ossimNitfCompressionHeader", ossimObject);

bool ossimNitfCompressionHeader::saveState(ossimKeywordlist& kwl, const ossimString& prefix)const
{
   return ossimObject::saveState(kwl, prefix);
}
