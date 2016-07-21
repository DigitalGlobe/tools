//*******************************************************************
// Copyright (C) 2004 Garrett Potts.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimNitfNameConversionTables.h 23664 2015-12-14 14:17:27Z dburken $
#ifndef ossimNitfNameConversionTables_HEADER
#define ossimNitfNameConversionTables_HEADER
#include <ossim/base/ossimConstants.h>

class ossimString;

class OSSIM_DLL ossimNitfNameConversionTables
{
public:
   ossimNitfNameConversionTables();

   ossimString convertMapProjectionNameToNitfCode(const ossimString& mapProjectionName)const;
   ossimString convertNitfCodeToOssimProjectionName(const ossimString& nitfProjectionCode)const;
   ossimString convertNitfCodeToNitfProjectionName(const ossimString& nitfProjectionCode)const;
   ossimString convertNitfProjectionNameToNitfCode(const ossimString& nitfProjectionName)const;
};

#endif

