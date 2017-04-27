//*******************************************************************
//
// LICENSE: MIT
//
// see top level LICENSE.txt
// 
// Author: Garrett Potts
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfRpcBTag.cpp 23666 2015-12-14 20:01:22Z rashadkm $

#include <ossim/support_data/ossimNitfRpcBTag.h>

RTTI_DEF1(ossimNitfRpcBTag, "ossimNitfRpcBTag", ossimNitfRpcBase);

ossimNitfRpcBTag::ossimNitfRpcBTag()
   : ossimNitfRpcBase()
{
   // Set the tag name in base.
   setTagName(std::string("RPC00B"));
}
