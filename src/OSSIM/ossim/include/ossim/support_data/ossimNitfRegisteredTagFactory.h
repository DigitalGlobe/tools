//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE: MIT see top level LICENSE.txt
// 
// Author: Garrett Potts
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfRegisteredTagFactory.h 23666 2015-12-14 20:01:22Z rashadkm $
#ifndef ossimNitfRegisteredTagFactory_HEADER
#define ossimNitfRegisteredTagFactory_HEADER 1

#include <ossim/support_data/ossimNitfTagFactory.h>

class ossimNitfRegisteredTagFactory : public ossimNitfTagFactory
{
public:
   virtual ~ossimNitfRegisteredTagFactory();
   static ossimNitfRegisteredTagFactory* instance();
   
   virtual ossimRefPtr<ossimNitfRegisteredTag> create(const ossimString &tagName)const;

protected:
   ossimNitfRegisteredTagFactory();

TYPE_DATA   
};

#endif
