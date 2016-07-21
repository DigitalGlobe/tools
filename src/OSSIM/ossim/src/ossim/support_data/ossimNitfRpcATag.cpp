//*******************************************************************
//
// LICENSE: MIT
//
// see top level LICENSE.txt
// 
// Author: Garrett Potts
//
// Description: Nitf support class for RPC00A -
// Rational Polynomial Coefficient extension.
//
//********************************************************************
// $Id: ossimNitfRpcATag.cpp 23666 2015-12-14 20:01:22Z rashadkm $

#include <ossim/support_data/ossimNitfRpcATag.h>

RTTI_DEF1(ossimNitfRpcATag, "ossimNitfRpcATag", ossimNitfRpcBase);

ossimNitfRpcATag::ossimNitfRpcATag()
   : ossimNitfRpcBase()
{
   // Set the tag name in base.
   setTagName(std::string("RPC00A"));
}
