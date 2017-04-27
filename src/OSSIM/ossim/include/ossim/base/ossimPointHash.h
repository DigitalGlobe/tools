//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// Description: Point Hash is a base for hashing functions
//              
//*******************************************************************
//  $Id: ossimPointHash.h 23664 2015-12-14 14:17:27Z dburken $

#ifndef ossimPointHash_HEADER
#define ossimPointHash_HEADER
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimFpt.h>

class OSSIMDLLEXPORT ossimPointHash
{
public:
   virtual ~ossimPointHash(){}
   virtual long operator ()(const ossimDpt &aPoint)=0;
   virtual long operator ()(const ossimFpt &aPoint)=0;
};

#endif

