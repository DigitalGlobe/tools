//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE: MIT  see top level LICENSE.txt
// 
// Author: Garrett Potts
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfRpfTagFactory.h 23666 2015-12-14 20:01:22Z rashadkm $
#ifndef ossimNitfRpfTagFactory_HEADER
#define ossimNitfRpfTagFactory_HEADER 1

#include <ossim/support_data/ossimNitfTagFactory.h>
class ossimNitfRegisteredTag;

class ossimNitfRpfTagFactory : public ossimNitfTagFactory
{
public:
   virtual ~ossimNitfRpfTagFactory();
   static ossimNitfRpfTagFactory* instance();
   virtual ossimRefPtr<ossimNitfRegisteredTag> create(const ossimString &tagName)const;
   
protected:
   ossimNitfRpfTagFactory();

private:
   /*!
    * Hide this.
    */
   ossimNitfRpfTagFactory(const ossimNitfRpfTagFactory & /* rhs */){}

   /*!
    * Hide this.
    */ 
   ossimNitfRpfTagFactory& operator =(const ossimNitfRpfTagFactory & /* rhs */){return *this;}

TYPE_DATA
};
#endif
