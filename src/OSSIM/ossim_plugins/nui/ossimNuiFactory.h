//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//*******************************************************************
//  $Id: ossimNuiFactory.h 23664 2015-12-14 14:17:27Z dburken $

#ifndef ossimNuiFactory_HEADER
#define ossimNuiFactory_HEADER

#include <ossim/imaging/ossimImageHandlerFactoryBase.h>
#include <ossim/base/ossimString.h>

class ossimGdal;
class ossimFilename;
class ossimKeywordlist;

//*******************************************************************
// CLASS:  ossimNuiFactory
//*******************************************************************
class ossimNuiFactory : public ossimImageHandlerFactoryBase
{
public:
   virtual ~ossimNuiFactory();
   static ossimNuiFactory* instance();
   
   virtual ossimImageHandler* open(const ossimFilename& fileName)const;
   virtual ossimImageHandler* open(const ossimKeywordlist& kwl,
                                   const char* prefix=0)const;

   
   virtual ossimObject* createObject(const ossimString& typeName)const;
   
   /*!
    * Creates and object given a keyword list.
    */
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   
   /*!
    * This should return the type name of all objects in all factories.
    * This is the name used to construct the objects dynamially and this
    * name must be unique.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
   virtual void getSupportedExtensions(ossimImageHandlerFactoryBase::UniqueStringList& extensionList)const;

protected:
   ossimNuiFactory(){}
   ossimNuiFactory(const ossimNuiFactory&){}
   void operator = (const ossimNuiFactory&){}

   static ossimNuiFactory* theInstance;

TYPE_DATA
};

#endif

