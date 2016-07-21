//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimDisplayInterface.cpp 23664 2015-12-14 14:17:27Z dburken $
#include <ossim/base/ossimDisplayInterface.h>

RTTI_DEF(ossimDisplayInterface, "ossimDisplayInterface");

ossimDisplayInterface::ossimDisplayInterface()
{
}

ossimDisplayInterface::~ossimDisplayInterface()
{
}

ossimString ossimDisplayInterface::getTitle()const
{
   ossimString result;
   
   getTitle(result);
   
   return result;
}

