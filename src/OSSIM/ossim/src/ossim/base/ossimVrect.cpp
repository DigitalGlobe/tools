//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// Description:
//
// Contains class declaration for vrect.
// Container class for four double points representing a rectangle
// where y is up
// 
//*******************************************************************
//  $Id: ossimVrect.cpp 23664 2015-12-14 14:17:27Z dburken $

#include <ossim/base/ossimVrect.h>

//*******************************************************************
// Public Method:
//*******************************************************************
void ossimVrect::print(std::ostream& os) const
{
   os << theUlCorner << theLrCorner;
}

//*******************************************************************
// friend function:
//*******************************************************************
std::ostream& operator<<(std::ostream& os, const ossimVrect& rect)
{
   rect.print(os);

   return os;
}

ossimVrect::~ossimVrect()
{
}

