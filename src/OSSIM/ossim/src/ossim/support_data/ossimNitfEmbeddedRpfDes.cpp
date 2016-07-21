//*******************************************************************
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfEmbeddedRpfDes.cpp 23664 2015-12-14 14:17:27Z dburken $

#include <istream>
#include <ostream>
#include <ossim/support_data/ossimNitfEmbeddedRpfDes.h>


RTTI_DEF1(ossimNitfEmbeddedRpfDes, "ossimNitfEmbeddedRpfDes", ossimNitfRegisteredTag)


ossimNitfEmbeddedRpfDes::ossimNitfEmbeddedRpfDes()
   : ossimNitfRegisteredTag(std::string("RPFDES"), 0)
{
}

ossimNitfEmbeddedRpfDes::~ossimNitfEmbeddedRpfDes()
{
}

void ossimNitfEmbeddedRpfDes::parseStream(std::istream& /* in */ )
{
}

std::ostream& ossimNitfEmbeddedRpfDes::print(std::ostream& out, const std::string& /* prefix */)const
{
   return out;
}

